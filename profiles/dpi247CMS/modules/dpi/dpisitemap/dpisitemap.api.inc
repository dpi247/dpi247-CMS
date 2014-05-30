<?php

/**
 * Generate XML file
 */
function dpisitemap_api_generate_xmlfile($xmlfiles_to_generate, $folder_path) {
  // @todo : handle gzip compression of XMLs

  $scheme = 'public';
  if (file_stream_wrapper_valid_scheme($scheme)) {
    if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://'.$folder_path)) {
      $folder_path = $wrapper->getDirectoryPath().'/'.$folder_path;
    }
    else {
      return FALSE;
    }
  }

  global $base_url;
  $settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
  $ref_base_url = isset($settings['dpisitemap_base_url']) ? $settings['dpisitemap_base_url'] : $base_url;

  $sitemaps_full_path = array();
  foreach ($xmlfiles_to_generate as $xmlfile_to_generate) {
    $sitemap_local_path = $folder_path.'/'.$xmlfile_to_generate['filename'];
    $sitemaps_full_path[] = $ref_base_url.'/'.$sitemap_local_path;
    $xmlfile_to_generate['dom']->save($sitemap_local_path);
  }

  dpisitemap_api_update_index($sitemaps_full_path, $folder_path);
}

function dpisitemap_api_update_index($paths, $folder_path) {
  $namespaces = dpisitemap_api_get_namespaces();

  $index_local_path = $folder_path.'/sitemapindex.xml';

  $dom_index = new DOMDocument('1.0', 'UTF-8');
  _dpisitemap_get_or_create_index($dom_index, $namespaces['sitemap'], $index_local_path);
  $dom_sitemapindex = $dom_index->getElementsByTagName('sitemapindex')->item(0);

  foreach ($paths as $path) {
    _dpisitemap_update_entry_in_index($dom_index, $dom_sitemapindex, $path);
  }

  _dpisitemap_check_index($dom_index);

  $dom_index->save($index_local_path);
}

function dpisitemap_api_get_namespaces() {
  return array(
    'w3' => 'http://www.w3.org/2000/xmlns/',
    'sitemap' => 'http://www.sitemaps.org/schemas/sitemap/0.9',
    'news' => 'http://www.google.com/schemas/sitemap-news/0.9',
    'image' => 'http://www.google.com/schemas/sitemap-image/1.1',
    'video' => 'http://www.google.com/schemas/sitemap-video/1.1',
  );
}