<?php

/**
 * @file
 * googlemap include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_googlemap_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps URL'),
    '#element_validate' => array('dpiscald_googlemap_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_googlemap_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // We use the permalink md5 as identifier.
  $atom->base_id = md5($url);

  $atom->title = 'Google Maps';

  $atom->data['urlN'] = $url;
  
  if(preg_match('/center=(.*?)&/', $url, $zone) || preg_match('/location=(.*?)&/', $url, $zone) || preg_match('/origin=(.*?)&/', $url, $zone) || preg_match('/q=(.*?)&/', $url, $zone)){
    $atom->title = urldecode($zone[1]);
    $thumbnail_url = 'https://maps.googleapis.com/maps/api/staticmap?center='.$zone[1].'&zoom=13&size=600x300&maptype=roadmap';
  }  
  
  if (!empty($thumbnail_url)) {
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($thumbnail_url);
    $dir = 'public://googlemap';
    if ($thumb->code == 200 && file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
      $dest = $dir . '/' . $atom->base_id . '.jpg';
      $file = file_save_data($thumb->data, $dest);

      if ($file) {
        // Set the file status to temporary.
        $query = db_update('file_managed')
          ->condition('fid', $file->fid)
          ->fields(array('status' => 0))
          ->execute();

        $atom->scald_thumbnail[LANGUAGE_NONE][0] = (array) $file;
      }
    }
  }
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_googlemap_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_googlemap_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_googlemap_player',
      array('vars' =>
        array(
          'urlN'      => $atom->data['urlN'],
          'thumbnail'     => $atom->rendered->thumbnail_source_url,
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_googlemap_theme() {
  return array(
      'dpiscald_googlemap_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/googlemap/dpiscald_googlemap_player'
      ),
      'dpiscald_googlemap_player_html5' => array(
        'variables' => array( 'urlN' => '', 'html_id' => 'player', 'width' => 640, 'height' => 390, 'origin' => ''),
        'template' => 'templates/players/dpiscald_googlemap_player_html5'
      ),
  );
}

/**
 * Checks if a media has already been imported, based on its media id.
 *
 * @param string $id
 *   The media identifier
 *
 * @return mixed
 *   FALSE if the media was never imported, the scald identifier of
 *   the media otherwise.
 */
function dpiscald_googlemap_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_googlemap_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  preg_match('/&z=(\d+)/', $url, $zoom); //Get the zoom paramater
  $url = str_replace($zoom[0], '', $url); //And remove it
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  elseif (dpiscald_googlemap_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an googlemap ID and check validity
 *
 */
function dpiscald_googlemap_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    $parsing = parse_url($string);
    if (isset($parsing['host']) && $parsing['host'] == "www.google.com") {
      $id = $string;
    }
    elseif (preg_match("/^http/", $string)) {
      // This string is a URL, most likely a shortened one. (http://dai.ly, http://bit.ly, etc...)
      $response = drupal_http_request($string);
      if ($response->code == 200 && isset($response->redirect_code) && ($response->redirect_code == 301 || $response->redirect_code == 302)) {
        return dpiscald_scribblelive_parse_id($response->redirect_url, $check);
      }
    }
  }
  else {
    $id = $string;
  }

  return $id;
}

function dpiscald_googlemap_get_media_infos_from_url($url) {
  $element = new stdClass();
  $element->title = 'Google Maps';
  $element->data['urlN'] = $url;
  if(preg_match('/center=(.*?)&/', $url, $zone) || preg_match('/location=(.*?)&/', $url, $zone) || preg_match('/origin=(.*?)&/', $url, $zone) || preg_match('/q=(.*?)&/', $url, $zone)){
    $element->title = urldecode($zone[1]);
    $element->scald_thumbnail['src'] = 'https://maps.googleapis.com/maps/api/staticmap?center='.$zone[1].'&zoom=13&size=600x300&maptype=roadmap';
  }  
  return $element;
}

function dpiscald_googlemap_get_import_info($importer, & $entity, & $wrapper, $url){
  $id = dpiscald_googlemap_parse_id ( $url, FALSE );
  $wrapper->base_id->set ( md5($id) );
  $info = dpiscald_googlemap_get_media_infos_from_url ( $id );
  if(isset($info->title)){
    $entity->title = $info->title;
  }
  if(isset($info->scald_thumbnail['src'])){
    $file = $importer->createFile ( $info->scald_thumbnail['src'], scald_atom_thumbnail_path ( $entity->type ) );
    $wrapper->scald_thumbnail->set ( ( array ) $file );
  }
  $entity->data['urlN'] = $info->data['urlN'];
}