<?php

function dpinewssitemap_generate_sitemap_xml($settings) {
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');

  $time = time();
  $timestamp = $time - $time%3600;

  $nids = dpinewssitemap_get_nids_to_generate($settings, $timestamp);

  if (!empty($nids)) {
    $xmlfiles_to_generate = dpinewssitemap_generate_urlset_xml($nids, $settings, $timestamp);
    dpisitemap_api_generate_xmlfile($xmlfiles_to_generate, $settings['dpinewssitemap_path']);
  }

  dpinewssitemap_cleanup_old_xmlfiles($settings, $timestamp - 3600);

  if ($settings['dpinewssitemap_consolidate_sitemap']) {
    dpinewssitemap_generate_consolidate_xml($settings);
  }
}

function dpinewssitemap_get_nids_to_generate($settings, $timestamp, $limit = 0) {
  $nids = array();

  $query = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.type', $settings['dpinewssitemap_content_types'], 'IN')
    ->condition('n.changed', $timestamp, '>');
  if ($limit) {
    $query->range(0, $limit);
  }
  $db_nids = $query->execute();
  foreach ($db_nids as $db_nid) {
    $nids[] = $db_nid->nid;
  }

  return $nids;
}

function dpinewssitemap_get_destinations_nids_to_generate($settings, $timestamp, $limit) {
  $nids = array();

  $global_sitemap_settings = dpi_variable_get('wallysitemap_sitemaps_settings', array());
  $dest_settings = $global_sitemap_settings['dpisitemap_dests_params'] ? $global_sitemap_settings['dpisitemap_dests_params'] : array();

  $query = db_select('field_data_field_destinations', 'fdfd')
    ->fields('fdfd', array('nid'));
  $query->join('node', 'n', 'n.nid = fdfd.nid');
  $query->condition('n.changed', $timestamp, '>')
    ->condition('n.type', $settings['dpinewssitemap_content_types'], 'IN')
    ->range(0, $limit);

  $terms = taxonomy_get_tree(dpidestinations_api_get_sections_vocabulary_vid());
  foreach ($terms as $term) {
    $tid = $term->tid;
    $term_settings = $dest_settings[$tid] ? $dest_settings[$tid] : array('check' => 1);
    if ($term_settings['check']) {
      $copied_query = clone $query;
      
      $db_results = $copied_query
        ->condition('fdfd.field_destination_tid', $term->tid)
        ->execute();
      foreach ($db_results as $db_result) {
        $nids[] = $db_result->nid;
      }
    }
  }

  return $nids;
}

function dpinewssitemap_cleanup_old_xmlfiles($settings, $timestamp) {
  $files_to_delete = array();
  $limit_timestamp = $timestamp - $settings['dpinewssitemap_delta_hours'] * 3600 + 1800;

  $current_files = file_scan_directory($settings['dpinewssitemap_path'], '.*\.xml');
  foreach ($current_files as $current_file) {
    list($sitemap_prefix, $uniq_timestamp) = explode('_', $current_file->name);
    list($file_timestamp, $a) = explode('-', $uniq_timestamp);
    if ($sitemap_prefix == 'sitemapnews' && is_numeric($file_timestamp)) {
      if ($file_timestamp < $limit_timestamp) {
        $files_to_delete[] = $current_file->filename;
      }
    }
  }

  dpisitemap_api_remove_files($files_to_delete, $settings['dpinewssitemap_path']);
}

