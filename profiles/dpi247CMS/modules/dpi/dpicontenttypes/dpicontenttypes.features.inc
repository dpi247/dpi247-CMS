<?php
/**
 * @file
 * dpicontenttypes.features.inc
 */

/**
 * Implements hook_node_info().
 *
 * We use hook_node_info() to define our node content type.
 */
function dpicontenttypes_node_info(){
  return array(
    'package' => array(
      'name' => t("DPI Package"),
      'base' => 'package',
      'description' => t('A Package from DPI'),
      'has_title' => 1,
      'title_label' => 'Title',      
      'help'=>""
    ),
    'package_gallery' => array(
      'name' => t("DPI Package gallery"),
      'base' => 'package',
      'description' => t('A package-gallery from DPI'),
      'has_title' => 1,
      'title_label' => 'Package gallery',
      'help'=>""
    ),
  );
}
