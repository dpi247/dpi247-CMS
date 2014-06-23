<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Plugin OPML blog Spirit'),
  'description' => t('Fetch an OPML blog Spirit feed.'),
  'type' => 'OPML Blog Spirit',
  'defaults' => dpiblocks_opml_blogspirit_flow_settings_default(),
);

/**
 * Settings form
 */
function dpiblocks_opml_blogspirit_flow_settings_form(&$form, &$form_state) {
  $conf = dpiblocks_opml_blogspirit_flow_settings_default($form_state);

  $form['uri'] = array(
    '#type'             => 'textfield',
    '#title'            => t('URI'),
    '#description'      => t('URI of the OPML (could be an url or something else).'),
    '#default_value'    => $conf['uri'],
    '#size'             => 80,
    '#maxlength'        => 512,
    '#required'         => TRUE,
  );

  $form['request_timeout'] = array(
    '#type'             => 'textfield',
    '#title'            => t('HTTP requests timeout (in seconds)'),
    '#description'      => t('Specifies the timeout limit (in seconds) for every HTTP request made for this URL'),
    '#default_value'    => $conf['request_timeout'],
  );

  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch for this feed'),
    '#default_value'    => $conf['maxitem'],
  );

  $form['embed_img'] = array(
    '#title'            => t('Use the first image embeded in the text if no media image found.'),
    '#type'             => 'checkbox',
    '#default_value'    => $conf['embed_img'],
  );
}

function dpiblocks_opml_blogspirit_flow_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }

  $defaults = array(
    'uri' => '',
    'request_timeout' => dpi_variable_get('dpicommons_http_request_timeout', 3),
    'maxitem' => '',
    'embed_img' => 0,
  );

  return $defaults;
}

/**
 * Validate the settings form
 */
function dpiblocks_opml_blogspirit_flow_settings_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];

  if (!is_numeric($values['request_timeout']) || $values['request_timeout'] < 0) {
    form_set_error('request_timeout', t('The timeout value has to be a numerical value greater than 0'));
  } else {
    $uri_response = drupal_http_request($values['uri'], array('timeout' => $values['request_timeout']));
    module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
    if (!dpicommons_is_http_request_successfull($uri_response)) {
      $message = t('The URI doesn\'t respond.');
      if (isset($uri_response->error) && !empty($uri_response->error)) {
        $message .= ' '.t('Error message : @msg', array('@msg' => $uri_response->error));
      }
      form_set_error('uri', $message);
    }
  }
}

function dpiblocks_opml_blogspirit_flow_build(&$feed_infos) {
  module_load_include('inc', 'dpiblocks', 'includes/dpiblocks.flowhelpers');

  // Initiate the packages array
  $struct_array = dpiblocks_flowhelpers_init_array($feed_infos);

  $maxitem = (isset($feed_infos['conf']['maxitem']) && $feed_infos['conf']['maxitem'] > 0) ? $feed_infos['conf']['maxitem'] : 1000;
  $request_timeout = (isset($feed_infos['conf']['request_timeout']) && !empty($feed_infos['conf']['request_timeout'])) ? $feed_infos['conf']['request_timeout'] : dpi_variable_get('dpicommons_http_request_timeout', 3);
  $feed_xml = drupal_http_request($feed_infos['conf']['uri'], array('timeout' => $request_timeout));

  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  if (dpicommons_is_http_request_successfull($feed_xml)) {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $head = $dom->getElementsByTagName('head');
    $opml_tags = $dom->getElementsByTagName('opml');
    $body = $dom->getElementsByTagName('body');
    $channel_last_build = 0;

    if ($body->item(0)) {
      foreach ($body->item(0)->getElementsByTagName('outline') as $item) {
        // Build a package form a BlogSpirit item
        $struct_array['Packages']['Package'][] = dpiblocks_opml_blogspirit_flow_item_to_node($head->item(0), $item, $feed_infos);

        if (sizeof($struct_array['Packages']['Package']) >= $maxitem) {
          break;
        }
      }

      if (empty($struct_array['Packages']['Package'])) {
        dpiblocks_opml_blogspirit_flow_set_log($struct_array['Packages'], FALSE, t('No modifications for feed').' : '.check_plain($feed_infos['name']));
      }
    } else {
      dpiblocks_opml_blogspirit_flow_set_log($struct_array['Packages'], FALSE, t('No channel in the feed').' : '.check_plain($feed_infos['name']));
    }
  } else {
    dpiblocks_opml_blogspirit_flow_set_log($struct_array['Packages'], FALSE, t('No HTTP response for feed').' : '.check_plain($feed_infos['name']));
  }

  return $struct_array;
}

