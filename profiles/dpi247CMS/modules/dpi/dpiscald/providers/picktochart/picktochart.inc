<?php

/**
 * @file
 * picktochart include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_picktochart_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Picktochart URL'),
    '#element_validate' => array('dpiscald_picktochart_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_picktochart_scald_add_form_fill(&$atoms, $form, $form_state) {

  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // Get the URL.
  $url = $form_state['values']['identifier'];

  // Get media infos.
  $infos = dpiscald_picktochart_get_media_infos_from_url($url);
  
  // We use the permalink md5 as identifier.
  $atom->base_id = md5($url);

  $atom->title = $infos['title'];

  $atom->data['id'] = $infos['id'];
  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
      'tid' => 0,
      'taxonomy_term' => (object)(array('name' => $infos['author']))
  );

  if (!empty($infos['thumbnail_url'])) {
    $infos['thumbnail_url'] = str_replace("https://", "http://", $infos['thumbnail_url']);
    // Download a copy of the media thumbnail. This makes it possible
    // to do interesting manipulation with image styles presets.
    $thumb = drupal_http_request($infos['thumbnail_url']);
    $dir = 'public://picktochart';
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
function dpiscald_picktochart_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_picktochart_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_picktochart_player',
      array('vars' =>
        array(
          'id'      => $atom->data['id'],
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
function dpiscald_picktochart_theme() {
  return array(
      'dpiscald_picktochart_player' => array(
          'variables' => array('vars' => NULL, 'atom' => NULL),
          'template' => 'providers/picktochart/dpiscald_picktochart_player'
      ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_picktochart_get_media_infos_from_url($url) {
  $response = drupal_http_request($url);
  if ($response->code >= 200 && $response->code < 400 && !empty($response->data)) {
    $id = explode('output/',$url);
    preg_match('/<meta name="author" content="(.*?)">/', $response->data, $author);
    preg_match('/<meta property="og:title" content="(.*?)" \/>/', $response->data, $title);
    preg_match('/<meta property="og:image" content="(.*?)" \/>/', $response->data, $thumb);
    $infos = array(
        'id' => $id[1],
        'title' => $title[1],
        'author' => $author[1],
        'thumbnail_url' => $thumb[1]
    );
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
function dpiscald_picktochart_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_picktochart_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  elseif (dpiscald_picktochart_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an picktochart ID and check validity
 *https://magic.piktochart.com/output/4155739-testing
 */
function dpiscald_picktochart_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    $parsing = parse_url($string);
    if (isset($parsing['host']) && $parsing['host'] == "magic.piktochart.com") {
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

function dpiscald_picktochart_get_import_info($importer, & $entity, & $wrapper, $url){
  $id = dpiscald_picktochart_parse_id ( $url, FALSE );
  $wrapper->base_id->set ( $id );
  $infos = dpiscald_picktochart_get_media_infos_from_url ( $url );
  $entity->title = $infos['title'];
  $entity->data['id'] = $infos['id'];
  $file = $importer->createFile ( $infos['thumbnail_url'], scald_atom_thumbnail_path ( $entity->type ) );
  $wrapper->scald_thumbnail->set ( ( array ) $file );
}