function dpinewssitemap_generate_consolidate_xml($settings) {
  $nids = array();
  switch ($settings['dpinewssitemap_consolidate_sitemap_type']) {
    case 'global':
      $nids = dpinewssitemap_get_nids_to_generate($settings, time() - $settings['dpinewssitemap_delta_hours'] * 3600, $settings['dpinewssitemap_consolidate_sitemap_amount']);
      break;
    case 'destination':
      $nids = dpinewssitemap_get_destinations_nids_to_generate($settings, time() - $settings['dpinewssitemap_delta_hours'] * 3600, $settings['dpinewssitemap_consolidate_sitemap_amount']);
      break;
  }

  if (!empty($nids)) {
    $main_settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
    $dom = new DOMDocument('1.0', 'UTF-8');
    $dom->load($main_settings['wallysitemap_path'].'/sitemapmain.xml');
    $dom_urlset = $dom->getElementsByTagName('urlset')->item(0);

    if (!$dom_urlset) {
      $dom_urlset = $dom->appendChild($dom->createElementNS($namespaces['sitemap'], 'urlset'));
    }

    $dom_urls = $dom->getElementsByTagName('url');
    foreach ($dom_urls as $dom_url) {
      $dom_lastmod = $dom_url->getElementsByTagName('lastmod');
      if ($dom_lastmod->item(0)) {
        $dom_urlset->removeChild($dom_url);
      }
    }

    $not_empty = FALSE;
    $site_infos = dpisitemap_api_get_site_infos();
    foreach ($nids as $nid) {
      $node = node_load($nid);
      $node->path = drupal_get_path_alias('node/'.$node->nid);
      if (in_array($node->path, $settings['dpinewssitemap_blacklist'])) {
        continue;
      }
      $url = $dom_urlset->appendChild($dom->createElement('url'));
      dpinewssitemap_generate_newsurl_xml_set_commons($dom, $url, $node, $site_infos, $settings);
      $not_empty = TRUE;
    }

    if ($not_empty) {
      $context = array('consolidate');
      drupal_alter('dpisitemap_dom', $dom, $context);

      $xmlfiles_to_generate = array(array(
        'dom' => $dom,
        'filename' => 'sitemapmain.xml',
      ));
    }

    dpisitemap_api_generate_xmlfile($xmlfiles_to_generate, $main_settings['dpisitemap_path']);
  }
}

/**
 * Generate news sitemap xml for a list of nodes
 *
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=75717&topic=10078&ctx=topic
 *
 * Output example :
 * <?xml version="1.0" encoding="UTF-8"?>
 * <urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9/"
 *     xmlns:news="http://www.google.com/schemas/sitemap-news/0.9/"
 *     xmlns:image="http://www.google.com/schemas/sitemap-image/1.1/"
 *     xmlns:video="http://www.google.com/schemas/sitemap-video/1.1/">
 *   <url>
 *     [...]
 *   </url>
 *   <url>
 *     [...]
 *   </url>
 * </urlset>
 */
function dpinewssitemap_generate_urlset_xml($nids, $settings, $timestamp) {
  $xmlfiles_to_generate = array();
  $namespaces = dpisitemap_api_get_namespaces();
  $site_infos = dpisitemap_api_get_site_infos();
  $dom = dpinewssitemap_new_dom($settings);
  $urlset = $dom->getElementsByTagName('urlset')->item(0);
  $not_empty = FALSE;

  $i = $n = 0;
  foreach ($nids as $nid) {
    $node = node_load($nid);
    $node->path = drupal_get_path_alias('node/'.$node->nid); 
    if (in_array($node->path, $settings['dpinewssitemap_blacklist'])) {
      continue;
    }

    if ($i == 999) {
      $i = 0;

      $context = array('news');
      drupal_alter('dpisitemap_dom', $dom, $context);

      $xmlfiles_to_generate[] = array(
        'dom' => $dom,
        'filename' => 'sitemapnews_'.$timestamp.'-'.$n++.'.xml',
      );
      $dom = dpinewssitemap_new_dom();
      $urlset = $dom->getElementsByTagName('urlset')->item(0);
    }
    $i++;

    $url = $urlset->appendChild($dom->createElement('url'));
    dpinewssitemap_generate_newsurl_xml($dom, $url, $namespaces, $node, $site_infos, $settings);

    $not_empty = TRUE;
  }

  if ($not_empty) {
    $context = array('news');
    drupal_alter('dpisitemap_dom', $dom, $context);

    $xmlfiles_to_generate[] = array(
      'dom' => $dom,
      'filename' => 'sitemapnews_'.$timestamp.'-'.$n++.'.xml',
    );
  }

  return $xmlfiles_to_generate;
}

/**
 * Generate news sitemap xml for a node
 *
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=75717&topic=10078&ctx=topic
 *
 * Output example :
 * <url>
 *   <loc>http://www.example.org/business/article55.html</loc>
 *   <news:news>
 *     [...]
 *   </news:news>
 *   <image:image>
 *     [...]
 *   </image:image>
 *   <video:video>
 *     [...]
 *   </video:video>
 * </url>
 */
