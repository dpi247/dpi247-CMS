<?php

/**
 * Return an array with the content types of type dpi (like article-package, photo, etc)
 */
function dpicontenttypes_api_getdpicontenttypes() {
  return array(
    'package',
  );
}

/**
 * Return a node by its external reference. If no node exists for this external reference return NULL
 *
 * @param String $external_reference
 *
 * @return Object $node
 */
function dpicontenttypes_api_getnodebyexternalreference($external_reference) {
  $node = NULL;
  $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
  if ($dpiproperties != NULL && $dpiproperties->id != NULL) {
    $node = node_load($dpiproperties->id);
  }
  return $node;
}

/**
 * Return an atom by its external reference. If no atom exists for this external reference return NULL
 *
 * @param String $external_reference
 *
 * @return Object $atom
 */
function dpicontenttypes_api_getatombyexternalreference($external_reference) {
  $atom = NULL;
  $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
  if ($dpiproperties != NULL && $dpiproperties->id != NULL) {
    $atom = scald_atom_load($dpiproperties->id);
  }
  return $atom;
}

/**
 * Search for taxonomy terms for a specific "path".
 *
 * @param $path
 *   The taxonomy term path to match ( /term1/term2/term3 ).
 * @param $vid
 *   The taxonomy vid used for filtering on a single vocabulary.
 *   If not set, will search into all vocabularies.
 *
 * @return
 *   an array of taxonomy term object
 */
function dpicontenttypes_api_taxonomy_get_term_by_path($path, $vid = NULL) {
  $result = NULL;
  $vocabulary_name = NULL;

  if ($vid != NULL){
    $vocabulary = taxonomy_vocabulary_load($vid);
    $vocabulary_name = $vocabulary->machine_name;
  }

  $exploded_path = explode('/', trim($path, '/'));
  $term_name = $exploded_path[count($exploded_path) - 1];

  //Get all terms with that term_name
  $terms = taxonomy_get_term_by_name($term_name, $vocabulary_name);

  // Reverse building of path for founded terms. And catch the ones that match
  foreach ($terms as $term) {
    $term_path = '';
    $parents = taxonomy_get_parents_all($term->tid);
    foreach ($parents as $parent){
      $term_path = strtolower($parent->name)."/".$term_path;
    }
    if (trim($term_path, '/') == strtolower(trim($path, '/'))) {
      $result[] = $term;
    }
  }
  return $result;
}

/**
 * Determine if a provider exists for this uri and in this case, return the provider (otherwise set provider to null)
 *
 * @return
 *   An array with the provider informations.
 *   following items:
 *   - "name": The machine name of the provider.
 *   - "type": The type of the provider (photo/video/audio/etc)
 */
function hook_dpicontenttypes_provider($uri) {
  return array(
    'module_name' => array('name' => 'provider_name', 'type' => 'provider_type'),
  );
}

/**
 * Return the provider of the uri
 *
 * @param String $uri
 *
 * @return Array $provider
 */
function dpicontenttypes_api_getprovider($uri) {
  $uri_provider = NULL;
  $providers = module_invoke_all('dpicontenttypes_provider', $uri);
  foreach ($providers as $pro) {
    if ($pro != NULL) {
      $uri_provider = $pro;
      break;
    }
  }

  return $uri_provider;
}

/**
 * Get cropings values for a given entity and atom
 * 
 * @param String $entity_type
 *   Entity type (i.e. "node")
 * @param Int $eid
 *   Entity ID (a node ID for example)
 * @param Int $aid
 *   Atom ID
 * @param String $style = ''
 *   Return only the values for a given style
 * 
 * @return Array
 *   The cropings values
 */
