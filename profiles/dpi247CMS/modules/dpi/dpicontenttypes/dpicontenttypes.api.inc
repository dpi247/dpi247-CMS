<?php

/**
 * Truncate a text in a coherent way. Cut a the end of, in order :
 *  - A paragraph
 *  - A line break
 *  - A sentence
 *  - A word
 *
 * @param string $text
 *   The text to truncate
 * @param int $size
 *   The maximum size of the truncated text
 * @param bool $nocut_words = NULL
 *   If TRUE, truncate the text to the closest to the given size without cutting any word
 *   but with no concern for paragraphs, line breaks and sentences.
 *   If FALSE, follow the full process and respect paragraphs, line breaks and sentences.
 * @param string $format = 'full_html'
 *   The format of the text, used to get informations about filters
 * @param bool $add_ellipsis
 *   Add '...' at the end of the resulting string or not
 *
 * @return string
 *   The truncated string
 */
function dpicontenttypes_api_truncate_text($text, $size, $nocut_words = NULL, $format = 'full_html', $add_ellipsis = TRUE) {
  // If the size is zero the entire text is the substring.
  if ($size == 0) {
    return $text;
  }

  if (isset($format)) {
    $filters = filter_list_format($format);
  }

  if (!$nocut_words) {
    // We check for the presence of the PHP evaluator filter in the current
    // format. If the body contains PHP code, we do not split it up to prevent
    // parse errors.
    if (isset($format) && isset($filters['php_code']) && $filters['php_code']->status && strpos($text, '<?') !== FALSE) {
      return $text;
    }

    // If we have a short text, the entire text is the substring.
    if (drupal_strlen($text) <= $size) {
      return $text;
    }

    // If the delimiter has not been specified, try to split at paragraph or
    // sentence boundaries.

    // The substring may not be longer than maximum length specified. Initial slice.
    $substring = truncate_utf8($text, $size);

    // Store the actual length of the UTF8 string -- which might not be the same
    // as $size.
    $max_rpos = strlen($substring);

    // How much to cut off the end of the substring so that it doesn't end in the
    // middle of a paragraph, sentence, or word.
    // Initialize it to maximum in order to find the minimum.
    $min_rpos = $max_rpos;

    // Store the reverse of the substring.  We use strpos on the reversed needle and
    // haystack for speed and convenience.
    $reversed = strrev($substring);

    // Build an array of arrays of break points grouped by preference.
    $break_points = array();

    // A paragraph near the end of sliced substring is most preferable.
    $break_points[] = array('</p>' => 0);

    // If no complete paragraph then treat line breaks as paragraphs.
    $break_points[] = array(
      '<br />' => 6,
      '<br>' => 4,
    );
    // Newline only indicates a line break if line break converter
    // filter is present.
    if (isset($filters['filter_autop'])) {
      $line_breaks["\n"] = 1;
    }

    // If the first paragraph is too long, split at the end of a sentence.
    $break_points[] = array(
      '. ' => 1,
      '! ' => 1,
      '? ' => 1,
      '。' => 0,
      '؟ ' => 1,
    );

    // Iterate over the groups of break points until a break point is found.
    foreach ($break_points as $points) {
      // Look for each break point, starting at the end of the substring.
      foreach ($points as $point => $offset) {
        // The substring is already reversed, but the break point isn't.
        $rpos = strpos($reversed, strrev($point));
        if ($rpos !== FALSE) {
          $min_rpos = min($rpos + $offset, $min_rpos);
        }
      }

      // If a break point was found in this group, slice and return the substring.
      if ($min_rpos !== $max_rpos) {
        // Don't slice with length 0.  Length must be <0 to slice from RHS.
        $substring = ($min_rpos === 0) ? $substring : substr($substring, 0, 0 - $min_rpos);
      }
    }
  } else {
    $substring = truncate_utf8($text, $size, " ");
    $substring = wordwrap($substring, $size);
  }

  // If the htmlcorrector filter is present, apply it to the generated summary.
  if (isset($format) && isset($filters['filter_htmlcorrector'])) {
    $substring = _filter_htmlcorrector($substring);
  }

  // If a substring was not found, still return a teaser.
  if ($add_ellipsis) {
    $substring .=" [...]";
  }

  // If a break point was not found, still return a substring.
  return $substring;
}

/**
 * Get the HTML render of the first embedded object of a node
 *
 * @param object $node
 *   The node
 * @param array $content
 *   The rendered contents of the node
 * @param string $type = FALSE
 *   A Scald media type, to get only this type of media, 'image' for example
 * @param string $provider = FALSE
 *   A Scald provider, to get only medias from this provider, 'scald_image' for example
 *
 * @return array | bool
 *   A single entry array (the key is the delta of the object) containing the rendered object or FALSE in case of failure
 */
function dpicontenttypes_api_get_first_embed_view($node, $content, $type = FALSE, $provider = FALSE) {
  $rendered_embeds = array();
  if (isset($content['field_embededobjects'])) {
    $embeds_content = $content['field_embededobjects'];
    if ($embeds = dpicontenttypes_api_get_first_embed($node, $type, $provider)) {
      foreach ($embeds as $delta => $embed) {
        if (isset($embeds_content[$delta]) && isset($embeds_content[$delta]['#markup'])) {
          $rendered_embeds[$delta] = $embeds_content[$delta]['#markup'];
        }
      }
    }
  }
  return empty($rendered_embeds) ? FALSE : $rendered_embeds;
}

