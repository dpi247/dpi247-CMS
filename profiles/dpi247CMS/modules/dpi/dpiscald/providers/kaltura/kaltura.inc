<?php
/**
 * @file
 *   Defines a Video RTL provider for Scald.
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_kaltura_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Vidéo Kaltura URL'),
    '#element_validate' => array('dpiscald_kaltura_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}


/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_kaltura_scald_add_form_fill(&$atom, $form, $form_state) {
    // Get the identifier
  $identifier = dpiscald_kaltura_parse_id($form_state['values']['identifier'], TRUE);

  $atom->base_id = $identifier;

  $infos = dpiscald_kaltura_get_media_infos_from_url($form_state['values']['identifier']);

  $atom->title = $infos['title'];

  $atom->provider = 'dpiscald';
  $atom->type = 'kaltura';

   // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
    'tid' => 0,
    'taxonomy_term' => (object)(array('name' => $infos->author))
  );

  // Save video width and height
  if (!isset($atom->data)) {
    $atom->data = array();
  }

  $atom->data['video_id'] = $identifier;
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_kaltura_scald_fetch($atom, $type) {
  $items = field_get_items('scald_atom', $atom, 'scald_thumbnail');
  if ($items && file_exists($items[0]['uri'])) {
    $atom->file_source = $atom->thumbnail_source = $items[0]['uri'];
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_kaltura_scald_prerender($atom, $context, $options, $mode) {
  $theme_name = 'dpiscald_kaltura_player';
  if ($mode == 'atom') {
    $atom->rendered->player = theme($theme_name,
      array('vars' =>
        array(
          'video_id'      => $atom->data['video_id'],
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_kaltura_theme() {
  return array(
    'dpiscald_kaltura_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/kaltura/dpiscald_kaltura_player'
    ),
  );
}

/**
 * Get media infos from url.
 */
function dpiscald_kaltura_get_media_infos_from_url($url, $id_elem = NULL) {
  $infos = array();

  $infos["id"] = dpiscald_kaltura_parse_id($url, false);

  parse_str($url, $url_parse);

  if (isset($url_parse["playerId"])) $infos['playerId'] = $url_parse["playerId"];
  if (isset($url_parse["width"])) $infos['width'] = $url_parse["width"];
  if (isset($url_parse["height"])) $infos['height'] = $url_parse["height"];

  if (preg_match('/\/p\/([0-9]*)\/sp/i', $url,$urlT)) {
    $infos['partnerId'] = $urlT[1];
  }

  if (preg_match('/\/uiconf_id\/([0-9]*)\/partner_id/i', $url,$urlT)) {
    $infos['uiConfId'] = $urlT[1];
  }

  $infos['title'] = "TEST";
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
function dpiscald_kaltura_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_kaltura_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }

  $id = dpiscald_kaltura_parse_id($url);

  if ($id ==  NULL) {
    form_error($element, t('Invalid Kaltura url.'));
  }
 
  if (dpiscald_kaltura_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an video kaltura ID and check validity
 *
 */
function dpiscald_kaltura_parse_id($string, $check) {
  $id = NULL;

  $string = trim($string);
  parse_str($string, $url_parse);

  if (isset($url_parse['entry_id']) && !empty($url_parse['entry_id'])) {
    $id = $url_parse['entry_id'];
  }
 
  return $id;
}


function dpiscald_kaltura_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $identifier = dpiscald_kaltura_parse_id($url, TRUE);
    $entity->base_id = $identifier;
    $wrapper->base_id->set ( md5($identifier) );
    $infos = dpiscald_kaltura_get_media_infos_from_url ( $url );
    if(isset($infos->thumbnail_url)){
      $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }
    $entity->title = $infos['title'];
    $entity->data['orig_identifier'] = $url;
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Video RTL : " . $url);
  }
}