function dpinewssitemap_generate_newsurl_xml($dom, &$url, $namespaces, $node, $site_infos, $settings) {
  dpinewssitemap_generate_newsurl_xml_set_commons($dom, $url, $node, $site_infos, $settings);

  $main_news = $url->appendChild($dom->createElementNS($namespaces['news'], 'news:news'));
  dpinewssitemap_generate_newsurl_xml_set_main_news($dom, $main_news, $namespaces['news'], $node, $site_infos, $settings);

  if ($settings['dpinewssitemap_add_images']) {
    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      dpinewssitemap_generate_newsurl_xml_set_images($dom, $url, $namespaces['image'], $node, $site_infos, $settings);
    }
  }

  if ($settings['dpinewssitemap_add_videos']) {
    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      dpinewssitemap_generate_newsurl_xml_set_videos($dom, $url, $namespaces['video'], $node, $site_infos, $settings);
    }
  }
}

function dpinewssitemap_generate_newsurl_xml_set_commons($dom, &$url, $node, $site_infos, $settings) {
  $global_sitemap_settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['dpisitemap_base_url']) ? $global_sitemap_settings['dpisitemap_base_url'] : $site_infos['base_url'];

  $url->appendChild($dom->createElement('loc', $ref_base_url.'/'.$node->path));
  if (isset($node->field_destinations) && !empty($node->field_destinations)) {
    $main_dest = dpidestinations_api_get_main_destination_from_node($node);
    $tid = $main_dest['tid'];
    $dest_settings = $global_sitemap_settings['dpisitemap_dests_params'] ? $global_sitemap_settings['dpisitemap_dests_params'] : array();
    $term_settings = $dest_settings[$tid] ? $dest_settings[$tid] :
    array(
      'check' => 1,
      'weight' => 0,
      'freq' => 'daily',
    );
    if (isset($settings['dpinewssitemap_nodes_freqs']) && isset($settings['dpinewssitemap_nodes_freqs'][$tid]) && isset($settings['dpinewssitemap_nodes_freqs'][$tid]['freq']) && !empty($settings['dpinewssitemap_nodes_freqs'][$tid]['freq'])) {
      $term_settings['freq'] = $settings['dpinewssitemap_nodes_freqs'][$tid]['freq'];
    }
    if ($term_settings['check']) {
      if (isset($node->field_editorialupdatedate)) {
        $editorial_update = field_get_items('node', $node, 'field_editorialupdatedate');
        $lastmod_date = $editorial_update[0]['value'];
      } else {
        $lastmod_date = $node->changed;
      }
      $url->appendChild($dom->createElement('lastmod', date('Y-m-d\TH:i:sP', $lastmod_date)));
      $url->appendChild($dom->createElement('changefreq', $term_settings['freq']));
      $url->appendChild($dom->createElement('priority', $term_settings['weight']));
    }
  } else {
    $url->appendChild($dom->createElement('lastmod', date('Y-m-d\TH:i:sP', $node->changed)));
    $url->appendChild($dom->createElement('changefreq', $settings['dpinewssitemap_default_freq']));
    // priority not handled yet
  }

  $alter_args = array(
    'node' => $node,
    'main_settings' => $global_sitemap_settings,
    'news_settings' => $settings,
  );
  dpisitemap_api_call_url_alter($dom, $url, 'news', $alter_args);
}

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=74288&topic=10078&ctx=topic
 *
 * Output example :
 * <news:news>
 *   <news:publication> // required
 *     <news:name>The Example Times</news:name>
 *     <news:language>en</news:language>
 *   </news:publication>
 *   <news:access>Subscription</news:access> // required if not open
 *   <news:genres>PressRelease, Blog</news:genres> // required if applies
 *   <news:publication_date>2008-12-23</news:publication_date> // required
 *   <news:title>Companies A, B in Merger Talks</news:title>
 *   <news:geo_location>Etterbeek, Belgium</news:geo_location>
 *   <news:keywords>business, merger, acquisition, A, B</news:keywords>
 *   <news:stock_tickers>NASDAQ:A, NASDAQ:B</news:stock_tickers>
 * </news:news>
 */
