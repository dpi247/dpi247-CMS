<?php
/**
 * @file
 * dpicontenttypes.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dpicontenttypes_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "scald" && $api == "context_config") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dpicontenttypes_views_api() {
  return array("api" => "3.0");
}
