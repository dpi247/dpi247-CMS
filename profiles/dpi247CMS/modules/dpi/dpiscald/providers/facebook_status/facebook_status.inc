<?php

/**
 * @file
 * Facebook status include
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_facebook_status_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textarea',
    '#rows' => '3',
    '#title' => t('Facebook status markup'),
    '#element_validate' => array('dpiscald_facebook_status_validate_id'),
    '#description' => t('Paste here the markup of the Facebook status :<ul><li>At the top right of a status, you can select an option to embed the status on a web page.</li><li>Copy and paste the given code here.</li><li>see : https://developers.facebook.com/blog/post/2013/07/31/introducing-embedded-posts/</li></ul>'),
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_facebook_status_scald_add_form_fill(&$atoms, $form, $form_state) {
  $atom = is_array($atoms) ? reset($atoms) : $atoms;

  // parse the values
  $values = $values = dpiscald_facebook_status_get_media_infos_from_url($form_state['values']['identifier']);
  // fill the values
  $atom->title = $values->title;
  $atom->base_id = $values->base_id;
  $atom->data['user_id'] = $values->user_id;
  $atom->data['data_href'] = $values->data_href;
  $atom->data['country'] = $values->country;

  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
    'tid' => 0,
    'taxonomy_term' => (object)(array('name' => $values->name))
  );
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_facebook_status_scald_fetch($atom, $type) {
  $file = $atom->scald_thumbnail[LANGUAGE_NONE][0]['uri'];
  if (file_exists($file)) {
    $atom->file_source = $atom->thumbnail_source = $file;
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_facebook_status_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_facebook_status',
      array('vars' =>
        array(
          'title'       => $atom->title,
          'base_id'     => $atom->base_id,
          'user_id'     => $atom->data['user_id'],
          'data_href'   => $atom->data['data_href'],
          'country'     => $atom->data['country'],
          'thumbnail'   => $atom->thumbnail_source
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_facebook_status_theme() {
  return array(
    'dpiscald_facebook_status' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/facebook_status/dpiscald_facebook_status_player'
    ),
  );
}


/**
 * Get media infos from url.
 */
function dpiscald_facebook_status_get_media_infos_from_url($identifier) {
  // pre format
  $values = new stdclass;
  $values->title = FALSE;
  $values->base_id = FALSE;
  $values->user_id = FALSE;
  $values->name = FALSE;
  $values->country = 'en_US';

  // get the data-href
  preg_match("/data-href=\"(.*)\"/siU", $identifier, $match);
  if (isset($match[1])) {
    $values->data_href = $match[1];
  }
  else {
    return $values;
  }

  // get the title
  preg_match_all("/<a\s[^>]*href=(\"??)([^\" >]*?)\\1[^>]*>(.*)<\/a>/siU", $identifier, $matches);
  if (isset($matches[3][1])) {
    $values->name = $matches[3][1];
  }

  // get the country
  preg_match("/connect\.facebook\.net\/(.*)\/all\.js/siU", $identifier, $match);
  if (isset($match[1])) {
    $values->country = $match[1];
  }

  // check if this is a permalink
  preg_match("/www\.facebook\.com\/permalink\.php\?story_fbid=(.*)\&amp\;id=(.*)\"/siU", $identifier, $match);
  if (isset($match[0])) {
    $values->base_id = $match[1];
    $values->user_id = $match[2];
  }
  else {
    // check if this is a photo
    preg_match("/www\.facebook\.com\/photo\.php\?fbid=(.*)\&amp\;set=(.*)\&amp\;type=/siU", $identifier, $match);
    if (isset($match[0])) {
      // get the values
      $data = explode('.', $match[2]);
      $values->base_id = $match[1];
      $values->user_id = $data[3];
    }
    else {
      // get something else like a page post (i don't have any video example to play with)
      preg_match("/www\.facebook\.com\/(.*)\/posts\/(.*)\"/siU", $identifier, $match);
      if (isset($match[0])) {
        $values->base_id = $match[2];
        $values->user_id = $match[1];
      }
    }
  }
  if ($values->name) {
    $values->title = $values->name. '-'. $values->base_id;
  }
  return $values;
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
function dpiscald_facebook_status_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for status identifier
 */
function dpiscald_facebook_status_validate_id($element, &$form_state) {
  $values = dpiscald_facebook_status_get_media_infos_from_url($form_state['values']['identifier']);
  dsm($values);
  if (!$values->base_id) {
    form_error($element, t('Invalid status ID or URL.'));
  }
  elseif (dpiscald_facebook_status_already_imported($values->base_id)) {
    form_error($element, t('Status has already been imported.'));
  }
}


/**
 * Parse an facebook status ID and check validity
 *
 */
function dpiscald_facebook_status_parse_id($string, $check) {
  $values = dpiscald_facebook_status_get_media_infos_from_url($string);
  $id = NULL;
  if (isset($values->base_id)) {
    $id = $values->base_id;
  }
  return $id;
}
