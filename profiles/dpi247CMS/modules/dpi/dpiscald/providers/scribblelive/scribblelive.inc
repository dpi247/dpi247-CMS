<?php

/**
 * @file
 * scribblelive include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_scribblelive_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Scribblelive URL'),
    '#element_validate' => array('dpiscald_scribblelive_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_scribblelive_scald_add_form_fill(&$atoms, $form, $form_state) {

  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  $infos = dpiscald_scribblelive_get_media_infos_from_url($url);
  
  // We use the permalink md5 as identifier.
  $atom->base_id = md5($url);

  $title = html_entity_decode($infos['title'], ENT_QUOTES, 'UTF-8');
  $atom->title = $title;

  $atom->data['id'] = $infos['id'];
  $atom->data['url'] = $infos['url'];

  if (!empty($infos['thumbnail_url'])) {
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($infos['thumbnail_url']);
    $dir = 'public://scribblelive';
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
function dpiscald_scribblelive_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_scribblelive_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_scribblelive_player',
      array('vars' =>
        array(
          'id'      => $atom->data['id'],
          'url'     => $atom->data['url'],
          //'thumbnail'     => $atom->rendered->thumbnail_source_url,
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_scribblelive_theme() {
  return array(
      'dpiscald_scribblelive_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/scribblelive/dpiscald_scribblelive_player'
      ),
      'dpiscald_scribblelive_player_html5' => array(
        'variables' => array('vars' => NULL, 'atom' => NULL),
        'template' => 'templates/players/dpiscald_scribblelive_player_html5'
    ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_scribblelive_get_media_infos_from_url($url) {
  $response = drupal_http_request($url);
  if ($response->code >= 200 && $response->code < 400 && !empty($response->data)) {
    preg_match('/\?id=(.*?)">/', $response->data, $id);
    preg_match('/<meta property="og:title" content="(.*?)">/', $response->data, $title);
    preg_match('/<meta property="og:image" content="(.*?)">/', $response->data, $thumb);
    if (empty($id) && empty($title) && empty($thumb)){
      preg_match('/\?Id=(.*?)">/', $response->data, $id);
      preg_match('/<meta property="og:title" content="(.*?)" \/>/', $response->data, $title);
      preg_match('/<meta property="og:image" content="(.*?)" \/>/', $response->data, $thumb);
    }
    $infos = array(
        'id' => $id,
        'url' => $url,
        'title' => $title[1],
        'thumbnail_url' => $thumb[1]
    );
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
function dpiscald_scribblelive_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_scribblelive_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  elseif (dpiscald_scribblelive_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an scribblelive ID and check validity
 *
 */
function dpiscald_scribblelive_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    $parsing = parse_url($string);
    if (isset($parsing['host']) && $parsing['host'] == "embed.scribblelive.com") {
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

function dpiscald_scribblelive_get_import_info($importer, & $entity, & $wrapper, $url){
  $id = dpiscald_scribblelive_parse_id ( $url, FALSE );
  $wrapper->base_id->set ( $id );
  $infos = dpiscald_scribblelive_get_media_infos_from_url ( $url );
  $entity->title = $infos['title'];
  $file = $importer->createFile ( $infos['thumbnail_url'], scald_atom_thumbnail_path ( $entity->type ) );
  $wrapper->scald_thumbnail->set ( ( array ) $file );
  $entity->data['id'] = $infos['id'];
  $entity->data['orig_identifier'] = $value;
  $entity->data['url'] = $infos['url'];
}