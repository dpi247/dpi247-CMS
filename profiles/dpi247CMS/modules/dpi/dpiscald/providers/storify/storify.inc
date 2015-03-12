<?php

/**
 * @file
 * Kewego include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_storify_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Storify URL'),
    '#element_validate' => array('dpiscald_storify_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_storify_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];
  
  if (startsWith($url,'https')){
    $url = explode('https://storify.com/',$url);
  } else {
    $url = explode('http://storify.com/',$url);
  }
  $url = $url[1];
  
  // Get media infos.
  $infos = dpiscald_storify_get_media_infos_from_url($url);
  
  $atom->base_id = md5($url);
  
  $infos = $infos->content;

  $atom->title = $infos->title;

  $atom->data['url'] = $url;
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

  if (!empty($infos->thumbnail)) {
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($infos->thumbnail);
    $dir = 'public://storify';
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
function dpiscald_storify_scald_fetch($atom, $type) {
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
function dpiscald_storify_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_storify_player',
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
function dpiscald_storify_theme() {
  return array(
      'dpiscald_storify_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/storify/dpiscald_storify_player'
      ),
      'dpiscald_storify_player_html5' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'templates/players/dpiscald_storify_player_html5'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_storify_get_media_infos_from_url($url) {
  $api_url = 'http://api.storify.com/v1/stories/';
  $url = $api_url . $url;
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
function dpiscald_storify_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_storify_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  if (startsWith($url,'https')){
    $url = explode('https://storify.com/',$url);
  } else {
    $url = explode('http://storify.com/',$url);
  }
  $url = $url[1];
  $id = md5($url);
  if (dpiscald_storify_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an storify ID and check validity
 *
 */
function dpiscald_storify_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    $parsing = parse_url($string);
    if (isset($parsing['host']) && $parsing['host'] == "storify.com") {
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

function dpiscald_storify_get_import_info($importer, & $entity, & $wrapper, $urls){
  try {
    $id = dpiscald_storify_parse_id ( $urls, FALSE );
    $wrapper->base_id->set ( $id );
    $url = $urls;
    if (startsWith($url,'https')){
      $url = explode('https://storify.com/',$url);
    } else {
      $url = explode('http://storify.com/',$url);
    }               
    $url = $url[1];
    $infos = dpiscald_storify_get_media_infos_from_url ( $url );
    $infos = $infos->content;                
    $entity->title = $infos->title;                
    $entity->data['url'] = $url;
    $entity->data['orig_identifier'] = $urls;   
    if(isset($infos->thumbnail)){
      $file = $importer->createFile ( $infos->thumbnail, scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Storify : " . $url);
  }
}