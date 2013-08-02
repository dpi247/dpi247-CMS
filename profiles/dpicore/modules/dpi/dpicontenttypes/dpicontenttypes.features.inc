<?php
/**
 * @file
 * etest.features.inc
 */



/**
 * Implements hook_node_info().
 *
 * We use hook_node_info() to define our node content type.
 */

function dpicontenttypes_node_info(){
  return array(
    'article_package' => array(
      'name' => t("Article Package"),
      'base' => 'article_package',
      'description' => t('Article package from DPI'),
      'has_title' => 1,
      'title_label' => 'Title',      
      'help'=>""
    ),
    'gallery_package' => array(
      'name' => t("Gallery Package"),
      'base' => 'galery_package',
      'description' => t('Article package from DPI'),
      'has_title' => 1,
      'title_label' => 'Title',      
      'help'=>""
    ),
    'poll_package' => array(
      'name' => t("Poll Package"),
      'base' => 'poll_package',
      'description' => t('Article package from DPI'),
      'has_title' => 1,
      'title_label' => 'Title',      
      'help'=>""
    ),
   
  );
}

