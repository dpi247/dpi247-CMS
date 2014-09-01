<?php

/**
 * @file
 * timelinejs include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_timelinejs_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeline JS URL'),
    '#element_validate' => array('dpiscald_timelinejs_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_timelinejs_scald_add_form_fill(&$atoms, $form, $form_state) {

  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  //$infos = dpiscald_timelinejs_get_media_infos_from_url($url);

  // We use the permalink md5 as identifier.
  $atom->base_id = md5($url);

  $atom->title = 'Timelinejs';
  
  $atom->data['url'] = $url;
  
  $thumbnail_url = 'http://www.outsystems.com/OSCore/Component_Image.aspx?ProjectImageId=1251';
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
  $thumb = drupal_http_request ( $thumbnail_url );
  $dir = 'public://timelinejs';
  if ($thumb->code == 200 && file_prepare_directory ( $dir, FILE_CREATE_DIRECTORY )) {
    $dest = $dir . '/' . $atom->base_id . '.jpg';
    $file = file_save_data ( $thumb->data, $dest );
    
    if ($file) {
      // Set the file status to temporary.
      $query = db_update ( 'file_managed' )->condition ( 'fid', $file->fid )->fields ( array (
          'status' => 0 
      ) )->execute ();
      
      $atom->scald_thumbnail [LANGUAGE_NONE] [0] = ( array ) $file;
    }
  }
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_timelinejs_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_timelinejs_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_timelinejs_player',
      array('vars' =>
        array(
          'url'      => $atom->data['url'],
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
function dpiscald_timelinejs_theme() {
  return array(
      'dpiscald_timelinejs_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/timelinejs/dpiscald_timelinejs_player'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_timelinejs_get_media_infos_from_url($url) {
  $api_url = 'http://cdn.knightlab.com/libs/timeline/latest/embed/index.html';
  $url = urlencode($url);
  $url = $api_url . '?source=' . '0Agl_Dv6iEbDadHdKcHlHcTB5bzhvbF9iTWwyMmJHdkE';
  //$url = 'https://magic.piktochart.com/embed/1869144-the-startup-way';
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
function dpiscald_timelinejs_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_timelinejs_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  elseif (dpiscald_timelinejs_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}
