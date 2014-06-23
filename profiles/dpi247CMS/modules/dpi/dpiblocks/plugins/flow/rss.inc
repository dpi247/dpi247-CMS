<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('RSS'),
  'description' => t('A common RSS 2.0 and RSS Media transformer'),
  'type' => 'RSS & RSS Media',
  'defaults' => dpiblocks_rss_flow_settings_default(),
);

/**
 * Settings form
*/
function dpiblocks_rss_flow_settings_form(&$form, &$form_state) {
  $conf = dpiblocks_rss_flow_settings_default($form_state);

  $form['uri'] = array(
    '#type'             => 'textfield',
    '#title'            => t('URI'),
    '#description'      => t('URI of the RSS (could be an url or something else).'),
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
    '#description'      => t('Set to 0 for no limitation'),
    '#default_value'    => $conf['maxitem'],
    '#required'         => TRUE,
  );

  $form['embed_img'] = array(
    '#title'            => t('Use the first image embeded in the text if no media image found.'),
    '#type'             => 'checkbox',
    '#default_value'    => $conf['embed_img'],
  );

  $form['channel_img'] = array(
    '#title'            => t('Use the default channel image if no media image found.'),
    '#type'             => 'checkbox',
    '#default_value'    => $conf['channel_img'],
  );
}

/**
 * Settings form default values
 */
function dpiblocks_rss_flow_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }

  $defaults = array(
    'uri' => '',
    'request_timeout' => dpi_variable_get('dpicommons_http_request_timeout', 3),
    'maxitem' => '',
    'embed_img' => 0,
    'channel_img' => 0,
  );

  return $defaults;
}

/**
 * Validate the settings form
 */
function dpiblocks_rss_flow_settings_form_validate(&$form, &$form_state) {
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

/**
 * Build function of the RSS flow plugin
 */
function dpiblocks_rss_flow_build(&$feed_infos) {
  module_load_include('inc', 'dpiblocks', 'includes/dpiblocks.flowhelpers');

  $return_msg = 'RSS feed "@feed_name": ';
  $return_substion = array('@feed_name' => $feed_infos['name']);

  // Initiate the packages array
  $struct_array = dpiblocks_flowhelpers_init_array($feed_infos);

  $maxitem = (isset($feed_infos['conf']['maxitem']) && $feed_infos['conf']['maxitem'] > 0) ? $feed_infos['conf']['maxitem'] : 1000;
  $request_timeout = (isset($feed_infos['conf']['request_timeout']) && !empty($feed_infos['conf']['request_timeout'])) ? $feed_infos['conf']['request_timeout'] : dpi_variable_get('dpicommons_http_request_timeout', 3);
  $feed_xml = drupal_http_request($feed_infos['conf']['uri'], array('timeout' => $request_timeout));

  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  if (dpicommons_is_http_request_successfull($feed_xml)) {
    set_error_handler('dpiblocks_rss_flow_handle_errors');

    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $channels = $dom->getElementsByTagName('channel');
    $rss_tags = $dom->getElementsByTagName('rss');
    if ($rss_tags->item(0)) {
      $media_namespace = $rss_tags->item(0)->getAttributeNS('http://www.w3.org/2000/xmlns/', 'media');

      if ($channels->item(0)) {
        foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
          // Build a package from an RSS item
          $struct_array['Packages']['Package'][] = dpiblocks_rss_flow_item_to_node($channels->item(0), $item, $feed_infos, $media_namespace);;

          if (sizeof($struct_array['Packages']['Package']) >= $maxitem) {
            break;
          }
        }

        if (empty($struct_array['Packages']['Package'])) {
          dpiblocks_rss_flow_set_log($struct_array['Packages'], TRUE, 'unchanged', t($return_msg.'No modifications', $return_substion));
        }
      } else {
        dpiblocks_rss_flow_set_log($struct_array['Packages'], FALSE, 'structure', t($return_msg.'No channel', $return_substion));
      }
    } else {
      dpiblocks_rss_flow_set_log($struct_array['Packages'], FALSE, 'structure', t($return_msg.'No "rss" tag', $return_substion));
    }
  } else {
    dpiblocks_rss_flow_set_log($struct_array['Packages'], FALSE, 'http-notfound', t($return_msg.'No HTTP response', $return_substion));
  }

  return $struct_array;
}

