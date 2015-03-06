<?php
/**
 * @file
 *   Defines a Vimeo provider for Scald.
 */
if (! (defined ( 'VIMEO_ID_REGEXP' ) && defined ( 'VIMEO_WEB_REGEXP' ) && defined ( 'VIMEO_API_VIDEO' ) && defined ( 'VIMEO_API' ))) {
  define ( 'VIMEO_ID_REGEXP', '/^[a-zA-Z0-9]+$/' );
  define ( 'VIMEO_WEB_REGEXP', '#^http://vimeo.com/([a-zA-Z0-9]+)(_.*)?$#' );
  define ( 'VIMEO_API_VIDEO', 'http://vimeo.com/api/v2/video/' );
  define ( 'VIMEO_API', 'http://vimeo.com/api/v2/' );
}

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_vimeo_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo video identifier or URL'),
    '#element_validate' => array('dpiscald_vimeo_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_vimeo_scald_add_form_fill(&$atom, $form, $form_state) {

  // Get the identifier
  $identifier = dpiscald_vimeo_parse_id($form_state['values']['identifier'], TRUE);

  // Get video info
  $infos = dpiscald_vimeo_video($identifier);

  $atom->base_id = $identifier;
  $atom->title = $infos->title;

  $atom->provider = 'dpiscald';
  $atom->type = 'vimeo';

  // Prefill the author
  $langcode = field_language('scald_atom', $atom, 'scald_authors');
  $atom->scald_authors[$langcode][0] = array(
    'tid' => 0,
    'taxonomy_term' => (object)(array('name' => $infos->author))
  );

  // Prefill tags
  $langcode = field_language('scald_atom', $atom, 'scald_tags');
  foreach ($infos->tags as $index => $tag) {
    $atom->scald_tags[$langcode][$index] = array(
      'tid' => $index,  // Beware, this is not a real tid, it's just an index
      'taxonomy_term' => (object)(array('name' => $tag))
    );
  }

  $atom->data['video_width'] = $infos->width;
  $atom->data['video_height'] = $infos->height;

  // Download a copy of the video thumbnail. This makes it possible
  // to do interesting manipulation with image styles presets.
  $thumb = drupal_http_request($infos->thumbnail['src']);
  if ($thumb->code == 200 && $directory = scald_atom_thumbnail_path('video')) {
    $dest = $directory . '/vimeo-' . $infos->id . '.jpg';
    $file = file_save_data($thumb->data, $dest);

    if ($file) {
      // Set the file status to temporary
      $query = db_update('file_managed')
        ->condition('fid', $file->fid)
        ->fields(array('status' => 0))
        ->execute();
      $langcode = field_language('scald_atom', $atom, 'scald_thumbnail');
      $atom->scald_thumbnail[$langcode][0] = (array) $file;
    }
  }
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_vimeo_scald_fetch($atom, $type) {
  $items = field_get_items('scald_atom', $atom, 'scald_thumbnail');
  if ($items && file_exists($items[0]['uri'])) {
    $atom->file_source = $atom->thumbnail_source = $items[0]['uri'];
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_vimeo_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_vimeo_player',
      array('vars' =>
        array(
          'video_id'      => $atom->base_id,
          'video_width'   => ($atom->data['video_width'] ? $atom->data['video_width'] : 480),
          'video_height'  => ($atom->data['video_height'] ? $atom->data['video_height'] : 365),
          'thumbnail'     => $atom->thumbnail_source
        ),
        'atom' => $atom,
      )
    );
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_vimeo_theme() {
  return array(
    'dpiscald_vimeo_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/vimeo/dpiscald_vimeo_player'
    ),
    'dpiscald_vimeo_player_html5' => array(
      'variables' => array( 'video_id' => '', 'html_id' => 'player', 'width' => 640, 'height' => 390, 'origin' => ''),
      'template' => 'templates/players/dpiscald_vimeo_player_html5'
    ),
  );
}


/**
 * Creates an atom based on a Vimeo video id or an object
 * containing the video informations..
 * @param $video
 *   Unique identifier of the video on Vimeo, or object
 *   returned by dpiscald_vimeo_video.
 * @return integer
 *   Unique identifier of the new atom
 */
function dpiscald_vimeo_register($video) {
  global $user;
  // Fetch the needed informations from Vimeo
  if (is_object($video)) {
    $infos = $video;
  }
  else {
    $infos = dpiscald_vimeo_video($video);
  }

  // Check if the video has already been imported to prevent duplicate
  $old = dpiscald_vimeo_already_imported($infos->id);
  if ($old) {
    return FALSE;
  }

  // Download a copy of the video thumbnail. This makes it possible
  // to do interesting manipulation with image styles presets.
  $thumb = drupal_http_request($infos->thumbnail['src']);
  $dir = 'public://vimeo';
  if ($thumb->code == 200 && file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    $dest = $dir . '/' . $infos->id . '.jpg';
    $file = file_save_data($thumb->data, $dest);
  }

  // Create an atom
  $atom = new ScaldAtom('vimeo', 'dpiscald', array(
    'base_id' => $infos->id,
    'title' => $infos->title,
  ));

  // And save it
  $atom_sid = scald_atom_save($atom);
  // Finally, return this id
  return $atom_sid;
}


/**
 * Analyze a Vimeo RSS feed to extract videos information
 * @param $type
 *   RSS Type of the feed. Example, 'user', 'video', 'tag'.
 * @param $id
 *   Identifier or string associated with the type. For example, user name, video id or tag
 * @return array
 *   An array of object, each one containing an analyzed video.
 */
function dpiscald_vimeo_feed($type, $id) {
  $items = array();
  switch ($type) {
    case 'user' :
      $url = VIMEO_API . $id . '/videos.php';
      $feed = drupal_http_request($url);
      if ($feed->code != 404 && !empty($feed->data)) {
        $data = unserialize($feed->data);
        if (count($data) > 0) {
          foreach ($data as $video) {
            $items[] = dpiscald_vimeo_parse_item($video);
          }
        }
      }
      break;
    case 'video' :
      $url = VIMEO_API_VIDEO . $id . '.php';
      $feed = drupal_http_request($url);

      if ($feed->code != 404 && !empty($feed->data)) {
        $data = unserialize($feed->data);
        if (count($data) > 0) {
          foreach ($data as $video) {
            $items[] = dpiscald_vimeo_parse_item($video);
          }
        }
      }
      break;
  }

  return $items;
}

/**
 * Analyze the Vimeo feed for a specific video.
 */
function dpiscald_vimeo_video($id) {
  $items = dpiscald_vimeo_feed('video', $id);
  return $items[0];
}

/**
 * Analyze a XML DOMElement item to extract interesting info.
 * @param $item
 *   An object of classe DOMElement corresponding to an element of the feed.
 * @return array
 *   An abject containing various video informations.
 */
function dpiscald_vimeo_parse_item($item) {

  $info = new stdClass();

  // Fetch the video ID
  $info->id = $item['id'];

  // Video Title
  $info->title = $item['title'];

  // Video dimensions
  $info->width = $item['width'];
  $info->height = $item['height'];

  // Thumbnail data
  $info->thumbnail = array(
    'src'    => $item['thumbnail_large'],
    'width'  => $item['width'],
    'height' => $item['height'],
  );

  // Video tags
  $info->tags = explode(', ' , $item['tags']);

  // Author
  $info->author = $item['user_name'];

  // Creation Date
  $info->pubDate = $item['upload_date'];

  // License
  $info->license = 'VM';
  return $info;
}



/**
 * Checks if a video has already been imported, based on its video
 * id.
 * @param $id
 *   The video identifier
 * @return mixed
 *   FALSE if the video was never imported, the scald identifier of
 *   the video otherwise.
 */
function dpiscald_vimeo_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for Vimeo identifier
 *
 */
function dpiscald_vimeo_validate_id($element, &$form_state) {
  $id = dpiscald_vimeo_parse_id($form_state['values']['identifier'], TRUE);
  if (!$id) {
    form_error($element, t('Invalid Vimeo video identifier.'));
  }
  elseif (dpiscald_vimeo_already_imported($id)) {
    form_error($element, t('Vimeo video already imported.'));
  }
}

/**
 * Parse a Vimeo ID and check validity
 *
 */
function dpiscald_vimeo_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match(VIMEO_ID_REGEXP, $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    if (preg_match(VIMEO_WEB_REGEXP, $string, $m)) {
      // This string is a full Vimeo URL
      $id = $m[1];
    }
    elseif (preg_match("/^http/", $string)) {
      // This string is a URL, most likely a shortened one. (http://dai.ly, http://bit.ly, etc...)
      $response = drupal_http_request($string);
      if ($response->code == 200 && isset($response->redirect_code) && ($response->redirect_code == 301 || $response->redirect_code == 302)) {
        return dpiscald_vimeo_parse_id($response->redirect_url, $check);
      }
    }
  }
  else {
    $id = $string;
  }

  if ($id && $check) {
    // Last check to confirm this video really exists on Vimeo
    if (!dpiscald_vimeo_video($id)) {
      $id = FALSE;
    }
  }
  return $id;
}

function dpiscald_vimeo_get_import_info($importer, & $entity, & $wrapper, $url){
  try {
    $value = str_replace ( '\\', '', $url );
    $id = dpiscald_vimeo_parse_id ( $value, FALSE );
    $wrapper->base_id->set ( md5($id) );
    $info = dpiscald_vimeo_video ( $id );
    if (!empty($info->thumbnail ['src'])) {
      $file = $importer->createFile ( $info->thumbnail ['src'], scald_atom_thumbnail_path ( $entity->type ) );
      $wrapper->scald_thumbnail->set ( ( array ) $file );
    }
    $entity->data['video_width'] = $info->width;
    $entity->data['video_height'] = $info->height;
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Vimeo : " . $url);
  }
}