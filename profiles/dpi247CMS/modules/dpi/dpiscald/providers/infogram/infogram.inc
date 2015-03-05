<?php

/**
 * @file
 * Spotify include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_infogram_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Infogram URL'),
    '#element_validate' => array('dpiscald_infogram_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_infogram_scald_add_form_fill(&$atoms, $form, $form_state) {
  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];
  
  // Get media infos.
  $infos = dpiscald_infogram_get_media_infos_from_url($url);
  
  $atom->base_id = md5($url);

  $atom->title = $infos->name;

  $atom->provider = 'dpiscald';
  $atom->type = 'infogram';

  $atom->data['url'] = $url;
  $atom->data['id'] = dpiscald_infogram_parse_id($url, false);

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
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_infogram_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  } else {
    // Default thumbnail
    $uri = 'public://atoms/infogram.png';
    $files = file_load_multiple(array(), array('uri' => $uri));
    $file = reset($files);
    if ($file != NULL) {
      file_usage_add($file, 'dpiscald', 'infogram', $atom->sid);
      $atom->scald_thumbnail[LANGUAGE_NONE][0] = (array) $file;
    }

    if (file_exists($uri)) {
      $atom->file_source = $atom->thumbnail_source = $uri;
    }
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_infogram_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_infogram_player',
      array('vars' =>
        array(
          'base_url'      => $atom->data['url'],
          'id'            => $atom->data['id'],
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
function dpiscald_infogram_theme() {
  return array(
      'dpiscald_infogram_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/infogram/dpiscald_infogram_player'
      ),
      'dpiscald_infogram_player_html5' => array(
          'variables' => array( 'vars' => NULL, 'atom' => NULL),
          'template' => 'templates/players/dpiscald_infogram_player_html5'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_infogram_get_media_infos_from_url($url, $id_elem = NULL) {
  // Pour infogram, on a que l'nformation de l'id pour le title / name
  $id = dpiscald_infogram_parse_id($url, FALSE);
  $infos->title = $id;
  $infos->name = $id;

  if (isset($infos)) {
    return $infos;
  }
  return FALSE;
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
function dpiscald_infogram_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_infogram_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  if (startsWith($url,'https')){
    $url = explode('https://infogr.am/',$url);
  } else {
    $url = explode('http://infogr.am/',$url);
  }
  $url = $url[1];
  $id = md5($url);
  if (dpiscald_infogram_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an spotify ID and check validity
 *
 */
function dpiscald_infogram_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  // get something else like a page post (i don't have any video example to play with)
  preg_match("/infogr\.am\/(.*)$/siU", $string, $match);
  if (isset($match[0])) {
    $id = $match[1];
  }

  return $id;
}

function dpiscald_infogram_get_import_info($importer, & $entity, & $wrapper, $url){
  $id = dpiscald_infogram_parse_id ( $url, FALSE );
  $wrapper->base_id->set ( $id );
  $infos = dpiscald_infogram_get_media_infos_from_url ( $url );
  $entity->title = $infos->name;
  if(isset($infos->thumbnail_url)){
    $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
    $wrapper->scald_thumbnail->set ( ( array ) $file );
  }
  $entity->base_id = $id;
  $entity->data['url'] = $url;
  $entity->data['id'] = $id;
}