/**
 * Build a node from a RSS item, the node will not be saved.
 *
 * @param $channel
 *   The full RSS channel, needed because of the media fields which can
 *   be written anywhere in the feed.
 * @param $item
 *   The RSS to be transformed.
 * @param $feed_infos
 *   The infos of the feed as recorded in the DB.
 *
 * @return
 *   The node.
 */
function dpiblocks_rss_flow_item_to_node($channel, $item, $feed_infos, $media_namespace = 'http://search.yahoo.com/mrss/') {
  // Initiate a package and its wrapper
  list($package, $package_wrapper) = dpiblocks_flowhelpers_new_package();

  $feed_name = $feed_infos['name'];
  $caid = $feed_infos['caid'];

  // Get package informations
  dpiblocks_rss_flow_set_package($package, $package_wrapper, $item, $feed_name);

  // Get enclosure content
  $enclosure = $item->getElementsByTagName('enclosure');
  if ($enclosure->item(0) && $enclosure->item(0)->getAttribute('type') && strstr($enclosure->item(0)->getAttribute('type'), 'image')) {
    dpiblocks_rss_flow_set_enclosure($package, $package_wrapper, $enclosure->item(0), $feed_name, $caid);
  }

  // Get media contents
  $media_contents = $item->getElementsByTagNameNS($media_namespace, 'content');
  if ($media_contents->item(0)) {
    dpiblocks_rss_flow_set_medias($package, $package_wrapper, $channel, $item, $media_contents, $feed_name, $caid, $media_namespace);
  }

  // Check wether there already are some photos in the package or not
  $embeds_count = dpicontenttypes_api_get_embeds_count_by_type($package);
  $no_photo = !isset($embeds_count['image']) || empty($embeds_count['image']);

  // Get embeded images in the text body
  if ($feed_infos['conf']['embed_img'] && $no_photo) {
    dpiblocks_rss_flow_set_embeded_image($package, $package_wrapper, $item, $feed_name, $caid);
  }

  // Get the channel image
  $channel_img = $channel->getElementsByTagName('image');
  if ($feed_infos['conf']['channel_img'] && $no_photo && $channel_img->item(0)) {
    dpiblocks_rss_flow_set_channel_image($package, $package_wrapper, $channel_img->item(0), $feed_name, $caid);
  }

  return $package;
}

/**
 * Get package available parameters from an RSS feed item domElement.
 *
 * @param object $package
 *   The package item under construction.
 * @param object $package_wrapper
 *   The wrapper of the package item under construction
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param string $feed_name
 *   Name of the feed being analyzed.
 */