/**
 * Build an array from a opml item. The array structure is based on the
 * structure required by Wallymport.
 *
 * The structure follows the rule :
 * $array[element_name][attribute_name] = attribute_value
 * or
 * $array[element_name][subelement_name] = subelement (array)
 * And so on..
 *
 * If the subelement is just a string, use 'value' as subelement_name.
 *
 * @param $channel
 *   The full opml channel, needed because of the media fields which can
 *   be written anywhere in the feed.
 * @param $item
 *   The opml to be transformed.
 * @param $feed_infos
 *   The infos of the feed as recorded in the DB.
 *
 * @return
 *   The array-restructured item.
 */
function dpiblocks_opml_blogspirit_flow_item_to_node($head, $item, $feed_infos) {
  // Initiate a package and its wrapper
  list($package, $package_wrapper) = dpiblocks_flowhelpers_new_package();

  $feed_name = $feed_infos['name'];
  $caid = $feed_infos['caid'];

  // Get package informations
  dpiblocks_opml_blogspirit_flow_set_package($package, $package_wrapper, $item, $feed_name);

  // Get Embeded Contents
  dpiblocks_opml_blogspirit_flow_set_medias($package, $package_wrapper, $item, $feed_name, $caid);

  // Check wether there already are some photos in the package or not
  $embeds_count = dpicontenttypes_api_get_embeds_count_by_type($package);
  $no_photo = !isset($embeds_count['image']) || empty($embeds_count['image']);

  // Get embeded images in the text body
  if ($feed_infos['conf']['embed_img'] && $no_photo) {
    dpiblocks_opml_blogspirit_flow_set_embeded_image($package, $package_wrapper, $item, $feed_name, $caid);
  }

  return $package;
}

/**
 * Get package available parameters from an opml feed item domElement
 * and set an array item according to the structure defined in
 * Wallymport.
 *
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param array $feed_infos
 *   General informations about the feed being analyzed.
 */
function dpiblocks_opml_blogspirit_flow_set_package($package, $package_wrapper, $item, $feed_name) {
  // Build DPIProperties
  $dpiproperties = &$package->dpiproperties;
  $dpiproperties->entity = 'node';
  $pack_id = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'blog_id').'_'.dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_id');
  if ($pack_id == '_') {
    $pack_id = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_permalink');
  }
  $external_reference = 'flowmix_'.$feed_name.'_'.md5($pack_id);
  $dpiproperties->external_reference = $external_reference;
  $dpiproperties->external_reference_2 = 'text-'.$external_reference;
  $dpiproperties->source = 'flowmix_'.$feed_name;
  $dpiproperties->product_id = dpi_variable_get('dpicommons_product', '');

  // The format is used for multiple fields, get it once for all
  $format = dpiblocks_flowhelpers_get_default_text_format();

  // This field is used in the flowmix process, if it is not present in the feed set it to current time
  if ($xml_pub_date = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_pubdate')) {
    $editorialpublicationdate = dpiblocks_flowhelpers_get_datestamp_from_datetime($xml_pub_date);
  }
  else {
    $editorialpublicationdate = time();
  }
  $package_wrapper->field_editorialpublicationdate->set($editorialpublicationdate);

  // Build title
  $value = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_title');
  $package_wrapper->title->set(dpiblocks_flowhelpers_filter_html_tags($value));
  $package_wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));

  // Build external URL
  if ($value = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_permalink')) {
    $package_wrapper->field_externalurl->set(array('url' => $value));
  }

  // Build copyright
  if ($value = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'user_username')) {
    $package_wrapper->field_copyright->set($value);
  }

  // Build text body
  if ($value = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_source')) {
    $value = str_replace('&nbsp', ' ', $value);
    $package_wrapper->field_textbody->set(array('value' => $value, 'format' => $format));
  }

  // Build IPTC tags
  $categories = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_tags');
  $categories = trim($categories);
  if (!empty($categories)) {
    $categories = explode(',', $categories);
    $field_categories = array();
    foreach ($categories as $category) {
      $orig_category = trim($category);
      $category = taxonomy_get_term_by_name($category, 'iptc');
      if (empty($category)) {
        if ($vid = dpi_variable_get('dpi_iptc', FALSE)) {
          $new_term = new stdClass();
          $new_term->vid = $vid;
          $new_term->weight = 0;
          $new_term->vocabulary_machine_name = 'iptc';
          $new_term->name = $orig_category;
          taxonomy_term_save($new_term);
          $category = $new_term;
        }
      } else {
        $category = array_shift($category);
      }
      $field_categories[] = $category->tid;
    }
    $package_wrapper->field_iptc->set($field_categories);
  }

  // Some additionnal fields
  $package->section = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'blog_title');
  $package->SectionUrl = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'blog_url');
  $package->PostDate = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_pubdate');
}

