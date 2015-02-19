<?php

/**
 * Initialise an array of packages
 */
function dpiblocks_flowhelpers_init_array($feed_infos = array()) {
  $array_packages = array();
  $array_packages['Packages']['Package'] = array();
  $array_packages['Packages']['__log'] = array();
  $array_packages['Packages']['__log']['success'] = TRUE;
  $array_packages['Packages']['__log']['msg'] = '';
  $array_packages['Packages']['__settings'] = $feed_infos;
  return $array_packages;
}

/**
 * Initialise a package object.
 *
 * @return
 *   The initialised object.
 */
function dpiblocks_flowhelpers_new_package() {
  $node = new stdClass();
  $node->type = 'package';
  $node->is_new = TRUE;
  node_object_prepare($node);
  $node->dpiproperties = new DPIPropertiesEntity(array('type' => 'package'), 'dpiproperties');
  $node->__log = array(
    'success' => TRUE,
    'msg' => '',
  );
  $node_wrapper = entity_metadata_wrapper('node', $node);
  return array($node, $node_wrapper);
}

/**
 * Get a Unix timestamp from a date
 *
 * For example, it returns 1371194441 for the date "2013-06-14T09:20:41+02:00"
 *
 * @param string $date
 */
function dpiblocks_flowhelpers_get_datestamp_from_datetime($date) {
  if (is_numeric($date)) {
    return $date;
  }

  $datetime = new DateTime($date);
  return $datetime->getTimestamp();
}

/**
 * Get the default text format
 */
function dpiblocks_flowhelpers_get_default_text_format() {
  static $default_format = '';
  if (empty($default_format)) {
    $default_format = filter_default_format();
  }
  return $default_format;
}

/**
 * Filter XSS if the module filter is enabled else, use PHP function strip_tags
 *
 * @param string $text
 */
function dpiblocks_flowhelpers_filter_html_tags($text) {
  if (module_exists('filter')) {
    $filtered_text = filter_xss($text, array());
  } else {
    $filtered_text = strip_tags($text);
  }
  return $filtered_text;
}

/**
 * Create or load an atom of a given type and provider from its external reference
 */
function dpiblocks_flowhelpers_create_or_load_atom($type, $provider, $external_reference) {
  $atom = scald_atom_defaults($type);
  $atom=clone($atom);
  $atom->provider = $provider;
  $atom->type = $type;

  if ($temp_atom = dpicontenttypes_api_getatombyexternalreference($external_reference)) {
    $atom = $temp_atom;
  }

  $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
  if ($dpiproperties !== FALSE) {
    $atom->dpiproperties = $dpiproperties;
  } else {
    $atom->dpiproperties = new DPIPropertiesEntity(array('type' => 'scald_atom'), 'dpiproperties');
  }

  $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);
  return array($atom, $atom_wrapper);
}

/**
 * Add an atom reference to a field, checking it's presence
 */
function dpiblocks_flowhelpers_add_atom_to_fied($wrapper, $entity, $field_name, $atom, $options = array()) {
  $array_atom = (array)$atom;

  if (!isset($array_atom['sid']) || empty($array_atom['sid']) || !is_numeric($array_atom['sid'])) {
    // This is a new atom, we don't have to check further
    $wrapper->{$field_name}->set((array)$array_atom);
    return;
  }

  $new_sid = $array_atom['sid'];
  $current_field = $wrapper->{$field_name}->value($options);
  if ($current_field && is_array($current_field)) {
    foreach ($current_field as $delta => $current_atom) {
      if (is_array($current_atom) && isset($current_atom['sid']) && $current_atom['sid'] === $new_sid) {
        return;
      }
    }
  }

  try {
    $wrapper->{$field_name}->set($array_atom);
  }
  catch (Exception $e) {
    // @todo : What should we do with the Exception ('Invalid data value given. Be sure it matches the required data type and format.')
  }
}

/**
 * Extract an URL and a alt attribute from the img tag in a text
 */
function dpiblocks_flowhelpers_extract_image_from_text($text) {
  $url = $alt = '';

  $substr = strstr($text, '<img');
  $substr = $substr ? $substr : strstr($text, rawurlencode('<img'));
  $tmp_substr = $substr ? strstr($substr, '>', TRUE) : FALSE;
  $substr = $tmp_substr ? $tmp_substr : ($substr ? strstr($substr, rawurlencode('>'), TRUE) : FALSE);

  if ($substr) {
    $src_substr = strstr($substr, 'src=');
    $src_delimiter = substr($src_substr, 4, 1);
    $src_substr = substr($src_substr, 5);
    $url = strstr($src_substr, $src_delimiter, TRUE);
    $url = urldecode(html_entity_decode($url));

    $alt_substr = strstr($substr, 'alt=');
    if ($alt_substr) {
      $alt_delimiter = substr($alt_substr, 4, 1);
      $alt_substr = substr($alt_substr, 5);
      $alt = strstr($src_substr, $alt_delimiter, TRUE);
    }
  }

  return array($url, $alt);
}
