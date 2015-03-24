<?php

/**
 * @file
 * ustream include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_ustream_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('ustream URL'),
    //'#element_validate' => array('dpiscald_ustream_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_ustream_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  $infos = dpiscald_ustream_get_media_infos_from_url($url);

  $atom->base_id = md5($url);

  $atom->title = $infos->title;

  $atom->provider = 'dpiscald';
  $atom->type = 'ustream';

  $atom->data['id'] = $infos->id;
  $atom->data['mode'] = $infos->mode;

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
    $dir = 'public://ustream';
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
function dpiscald_ustream_scald_fetch($atom, $type) {
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
function dpiscald_ustream_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_ustream_player',
      array('vars' =>
        array(
          'id'            => $atom->data['id'],
          'mode'          => $atom->data['mode'],
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
function dpiscald_ustream_theme() {
  return array(
      'dpiscald_ustream_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/ustream/dpiscald_ustream_player'
      ),
      'dpiscald_ustream_player_html5' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'templates/players/dpiscald_ustream_player_html5'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_ustream_get_media_infos_from_url($url, $id_elem = NULL) {
  $id = dpiscald_ustream_parse_id($url, false);
  $infos = new stdclass;
  $infos->title = "Ustream - " . $id[1];
  $infos->id = $id[1];
  $infos->mode = $id[0];

  return $infos;
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
function dpiscald_ustream_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_ustream_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  $id = md5($url);
  if (dpiscald_ustream_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an ustream ID and check validity
 *
 */
function dpiscald_ustream_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);

  if (preg_match('#^http://www.ustream.tv/([a-zA-Z0-9\-_]*)/([a-zA-Z0-9\-_]+)#', $string, $match)){
    $id = array($match[1], $match[2]);
  } elseif (preg_match('#^https://www.ustream.tv/([a-zA-Z0-9\-_]*)/([a-zA-Z0-9\-_]+)#', $string, $match)) {
    $id = array($match[1], $match[2]);
  }

  return $id;
}

function dpiscald_ustream_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $id = dpiscald_ustream_parse_id ( $url, FALSE );
    $wrapper->base_id->set ( md5($url) );
    $infos = dpiscald_ustream_get_media_infos_from_url ( $url );
    $entity->title = $infos->title;
    $entity->data['id'] = $infos->id;
    $entity->data['mode'] = $infos->mode;
    if(isset($infos->thumbnail_url)){
      $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media ustream : " . $url);
  }
}