function dpicontenttypes_api_get_cropings_for_entity_and_atom($entity_type, $eid, $aid, $style_name = '') {
  // Get saved cropings values from DB
  $existing_cropings = _dpicontenttypes_get_saved_cropings_for_entity_and_atom($entity_type, $eid, $aid, $style_name);

  // Get default cropings values, if not set in DB
  if ($style_name) {
    $styles = array(image_style_load($style_name));
  } else {
    $styles = image_styles();
  }
  $merged_cropings = array();
  foreach ($styles as $style) {
    if ($style && is_array($style) && !empty($style)) {
      $existing_ratio = 0;
      if ($existing_cropings && isset($existing_cropings[$style['name']]) && sizeof($existing_cropings[$style['name']]) == 4) {
        $merged_cropings[$style['name']] = $existing_cropings[$style['name']];
        $existing_ratio = $merged_cropings[$style['name']][2]/$merged_cropings[$style['name']][3];
      }

      if (isset($style['effects'])) {
        foreach ($style['effects'] as $effect) {
          if (isset($effect['name']) && $effect['name'] == 'dpicontenttypes_dpicrop') {
            $data = $effect['data'];
            $style_ratio = $data['width']/$data['height'];
            $ratio_error = abs($existing_ratio - $style_ratio)/$style_ratio;
            if (!isset($merged_cropings[$style['name']]) || ($ratio_error > 0.02)) {
              // The croping has no existing value or the ratio has changed (2% error)

              $crop = array(
                is_numeric($data['xoffset']) ? (int)$data['xoffset'] : $data['xoffset'],
                is_numeric($data['yoffset']) ? (int)$data['yoffset'] : $data['yoffset'],
                (int)$data['width'],
                (int)$data['height'],
              );
              $merged_cropings[$style['name']] = $crop;
            }
            break;
          }
        }
      }
    }
  }

  return $merged_cropings;
}

/**
 * Returns the URL for an image derivative given a style and dpi image path.
 *
 * @param $style_name
 *   The name of the style to be used with this image.
 * @param $path
 *   The path to the image.
 *
 * @return
 *   The absolute URL where a style image can be downloaded, suitable for use
 *   in an <img> tag. Requesting the URL will cause the image to be created.
 * @see image_style_deliver()
 */
function dpicontenttypes_api_image_style_url($style_name, $entity_id, $atom_id, $path) {
  $uri = _dpicontenttypes_image_style_path($style_name, $entity_id, $atom_id, $path);
  // The token query is added even if the 'image_allow_insecure_derivatives'
  // variable is TRUE, so that the emitted links remain valid if it is changed
  // back to the default FALSE.
  $token_query = array(IMAGE_DERIVATIVE_TOKEN => image_style_path_token($style_name, file_stream_wrapper_uri_normalize($path)));

  // If not using clean URLs, the image derivative callback is only available
  // with the query string. If the file does not exist, use url() to ensure
  // that it is included. Once the file exists it's fine to fall back to the
  // actual file path, this avoids bootstrapping PHP once the files are built.
  if (!variable_get('clean_url') && file_uri_scheme($uri) == 'public' && !file_exists($uri)) {
    $directory_path = file_stream_wrapper_get_instance_by_uri($uri)->getDirectoryPath();
    return url($directory_path . '/' . file_uri_target($uri), array('absolute' => TRUE, 'query' => $token_query));
  }

  $file_url = file_create_url($uri);
  // Append the query string with the token.
  return $file_url . (strpos($file_url, '?') !== FALSE ? '&' : '?') . drupal_http_build_query($token_query);
}

/**
 * Clears cached versions of a specific file in all styles for the dpi paths
 *
 * @param $path
 *   The Drupal file path to the original image.
 */
function dpicontenttypes_api_image_path_flush($path) {
  $styles = image_styles();
  foreach ($styles as $style) {
    $style_name = $style['name'];
    $file_directory_path = file_stream_wrapper_get_instance_by_scheme('public')->getDirectoryPath();
    $entities_dir = file_scan_directory($file_directory_path.'/dpistyles/'.$style_name, '/.*/', array('recurse' => FALSE));
    foreach ($entities_dir as $entity_dir) {
      if (is_dir($entity_dir->uri)) {
        $entity_name = $entity_dir->name;
        $atoms_dir = file_scan_directory($file_directory_path.'/dpistyles/'.$style_name.'/'.$entity_name, '/.*/', array('recurse' => FALSE));
        foreach ($atoms_dir as $atom_dir) {
          if (is_dir($atom_dir->uri)) {
            $image_path = _dpicontenttypes_image_style_path($style_name, $entity_name, $atom_dir->name, $path);
            if (file_exists($image_path)) {
              file_unmanaged_delete($image_path);
            }
          }
        }
      }
    }
  }

  // Throw event to allow specific actions on a path flush
  module_invoke_all('dpistyles_path_flush', $path);
}
