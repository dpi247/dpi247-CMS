<?php

/**
 * @file
 * Scald Embed.ly Module
 */

/**
 * Implements hook_scald_atom_providers().
 *
 * Tell Scald that we'll be providing some embedded media atoms.
 */
function dpiscald_embedly_scald_atom_providers() {
  return array(
    'embed' => 'Media embedded through Embed.ly',
  );
}

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_embedly_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Media URL'),
    '#element_validate' => array('dpiscald_embedly_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_embedly_scald_add_form_fill(&$atoms, $form, $form_state) {

  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  $infos = dpiscald_embedly_get_media_infos_from_url($url);

  // We use the permalink md5 as identifier.
  $atom->base_id = md5($url);

  $atom->title = $infos->title;

  $atom->data['url'] = $infos->url;
  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
      'tid' => 0,
      'taxonomy_term' => (object)(array('name' => $infos->author_name))
  );

  if (!empty($infos->thumbnail_url)) {
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($infos->thumbnail_url);
    $dir = 'public://embedly';
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
function dpiscald_embedly_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_embedly_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    if ($context != 'sdl_library_item') {
      $atom->rendered->player = theme('dpiscald_embedly_player',
      array('vars' =>
        array(
          'url'      => $atom->data['url'],
          'title'     => $atom->title,
        ),
        'atom' => $atom,
      )
    );
    }
  }
}


/**
 * Get media infos from url.
 */
function dpiscald_embedly_get_media_infos_from_url($url) {
  $api_url = 'http://api.embed.ly/1/oembed';
  $url = urlencode($url);
  $response = drupal_http_request($api_url . '?url=' . $url);
  if ($response->code >= 200 && $response->code < 400 && !empty($response->data)) {
    $infos = json_decode($response->data);
    return $infos;
  }
  else {
    return FALSE;
  }
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
function dpiscald_embedly_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_embedly_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  elseif (dpiscald_embedly_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_embedly_theme() {
  return array(
    'dpiscald_embedly_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/embedly/dpiscald_embedly_player'
    ),
  );
}

/**
 * Parse an embedly ID and check validity
 *
 */
function dpiscald_embedly_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    if (valid_url($string)) {
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

function dpiscald_embedly_get_import_info($importer, & $entity, & $wrapper, $url){
  $id = dpiscald_embedly_parse_id ( $url, FALSE );
  $wrapper->base_id->set ( md5($id) );
  $infos = dpiscald_embedly_get_media_infos_from_url ( $id );
  $entity->title = $infos->title;
  if(isset($infos->thumbnail_url)){
    $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
    $wrapper->scald_thumbnail->set ( ( array ) $file );
  }
  $entity->data['url'] = $infos->url;
  $entity->data['orig_identifier'] = $url;
}