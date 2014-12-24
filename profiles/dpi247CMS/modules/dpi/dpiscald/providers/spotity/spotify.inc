<?php
/**
 * @file
 *   Defines a Spotify provider for Scald.
 */
if (! (defined ( 'SPOTIFY_WEB' ) && defined ( 'SPOTIFY_OEMBED' ) && defined ( 'SPOTIFY_ID_REGEXP' ) &&
    defined ( 'SPOTIFY_WEB_REGEXP' ) && defined ( 'SPOTIFY_RSS' ) && defined ( 'SPOTIFY_MEDIA' ))) {
  define('SPOTIFY_WEB',         'http://www.youtube.com/watch');
  define('SPOTIFY_OEMBED',      'http://www.youtube.com/oembed');
  define('SPOTIFY_ID_REGEXP',   '/^[a-zA-Z0-9\-_]+$/');
  define('SPOTIFY_WEB_REGEXP',  '#https?://www\.youtube\.com/\S+[\?&]v=([a-zA-Z0-9\-_]+)#');
  define('SPOTIFY_RSS',         'http://gdata.youtube.com/feeds/api/videos/');
  define('SPOTIFY_MEDIA',       'http://search.yahoo.com/mrss/');
}

/**
 * Implements hook_scald_atom_providers().
 * Tell Scald that we'll be providing some video atoms.
 */
