<?php
/**
 * @file
 *   Defines a Video RTL provider for Scald.
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_imageexterne_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('External image URL (the image will not be downloaded locally. So they are no available thumbnail or other information)'),
    '#element_validate' => array('dpiscald_imageexterne_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}


/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_imageexterne_scald_add_form_fill(&$atom, $form, $form_state) {
    // Get the identifier
  $identifier = dpiscald_imageexterne_parse_id($form_state['values']['identifier'], TRUE);

  $atom->base_id = $identifier;

  $infos = dpiscald_imageexterne_get_media_infos_from_url($form_state['values']['identifier']);

  $atom->title = $infos['title'];

  $atom->provider = 'dpiscald';
  $atom->type = 'imageexterne';

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
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_imageexterne_scald_fetch($atom, $type) {
  $items = field_get_items('scald_atom', $atom, 'scald_thumbnail');
  if ($items && file_exists($items[0]['uri'])) {
    $atom->file_source = $atom->thumbnail_source = $items[0]['uri'];
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_imageexterne_scald_prerender($atom, $context, $options, $mode) {
  $theme_name = 'dpiscald_imageexterne_player';
  if ($mode == 'atom') {
    $atom->rendered->player = theme($theme_name,
      array('vars' =>
        array(
          'url'      => $atom->base_id,
          'title'    => $atom->title,
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_imageexterne_theme() {
  return array(
    'dpiscald_imageexterne_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/imageexterne/dpiscald_imageexterne_player'
    ),
    'dpiscald_imageexterne_player_html5' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'templates/players/dpiscald_imageexterne_player_html5'
    ),
  );
}

/**
 * Get media infos from url.
 */
function dpiscald_imageexterne_get_media_infos_from_url($url, $id_elem = NULL) {
  $infos = array();
  // On a pas d'infos :(
  $infos['title'] = '';
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
function dpiscald_imageexterne_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_imageexterne_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }

  // $id = $url;

  if (dpiscald_imageexterne_already_imported($url)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an video rtl ID and check validity
 *
 */
function dpiscald_imageexterne_parse_id($string, $check) {
  $string = trim($string);
  return $string;
}


function dpiscald_imageexterne_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $identifier = dpiscald_imageexterne_parse_id($url, TRUE);
    $entity->base_id = $identifier;
    $wrapper->base_id->set ( $identifier );
    $infos = dpiscald_imageexterne_get_media_infos_from_url ( $url );
    if(isset($infos->thumbnail_url)){
      $file = $importer->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }
    $entity->title = $infos['title'];
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Image externe : " . $url);
  }
}