<?php
/**
 * @file
 * dpicontenttypes.context_config.inc
 */

/**
 * Implements hook_default_scald_context_config().
 */
function dpicontenttypes_default_scald_context_config() {
  $export = array();

  $context_config = new stdClass();
  $context_config->disabled = FALSE; /* Edit this to true to make a default context_config disabled initially */
  $context_config->api_version = 1;
  $context_config->context = 'sdl_library_item';
  $context_config->transcoder = array(
    'image' => array(
      '*' => 'style-Library',
    ),
  );
  $context_config->player = array(
    'image' => array(
      '*' => 'default',
    ),
  );
  $export['sdl_library_item'] = $context_config;

  return $export;
}
