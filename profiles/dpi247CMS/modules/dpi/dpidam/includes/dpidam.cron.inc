<?php

/**
 * Cron task to post packages to be archived in the DAM
 */
function dpidam_post_packages_cron() {
  return;
  $max_number = dpi_variable_get('dpidam_cronmaxnumber', 50);

  // Packages
  print '<strong>'.t('Post packages :').'</strong><br>';
  $db_packages = db_select('dpidam_packagestosend', 'dp')
    ->fields('dp', array('nid'))
    ->orderBy('dp.timestamp', 'ASC')
    ->range(0, $max_number)
    ->execute();
  foreach ($db_packages as $obj) {
    $post_result = _dpidam_post_package($obj->nid);
    if ($post_result['success']) {
      _dpidam_remove_package_from_buffer($obj->nid);
      print t('Posted package NID @nid', array('@nid' => $obj->nid));
    } else {
      print t('Post package NID @nid failed. Error message : @message', array('@nid' => $obj->nid, '@message' => $post_result['message']));
    }
    print '<br>';
  }
}

/**
 * Cron task to post URLs to be updated in the DAM
 */
function dpidam_post_urls_cron() {
  return;
  $max_number = dpi_variable_get('dpidam_cronmaxnumber', 50);

  // URL
  print '<strong>'.t('Post URL :').'</strong><br>';
  $db_urls = db_select('dpidam_urltosend', 'du')
    ->fields('du', array('nid', 'url'))
    ->orderBy('du.timestamp', 'ASC')
    ->range(0, $max_number)
    ->execute();
  foreach ($db_urls as $obj) {
    $node = node_load($obj->nid);
    $post_result = _dpidam_post_url($node, $obj->url);
    if ($post_result['success']) {
      _dpidam_remove_url_from_buffer($obj->nid);
      print t('Posted URL NID @nid', array('@nid' => $obj->nid));
    } else {
      print t('Post URL NID @nid failed. Error message : @message', array('@nid' => $obj->nid, '@message' => $post_result['message']));
    }
    print '<br>';
  }
}