/**
 * Get available parameters from medias domElement of a opml feed item
 *
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function dpiblocks_opml_blogspirit_flow_set_medias($package, $package_wrapper, $item, $feed_name, $caid) {
  // Is the media content an image?
  if (dpiblocks_opml_blogspirit_flow_check_is_image($item)) {
    dpiblocks_opml_blogspirit_flow_set_media_image($package, $package_wrapper, $item, $feed_name, $caid);
  }
  else {
    // @todo : Handle other types
  }
}

function dpiblocks_opml_blogspirit_flow_set_media_image($package, $package_wrapper, $item, $feed_name, $caid) {
  // Get its URL
  $url = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'media_url');
  // Make sur the URL is clean
  $url = urldecode(html_entity_decode($url));
  if (valid_url($url, TRUE)) {
    $media_id = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'post_media_id', 'none', md5($url));
    // Load an existing atom or create a new one from an URL-based external reference
    $external_reference = 'flowmix_'.$feed_name.'_'.$media_id;
    list($atom, $atom_wrapper) = dpiblocks_flowhelpers_create_or_load_atom('image', 'scald_image', $external_reference);
    if ($atom && $atom_wrapper) {
      // The format is used for multiple fields, get it once for all
      $format = dpiblocks_flowhelpers_get_default_text_format();

      // Set title and display title
      $atom_wrapper->title->set($package_wrapper->title->value());
      $atom_wrapper->field_displaytitle->set($package_wrapper->field_displaytitle->value());

      // Set copyright
      if ($value = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'user_username')) {
        $atom_wrapper->field_copyright->set($value);
      }

      // Set thumbnail
      $thumbnail_set = FALSE;
      if ($thumb_url = dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($item, 'media_thumbnail')) {
        if ($file = dpicontenttypes_api_create_or_update_file($thumb_url, 'Flows/'.$caid, 'public', $caid)) {
          $atom_wrapper->scald_thumbnail->set((array)$file);
          $atom->thumbnail_source = $file->uri;
          $thumbnail_set = TRUE;
        }
      }

      // Create a file from the URL
      if ($file = dpicontenttypes_api_create_or_update_file($url, 'Flows/'.$caid, 'public', $caid)) {
        $atom->base_entity = $file;
        $atom->base_id = $file->fid;
        $atom->file_source = $file->uri;

        // If no thumbnail where found before, set a copy of the main file
        if (!$thumbnail_set) {
          $atom_wrapper->scald_thumbnail->set((array)$file);
          $atom->thumbnail_source = $file->uri;
        }

        // We have to save the atom for all the following processes
        scald_atom_save($atom);

        // Link the atom to the package
        dpiblocks_flowhelpers_add_atom_to_fied($package_wrapper, $package, 'field_embededobjects', $atom, array('identifier' => TRUE));
      }
    }
  }
}

function dpiblocks_opml_blogspirit_flow_set_videoobject($package, $package_wrapper, $item, $feed_name, $caid) {
  return;
  $video_id = $prefix.':::'._wallytoolbox_get_dom_element_attribute_opml($item, 'post_media_id');
  $tempobject['type'] = 'VideoObjectType';
  $tempobject['ObjectID'] = $video_id;
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_type');
  $tempobject['Title']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_title');
  $tempobject['ExternalReference']['value'] = $video_id;
  $tempobject['Copyright']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'user_username');
  $tempobject['FileURI']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_url');
  $tempobject['FileSize']['value'] = 0;
  $tempobject['Thumbnail']['URI'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_thumbnail');
}

/**
 * Get embeded image in the text body of a RSS feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 *
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 */
function dpiblocks_opml_blogspirit_flow_set_embeded_image($package, $package_wrapper, $item, $feed_name, $caid) {
  $field_body = $package_wrapper->field_textbody->value();
  $text_body = $field_body['value'];

  // Extract image informations from text
  list($url, $alt) = dpiblocks_flowhelpers_extract_image_from_text($text_body);

  if (!empty($url) && valid_url($url, TRUE)) {
    $external_reference = 'flowmix_'.$feed_name.'_'.md5($url);
    list($atom, $atom_wrapper) = dpiblocks_flowhelpers_create_or_load_atom('image', 'scald_image', $external_reference);
    if ($atom && $atom_wrapper) {
      // The format is used for multiple fields, get it once for all
      $format = dpiblocks_flowhelpers_get_default_text_format();

      // Set title and display title
      if (!empty($alt)) {
        $package_wrapper->title->set(dpiblocks_flowhelpers_filter_html_tags($alt));
        $package_wrapper->field_displaytitle->set(array('value' => $alt, 'format' => $format));
      }
      // If no title found, use the package title
      else {
        $atom_wrapper->title->set($package_wrapper->title->value());
        $atom_wrapper->field_displaytitle->set($package_wrapper->field_displaytitle->value());
      }

      // Create a file from the URL
      if ($file = dpicontenttypes_api_create_or_update_file($url, 'Flows/'.$caid, 'public', $caid)) {
        $atom->base_entity = $file;
        $atom_wrapper->scald_thumbnail->set((array)$file);
        $atom->base_id = $file->fid;

        // Set a copy of the main file as thumbnail
        $atom_wrapper->scald_thumbnail->set((array)$file);
        $atom->thumbnail_source = $file->uri;

        // We have to save the atom for all the following processes
        scald_atom_save($atom);

        // Link the atom to the package
        dpiblocks_flowhelpers_add_atom_to_fied($package_wrapper, $package, 'field_embededobjects', $atom, array('identifier' => TRUE));
      }
    }
  }
}

