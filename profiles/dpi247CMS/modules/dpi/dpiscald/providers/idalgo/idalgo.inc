<?php

/**
 * @file
 * idalgo include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_idalgo_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Idalgo URL'),
    '#element_validate' => array('dpiscald_idalgo_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_idalgo_scald_add_form_fill(&$atoms, $form, $form_state) {

  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  $infos = dpiscald_idalgo_get_media_infos_from_url($url);
  
  // We use the permalink md5 as identifier.
  $atom->base_id = md5($infos->url);

  $atom->title = $infos->title;

  $atom->data['url'] = $url;

}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_idalgo_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  } else {
    // Default thumbnail
    $uri = 'public://atoms/idalgo.png';
    $files = file_load_multiple(array(), array('uri' => $uri));
    $file = reset($files);
    if (isset($file)) {
      file_usage_add($file, 'dpiscald', 'idalgo', $atom->sid);
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
function dpiscald_idalgo_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_idalgo_player',
      array('vars' =>
        array(
          'url'      => $atom->data['url'],
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_idalgo_theme() {
  return array(
      'dpiscald_idalgo_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/idalgo/dpiscald_idalgo_player'
      ),
      'dpiscald_idalgo_player_html5' => array(
        'variables' => array('vars' => NULL, 'atom' => NULL),
        'template' => 'templates/players/dpiscald_idalgo_player_html5'
    ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_idalgo_get_media_infos_from_url($url) {
  $response = drupal_http_request($url);
  if ($response->code >= 200 && $response->code < 400 && !empty($response->data)) {
    preg_match('/txt_competition=(.*)$/', $url, $title);
    if ($title[0]) {
      $infos->title = $title[1];
    } else {
      $infos->title = 'Idalgo widget';
    }
    $infos->url = $url;
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
function dpiscald_idalgo_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_idalgo_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  elseif (dpiscald_idalgo_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an idalgo ID and check validity
 *
 */
function dpiscald_idalgo_parse_id($string, $check) {
  $id = md5($url);
  return $id;
}

function dpiscald_idalgo_get_import_info($importer, & $entity, & $wrapper, $url){
  $id = dpiscald_idalgo_parse_id ( $url, FALSE );
  $wrapper->base_id->set ( $id );
  $infos = dpiscald_idalgo_get_media_infos_from_url ( $url );
  $entity->title = $infos->title;
  if(isset($infos->thumbnail_url)){
    $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
    $wrapper->scald_thumbnail->set ( ( array ) $file );
  }
  $entity->base_id = $id;
  $entity->data['url'] = $infos->url;
}