function dpiblocks_rss_flow_set_package($package, $package_wrapper, $item, $feed_name) {
  // Set dpiproperties
  $dpiproperties = &$package->dpiproperties;
  $dpiproperties->entity = 'node';
  if ($guid = dpiblocks_rss_flow_check_if_element_exists($item, 'guid')) {
    $pack_id = $guid;
  }
  else {
    $pack_id = dpiblocks_rss_flow_check_if_element_exists($item, 'link');
  }
  $external_reference = 'flowmix_'.$feed_name.'_'.md5($pack_id);
  $dpiproperties->external_reference = $external_reference;
  $dpiproperties->external_reference_2 = 'text-'.$external_reference;
  $dpiproperties->source = 'flowmix_'.$feed_name;
  $dpiproperties->product_id = dpi_variable_get('dpicommons_product', '');

  // The format is used for multiple fields, get it once for all
  $format = dpiblocks_flowhelpers_get_default_text_format();

  // This field is used in the flowmix process, if it is not present in the feed set it to current time
  if ($xml_pub_date = dpiblocks_rss_flow_check_if_element_exists($item, 'pubDate')) {
    $editorialpublicationdate = dpiblocks_flowhelpers_get_datestamp_from_datetime($xml_pub_date);
  }
  else {
    $editorialpublicationdate = time();
  }
  $package_wrapper->field_editorialpublicationdate->set($editorialpublicationdate);

  // Set title and display title
  $value = dpiblocks_rss_flow_check_if_element_exists($item, 'title');
  $package_wrapper->title->set(dpiblocks_flowhelpers_filter_html_tags($value));
  $package_wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));

  // Set external URL
  if ($value = dpiblocks_rss_flow_check_if_element_exists($item, 'link')) {
    $package_wrapper->field_externalurl->set(array('url' => $value));
  }

  // Set copyright
  if ($value = dpiblocks_rss_flow_check_if_element_exists($item, 'author')) {
    $package_wrapper->field_copyright->set($value);
  }

  // Set text body
  if ($value = dpiblocks_rss_flow_check_if_element_exists($item, 'description')) {
    $package_wrapper->field_textbody->set(array('value' => $value, 'format' => $format));
  }

  // Set IPTC
  $category_name = dpiblocks_rss_flow_check_if_element_exists($item, 'category');
  $category_name = trim($category_name);
  if (!empty($category_name)) {
    $category = taxonomy_get_term_by_name($category_name, 'iptc');
    if (empty($category)) {
      if ($vid = dpi_variable_get('dpi_iptc', FALSE)) {
        $new_term = new stdClass();
        $new_term->vid = $vid;
        $new_term->weight = 0;
        $new_term->vocabulary_machine_name = 'iptc';
        $new_term->name = $category_name;
        taxonomy_term_save($new_term);
        $category = $new_term;
      }
    } else {
      $category = array_shift($category);
    }
    $package_wrapper->field_iptc->set(array($category->tid));
  }
}

/**
 * Get available parameters from an enclosure domElement of a RSS feed item
 *
 * @param object $package
 *   The package item under construction.
 * @param object $package_wrapper
 *   The wrapper of the package item under construction
 * @param domElement $enclosure
 *   The enclosure domElement from the XML RSS feed item.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 * @param string $caid
 *   Cache ID of the flowmix
 */
function dpiblocks_rss_flow_set_enclosure($package, $package_wrapper, $enclosure, $feed_name, $caid) {
  $url = dpiblocks_rss_flow_check_if_attribute_exists($enclosure, 'url');
  // Make sur the URL is clean
  $url = urldecode(html_entity_decode($url));
  if (valid_url($url, TRUE)) {
    // Check the type of the enclosure
    if (dpiblocks_rss_flow_check_is_image($enclosure)) {
      $external_reference = 'flowmix_'.$feed_name.'_'.md5($url);
      list($atom, $atom_wrapper) = dpiblocks_flowhelpers_create_or_load_atom('image', 'scald_image', $external_reference);
      if ($atom && $atom_wrapper) {
        $atom_wrapper->title->set($package_wrapper->title->value());
        $atom_wrapper->field_displaytitle->set($package_wrapper->field_displaytitle->value());

        if ($file = dpicontenttypes_api_create_or_update_file($url, 'Flows/'.$caid, 'public', $caid)) {
          $atom->base_entity = $file;
          $atom_wrapper->scald_thumbnail->set((array)$file);
          $atom->base_id = $file->fid;

          // We have to save the atom for all the following processes
          scald_atom_save($atom);

          // Add or update the atom in the embeded objects
          dpiblocks_flowhelpers_add_atom_to_fied($package_wrapper, $package, 'field_embededobjects', $atom, array('identifier' => TRUE));
        }
      }
    }
    else {
      // @todo : Handle other types
    }
  }
}

/**
 * Get available parameters from medias domElement of a RSS feed item
 *
 * @param object $package
 *   The package item under construction.
 * @param object $package_wrapper
 *   The wrapper of the package item under construction
 * @param domNode $channel
 *   The full RSS channel from a RSS feed.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param domElement $media_contents
 *   The medias domElements from a RSS feed.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 * @param string $caid
 *   Cache ID of the flowmix
 * @param string $media_namespace = 'http://search.yahoo.com/mrss/'
 *   RSS media namespace
 */
