<?php

/**
 * Get informations about the site which are necessary to build the sitemap.
 */
function _dpisitemap_get_site_infos() {
  global $language, $base_url;
  return array(
    'language' => $language->language,
    'base_url' => $base_url,
  );
}
