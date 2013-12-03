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
    'form callback' => 'dpicontenttypes_dpicrop_form',
    'summary theme' => 'dpicontenttypes_dpicrop_summary',
  );

  return $effects;
}

/**
 * Returns HTML for a summary of an image DPI Crop effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this DPI Crop effect.
 *
 * @ingroup themeable
 */
function theme_dpicontenttypes_dpicrop_summary($variables) {
  $data = $variables['data'];

  $output = check_plain($data['width']) . 'x' . check_plain($data['height']);
  $output .= ', Ratio : '.round($data['width']/$data['height'], 2);
  $output .= ', X offset : '.$data['xoffset'];
  $output .= ', Y offset : '.$data['yoffset'];
  $output = '('.$output.')';

  return $output;
}

/**
 * Settings form for the DPI Crop image effect
 *
 * @param $data
 *   The current configuration for this resize effect.
 */
function dpicontenttypes_dpicrop_form($data = array()) {
  drupal_set_message(t('These values are used as default values as long as the croping has not been edited for a given image.<br>
      However, the ratio <em>width/height</em> will remains unchanged for all cropings using this action.'));

  $data += array(
    'width' => '',
    'height' => '',
    'xoffset' => '',
    'yoffset' => '',
  );

  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $data['width'],
    '#description' => t('Enter a width in pixels. i.e. 500.<br>
        The width and height values are only used to fix a ratio for the croping.'),
    '#required' => TRUE,
  );

  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $data['height'],
    '#description' => t('Enter a height in pixels. i.e. 500.<br>
        The width and height values are only used to fix a ratio for the croping.'),
    '#required' => TRUE,
  );

  $form['xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('X offset'),
    '#default_value' => $data['xoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, <em>center_max</em>,or <em>right</em>.'),
    '#required' => TRUE,
  );

  $form['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Y offset'),
    '#default_value' => $data['yoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, <em>center_max</em>, or <em>bottom</em>.'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Validate function for the DPI Crop image effect settings form
 */
function dpicontenttypes_dpicrop_form_validate($form, $form_state, $data) {
  $width = $data['width'];
  if (!is_numeric($width) || $width <= 0 || floor($width) != $width) {
    form_set_error('data][width', t('The width must be a numerical integer value greater than 0.'));
  }

  $height = $data['height'];
  if (!is_numeric($height) || $height <= 0 || floor($height) != $height) {
    form_set_error('data][height', t('The height must be a numerical integer value greater than 0.'));
  }

  $x_allowed_tokens = array('left', 'right', 'center', 'center_max');
  $x = $data['xoffset'];
  if (!in_array($x, $x_allowed_tokens) && (!is_numeric($x) || $x <= 0 || floor($x) != $x)) {
    form_set_error('data][xoffset', t('The X offset must be a numerical integer value greater than 0, or one of the allowed keywords (<em>left</em>, <em>right</em>, <em>center</em> or <em>center_max</em>).'));
  }

  $y_allowed_tokens = array('top', 'bottom', 'center', 'center_max');
  $y = $data['yoffset'];
  if (!in_array($y, $y_allowed_tokens) && (!is_numeric($y) || $y <= 0 || floor($y) != $y)) {
    form_set_error('data][yoffset', t('The Y offset must be a numerical integer value greater than 0, or one of the allowed keywords (<em>bottom</em>, <em>top</em>, <em>center</em> or <em>center_max</em>).'));
  }
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
  if (isset($data['entity']) && isset($data['atom'])) {
    $entity = $data['entity'];
    $atom = $data['atom'];
    $style = $data['style'];
    list($eid, $revision, $bundle) = entity_extract_ids($entity->entity_type, $entity);

    $cropings = _dpicontenttypes_get_saved_cropings_for_entity_and_atom($entity->entity_type, $eid, $atom->sid, $style['name']);
    if ($cropings && is_array($cropings) && !empty($cropings)) {
      $crop = $cropings;
      $data['xoffset'] = $crop[0];
      $data['yoffset'] = $crop[1];
      $data['width'] = $crop[2];
      $data['height'] = $crop[3];
    }

    if ($data['xoffset'] === 'center_max' || $data['yoffset'] === 'center_max') {
      // If the effect has a "center_max" value as parameter, we have to maximise the crop
      if (image_scale_and_crop($image, $data['width'], $data['height'])) {
        return TRUE;
      }
    } else {
      // Simply apply the crop effect
      if (!is_numeric($data['xoffset'])) {
        $data['xoffset'] = image_filter_keyword($data['xoffset'], $image->info['width'], $data['width']);
      }
      if (!is_numeric($data['yoffset'])) {
        $data['yoffset'] = image_filter_keyword($data['yoffset'], $image->info['height'], $data['height']);
      }
      if (image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
        return TRUE;
      }
    }

    watchdog('image', 'Image dpicrop failed using the %toolkit toolkit on %path (%mimetype, %dimensions)', array('%toolkit' => $image->toolkit, '%path' => $image->source, '%mimetype' => $image->info['mime_type'], '%dimensions' => $image->info['width'] . 'x' . $image->info['height']), WATCHDOG_ERROR);
  }

  return FALSE;
}
