<?php

/**
 * @file
 * Deezer include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_deezer_scald_add_form(&$form, &$form_state) {
  $form['url_deezer'] = array(
    '#type' => 'textfield',
    '#title' => t('Url Deezer (track, album or playlist'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
  $form['#validate'][] = 'dpiscald_deezer_form_validate'; 
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_deezer_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  $url = $form_state['values']['url_deezer'];

  $parse_id = dpiscald_deezer_parse_id($url, null);

  $type = $parse_id[0];
  $id = $parse_id[1];

  // Get media infos.
  $infos = dpiscald_deezer_get_media_infos_from_url($type, $id);
  
  $url = dpiscald_deezer_build_url($type, $id);
  $atom->base_id = md5($url);

  $atom->title = $infos->title;

  $atom->provider = 'dpiscald';
  $atom->type = 'deezer';

  $atom->data['url'] = $url;
  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
      'tid' => 0,
      'taxonomy_term' => (object)(array('name' => $type=="playlist"?$infos->creator->name:$infos->artist->name))
  );

  if ($type == "track") {
   $thumbnail_url =  $infos->album->cover;
  } elseif ($type == "album") {
    $thumbnail_url = $infos->cover;
  } else {
    $thumbnail_url = $infos->picture;
  }

  if (!empty($thumbnail_url)) {
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($thumbnail_url);
    $dir = 'public://deezer';
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
function dpiscald_deezer_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_deezer_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_deezer_player',
      array('vars' =>
        array(
          'base_url'      => $atom->data['url'],
          'title'         => $atom->title,
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
function dpiscald_deezer_theme() {
  return array(
      'dpiscald_deezer_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/deezer/dpiscald_deezer_player'
      ),
      'dpiscald_deezer_player_html5' => array(
          'variables' => array( 'base_url' => '', 'html_id' => 'player', 'width' => 640, 'height' => 390, 'origin' => ''),
          'template' => 'templates/players/dpiscald_deezer_player_html5'
    ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_deezer_get_media_infos_from_url($type, $id) {
  $api_url = 'http://api.deezer.com/';

  if ($type == 'tracks') {
    $type = 'track';
  }

  $url = $api_url . $type . '/' . $id;

  $response = drupal_http_request($url);
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
function dpiscald_deezer_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Build url player
 *
 * @param array $form_state
 *
 * @return string url
 */
function dpiscald_deezer_build_url($type, $id) {
  if ($type  == 'track') {
    $type = 'tracks';
  }
  $url = 'http://www.deezer.com/plugins/player?type=' . $type . '&id=' . $id;

  return $url;
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_deezer_form_validate(&$form, &$form_state) {
  $url = $form_state['values']['url_deezer'];

  $parse_id = dpiscald_deezer_parse_id($url, null);

  $type = $parse_id[0];
  $id = $parse_id[1];

  if (!in_array($type, array('track', 'album', 'playlist'))) {
    form_error($form, t('Type is not defined'));
  }

  if (!isset($id) || empty($id)) {
    form_error($form, t('Please enter an identifier'));
  }

  $url = dpiscald_deezer_build_url($type, $id);

  $id = md5($url);
  if (dpiscald_deezer_already_imported($id)) {
    form_error($form, t('Media already imported.'));
  }
}

/**
 * Parse an deezer ID and check validity
 *
 */
function dpiscald_deezer_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    if (preg_match('#http?://www\.deezer\.com/([a-zA-Z0-9\-_]*)/([a-zA-Z0-9\-_]+)#', $string, $m)) {
      // This string os a full dailymotion URL
      $id = array( $m[1], $m[2]);
    }
    elseif (preg_match('#^https://www.deezer.com/([a-zA-Z0-9]+)(_.*)?$#', $string, $m)) {
      // This string os a full dailymotion URL
      $id = array( $m[1], $m[2]);
    }
    elseif (preg_match("/^http/", $string)) {
      // This string is a URL, most likely a shortened one. (http://dai.ly, http://bit.ly, etc...)
      $response = drupal_http_request($string);
      if ($response->code == 200 && isset($response->redirect_code) && ($response->redirect_code == 301 || $response->redirect_code == 302)) {
        return dpiscald_deezer_parse_id($response->redirect_url, $check);
      }
    }
  }
  else {
    $id = $string;
  }
 
  return $id;
}

function dpiscald_deezer_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $entity->provider = 'dpiscald';
    $id = dpiscald_deezer_parse_id ( $url, FALSE );
    $wrapper->base_id->set ( $id [1] );
    $infos = dpiscald_deezer_get_media_infos_from_url ( $id [0], $id [1] );
    if ($id [0] == "tracks") {
      $thumbnail_url = $infos->album->cover;
    } elseif ($id [0] == "album") {
      $thumbnail_url = $infos->cover;
    } else {
      $thumbnail_url = $infos->picture;
    }
    $file = $importer->createFile ( $thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
    $wrapper->scald_thumbnail->set ( ( array ) $file );
    $entity->data ['url'] = 'http://www.deezer.com/plugins/player?type=' . $id [0] . '&id=' . $id [1];
  } catch (Exception $e) {
  }
}