/**
 * Get the HTML render of all embedded objects of a node
 *
 * @param object $node
 *   The node
 * @param array $content
 *   The rendered contents of the node
 * @param string $type = FALSE
 *   A Scald media type, to get only this type of media, 'image' for example
 * @param string $provider = FALSE
 *   A Scald provider, to get only medias from this provider, 'scald_image' for example
 * @param bool $inline = NULL
 *   Three possibilities :
 *     - TRUE : only return objects used as inline
 *     - FALSE : do not return inline objects
 *     - NULL : don't care if inline or not, return all objects
 *
 * @return array | bool
 *   An array (the keys are the deltas of the objects) containing the rendered objects or FALSE in case of failure
 */
function dpicontenttypes_api_get_embeds_view($node, $content, $type = FALSE, $provider = FALSE, $inline = NULL) {
  $rendered_embeds = array();
  if (isset($content['field_embededobjects'])) {
    $embeds_content = $content['field_embededobjects'];
    if($embeds = dpicontenttypes_api_get_embeds($node, $type, $provider, FALSE, $inline)) {
      foreach ($embeds as $delta => $embed) {
        if (isset($embeds_content[$delta]) && isset($embeds_content[$delta]['#markup'])) {
          $rendered_embeds[$delta] = $embeds_content[$delta]['#markup'];
        }
      }
    }
  }
  return empty($rendered_embeds) ? FALSE : $rendered_embeds;
}

/**
 * Get the first embedded object of a node
 *
 * @param object $node
 *   The node
 * @param string $type = FALSE
 *   A Scald media type, to get only this type of media, 'image' for example
 * @param string $provider = FALSE
 *   A Scald provider, to get only medias from this provider, 'scald_image' for example
 * @param bool $load_atom = FALSE
 *   Add the full Scald atom to the returned object
 *
 * @return array | bool
 *   An array (the key is the delta of the object) containing the first object or FALSE in case of failure
 */
function dpicontenttypes_api_get_first_embed($node, $type = FALSE, $provider = FALSE, $load_atom = FALSE) {
  $embed = array();
  if ($embeds = dpicontenttypes_api_get_embeds($node, $type, $provider)) {
    foreach ($embeds as $delta => $temp_embed) {
      $embed[$delta] = $temp_embed;
      $embed[$delta]['delta'] = $delta;
      if ($load_atom) {
        $embed[$delta]['atom'] = scald_atom_load($temp_embed['sid']);
      }
      break;
    }
  }
  return empty($embed) ? FALSE : $embed;
}

/**
 * Get the count of embedded objects for a node, grouped by type
 *
 * @param object $node
 *
 * @return array | bool
 *   An array (the keys are the types of the objects) containing the objects count or FALSE in case of failure
 */
function dpicontenttypes_api_get_embeds_count_by_type($node) {
  $count_embeds = array();
  if ($type_embeds = dpicontenttypes_api_get_embeds_by_type($node)) {
    foreach ($type_embeds as $type => $embeds) {
      $count_embeds[$type] = count($embeds);
    }
  }
  return empty($count_embeds) ? FALSE : $count_embeds;
}

/**
 * Get all embedded objects for a node, grouped by type
 *
 * @param object $node
 *
 * @return array | bool
 *   An array (the keys are the types of the objects) containing the objects or FALSE in case of failure
 */
function dpicontenttypes_api_get_embeds_by_type($node) {
  $type_embeds = array();
  if ($embeds = dpicontenttypes_api_get_embeds($node)) {
    foreach ($embeds as $embed) {
      if (!isset($type_embeds[$embed['type']])) {
        $type_embeds[$embed['type']] = array();
      }
      $type_embeds[$embed['type']][] = $embed;
    }
  }
  return empty($type_embeds) ? FALSE : $type_embeds;
}

/**
 * Get all embedded objects of a node
 *
 * @param object $node
 *   The node
 * @param string $type = FALSE
 *   A Scald media type, to get only this type of media, 'image' for example
 * @param string $provider = FALSE
 *   A Scald provider, to get only medias from this provider, 'scald_image' for example
 * @param bool $load_atom = FALSE
 *   Add the full Scald atom to the returned objects
 * @param bool $inline = NULL
 *   Three possibilities :
 *     - TRUE : only return objects used as inline
 *     - FALSE : do not return inline objects
 *     - NULL : don't care if inline or not, return all objects
 *
 * @return array | bool
 *   An array (the keys are the deltas of the objects) containing the objects or FALSE in case of failure
 */
function dpicontenttypes_api_get_embeds($node, $type = FALSE, $provider = FALSE, $load_atom = FALSE, $inline = NULL) {
  static $static_embeds = array();
  if (isset($node->nid)) {
    if(isset($static_embeds[$node->nid])) {
      $embeds = $static_embeds[$node->nid];
    } else {
      $embeds = dpicommons_get_field_from_entity($node, 'node', 'field_embededobjects', array(), array('identifier' => TRUE));
      $static_embeds[$node->nid] = $embeds;
    }
  }

  if ($type || $provider) {
    if (is_string($type)) {
      $type = array($type);
    }
    if (is_string($provider)) {
      $provider = array($provider);
    }

    foreach ($embeds as $delta => $embed) {
      if ($type && !in_array($embed['type'], $type)) {
        unset($embeds[$delta]);
        continue;
      }
      if ($provider && !in_array($embed['provider'], $provider)) {
        unset($embeds[$delta]);
        continue;
      }
      if (is_bool($inline) && (($inline && !$embed['inline']) || !$inline && $embed['inline'])) {
        unset($embeds[$delta]);
        continue;
      }
    }
  }

  foreach ($embeds as $delta => $embed) {
    $embeds[$delta]['delta'] = $delta;
    if ($load_atom) {
      $embeds[$delta]['atom'] = scald_atom_load($embed['sid']);
    }
  }

  return empty($embeds) ? FALSE : $embeds;
}

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