function dpinewssitemap_generate_newsurl_xml_set_main_news($dom, &$main_news, $newsNS, $node, $site_infos, $settings) {
  $news_pub = $main_news->appendChild($dom->createElementNS($newsNS, 'news:publication'));
  $news_pub->appendChild($dom->createElementNS($newsNS, 'news:name', $settings['dpinewssitemap_publication_name']));
  $news_pub->appendChild($dom->createElementNS($newsNS, 'news:language', $site_infos['language']));
  if (isset($node->field_freeaccess[0]['value']) && $node->field_freeaccess[0]['value'] == 'No') {
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:access', 'Subscription'));
  }
  if (isset($node->field_sitemap_genre)) {
    $sitemap_genres = array();
    foreach (field_get_items('node', $node, 'field_sitemap_genre') as $node_sitemap_genre) {
      $term_sitemap_genre = taxonomy_get_term($node_sitemap_genre['value']);
      $sitemap_genres[] = $term_sitemap_genre->name;
    }
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:genres', implode(', ', $sitemap_genres)));
  }
  if (isset($node->field_publicationdate)) {
    $field_pub_date = field_get_items('node', $node, 'field_publicationdate');
    $form_date = date_make_date($field_pub_date[0]['value'], 'UTC');
    $form_date = (object)date_timezone_set($form_date, timezone_open($field_pub_date[0]['timezone']));
    $form_date = unserialize(serialize($form_date));
    $publication_date = strtotime($form_date->date);
  } else {
    $publication_date = $node->created;
  }
  $main_news->appendChild($dom->createElementNS($newsNS, 'news:publication_date', date('Y-m-d\TH:i:sP', $publication_date)));
  $main_news->appendChild($dom->createElementNS($newsNS, 'news:title', $node->title));
  // @todo : Equivalent DPI7 du field_markers ?
  /*
  $sitemap_geoloc = array();
  if (isset($node->field_markers[0]['city']) && !empty($node->field_markers[0]['city'])) {
    $sitemap_geoloc[] = $node->field_markers[0]['city'];
  }
  if (isset($node->field_markers[0]['province_name']) && !empty($node->field_markers[0]['province_name'])) {
    $sitemap_geoloc[] = $node->field_markers[0]['province_name'];
  }
  if (isset($node->field_markers[0]['country_name']) && !empty($node->field_markers[0]['country_name'])) {
    $sitemap_geoloc[] = $node->field_markers[0]['country_name'];
  }
  if (!empty($sitemap_geoloc)) {
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:geo_location', implode(', ', $sitemap_geoloc)));
  }
  */
  $sitemap_tags = array();
  foreach ($settings['dpinewssitemap_tags_fields'] as $tag_field) {
    if (isset($node->{$tag_field})) {
      foreach (field_get_items('node', $node, $tag_field) as $node_tag) {
        $term_tag = taxonomy_get_term($node_tag['value']);
        $sitemap_tags[] = $term_tag->name;
      }
    }
  }
  if (!empty($sitemap_tags)) {
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:keywords', implode(', ', $sitemap_tags)));
  }
}

function dpinewssitemap_generate_newsurl_xml_set_images($dom, &$url, $imageNS, $node, $site_infos, $settings) {
  if ($embeds = dpicontenttypes_api_get_embeds($node, 'image', FALSE, TRUE)) {
    foreach ($embeds as $embed) {
      $image = $url->appendChild($dom->createElement('image:image'));
      dpinewssitemap_generate_newsurl_xml_set_image($dom, $image, $imageNS, $node, $embed, $site_infos, $settings);
    }
  }
}

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=178636&topic=20986&ctx=topic
 *
 * Output example :
 * <image:image>
 *   <image:loc>http://example.com/photo.jpg</image:loc> // required
 *   <image:caption>Caption of my image</image:caption>
 *   <image:geo_location>Etterbeek, Belgium</image:geo_location>
 *   <image:title>Companies A, B in Merger Talks</image:title>
 *   <image:license>http://www.mylicense.com/hello</image:license>
 * </image:image>
 */
