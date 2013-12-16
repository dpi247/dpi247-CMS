<?php

/**
 * Returns HTML for an image using a specific image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - style_name: The name of the style to be used to alter the original image.
 *   - path: The path of the image file relative to the Drupal files directory.
 *     This function does not work with images outside the files directory nor
 *     with remotely hosted images. This should be in a format such as
 *     'images/image.jpg', or using a stream wrapper such as
 *     'public://images/image.jpg'.
 *   - entity: Array containing informations (type, bundle, id and vid) about the current entity
 *   - width: The width of the source image (if known).
 *   - height: The height of the source image (if known).
 *   - alt: The alternative text for text-based browsers.
 *   - title: The title text is displayed when the image is hovered in some
 *     popular browsers.
 *   - attributes: Associative array of attributes to be placed in the img tag.
 *
 * @ingroup themeable
 */
function theme_dpicontenttypes_image_style($variables) {
  // Determine the dimensions of the styled image.
  $dimensions = array(
    'width' => $variables['width'],
    'height' => $variables['height'],
  );

  image_style_transform_dimensions($variables['style_name'], $dimensions);

  $variables['width'] = $dimensions['width'];
  $variables['height'] = $dimensions['height'];

  $entity = $variables['entity'];
  $entity_id = (isset($entity['type']) && isset($entity['id'])) ? $entity['type'].'_'.$entity['id'] : 'default';
  $atom_id = $variables['atom']->sid;
  // Determine the URL for the styled image.
  $variables['path'] = dpicontenttypes_api_image_style_url($variables['style_name'], $entity_id, $atom_id, $variables['path']);
  return theme('image', $variables);
}

function dpicontenttypes_preprocess_dpicontenttypes_image_render_context(&$variables, $base_theme) {
  if (isset($variables['context'])) {
    $variables['theme_hook_suggestions'][] = $base_theme.'__'.str_replace('-', '_', $variables['context']);
  }
}
