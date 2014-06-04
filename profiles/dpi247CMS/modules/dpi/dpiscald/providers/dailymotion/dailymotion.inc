<?php
/**
 * @file
 *   Defines a Dailymotion provider for Scald.
 */
if (! (defined ( 'DAILYMOTION_RSS' ) && defined ( 'DAILYMOTION_WEB' ) && defined ( 'DAILYMOTION_OEMBED' ) &&
     defined ( 'NS_MEDIA' ) && defined ( 'NS_DM' ))) {
  define ( 'DAILYMOTION_RSS', 'http://www.dailymotion.com/rss/' );
  define ( 'DAILYMOTION_WEB', 'http://www.dailymotion.com/video/' );
  define ( 'DAILYMOTION_WEBS', 'https://www.dailymotion.com/video/' );
  define ( 'DAILYMOTION_OEMBED', 'http://www.dailymotion.com/services/oembed' );
  define ( 'NS_MEDIA', 'http://search.yahoo.com/mrss' );
  define ( 'NS_DM', 'http://www.dailymotion.com/dmrss' );
}

/**
 * Implements hook_scald_atom_providers().
 * Tell Scald that we'll be providing some video atoms.
 */
function dpiscald_dailymotion_scald_atom_providers() {
  return array(
    'video' => 'Video hosted on Dailymotion'
  );
  // This code will never be hit, but is necessary to mark the string
  // for translation on localize.d.o
  t('Video hosted on Dailymotion');
}

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_dailymotion_scald_add_form(&$form, &$form_state) {
  if (isset($_GET['dm-id'])) {
    $identifier = $_GET['dm-id'];
  }
  else {
    $identifier = '';
  }

  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Dailymotion video identifier or URL'),
    '#element_validate' => array('dpiscald_dailymotion_validate_id'),
    '#default_value' => $identifier,
  );

  if (!$identifier) {
    $attributes = array();
    if (!empty($form_state['ajax'])) {
      $attributes['target'] = '_blank';
      $attributes['class'][] = 'overlay-exclude';
    }
    $form['search'] = array(
      '#type' => 'item',
      '#markup' => l(t('Search on Dailymotion'), 'atoms/dailymotion/search', array('attributes' => $attributes)),
    );
  }
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_dailymotion_scald_add_form_fill(&$atom, $form, $form_state) {
  // Get the DM identifier
  $identifier = dpiscald_dailymotion_parse_id($form_state['values']['identifier'], TRUE);

  // Get video info
  $infos = dpiscald_dailymotion_video($identifier);

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
  if ($thumb->code == 200 && $directory = scald_atom_thumbnail_path('video')) {
    $dest = $directory . '/dailymotion-' . $infos->id . '.jpg';
    $file = file_save_data($thumb->data, $dest);

    // Set the file status to temporary
    $query = db_update('file_managed')
      ->condition('fid', $file->fid)
      ->fields(array('status' => 0))
      ->execute();
    $langcode = field_language('scald_atom', $atom, 'scald_thumbnail');
    $atom->scald_thumbnail[$langcode][0] = (array)$file;
  }
}

/**
 * Implements hook_scald_fetch().
 */
