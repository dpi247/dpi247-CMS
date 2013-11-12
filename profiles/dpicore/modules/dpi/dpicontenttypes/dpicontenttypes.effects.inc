<?php

/**
 * Implementation of hook_image_effect_info
 * Defines DPICrop and DPICropCopy effects
 */
function dpicontenttypes_image_effect_info() {
  $effects = array();

  $effects['dpicontenttypes_dpicrop'] = array(
    'label' => t('DPI Crop'),
    'help' => t('Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.'),
    'effect callback' => 'dpicontenttypes_dpicrop_effect',
    //'dimensions callback' => 'dpicontenttypes_dpicrop_dimensions',
    //'form callback' => 'dpicontenttypes_dpicrop_form',
    //'summary theme' => 'dpicontenttypes_dpicrop_summary',
  );

  return $effects;
}

/**
 * Image effect callback; Apply DPI Crop effect on an image resource.
 *
 * @param $image
 *   An image object returned by image_load().
 * @param $data
 *   An array of attributes to use when performing the DPI Crop effect.
 *
 * @return
 *   TRUE on success. FALSE on failure to resize image.
 */
function dpicontenttypes_dpicrop_effect(&$image, $data) {
  dsm(func_get_args());
}
