<?php

/**
 * Extract attributes from a CCI Inline Object tag
 */
function _cciinlineobjects_inline_objects_get_tag_values($raw_attributes) {
  $tag_values = array();
  $attr_regex = '@\w.*\s?=\s?[\'"].*[\'"]@U';
  $attr_matches = array();
  preg_match_all($attr_regex, $raw_attributes, $attr_matches);

  foreach ($attr_matches[0] as $raw_attr) {
    $expl_attr = explode('=', $raw_attr);
    $tag_values[$expl_attr[0]] = trim($expl_attr[1], '\'"');
  }

  return $tag_values;
}
