<?php 

/**
 * Generate HTML tag for meta tags.
 */
function pub_get_meta() {
  if (!isset($metas)) $metas = pub_add_meta();
  if (empty($metas)) return '';
  $output = '';
  foreach ($metas as $meta_k => $meta) {
    if (!$meta) {
      continue;
    }
    $output .= "<meta ";
		foreach($meta as $data_k => $data_v) {
			$output .= " ".$data_k."=\"".$data_v."\"";
		}
    $output .= " />\n";
  }
  return $output;
}

/**
 * Adds a META TAG to the process template queue.
 *
 * @param $attributes
 *   Array of meta key keyword & key value.
 *   If key keyword is one of "http-equiv, name, property"
 *   the Meta is updated in case of 2 calls.
 * @param $content
 *   Content of the meta tag.
 *
 * @return
 * 	Return an array of meta.
 */
function pub_add_meta($attributes = array(), $content = NULL) {
  static $metas = array();
  $meta_key = NULL;

  // Supported "Meta Keys" are : http-equiv, name, property.
  if (is_array($attributes)) {
    $content = strip_tags($content);

    foreach ($attributes as $attribute_name => $attribute_value) {
      switch (strtolower($attribute_name)) {
        case 'name':
        case 'http-equiv':
        case 'property':
          $meta_key = strtolower($attribute_name).'_'.$attribute_value;
          $metas[$meta_key] = array($attribute_name => $attribute_value, 'content' => $content);
          break;
        default:
          $metas[] = array($attribute_name => $attribute_value, 'content' => $content);
          break;
      }
    }
  }
  return $metas;
}