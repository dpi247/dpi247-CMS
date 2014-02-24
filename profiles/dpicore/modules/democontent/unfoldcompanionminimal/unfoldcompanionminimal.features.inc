<?php
/**
 * @file
 * unfoldcompanionminimal.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function unfoldcompanionminimal_image_default_styles() {
  $styles = array();

  // Exported image style: 120x120.
  $styles['120x120'] = array(
    'name' => '120x120',
    'label' => '120x120',
    'effects' => array(
      1 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 743,
          'height' => 430,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      2 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 743,
          'height' => 430,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 1440x1080.
  $styles['1440x1080'] = array(
    'name' => '1440x1080',
    'label' => '1440x1080',
    'effects' => array(
      5 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 1440,
          'height' => 1080,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      6 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 1440,
          'height' => 1080,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 241x150.
  $styles['241x150'] = array(
    'name' => '241x150',
    'label' => '241x150',
    'effects' => array(
      7 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 241,
          'height' => 150,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      8 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 241,
          'height' => 150,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 300x300.
  $styles['300x300'] = array(
    'name' => '300x300',
    'label' => '300x300',
    'effects' => array(
      9 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 300,
          'height' => 300,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      10 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 300,
          'height' => 300,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 366x228.
  $styles['366x228'] = array(
    'name' => '366x228',
    'label' => '366x228',
    'effects' => array(
      11 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 366,
          'height' => 228,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      12 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 366,
          'height' => 228,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 500x500.
  $styles['500x500'] = array(
    'name' => '500x500',
    'label' => '500x500',
    'effects' => array(
      13 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 500,
          'height' => 500,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      14 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 500,
          'height' => 500,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 70x70.
  $styles['70x70'] = array(
    'name' => '70x70',
    'label' => '70x70',
    'effects' => array(
      15 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 70,
          'height' => 70,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      16 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 70,
          'height' => 70,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: 743x430.
  $styles['743x430'] = array(
    'name' => '743x430',
    'label' => '743x430',
    'effects' => array(
      3 => array(
        'label' => 'DPI Crop',
        'help' => 'Crop an image using user-defined dimensions, following a given ratio. The croping values for a given image can vary in every package it is used.',
        'effect callback' => 'dpicontenttypes_dpicrop_effect',
        'form callback' => 'dpicontenttypes_dpicrop_form',
        'summary theme' => 'dpicontenttypes_dpicrop_summary',
        'module' => 'dpicontenttypes',
        'name' => 'dpicontenttypes_dpicrop',
        'data' => array(
          'width' => 743,
          'height' => 430,
          'xoffset' => 'center_max',
          'yoffset' => 'center_max',
        ),
        'weight' => 1,
      ),
      4 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 743,
          'height' => 430,
          'upscale' => 1,
        ),
        'weight' => 2,
      ),
    ),
  );

  return $styles;
}
