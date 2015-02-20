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
    '#type' => 'textfield',
    '#title' => t('Facebook status'),
    '#element_validate' => array('dpiscald_facebook_status_validate_id'),
    '#description' => t('Paste here the link post or markup of the Facebook status :<ul><li>At the top right of a status, you can select an option to embed the status on a web page.</li><li>Copy and paste the given code here.</li><li>see : https://developers.facebook.com/blog/post/2013/07/31/introducing-embedded-posts/</li></ul>'),
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
  } else {
    // Default thumbnail
    $uri = 'public://atoms/facebook_status.png';
    $files = file_load_multiple(array(), array('uri' => $uri));
    $file = reset($files);
    if (isset($file)) {
      file_usage_add($file, 'dpiscald', 'facebook_status', $atom->sid);
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
      'template' => 'templates/players/dpiscald_facebook_status_player'
    ),
    'dpiscald_facebook_status_player_html5' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'templates/players/dpiscald_facebook_status_player_html5'
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


  if (substr($identifier, 0, 4) == "http") {
    $regexEndChar = "$";
  } else {
    $regexEndChar = "\"";
  }

  // get the data-href
  preg_match("/data-href=\"(.*)\"/siU", $identifier, $match);
  if (isset($match[1])) {
    $values->data_href = $match[1];
  }
  else {
    $values->data_href = $identifier;
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
  preg_match("/www\.facebook\.com\/permalink\.php\?story_fbid=(.*)\&id=(.*)".$regexEndChar."/siU", $identifier, $match);
  if (isset($match[0])) {
    $values->base_id = $match[1];
    $values->user_id = $match[2];
  }
  else {
    // check if this is a photo
    preg_match("/www\.facebook\.com\/photo\.php\?fbid=(.*)\&set=(.*)\&type=/siU", $identifier, $match);
    if (isset($match[0])) {
      // get the values
      $data = explode('.', $match[2]);
      $values->base_id = $match[1];
      $values->user_id = $data[3];
    }
    else {
      // get something else like a page post (i don't have any video example to play with)
      preg_match("/www\.facebook\.com\/(.*)\/posts\/(.*)".$regexEndChar."/siU", $identifier, $match);
      if (isset($match[0])) {
        $values->base_id = $match[2];
        $values->user_id = $match[1];
      }
    }
  }
  if (!$values->name) {
    $values->name = $values->user_id;
  }
  $values->title = $values->name. '-'. $values->base_id;
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
