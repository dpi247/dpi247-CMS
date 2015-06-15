<?php

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=183668&topic=8476&ctx=topic
 */
function dpisitemap_generate_sitemap_xml($site_infos, $settings) {
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');

  $namespaces = dpisitemap_api_get_namespaces();
  $dom = new DOMDocument('1.0', 'UTF-8');
  $urlset = $dom->appendChild($dom->createElementNS($namespaces['sitemap'], 'urlset'));

  $dest_terms = taxonomy_get_tree(dpidestinations_api_get_sections_vocabulary_vid());
  dpisitemap_generate_sitemap_xml_for_taxonomy($dom, $urlset, $site_infos, $settings, $dest_terms);

  $menus = $settings['dpisitemap_handled_menus'];
  foreach ($menus as $menu_name => $menu_settings) {
    if ($menu_settings['check']) {
      $menu_infos = menu_tree_all_data($menu_name, NULL);
      dpisitemap_generate_sitemap_xml_for_menu($dom, $urlset, $site_infos, $settings, $menu_infos, $menu_settings);
    }
  }

  $context = array('main');
  drupal_alter('dpisitemap_dom', $dom, $context);

  $xmlfiles_to_generate = array(array(
    'dom' => $dom,
    'filename' => 'sitemapmain.xml',
  ));

  dpisitemap_api_generate_xmlfile($xmlfiles_to_generate, $settings['dpisitemap_path']);
}

function dpisitemap_generate_sitemap_xml_for_menu($dom, &$urlset, $site_infos, $settings, $menu_infos, $menu_settings) {
  global $base_url;
  $ref_base_url = $ref_base_url = isset($settings['dpisitemap_base_url']) ? $settings['dpisitemap_base_url'] : $base_url;

  $processed_dest_tids = &dpisitemap_get_static_processed_dest_tids();
  foreach ($menu_infos as $menu_item) {
    if (is_array($menu_item['below'])) {
      dpisitemap_generate_sitemap_xml_for_menu($dom, $urlset, $site_infos, $settings, $menu_item['below'], $menu_settings);
    }

    if (!$menu_item['link']['external']) {
      $menu_item_aliases = dpicommons_get_all_aliases($menu_item['link']['link_path']);
      $menu_item_aliases_strings = array();
      foreach ($menu_item_aliases as $alias) {
        $menu_item_aliases_strings[] = $alias->alias;
      }
      if (array_intersect($menu_item_aliases_strings, $settings['dpisitemap_blacklist'])) {
        continue;
      }

      if ($menu_item['link']['router_path'] == 'taxonomy/term/%') {
        $expl_path = explode('/', $menu_item['link']['link_path']);
        $tid = $expl_path[2];
        $term = taxonomy_get_term($tid);
        $dest_vid = dpidestinations_api_get_sections_vocabulary_vid();
        if ($term->vid == $dest_vid || in_array($tid, $processed_dest_tids)) {
          continue;
        } else {
          $processed_dest_tids[] = $tid;
        }
      }

      if ($menu_item['link']['link_path'] == '<front>') {
        $link_path = '';
      } else {
        $link_path = $menu_item['link']['link_path'];
      }

      $url = $urlset->appendChild($dom->createElement('url'));
      $nice_path = dpicommons_get_main_alias($link_path);
      $url->appendChild($dom->createElement('loc', $ref_base_url.'/'.$nice_path[0]->alias));
      // lastmod not handled yet
      $url->appendChild($dom->createElement('changefreq', $menu_settings['freq']));
      $url->appendChild($dom->createElement('priority', $menu_settings['weight']));

      $alter_args = array(
        'menu_item' => $menu_item,
        'settings' => $settings,
      );
      dpisitemap_api_call_url_alter($dom, $url, 'menu', $alter_args);
    }
  }
}

function dpisitemap_generate_sitemap_xml_for_taxonomy($dom, &$urlset, $site_infos, $settings, $terms) {
  global $base_url;
  $ref_base_url = $ref_base_url = isset($settings['dpisitemap_base_url']) ? $settings['dpisitemap_base_url'] : $base_url;

  $processed_dest_tids = &dpisitemap_get_static_processed_dest_tids();
  $terms_settings = $settings['dpisitemap_dests_params'] ? $settings['dpisitemap_dests_params'] : array();
  foreach ($terms as $term) {

		$uri=entity_uri('taxonomy_term', $term);

		dsm($term);
		$dest_term_aliases = dpicommons_get_all_aliases($uri['path']);
    $dest_term_aliases_strings = array();
    foreach ($dest_term_aliases as $alias) {
      $dest_term_aliases_strings[] = $alias->alias;
    }
    if (array_intersect($dest_term_aliases_strings, $settings['dpisitemap_blacklist'])) {
      continue;
    }

    $term_settings = $terms_settings[$term->tid] ?
    $terms_settings[$term->tid] :
    array(
      'check' => 1,
      'weight' => 0,
      'freq' => 'daily',
    );
    if (!$term_settings['check'] || in_array($term->tid, $processed_dest_tids)) {
      continue;
    } else {
      $processed_dest_tids[] = $term->tid;
    }
    $url = $urlset->appendChild($dom->createElement('url'));
    $nice_path = dpicommons_get_main_alias($uri['path']);
    $url->appendChild($dom->createElement('loc', $ref_base_url.'/'.$nice_path[0]->alias));
    // lastmod not handled yet
    $url->appendChild($dom->createElement('changefreq', $term_settings['freq']));
    $url->appendChild($dom->createElement('priority', $term_settings['weight']));

    $alter_args = array(
      'term' => $term,
      'settings' => $settings,
    );
    dpisitemap_api_call_url_alter($dom, $url, 'destination', $alter_args);
  }
}

function &dpisitemap_get_static_processed_dest_tids() {
  static $processed_dest_tids = array();
  return $processed_dest_tids;
}