function dpiscald_spotify_scald_atom_providers() {
  return array(
    'video' => 'Video hosted on Spotify'
  );
  // This code will never be hit, but is necessary to mark the string
  // for translation on localize.d.o
  t('Video hosted on Spotify');
}

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_spotify_scald_add_form(&$form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Spotify video identifier or URL'),
    '#element_validate' => array('dpiscald_spotify_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_spotify_scald_add_form_fill(&$atom, $form, $form_state) {

  // Get the identifier
  $identifier = dpiscald_spotify_parse_id($form_state['values']['identifier'], TRUE);

  // Get video info
  $infos = dpiscald_spotify_video($identifier);

  $atom->base_id = $identifier;
  $atom->title = $infos->title;

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

  // Save video width and height
  if (!isset($atom->data)) {
    $atom->data = array();
  }
  $atom->data['video_width'] = $infos->width;
  $atom->data['video_height'] = $infos->height;

  // Download a copy of the video thumbnail. This makes it possible
  // to do interesting manipulation with image styles presets.
  $thumb = drupal_http_request($infos->thumbnail['src']);
  $dir = 'public://youtube';
  if ($thumb->code == 200 && file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    $dest = $dir . '/youtube-' . $infos->id . '.jpg';
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
function dpiscald_spotify_scald_fetch($atom, $type) {
  $items = field_get_items('scald_atom', $atom, 'scald_thumbnail');
  if ($items && file_exists($items[0]['uri'])) {
    $atom->file_source = $atom->thumbnail_source = $items[0]['uri'];
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_spotify_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_spotify_player',
      array('vars' =>
        array(
          'video_id'      => $atom->base_id,
          'video_width'   => ($atom->data['video_width'] ? $atom->data['video_width'] : 480),
          'video_height'  => ($atom->data['video_height'] ? $atom->data['video_height'] : 365),
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
function dpiscald_spotify_theme() {
  return array(
    'dpiscald_spotify_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/youtube/dpiscald_spotify_player'
    ),
  );
}


/**
 * Creates an atom based on a Spotify video id or an object
 * containing the video informations..
 * @param $video
 *   Unique identifier of the video on Spotify, or object
 *   returned by dpiscald_spotify_video.
 * @return integer
 *   Unique identifier of the new atom
 */
function dpiscald_spotify_register($video) {
  // Fetch the needed informations from Spotify
  if (is_object($video)) {
    $infos = $video;
  }
  else {
    $infos = dpiscald_spotify_video($video);
  }

  // Check if the video has already been imported to prevent duplicate
  $old = dpiscald_spotify_already_imported($infos->id);
  if ($old) {
    return FALSE;
  }

  // Download a copy of the video thumbnail. This makes it possible
  // to do interesting manipulation with image styles presets.
  $thumb = drupal_http_request($infos->thumbnail['src']);
  $dir = 'public://youtube';
  if ($thumb->code == 200 && file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    $dest = $dir . '/' . $infos->id . '.jpg';
    $file = file_save_data($thumb->data, $dest);
  }

  // Create an atom
  $atom = new ScaldAtom('youtube', 'dpiscald', array(
    'base_id' => $infos->id,
    'title' => $infos->title,
  ));

  // And save it
  $atom_sid = scald_atom_save($atom);
  // Finally, return this id
  return $atom_sid;
}


/**
 * Analyze a Spotify RSS feed to extract videos information
 * @param $type
 *   RSS Type of the feed. Example, 'user', 'video', 'tag'.
 * @param $id
 *   Identifier or string associated with the type. For example, user name, video id or tag
 * @return array
 *   An array of object, each one containing an analyzed video.
 */
function dpiscald_spotify_feed($type, $id) {
  if ($type == 'user') {
    $url = SPOTIFY_RSS . '?author=' . $id;
  }
  elseif ($type == 'tag') {
    $url = SPOTIFY_RSS . '?category=' . $id;
  }
  else {
    $url = SPOTIFY_RSS . $id;
  }
  $response = drupal_http_request($url);
  $items = array();
  if ($response->code >= 200 && $response->code < 400 && !empty($response->data)) {
    $dom = new DOMDocument();
    if ($dom->loadXML($response->data)) {
      foreach ($dom->getElementsByTagName('entry') as $item) {
        $items[] = dpiscald_spotify_parse_item($item);
      }
    }
  }
  return $items;
}

/**
 * Analyze the youtube feed for a specific video.
 */
function dpiscald_spotify_video($id) {
  $items = dpiscald_spotify_feed('video', $id);
  return $items[0];
}

/**
 * Analyze a XML DOMElement item to extract interesting info.
 * @param $item
 *   An object of classe DOMElement corresponding to an element of the feed.
 * @return array
 *   An abject containing various video informations.
 */
function dpiscald_spotify_parse_item($item) {
  $info = new stdClass();

  // Fetch the video ID
  $api_url_full = $item->getElementsByTagName('id')->item(0)->nodeValue;
  $api_url_parts = explode('/', $api_url_full);
  $info->id = array_pop($api_url_parts);

  // Video Title
  $title = $item->getElementsByTagName('title')->item(0);
  $info->title = $title->nodeValue;

  // Video dimensions
  // Unfortunately the Spotify API doesn't provide original video dimensions
  // see : http://code.google.com/p/gdata-issues/issues/detail?id=1083
  $info->width = 480;
  $info->height = 365;
  // Now we try to get the dimensions from the OEmbed API instead
  if ($oembed = dpiscald_spotify_video_oembed($info->id)) {
    $info->width = ($oembed->width ? $oembed->width : $info->width);
    $info->height = ($oembed->height ? $oembed->height : $info->height);
  }

  // Thumbnail data
  $thumbs = $item->getElementsByTagNameNS(SPOTIFY_MEDIA, 'thumbnail');
  if ($thumbs->length) {
    $thumb = $thumbs->item(0);
    $info->thumbnail = array(
      'src'    => $thumb->getAttribute('url'),
      'width'  => $thumb->getAttribute('width'),
      'height' => $thumb->getAttribute('height')
    );
  }

  // Video tags
  $tags = $item->getElementsByTagNameNS(SPOTIFY_MEDIA, 'keywords')->item(0)->nodeValue;
  $info->tags = explode(', ', $tags);

  // Author
  $info->author = $item->getElementsByTagName('name')->item(0)->nodeValue;

  // Creation Date
  $info->pubDate = $item->getElementsByTagName('published')->item(0)->nodeValue;

  // License
  $info->license = 'YT';
  return $info;
}


/**
 * Analyze OEmbed response for a given video ID
 * Note: OEmbed does not include tags but does include video dimensions
 */
function dpiscald_spotify_video_oembed($id) {
  $info = FALSE;
  // Build the full OEmbed URL
  $url = SPOTIFY_OEMBED . '?url=' . SPOTIFY_WEB . '?v=' . $id;
  $response = drupal_http_request($url);
  if ($response->code >= 200 && $response->code < 400 && !empty($response->data)) {
    $data = json_decode($response->data);
    $info = new stdClass();
    // ID
    $info->id = $id;
    // Title
    $info->title = $data->title;
    // Author
    $info->author = $data->author_name;
    // Video dimensions
    $info->width = $data->width;
    $info->height = $data->height;
    // Thumbnail
    if ($data->thumbnail_url) {
      $info->thumbnail = array(
        'src'    => $data->thumbnail_url,
        'width'  => $data->thumbnail_width,
        'height' => $data->thumbnail_height,
      );
    }
    // License
    $info->license = 'YT';
  }
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
function dpiscald_spotify_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for Spotify identifier
 *
 */
function dpiscald_spotify_validate_id($element, &$form_state) {
  $id = dpiscald_spotify_parse_id($form_state['values']['identifier'], TRUE);
  if (!$id) {
    form_error($element, t('Invalid Spotify video identifier.'));
  }
  elseif (dpiscald_spotify_already_imported($id)) {
    form_error($element, t('Spotify video already imported.'));
  }
}

/**
 * Parse a Spotify ID and check validity
 *
 */
function dpiscald_spotify_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match(SPOTIFY_ID_REGEXP, $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    if (preg_match(SPOTIFY_WEB_REGEXP, $string, $m)) {
      // This string is a full Spotify URL
      $id = $m[1];
    }
    elseif (preg_match("/^http/", $string)) {
      // This string is a URL, most likely a shortened one. (http://dai.ly, http://bit.ly, etc...)
      $response = drupal_http_request($string);
      if ($response->code == 200 && isset($response->redirect_code) && ($response->redirect_code == 301 || $response->redirect_code == 302)) {
        return dpiscald_spotify_parse_id($response->redirect_url, $check);
      }
    }
  }
  else {
    $id = $string;
  }

  if ($id && $check) {
    // Last check to confirm this video really exists on Spotify
    if (!dpiscald_spotify_video($id)) {
      $id = FALSE;
    }
  }
  return $id;
}

