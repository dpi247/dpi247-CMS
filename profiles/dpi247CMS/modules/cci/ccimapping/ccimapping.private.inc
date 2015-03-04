<?php

/**
 * Get the mapped name of a given content type
 */
function _ccimapping_type_converter($type) {
  $mappings = dpi_variable_get('ccimapping_type', '');
  $mappings = explode('\n', $mappings);
  foreach ($mappings as $mapping) {
    $temp = explode('|', $mapping);
    $correct_mapping[$temp[0]] = $temp[1];
  }
  if(array_key_exists($type, $correct_mapping)) {
    return $correct_mapping[$type];
  } else {
    return $type;
  }
}

/**
 * Convert marker (auto-closing tags) to regular tags.
 * We have to do this because CKEditor doesn't handle markers well
 */
function _ccimapping_convert_marker_to_tags($text) {
  $regex = "@<(cci.*)/?>@U";
  $tag_regex = "@(\S)*\s@U";
  $matches = $tag_matches = array();
  preg_match_all($regex, $text, $matches);
  foreach ($matches[0] as $key => $match) {
    if (substr($match, -2, 1) == '/') {
      $tag_content = trim($matches[1][$key]); // This is the full tag contents, tag + attributes
      preg_match_all($tag_regex, $tag_content, $tag_matches);
      if (empty($tag_matches[0])) {
        $tag = $tag_content;
      } else {
        $tag = trim($tag_matches[0][0]); // This is only the tag
      }
      $text = str_replace($match, '<'.$tag_content.'></'.$tag.'>', $text);
    }
  }
  return $text;
}