function dpiscald_dailymotion_scald_fetch($atom, $type) {
  $items = field_get_items('scald_atom', $atom, 'scald_thumbnail');
  if ($items && file_exists($items[0]['uri'])) {
    $atom->file_source = $atom->thumbnail_source = $items[0]['uri'];
  }
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_dailymotion_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    $atom->rendered->player = theme('dpiscald_dailymotion_player',
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
function dpiscald_dailymotion_theme() {
  return array(
    'dpiscald_dailymotion_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/dailymotion/dpiscald_dailymotion_player'
    ),
    'dpiscald_dailymotion_imports_table' => array(
      'render element' => 'form',
      'file' => 'providers/dailymotion/dpiscald_dailymotion.admin.inc'
    ),
    'dpiscald_dailymotion_search_results_table' => array(
      'render element' => 'form',
      'file' => 'providers/dailymotion/dpiscald_dailymotion.pages.inc'
    )
  );
}

/**
 * Implements hook_permission().
 */
function dpiscald_dailymotion_permission() {
  return array(
    'administer dailymotion imports' => array(
      'title' => t('Administer DailyMotion imports'),
      'description' => t('Add or remove DailyMotion feeds of videos that should be imported automatically.'),
    ),
    'import dailymotion videos' => array(
      'title' => t('Import DailyMotion videos'),
    ),
  );
}

/**
 * Implements hook_cron().
 */
function dpiscald_dailymotion_cron() {
  $imports = variable_get('dpiscald_dailymotion_imports', array());
  foreach ($imports as $import) {
    $items = dpiscald_dailymotion_feed($import['type'], $import['value']);
    foreach ($items as $item) {
      dpiscald_dailymotion_direct_import($item);
    }
  }
}

/**
 * Implements hook_menu(). (not anymore)
 */
function dpiscald_dailymotion_menu(&$items) {
  $items['admin/config/media/dpiscald_dailymotion'] = array(
    'title' => 'Dailymotion imports',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('dpiscald_dailymotion_imports_form'),
    'access arguments' => array('administer dailymotion imports'),
    'description' => 'Configure the videos that should be automatically imported from Dailymotion',
    'file' => 'providers/dailymotion/dpiscald_dailymotion.admin.inc'
  );

  $items['atoms/dailymotion/search'] = array(
    'title' => 'Dailymotion Video search',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('dpiscald_dailymotion_search_form', 2),
    'access arguments' => array('import dailymotion videos'),
    'description' => 'Search for new videos to import into this site',
    'file' => 'providers/dailymotion/dpiscald_dailymotion.pages.inc',
    'type' => MENU_CALLBACK
  );

  $items['atoms/dailymotion/import'] = array(
    'title' => 'Dailymotion Video import',
    'page callback' => 'dpiscald_dailymotion_import',
    'page arguments' => array(),
    'access arguments' => array('import dailymotion videos'),
    'description' => 'Import a video',
    'type' => MENU_CALLBACK
  );
}

/**
 * Automatically creates an atom based on a Dailymotion id or
 * an object containing the video informations.
 *
 * This function is used by the CRON importer and does not require user intervention
 *
 * @param $video
 *   Unique identifier of the video on dailymotion, or object
 *   returned by dpiscald_dailymotion_video.
 * @return integer
 *   Unique identifier of the new atom
 */
function dpiscald_dailymotion_direct_import($video) {
  // Fetch the needed informations from Dailymotion
  if (is_object($video)) {
    $infos = $video;
  }
  else {
    $infos = dpiscald_dailymotion_video($video);
  }

  // Check if the video has already been imported to prevent duplicate
  $old = dpiscald_dailymotion_already_imported($infos->id);
  if ($old) {
    return FALSE;
  }

  // Create an atom
  $atom = new ScaldAtom('video', 'dpiscald', array(
    'base_id' => $infos->id,
    'title' => $infos->title,
  ));

  // Create and link author term
  if ($term = dpiscald_dailymotion_taxonomy_get_create($infos->author, variable_get('scald_author_vocabulary', 0))) {
    $langcode = field_language('scald_atom', $atom, 'scald_authors');
    $atom->scald_authors[$langcode][0] = array(
      'tid' => $term->tid,
    );
  }

  // Create and link tags terms
  $langcode = field_language('scald_atom', $atom, 'scald_tags');
  foreach ($infos->tags as $index => $tag) {
    if ($term = dpiscald_dailymotion_taxonomy_get_create($tag, variable_get('scald_tags_vocabulary', 0))) {
      $atom->scald_tags[$langcode][$index] = array(
        'tid' => $term->tid,
      );
    }
  }

  // Save video width and height
  $atom->data['video_width'] = $infos->width;
  $atom->data['video_height'] = $infos->height;

  // Download a copy of the video thumbnail. This makes it possible
  // to do interesting manipulation with image styles presets.
  $thumb = drupal_http_request($infos->thumbnail['src']);
  $dir = 'public://dailymotion';
  if ($thumb->code == 200 && file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    $dest = $dir . '/' . $infos->id . '.jpg';
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

  // And save it
  $atom_sid = scald_atom_save($atom);

  // Finally, return this id
  return $atom_sid;
}


/**
 * Get a taxonomy term by name from a given vocabulary.
 * Create the term if no matches found.
 *
 * @param $name
 *   The name or the taxonomy term.
 * @param $vid
 *   Vocabulary ID to search and create from/to.
 * @return object
 *   The taxonomy term.
 */
function dpiscald_dailymotion_taxonomy_get_create($name, $vid) {
  // remove extra white spaces
  $name = trim($name);

  // find existing term(s)
  $terms = taxonomy_term_load_multiple(
    array(),
    array(
      'name' => $name,
      'vid' =>  $vid,
    )
  );

  if (!count($terms)) {
    // create a new term
    $term = new stdClass();
    $term->name = trim($name);
    $term->vid = $vid;
    taxonomy_term_save($term);
  }
  else {
    // get the first found term
    $term = reset($terms);
  }
  return $term;
}

/**
 * Analyze a Dailymotion RSS feed, reformating the informations about its
 * items in an easy to manipulate objects containing the informations we're
 * interested in.
 * @param $type
 *   Dailymotion RSS feed type. Examples include 'user', 'video', 'tag', 'search', ...
 * @param $id
 *   The identifier, related to the type mentionned above. If you're requestion
 *   a user feed, then, its the username...
 * @return array
 *   Each array value is an object containing the following members:
 *     - id: the Dailymotion video id
 *     - author: the Dailymotion username of the user who uploaded the video
 *     - title: the video title
 *     - thumbnail: an associative array, containing the source ('src'), width
 *       and height of the video's thumbnail
 *     - pubDate: the publication date of the video
 */
function dpiscald_dailymotion_feed($type, $id) {
  $url = DAILYMOTION_RSS . $type . '/' . urlencode($id);
  $response = drupal_http_request($url);
  $items = array();
  if ($response->code != 404 && !empty($response->data)) {
    $dom = new DOMDocument();
    if ($dom->loadXML($response->data)) {
      foreach ($dom->getElementsByTagName('item') as $item) {
        $info = new stdClass();
        // Fetch from the feed
        // ... the video id
        $info->id = $item->getElementsByTagNameNS(NS_DM, 'id')->item(0)->nodeValue;
        // ... its title
        $title = $item->getElementsByTagName('title')->item(0);
        $info->title = $title->nodeValue;
        // Video size
        $player = $item->getElementsByTagNameNS(NS_MEDIA, 'player')->item(0);
        $info->width = $player->getAttribute('width');
        $info->height = $player->getAttribute('height');
        // ... and usefull informations about its thumbnails
        $thumb = $item->getElementsByTagNameNS(NS_MEDIA, 'thumbnail')->item(0);
        $info->thumbnail = array(
          'src'    => $thumb->getAttribute('url'),
          'width'  => $thumb->getAttribute('width'),
          'height' => $thumb->getAttribute('height')
        );
        // Tags
        $info->tags = array();
        foreach ($item->getElementsByTagNameNS(NS_MEDIA, 'category') as $tag) {
          $info->tags[] = $tag->nodeValue;
        }
        // ... also get the author
        $info->author = $item->getElementsByTagNameNS(NS_DM, 'author')->item(0)->nodeValue;
        // ... and the publication date
        $info->pubDate = date('c', strtotime($item->getElementsByTagName('pubDate')->item(0)->nodeValue));
        $info->license = 'DM';
        $items[] = $info;
      }
    }
  }
  return $items;
}

/**
 * Get information on a specific video.
 * @param $id
 *   The video id
 * @return object
 *   An object containing the video informations. For information on
 *   the object format, see @dpiscald_dailymotion_feed.
 */
function dpiscald_dailymotion_video($id) {
  $items = dpiscald_dailymotion_feed('video', $id);
  return $items[0];
}

/**
 * Get OEmbed information on a specific video.
 * @param $id
 *   The video id
 * @return object
 *   An object containing the video informations.
 */
function dpiscald_dailymotion_oembed($id) {
  $url = DAILYMOTION_OEMBED . '?url=' . urlencode(DAILYMOTION_WEB . $id) . '&format=json';
  $response = drupal_http_request($url);
  if ($response->code >= 200 && $response->code < 400) {
    $data = json_decode($response->data);
    if ($data) {
      $item = new stdClass();
      $item->id = $id;
      $item->title = $data->title;
      $item->width = $data->width;
      $item->height = $data->height;
      $item->thumbnail = array(
        'src' => $data->thumbnail_url,
        'width' => $data->thumbnail_width,
        'height' => $data->thumbnail_height
      );
      $item->author = $data->author_name;
      $item->license = 'DM';
      $item->tags = array();
      return $item;
    }
  }

  return FALSE;
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
function dpiscald_dailymotion_already_imported($id) {
  $query = array('provider' => 'dpiscald', 'base_id' => $id);
  return scald_search($query, FALSE, TRUE);
}

/**
 * Form element validation handler for Dailymotion identifier
 *
 */
function dpiscald_dailymotion_validate_id($element, &$form_state) {
  $id = dpiscald_dailymotion_parse_id($form_state['values']['identifier'], TRUE);
  if (!$id) {
    form_error($element, t('Invalid Dailymotion video identifier.'));
  }
  elseif (dpiscald_dailymotion_already_imported($id)) {
    form_error($element, t('Dailymotion video already imported.'));
  }
}

/**
 * Parse an Dailymotion ID and check validity
 *
 */
function dpiscald_dailymotion_parse_id($string, $check) {
  $id = NULL;
  $string = trim($string);
  if (!preg_match('/^[a-zA-Z0-9]+$/', $string)) {
    // The string ID is not easy to parse, let's try to analyze it
    if (preg_match('#^' . DAILYMOTION_WEB . '([a-zA-Z0-9]+)(_.*)?$#', $string, $m)) {
      // This string os a full dailymotion URL
      $id = $m[1];
    }
    elseif (preg_match('#^' . DAILYMOTION_WEBS . '([a-zA-Z0-9]+)(_.*)?$#', $string, $m)) {
      // This string os a full dailymotion URL
      $id = $m[1];
    }
    elseif (preg_match("/^http/", $string)) {
      // This string is a URL, most likely a shortened one. (http://dai.ly, http://bit.ly, etc...)
      $response = drupal_http_request($string);
      if ($response->code == 200 && isset($response->redirect_code) && ($response->redirect_code == 301 || $response->redirect_code == 302)) {
        return dpiscald_dailymotion_parse_id($response->redirect_url, $check);
      }
    }
  }
  else {
    $id = $string;
  }

  if ($id && $check) {
    // Last check to confirm this video really exists on Dailymotion
    if (!dpiscald_dailymotion_video($id)) {
      $id = FALSE;
    }
  }
  return $id;
}


/**
 * Import proxy page, fill the identifier
 *
 */
function dpiscald_dailymotion_import() {
  // The edit page is nothing else other than the add page, at the Add step. We
  // prepare data for this step then send back to the add page.
  $types = scald_types();
  $storage = array(
    'type' => $types['video'],
    'source' => 'dpiscald_dailymotion',
  );
  ctools_include('object-cache');
  ctools_object_cache_set('scald_atom', 'add', $storage);

  module_load_include('inc', 'scald', 'includes/scald.pages');
  return scald_atom_add_page(FALSE, $types['video'], 'add');
}
