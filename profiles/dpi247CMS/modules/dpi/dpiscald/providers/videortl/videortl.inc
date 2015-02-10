<?php
/**
 * @file
 *   Defines a Video RTL provider for Scald.
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_videortl_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Vido RTL identifier or URL'),
    '#element_validate' => array('dpiscald_videortl_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}


/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_videortl_scald_add_form_fill(&$atom, $form, $form_state) {
    // Get the identifier
  $identifier = dpiscald_videortl_parse_id($form_state['values']['identifier'], TRUE);
  $key = dpiscald_videortl_parse_key($form_state['values']['identifier'], TRUE);
  $live_id = dpiscald_videortl_parse_liveid($form_state['values']['identifier'], TRUE);
  $domain = dpiscald_videortl_parse_domain($form_state['values']['identifier'], TRUE);

  $atom->base_id = $identifier;

  $atom->title = 'Titre par défaut de la video RTL';

  $atom->provider = 'dpiscald';
  $atom->type = 'videortl';

   // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
    'tid' => 0,
    'taxonomy_term' => (object)(array('name' => $infos->author))
  );

  // // Prefill tags
  // $langcode = field_language('scald_atom', $atom, 'scald_tags');
  // foreach ($infos->tags as $index => $tag) {
  //   $atom->scald_tags[$langcode][$index] = array(
  //     'tid' => $index,  // Beware, this is not a real tid, it's just an index
  //     'taxonomy_term' => (object)(array('name' => $tag))
  //   );
  // }

  // Save video width and height
  if (!isset($atom->data)) {
    $atom->data = array();
  }

  $atom->data['video_key'] = $key;
  $atom->data['video_liveid'] = $liveid;
  $atom->data['video_domain'] = $domain;


}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_videortl_scald_fetch($atom, $type) {
  $items = field_get_items('scald_atom', $atom, 'scald_thumbnail');
  if ($items && file_exists($items[0]['uri'])) {
    $atom->file_source = $atom->thumbnail_source = $items[0]['uri'];
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_videortl_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_videortl_player',
      array('vars' =>
        array(
          'video_id'      => $atom->base_id,
          'video_key'   => $atom->data['video_key'],
          'video_liveid'  => $atom->data['video_liveid'],
          'video_domain'     => $atom->data['video_domain'],
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_videortl_theme() {
  return array(
    'dpiscald_videortl_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/videortl/dpiscald_videortl_player'
    ),
  );
}

/**
 * Get media infos from url.
 */
function dpiscald_videortl_get_media_infos_from_url($url, $id_elem = NULL) {
  $infos = array('title'=>'coucou');
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
function dpiscald_videortl_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_videortl_validate_id($element, &$form_state) {
  $url = $form_state['values']['identifier'];
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
  // if (startsWith($url,'https')){
  //   $url = explode('https://play.spotify.com/',$url);
  // } else {
  //   $url = explode('http://play.spotify.com/',$url);
  // }
  // $url = $url[1];
  $id = md5($url);
  if (dpiscald_videortl_already_imported($id)) {
    form_error($element, t('Media already imported.'));
  }
}

/**
 * Parse an video rtl ID and check validity
 *
 */
function dpiscald_videortl_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);

  if (preg_match('/videoid=([0-9]*)/i', $string,$urlT)) {
  	$id = $urlT[1];
  }

  return $id;
}

/**
 * Parse an video rtl Key and check validity
 *
 */
function dpiscald_videortl_parse_key($string, $check) {
  $key = NULL;
  $string = trim($string);

  if (preg_match('/key=([0-9a-z-]*)/i', $string,$urlT)) {
  	$key = $urlT[1];
  }

  return $key;
}

/**
 * Parse an video rtl Live ID and check validity
 *
 */
function dpiscald_videortl_parse_liveid($string, $check) {
  $liveid = NULL;
  $string = trim($string);

  if (preg_match('/LiveID=([0-9a-z-]*)/i', $string,$urlT)) {
  	$liveid = $urlT[1];
  }

  return $liveid;
}

/**
 * Parse an video rtl domain and check validity
 *
 */
function dpiscald_videortl_parse_domain($string, $check) {
  $domain = NULL;
  $string = trim($string);

  if (preg_match('/http\:\/\/www\.rtl\.be\/([0-9a-z-]*)\//', $string,$urlT)) {
  	$domain = $urlT[1];
  }

  return $domain;
}