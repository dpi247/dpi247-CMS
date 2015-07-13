<?php

/**
 * @file
 * slideshare include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_slideshare_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('slideshare URL'),
    //'#element_validate' => array('dpiscald_slideshare_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_slideshare_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  $infos = dpiscald_slideshare_get_media_infos_from_url($url);

  $atom->base_id = md5($url);

  $atom->title = $infos->title;

  $atom->provider = 'dpiscald';
  $atom->type = 'slideshare';

  $atom->data['html'] = $infos->html;
  $atom->data['slideshow_id'] = $infos->slideshow_id;

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
    $thumbnail_url = $infos->thumbnail;
    if (substr($thumbnail_url, 0, 2) == '//') {
      $thumbnail_url = 'http:' . $thumbnail_url;
    }

    $thumb = drupal_http_request($thumbnail_url);
    $dir = 'public://slideshare';
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
function dpiscald_slideshare_scald_fetch($atom, $type) {
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
function dpiscald_slideshare_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_slideshare_player',
      array('vars' =>
        array(
          'html'      => $atom->data['html'],
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
function dpiscald_slideshare_theme() {
  return array(
      'dpiscald_slideshare_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/slideshare/dpiscald_slideshare_player'
      ),
      'dpiscald_slideshare_player_html5' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'templates/players/dpiscald_slideshare_player_html5'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_slideshare_get_media_infos_from_url($url, $id_elem = NULL) {
  $api_url = 'http://fr.slideshare.net/api/oembed/2?url=';

  $url = $api_url . $url . '&format=json';
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
function dpiscald_slideshare_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_slideshare_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  if (startsWith($url,'https')){
    $url = explode('https://fr.slideshare.net/',$url);
  } else {
    $url = explode('http://fr.slideshare.net/',$url);
  }
  $url = $url[1];
  $id = md5($url);
  if (dpiscald_slideshare_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an slideshare ID and check validity
 *
 */
function dpiscald_slideshare_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);

  $infos = dpiscald_slideshare_get_media_infos_from_url($string);
  if (isset($infos->slideshow_id)) {
    $id = $infos->slideshow_id;
  }

  return $id;
}

function dpiscald_slideshare_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $id = dpiscald_slideshare_parse_id ( $url, FALSE );
    $wrapper->base_id->set ( $id );
    $infos = dpiscald_slideshare_get_media_infos_from_url ( $url );
    $entity->title = $infos->title;
    $entity->data['html'] = $infos->html;
    $entity->data['slideshow_id'] = $infos->slideshow_id;
    if(isset($infos->thumbnail_url)){
      $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Slideshare : " . $url);
  }
}