function dpinewssitemap_generate_newsurl_xml_set_image($dom, &$image, $imageNS, $node, $embed, $site_infos, $settings) {
  global $base_url;
  $global_sitemap_settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['dpisitemap_base_url']) ? $global_sitemap_settings['dpisitemap_base_url'] : $site_infos['base_url'];

  $atom = $embed['atom'];
  $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);

  $image_style_url = dpicontenttypes_api_image_style_url($settings['dpinewssitemap_image_style'], $node->nid, $atom->sid, $atom->file_source);
  $image->appendChild($dom->createElementNS($imageNS, 'image:loc', str_replace($base_url, $ref_base_url, $image_style_url)));
  if (isset($atom_wrapper->field_caption)) {
    $field_caption = $atom_wrapper->field_caption->value();
    $image_caption = $field_caption['safe_value'];
  }
  elseif (isset($atom_wrapper->field_displaytitle)) {
    $field_displaytitle = $atom_wrapper->field_displaytitle->value();
    $image_caption = $field_displaytitle['safe_value'];
  }
  else {
    $image_caption = $atom->title;
  }
  $image->appendChild($dom->createElementNS($imageNS, 'image:caption', $image_caption));
  // @todo : Equivalent DPI7 du field_markers ?
  /*
  $sitemap_geoloc = array();
  if (isset($embed->field_markers[0]['city']) && !empty($embed->field_markers[0]['city'])) {
    $sitemap_geoloc[] = $embed->field_markers[0]['city'];
  }
  if (isset($embed->field_markers[0]['province_name']) && !empty($embed->field_markers[0]['province_name'])) {
    $sitemap_geoloc[] = $embed->field_markers[0]['province_name'];
  }
  if (isset($embed->field_markers[0]['country_name']) && !empty($embed->field_markers[0]['country_name'])) {
    $sitemap_geoloc[] = $embed->field_markers[0]['country_name'];
  }
  if (!empty($sitemap_geoloc)) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:geo_location', implode(', ', $sitemap_geoloc)));
  }
  */
  $image->appendChild($dom->createElementNS($imageNS, 'image:title', $atom->title));
  if (!empty($settings['dpinewssitemap_image_license'])) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:license', $settings['dpinewssitemap_image_license']));
  }
}

function dpinewssitemap_generate_newsurl_xml_set_videos($dom, &$url, $videoNS, $node, $site_infos, $settings) {
  if ($embeds = dpicontenttypes_api_get_embeds($node, 'video', FALSE, TRUE)) {
    foreach ($embeds as $embed) {
      $video = $url->appendChild($dom->createElement('video:video'));
      dpinewssitemap_generate_newsurl_xml_set_video($dom, $video, $videoNS, $node, $embed, $site_infos, $settings);
    }
  }
}

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=80472&topic=10079&ctx=topic
 *
 * Output example :
 * <video:video>
 *   <video:thumbnail_loc>http://www.example.com/thumbs/123.jpg</video:thumbnail_loc> // required
 *   <video:title>Grilling steaks for summer</video:title> // required
 *   <video:description>Alkis shows you how to get perfectly done steaks every time</video:description> // required
 *   <video:content_loc>http://www.example.com/video123.flv</video:content_loc> // required (or player_loc)
 *   <video:player_loc allow_embed="yes" autoplay="ap=1">http://www.example.com/videoplayer.swf?video=123</video:player_loc> // required (or content_loc)
 *   <video:duration>600</video:duration>
 *   <video:expiration_date>2009-11-05T19:20:30+08:00</video:expiration_date>
 *   <video:rating>4.2</video:rating>
 *   <video:view_count>12345</video:view_count>
 *   <video:publication_date>2007-11-05T19:20:30+08:00</video:publication_date>
 *   <video:family_friendly>yes</video:family_friendly>
 *   <video:tag>business, merger, acquisition, A, B</video:tag>
 *   <video:category>Cooking</video:category>
 *   <video:restriction relationship="allow">IE GB US CA</video:restriction>
 *   <video:gallery_loc title="Cooking Videos">http://cooking.example.com</video:gallery_loc>
 *   <video:price currency="EUR">1.99</video:price>
 *   <video:requires_subscription>yes</video:requires_subscription>
 *   <video:uploader info="http://www.example.com/users/grillymcgrillerson">GrillyMcGrillerson</video:uploader>
 *   <video:platform>web mobile tv</video:platform>
 *   <video:live>no</video:live>
 * </video:video>
 */
