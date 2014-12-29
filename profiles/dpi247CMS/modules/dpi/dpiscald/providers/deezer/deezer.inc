<?php

/**
 * @file
 * Deezer include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_deezer_scald_add_form(&$form, &$form_state) {
  $form['type_deezer'] = array(
    '#title' => t('Select item type'),
    '#type' => 'select',
    '#description' => 'Select the desired item',
    '#options' => array('tracks' => t('Track'), 'album' => t('Album'), 'playlist' => t('Playlist')),
  );
  $form['id_deezer'] = array(
    '#type' => 'textfield',
    '#title' => t('ID Deezer'),
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

  $type = $form_state['values']['type_deezer'];
  $id = $form_state['values']['id_deezer'];

  // Get media infos.
  $infos = dpiscald_deezer_get_media_infos_from_url($type, $id);
  
  $url = dpiscald_deezer_build_url($form_state);
  $atom->base_id = md5($url);

  $atom->title = $infos->title;

  $atom->data['url'] = $url;
  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
      // 'tid' => 0,
      // 'taxonomy_term' => (object)(array('name' => $infos->author->username))
  );
  
  // Prefill tags
  // $langcode = field_language('scald_atom', $atom, 'scald_tags');
  // foreach ($infos->meta->hashtags as $index => $tag) {
  //   $atom->scald_tags[$langcode][$index] = array(
  //       'tid' => $index,  // Beware, this is not a real tid, it's just an index
  //       'taxonomy_term' => (object)(array('name' => $tag))
  //   );
  // }

  if ($type == "track") {
   $thumbnail_url =  $infos->album->cover;
  } elseif ($type == "album") {
    $thumbnail_url ⁼ $infos->cover;
  } else {
    $thumbnail_url = $infos->picture;
  }

  if (!empty($thumbnail_url) {
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
function dpiscald_deezer_build_url($form_state) {
  $type = $form_state['values']['type_deezer'];
  $id = $form_state['values']['id_deezer'];
  $url = 'http://www.deezer.com/plugins/player?type=' . $type . '&id=' . $id;

  return $url;
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_deezer_form_validate(&$form, &$form_state) {
  $type = $form_state['values']['type_deezer'];
  $id = $form_state['values']['id_deezer'];

  if (!in_array($type, array('tracks', 'album', 'playlist'))) {
    form_error($form, t('Type is not defined'));
  }

  if (!isset($id) || empty($id)) {
    form_error($form, t('Please enter an identifier'));
  }

  $url = dpiscald_deezer_build_url($form_state);

  $id = md5($url);
  if (dpiscald_deezer_already_imported($id)) {
    form_error($form, t('Media already imported.'));
  }
}