/**
 * Search an item for a desired element.
 *
 * @param $dom_item
 *   The item in which the search is performed.
 * @param $name
 *   The name of the searched element.
 * @param $namespace
 *   Value of the namespace if used.
 * @param $default
 *   A default value if the element is not found
 *
 * @return
 *   The element value or ''.
 */
function dpiblocks_opml_blogspirit_flow_check_if_element_exists($dom_item, $name, $namespace = 'none', $default = '') {
  $return = $default;
  if (is_object($dom_item)) {
    if ($namespace != 'none') {
      $return = $dom_item->getElementsByTagNameNS($namespace, $name)->item(0) ? $dom_item->getElementsByTagNameNS($namespace, $name)->item(0)->firstChild->wholeText : '';
    }
    else {
      $return = $dom_item->getElementsByTagName($name)->item(0) ? $dom_item->getElementsByTagName($name)->item(0)->firstChild->wholeText : '';
    }
  }
  return $return;
}

/**
 * Check the presence of an attribute in a given DOM Node and return its value
 *
 * @param DOM Node $dom_item
 * @param string $name
 * @param string $namespace
 * @param string $default
 *
 * @return string
 */
function dpiblocks_opml_blogspirit_flow_check_if_attribute_exists($dom_item, $name, $namespace = 'none', $default = '') {
  $return = $default;
  if (is_object($dom_item)) {
    if ($namespace != 'none' && $dom_item->hasAttributeNS($namespace, $name)) {
      $return = $dom_item->getAttributeNS($namespace, $name);
    }
    elseif ($dom_item->hasAttribute($name)) {
      $return = $dom_item->getAttribute($name);
    }
  }
  return $return;
}

/**
 * Is the given media an image or not?
 */
function dpiblocks_opml_blogspirit_flow_check_is_image($media_content) {
  if ($media_content->hasAttribute('media_type') && stristr($media_content->getAttribute('media_type'), 'image')) {
    return TRUE;
  } elseif ($media_url = $media_content->getAttribute('media_url')) {
    if (stristr($media_url,'.jpg') || stristr($media_url,'.jpeg') || stristr($media_url,'.gif') || stristr($media_url,'.png')) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Is the given media a video or not?
 */
function dpiblocks_opml_blogspirit_flow_check_is_video($media_content) {
  if ($media_content->getAttribute('media_type') && stristr($media_content->getAttribute('media_type'), 'video')) {
    return TRUE;
  } elseif ($media_url = $media_content->getAttribute('media_url')) {
    //@todo : ajouter un check sur le format de la video, comme pour les images
  }

  return FALSE;
}

/**
 * Set a log in an item
 */
function dpiblocks_opml_blogspirit_flow_set_log(&$item, $success, $type, $msg, $severity = 'notice') {
  if (user_access('access administration pages')) {
    drupal_set_message($msg, $severity);
  }
  $item->__log['success'] = $success;
  $item->__log['msg'] = $msg;
  $item->__log['type'] = $type;
}