function dpinewssitemap_generate_newsurl_xml_set_video($dom, &$video, $videoNS, $node, $embed, $site_infos, $settings) {
  global $base_url;

  $global_sitemap_settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['dpisitemap_base_url']) ? $global_sitemap_settings['dpisitemap_base_url'] : $site_infos['base_url'];

  $atom = $embed['atom'];
  $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);

  if (isset($atom_wrapper->field_thumbnail)) {
    $video_thumb = dpicontenttypes_api_image_style_url($settings['wallynewssitemap_video_style'], $node->nid, $atom->sid, $embed->field_photofile[0]['filepath']);
    $video_thumb = str_replace($base_url, $ref_base_url, $video_thumb);
  }
  else {
    $default_image_path = $settings['dpinewssitemap_default_img'];
    $video_thumb = dpicontenttypes_api_image_style_url($settings['wallynewssitemap_video_style'], $node->nid, $atom->sid, $default_image_path);
    $video_thumb = str_replace($base_url, $ref_base_url, $video_thumb);
  }
  $video->appendChild($dom->createElementNS($videoNS, 'video:thumbnail_loc', $video_thumb));
  $video->appendChild($dom->createElementNS($videoNS, 'video:title', $atom->title));
  if (isset($atom_wrapper->field_caption)) {
    $field_caption = $atom_wrapper->field_caption->value();
    $video_desc = $field_caption['safe_value'];
  }
  elseif (isset($atom_wrapper->field_displaytitle)) {
    $field_displaytitle = $atom_wrapper->field_displaytitle->value();
    $video_desc = $field_displaytitle['safe_value'];
  }
  else {
    $video_desc = $atom->title;
  }
  $video->appendChild($dom->createElementNS($videoNS, 'video:description', $video_desc));
  if (true) {
    $player_loc = $video->appendChild($dom->createElementNS($videoNS, 'video:player_loc', $embed->field_video3rdparty[0]['embed']));
    $player_loc->setAttribute('allow_embed', $settings['wallynewssitemap_allow_embed']);
    // @todo : gérer l'autoplay
    // @todo : gérer la duration
  }
  else {
    // @todo : comment savoir si c'est un 3rd party ou un embed local?
  }
  // video:expiration_date not handled yet
  // video:rating not handled yet
  // video:view_count not handled yet
  $video->appendChild($dom->createElementNS($videoNS, 'video:publication_date', date('Y-m-d\TH:i:sP', $embed->created)));
  if (isset($atom_wrapper->field_rating)) {
    $rating_term = taxonomy_get_term($atom_wrapper->field_rating->value());
    if ($rating_term->name == 'G') {
      $video->appendChild($dom->createElementNS($videoNS, 'video:family_friendly', 'yes'));
    } else {
      $video->appendChild($dom->createElementNS($videoNS, 'video:family_friendly', 'no'));
    }
  }
  $video_tags = array();
  foreach ($settings['dpinewssitemap_tags_fields'] as $tag_field) {
    if (isset($atom_wrapper->{$tag_field})) {
      foreach ($atom_wrapper->{$tag_field}->value() as $embed_tag) {
        $term_tag = taxonomy_get_term($embed_tag['value']);
        $video_tags[] = $term_tag->name;
      }
    }
  }
  if (!empty($video_tags)) {
    $video->appendChild($dom->createElementNS($videoNS, 'video:tag', implode(', ', $video_tags)));
  }
  // video:category not handled yet
  // video:restriction not handled yet
  // video:gallery_loc not handled yet
  // video:price not handled yet
  // video:requires_subscription not handled yet
  // video:uploader not handled yet
  // video:platform not handled yet
  // video:live not handled yet
}

function dpinewssitemap_new_dom($settings) {
  $namespaces = dpisitemap_api_get_namespaces();

  $dom = new DOMDocument('1.0', 'UTF-8');
  $urlset = $dom->appendChild($dom->createElementNS($namespaces['sitemap'], 'urlset'));
  $urlset->setAttributeNS($namespaces['w3'], 'xmlns:news', $namespaces['news']);
  if ($settings['dpinewssitemap_add_images']) {
    $urlset->setAttributeNS($namespaces['w3'], 'xmlns:image', $namespaces['image']);
  }
  if ($settings['dpinewssitemap_add_videos']) {
    $urlset->setAttributeNS($namespaces['w3'], 'xmlns:video', $namespaces['video']);
  }

  return $dom;
}

/**
 * Snipet de code pour insérer un DOM node dans un autre DOM document
 *
 * $a = '<loc>http://wally.drupal.dev/category/destination-path/home</loc><changefreq>hourly</changefreq><priority>1</priority>';
 $b = '<?xml version="1.0" encoding="UTF-8"?><url>'.$a.'</url>';
 $dom_a = new DomDocument();
 $dom_a->formatOutput = true;
 $dom_a->loadXML($b);
 dsm($dom_a->saveXML($dom_a));
 $dom = new DomDocument();
 $dom->formatOutput = true;
 $c = $dom_a->getElementsByTagName('url')->item(0);
 $url = $dom->appendChild($dom->importNode($c, true));
 dsm($dom->saveXML($dom));
 *
 */
