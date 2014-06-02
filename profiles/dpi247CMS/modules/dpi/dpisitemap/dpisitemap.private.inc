<?php

function _dpisitemap_get_or_create_index(&$dom, $sitemapNS, $index_path) {
  if (file_exists($index_path)) {
    $dom->load($index_path);
  } else {
    $dom->appendChild($dom->createElementNS($sitemapNS, 'sitemapindex'));
  }
}

function _dpisitemap_update_entry_in_index(&$dom_index, &$dom_sitemapindex, $path) {
  $found = FALSE;
  $dom_sitemaps = $dom_index->getElementsByTagName('sitemap');
  if ($dom_sitemaps->item(0)) {
    foreach ($dom_sitemaps as $dom_sitemap) {
      $sitemap_loc = $dom_sitemap->getElementsByTagName('loc')->item(0)->nodeValue;
      if ($sitemap_loc == $path) {
        $found = TRUE;
        $dom_old_lastmod = $dom_sitemap->getElementsByTagName('lastmod')->item(0);
        $dom_old_lastmod->nodeValue = date('Y-m-d\TH:i:sP');
      }
    }
  }
  if (!$found) {
    $dom_new_sitemap = $dom_sitemapindex->appendChild($dom_index->createElement('sitemap'));
    $dom_new_sitemap->appendChild($dom_index->createElement('loc', $path));
    $dom_new_sitemap->appendChild($dom_index->createElement('lastmod', date('Y-m-d\TH:i:sP')));
  }
}

function _dpisitemap_check_index(&$dom_index) {
  global $base_url;
  $settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
  $ref_base_url = isset($settings['dpisitemap_base_url']) ? $settings['dpisitemap_base_url'] : $base_url;

  $dom_sitemapindex = $dom_index->getElementsByTagName('sitemapindex')->item(0);
  $dom_sitemaps = $dom_index->getElementsByTagName('sitemap');
  if ($dom_sitemaps->item(0)) {
    foreach ($dom_sitemaps as $dom_sitemap) {
      $sitemap_loc = $dom_sitemap->getElementsByTagName('loc')->item(0)->nodeValue;
      $local_path = str_replace($ref_base_url.'/', '', $sitemap_loc);
      if (!file_exists($local_path)) {
        $dom_sitemapindex->removeChild($dom_sitemap);
      }
    }
  }
}

function _dpisitemap_delete_entry_in_index(&$dom_index, &$dom_sitemapindex, $path) {
  global $base_url;
  $settings = dpi_variable_get('dpisitemap_sitemaps_settings', array());
  $ref_base_url = isset($settings['dpisitemap_base_url']) ? $settings['dpisitemap_base_url'] : $base_url;

  $dom_sitemapindex = $dom_index->getElementsByTagName('sitemapindex')->item(0);
  $dom_sitemaps = $dom_index->getElementsByTagName('sitemap');
  if ($dom_sitemaps->item(0)) {
    foreach ($dom_sitemaps as $dom_sitemap) {
      $sitemap_loc = $dom_sitemap->getElementsByTagName('loc')->item(0)->nodeValue;
      if ($sitemap_loc == $path || $sitemap_loc == $ref_base_url.'/'.$path) {
        $dom_sitemapindex->removeChild($dom_sitemap);
        break;
      }
    }
  }
}