function dpiblocks_rss_flow_set_medias($package, $package_wrapper, $channel, $item, $media_contents, $feed_name, $caid, $media_namespace = 'http://search.yahoo.com/mrss/') {
  foreach ($media_contents as $media_content) {
    // Is the media content an image?
    if (dpiblocks_rss_flow_check_is_image($media_content)) {
      dpiblocks_rss_flow_set_media_image($package, $package_wrapper, $channel, $item, $media_content, $feed_name, $caid, $media_namespace);
    }
    else {
      // @todo : Handle other types
    }
  }
}

/**
 * Get available parameters from media domElement of a RSS feed item
 *
 * @param object $package
 *   The package item under construction.
 * @param object $package_wrapper
 *   The wrapper of the package item under construction
 * @param domNode $channel
 *   The full RSS channel from a RSS feed.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param domElement $media_content
 *   The media domElement from a RSS feed.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 * @param string $caid
 *   Cache ID of the flowmix
 * @param string $media_namespace = 'http://search.yahoo.com/mrss/'
 *   RSS media namespace
 */
function dpiblocks_rss_flow_set_media_image($package, $package_wrapper, $channel, $item, $media_content, $feed_name, $caid, $media_namespace = 'http://search.yahoo.com/mrss/') {
  // Get its URL
  $url = dpiblocks_rss_flow_check_if_attribute_exists($media_content, 'url');
  // Make sur the URL is clean
  $url = urldecode(html_entity_decode($url));
  if (valid_url($url, TRUE)) {
    // Load an existing atom or create a new one from an URL-based external reference
    $external_reference = 'flowmix_'.$feed_name.'_'.md5($url);
    list($atom, $atom_wrapper) = dpiblocks_flowhelpers_create_or_load_atom('image', 'scald_image', $external_reference);
    if ($atom && $atom_wrapper) {
      // The format is used for multiple fields, get it once for all
      $format = dpiblocks_flowhelpers_get_default_text_format();

      // Set title and display title
      if ($value = dpiblocks_rss_flow_check_if_media_element_exists($media_content, $item, $channel, $media_namespace, 'title')) {
        $package_wrapper->title->set(dpiblocks_flowhelpers_filter_html_tags($value));
        $package_wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
      }
      // If no title found, use the package title
      else {
        $atom_wrapper->title->set($package_wrapper->title->value());
        $atom_wrapper->field_displaytitle->set($package_wrapper->field_displaytitle->value());
      }

      // Set caption
      if ($value = dpiblocks_rss_flow_check_if_media_element_exists($media_content, $item, $channel, $media_namespace, 'description')) {
        $atom_wrapper->field_caption->set(array('value' => $value, 'format' => $format));
      }

      // Set rating
      if ($value = dpiblocks_rss_flow_check_if_media_element_exists($media_content, $item, $channel, $media_namespace, 'rating')) {
        $vid = dpi_variable_get('dpi_rating', NULL);
        $item = dpicontenttypes_api_taxonomy_get_term_by_path($value, $vid);
        // We take the first match even is there are more.
        $atom_wrapper->field_rating->set($item[0]);
      }

      // Set copyright
      if ($value = dpiblocks_rss_flow_check_if_media_element_exists($media_content, $item, $channel, $media_namespace, 'copyright')) {
        $atom_wrapper->field_copyright->set($value);
      }

      // Set thumbnail
      $thumbnail_set = FALSE;
      if ($value = dpiblocks_rss_flow_check_if_media_element_exists($media_content, $item, $channel, $media_namespace, 'thumbnail')) {
        $thumb_url = $this->checkIfAttributeExists($dom, 'url');
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

/**
 * Get embeded image in the text body of a RSS feed item
 *
 * @param object $package
 *   The package item under construction.
 * @param object $package_wrapper
 *   The wrapper of the package item under construction
 * @param domElement $item
 *   The XML RSS feed item domElement.
 */
function dpiblocks_rss_flow_set_embeded_image($package, $package_wrapper, $item, $feed_name, $caid) {
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
 * Get available parameters from an image domElement of a RSS feed item
 *
 * @param object $package
 *   The package item under construction.
 * @param object $package_wrapper
 *   The wrapper of the package item under construction
 * @param domElement $channel_img
 *   The image domElement from the XML RSS feed item.
 */
function dpiblocks_rss_flow_set_channel_image($package, $package_wrapper, $channel_img, $feed_name, $caid) {
  $url = dpiblock_rss_flow_check_if_element_exists($channel_img, 'url');
  // Make sur the URL is clean
  $url = urldecode(html_entity_decode($url));
  if (valid_url($url, TRUE)) {
    $external_reference = 'flowmix_'.$feed_name.'_'.md5($url);
    list($atom, $atom_wrapper) = dpiblocks_flowhelpers_create_or_load_atom('image', 'scald_image', $external_reference);
    if ($atom && $atom_wrapper) {
      // The format is used for multiple fields, get it once for all
      $format = dpiblocks_flowhelpers_get_default_text_format();

      // Set title and display title
      if ($value = dpiblock_rss_flow_check_if_element_exists($channel_img, 'title')) {
        $package_wrapper->title->set(dpiblocks_flowhelpers_filter_html_tags($value));
        $package_wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
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
    else {
      // @todo : Handle other types
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
function dpiblocks_rss_flow_check_if_element_exists($dom_item, $name, $namespace = 'none', $default = '') {
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
 * Perform a recursive search in a media item for a desired element.
 *
 * @param $media_content
 *   The item in which the first step search is performed.
 * @param $item
 *   The item in which the second step search is performed.
 * @param $channel
 *   The channel in which the third and last step search is performed.
 * @param $namespace
 *   Value of the namespace if used.
 * @param $name
 *   The name of the searched element.
 *
 * @return
 *   The element value or ''.
 */
function dpiblocks_rss_flow_check_if_media_element_exists($media_content, $item, $channel, $namespace, $name) {
  $return = dpiblocks_rss_flow_check_if_element_exists($media_content, $name, $namespace);
  if ($return == '') {
    $return = dpiblocks_rss_flow_check_if_element_exists($item, $name, $namespace);
    if ($return == '') {
      $return = dpiblocks_rss_flow_check_if_element_exists($channel, $name, $namespace);
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
function dpiblocks_rss_flow_check_if_attribute_exists($dom_item, $name, $namespace = 'none', $default = '') {
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
function dpiblocks_rss_flow_check_is_image($media_content) {
  if ($media_content->hasAttribute('type') && stristr($media_content->getAttribute('type'), 'image')) {
    return TRUE;
  } elseif ($media_content->hasAttribute('medium') && stristr($media_content->getAttribute('medium'), 'image')) {
    return TRUE;
  } elseif ($media_url = $media_content->getAttribute('url')) {
    if (stristr($media_url,'.jpg') || stristr($media_url,'.jpeg') || stristr($media_url,'.gif') || stristr($media_url,'.png')) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Is the given media a video or not?
 */
function dpiblocks_rss_flow_check_is_video($media_content) {
  if ($media_content->getAttribute('type') && stristr($media_content->getAttribute('type'), 'video')) {
    return TRUE;
  } elseif ($media_content->getAttribute('medium') && stristr($media_content->getAttribute('medium'), 'video')) {
    return TRUE;
  } elseif ($media_url = $media_content->getAttribute('url')) {
    //@todo : ajouter un check sur le format de la video, comme pour les images
  }

  return FALSE;
}

/**
 * Error handler
 */
function dpiblocks_rss_flow_handle_errors($errno, $errstr, $errfile, $errline, $args) {
  if ($errno == 1) {
    watchdog('php', 'Fatal error: @errstr in @errfile at line @errline', array('@errstr' => $errstr, '@errfile' => $errfile, '@errline' => $errline), WATCHDOG_ERROR);
  }
}

/**
 * Set a log in an item
 */
function dpiblocks_rss_flow_set_log(&$item, $success, $type, $msg, $severity = 'notice') {
  if (user_access('access administration pages')) {
    drupal_set_message($msg, $severity);
  }
  $item->__log['success'] = $success;
  $item->__log['msg'] = $msg;
  $item->__log['type'] = $type;
}
