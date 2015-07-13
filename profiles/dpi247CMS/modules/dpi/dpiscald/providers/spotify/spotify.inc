<?php

/**
 * @file
 * Spotify include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_spotify_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Spotify URL'),
    //'#element_validate' => array('dpiscald_spotify_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_spotify_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];
  $url = _dpiscald_spotify_url_cleaner($url);

  $atom->base_id = md5($url);
  
  if(startsWith($url, 'https://play')){
    $url = explode('https://play.spotify.com/', $url);
  }elseif(startsWith($url, 'https://open')){
    $url = explode('https://open.spotify.com/', $url);
  }elseif(startsWith($url, 'http://play')){
    $url = explode('http://play.spotify.com/', $url);
  }else{
    $url = explode('http://open.spotify.com/', $url);
  }

  // http://open.spotify.com/user/spotify/playlist/7EofMpXtFZCgQjJZoKaIXK

  $url = explode('/', $url[1]);

  // Get media infos.
  $infos = dpiscald_spotify_get_media_infos_from_url($url);

  $atom->title = $infos->name;

  $atom->provider = 'dpiscald';
  $atom->type = 'spotify';

  $atom->data['url'] = 'spotify:'.implode(':', $url);

  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
      'tid' => 0,
      'taxonomy_term' => (object)(array('name' => $infos->author->username))
  );
  
  // Prefill tags
  $langcode = field_language('scald_atom', $atom, 'scald_tags');
  foreach ($infos->meta->hashtags as $index => $tag) {
    $atom->scald_tags[$langcode][$index] = array(
        'tid' => $index,  // Beware, this is not a real tid, it's just an index
        'taxonomy_term' => (object)(array('name' => $tag))
    );
  }

  if (!empty($infos->album->images[2]->url)) {
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($infos->album->images[2]->url);
    $dir = 'public://spotify';
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
function dpiscald_spotify_scald_fetch($atom, $type) {
  if (isset($atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'])) {
    $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
    if (file_exists($file)) {
      $atom->file_source = $atom->thumbnail_source = $file;
    }
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_spotify_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_spotify_player',
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
function dpiscald_spotify_theme() {
  return array(
      'dpiscald_spotify_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/spotify/dpiscald_spotify_player'
      ),
      'dpiscald_spotify_player_html5' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'templates/players/dpiscald_spotify_player_html5'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_spotify_get_media_infos_from_url($url, $id_elem = NULL) {
  $api_url = 'https://api.spotify.com/v1/';

  $url[0] = $url[0] . 's';

  $url = $api_url . $url[0] . '/' . $url[1];

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
function dpiscald_spotify_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_spotify_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  if (startsWith($url,'https')){
    $url = explode('https://play.spotify.com/',$url);
  } else {
    $url = explode('http://play.spotify.com/',$url);
  }
  $url = $url[1];
  $id = md5($url);
  if (dpiscald_spotify_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an spotify ID and check validity
 *
 */
function dpiscald_spotify_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);

  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {   

    if (preg_match('/^(http\:\/\/)?(play|open).spotify.com\/user\/spotify\/([a-zA-Z0-9]*)\/([a-zA-Z0-9]{22})$/Si',$string,$m)) {
      $id = $m;
    } elseif (preg_match('/^(https\:\/\/)?(play|open).spotify.com\/user\/spotify\/([a-zA-Z0-9]*)\/([a-zA-Z0-9]{22})$/Si',$string,$m)) {
      $id = $m;
    }
    // The string ID is not easy to parse, let's try to analyze it
    elseif (preg_match('/^(http\:\/\/)?(play|open).spotify.com\/(track|album|artist)\/([a-zA-Z0-9]{22})$/Si',$string,$m)) {
      // This string os a full dailymotion URL
      $id = $m;
    }
    elseif (preg_match('/^(https\:\/\/)?(play|open).spotify.com\/(track|album|artist)\/([a-zA-Z0-9]{22})$/Si',$string,$m)) {
      // This string os a full dailymotion URL
      $id = $m;
    }
    elseif (preg_match("/^http/", $string)) {      
      // This string is a URL, most likely a shortened one. (http://dai.ly, http://bit.ly, etc...)
      $response = drupal_http_request($string);
      if ($response->code == 200 && isset($response->redirect_code) && ($response->redirect_code == 301 || $response->redirect_code == 302)) {
        return dpiscald_spotify_parse_id($response->redirect_url, $check);
      }
    }
  }
  else {
    $id = $string;
  }

  return $id;
}

function _dpiscald_spotify_url_cleaner($url) {
  $parts = parse_url($url);
  $clean_url = $url;
  if (!empty($parts['scheme']) && !empty($parts['host']) && !empty($parts['path'])) {
    $clean_url = $parts['scheme'] . '://' . $parts['host'] . $parts['path'];
  }
  return $clean_url;
}

function dpiscald_spotify_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $id = dpiscald_spotify_parse_id ( $url, FALSE );              
    $url = _dpiscald_spotify_url_cleaner($url);
    if(startsWith($url, 'https://play')){
      $url = explode('https://play.spotify.com/', $url);
    }elseif(startsWith($url, 'https://open')){
      $url = explode('https://open.spotify.com/', $url);
    }elseif(startsWith($url, 'http://play')){
      $url = explode('http://play.spotify.com/', $url);
    }else{
      $url = explode('http://open.spotify.com/', $url);
    }
    $url = explode('/', $url[1]);

    $infos = dpiscald_spotify_get_media_infos_from_url ( $url, $id[count($id)-1] );
    $wrapper->base_id->set ( md5($id[0]) );
    if (isset($infos->name)) {
      $entity->title = $infos->name;
    } else {
      $entity->title = 'spotify default title';
    }

    if(isset($infos->album->images[0])){
      $file = $importer->createFile ( $infos->album->images[0]->url, scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }

    $entity->data['url'] = 'spotify:'.implode(':', $url);
    $entity->data['orig_identifier'] = $url;
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Spotify : " . $url);
  }
}