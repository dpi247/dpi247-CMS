<?php

/**
 * Return an array of all existing aliases for an internal url.
 */
function dpicommons_get_all_aliases($internal_url) {
  $db_aliases = db_select('url_alias', 'ua')
    ->fields('ua')
    ->condition('source', $internal_url)
    ->orderBy('ua.pid', 'DESC')
    ->execute();

  $return_aliases = array();
  foreach ($db_aliases as $db_alias) {
    $return_aliases[] = $db_alias;
  }

  if(count($return_aliases) == 0) {
    $internal_alias = new stdClass();
    $internal_alias->pid = 0;
    $internal_alias->source = $internal_url;
    $internal_alias->alias = $internal_url;
    $internal_alias->language = '';
    $return_aliases[] = $internal_alias;
  }

  return $return_aliases;
}

/**
 * Get the latest created alias for an internal URL
 * @param $internal_url {String}
 *   Internal URL, it can't begin with "/" or "http://".
 * @return {String}
 *   The alias if found, a fake alias, based on the $internal_url, if not.
 */
function dpicommons_get_main_alias($internal_url) {
  $db_aliases = db_select('url_alias', 'ua')
    ->fields('ua')
    ->condition('source', $internal_url)
    ->orderBy('ua.pid', 'DESC')
    ->range(0, 1)
    ->execute();

  $return_aliases = array();
  foreach ($db_aliases as $db_alias) {
    $return_aliases[] = $db_alias;
  }

  if(count($return_aliases) == 0) {
    $internal_alias = new stdClass();
    $internal_alias->pid = 0;
    $internal_alias->source = $internal_url;
    $internal_alias->alias = $internal_url;
    $internal_alias->language = '';
    $return_aliases[] = $internal_alias;
  }

  return $return_aliases;
}

/**
 * Take a flat vocabulary tree and turn it into a multi-tiered array
 *
 * @see taxonomy_tree()
 *
 * @param $array_of_terms_flat
 *   A flat array of taxonomy_term -tipicaly the return of taxonomy_tree() -
 *
 * @return
 *   The return array will simply contain an array of pointers rather then physical objects
 */
function dpicommons_convert_taxonomy_flat_to_tree(&$array_of_terms_flat) {
  $map = array();

  // Go through the tree and find the lowest depth as well as all available depth levels to scan
  $startdepth = FALSE;
  $depthlist = array();
  foreach($array_of_terms_flat as $vocindex => &$vocitem) {
    if ($startdepth === FALSE || $startdepth > $vocitem->depth) {
      //new start depth found
      $startdepth = $vocitem->depth;
    }

    // Add this item to the speciffic depth level
    $depthlist[$vocitem->depth][$vocitem->tid] = &$vocitem;
  }

  // Sort the depthlist so lowest is first
  ksort($depthlist);

  // Scan all depth levels found
  $lastdepth = NULL;
  foreach($depthlist as $depthlevel => &$depths) {
    // Check for first depth (root) level
    $currentdepthdata = array();

    if ($depthlevel == $startdepth) {
      // Add root items to the map
      foreach($depths as $itemindex => &$item) {
        $mapindex = count($map);
        $map[$mapindex] = array('term' => &$item, 'childrens' => array());
        $currentdepthdata[$itemindex] = &$map[$mapindex];
      }
    } else {
      // Scan through all items within this depth
      foreach($depths as $itemindex => &$item) {
        // Find a matching 'map' in teh last depth to assign the item too
        foreach($item->parents as $parentindex) {
          $mapindex = count($lastdepthdata[$parentindex]['childrens']);
          $lastdepthdata[$parentindex]['childrens'][$mapindex] = array('term' => &$item,'childrens' => array());
          $currentdepthdata[$itemindex] = &$lastdepthdata[$parentindex]['childrens'][$mapindex];
        }
      }
    }

    $lastdepthdata = $currentdepthdata;
  }

  return $map;
}

/**
 * Call the getimagesize function with a cleaned URL
 */
function dpicommons_getimagesize_cleaned_url($url) {
  // Url encode white spaces
  $url = preg_replace('/\s/', '%20', $url);
  return getimagesize($url);
}

/**
 * Check wheter or not an HTTP request is successfull :
 *   - Code 200
 *   - Not empty
 *
 * @param object $request
 *   The result of the HTTP request
 *
 * @return bool
 *   TRUE or FALSE
 */
function dpicommons_is_http_request_successfull($request) {
  if (is_object($request) &&
      ((isset($request->code) && $request->code == 200) || (isset($request->redirect_code) && $request->redirect_code == 200)) &&
      isset($request->data) && !empty($request->data)) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Helper fonction to calculate time consuming part of a script.
 *
 * @param $step
 *   The step of the process we evaluate. also used in the dsm output
 * @param $print
 *   Yes if the output should be a dsm false if you want the consumint time as a return.
 * @param $namespace
 *   To avoid collision between two debug timer use a namespace
 * @param $restart
 *   Restart the static variable begin time.
 *
 * @return
 *   The difference between the time between the first call of this fuction for this namespace and the current call of this function for this namespace
 */
function dpicommons_set_microtime_step($step = '', $print = TRUE, $namespace = "default", $restart = FALSE) {
  static $begin_time ;
  static $first;

  if(!isset($begin_time[$namespace])) {
    $begin_time[$namespace] = 0;
    $first[$namespace] = TRUE;
  }

  if ($first[$namespace] || $restart) {
    $begin_time[$namespace] = microtime(TRUE);
    $first[$namespace] = FALSE;
  } else {
    $end_time = microtime(TRUE);
    $t = $end_time - $begin_time[$namespace];
    if ($print) {
      $micro = sprintf('%06d', ($t - floor($t)) * 1000000);
      $d = new DateTime(date('Y-m-d H:i:s.'.$micro, $t));
      $formatted = $d->format('i:s.u');
      dsm($formatted, $step);
    }
    return $t;
  }
}

/**
 * Return a boolean form the XML value (int, string or boolean)
 *
 * @param bool $xmlboolean
 */
function dpicommons_toboolean($boolean_to_check, $default = NULL){
  $boolean = $default;
  if ($boolean_to_check === FALSE) $boolean = FALSE;
  elseif ($boolean_to_check === TRUE) $boolean = TRUE;
  elseif (strtolower($boolean_to_check) == "true") $boolean = TRUE;
  elseif (strtolower($boolean_to_check) == "false") $boolean = FALSE;
  elseif (strtolower($boolean_to_check) == "null") $boolean = FALSE;
  elseif (is_string($boolean_to_check) && !empty($boolean_to_check)) $boolean = TRUE;
  elseif ($boolean_to_check == 0) $boolean = FALSE;
  elseif ($boolean_to_check == 1) $boolean = TRUE;
  return $boolean;
}

/**
 * Autoindent an XML string
 */
function dpicommons_format_xml_string($xml) {
  // add marker linefeeds to aid the pretty-tokeniser (adds a linefeed between all tag-end boundaries)
  $xml = preg_replace('/(>)(<)(\/*)/', "$1\n$2$3", $xml);

  // now indent the tags
  $token      = strtok($xml, "\n");
  $result     = ''; // holds formatted version as it is built
  $pad        = 0; // initial indent
  $matches    = array(); // returns from preg_matches()

  // scan each line and adjust indent based on opening/closing tags
  while ($token !== false) :
  // test for the various tag states

  // 1. open and closing tags on same line - no change
  if (preg_match('/.+<\/\w[^>]*>$/', $token, $matches)) :
  $indent=0;
  // 2. closing tag - outdent now
  elseif (preg_match('/^<\/\w/', $token, $matches)) :
  $pad--;
  // 3. opening tag - don't pad this one, only subsequent tags
  elseif (preg_match('/^<\w[^>]*[^\/]>.*$/', $token, $matches)) :
  $indent=1;
  // 4. no indentation needed
  else :
  $indent = 0;
  endif;

  // pad the line with the required number of leading spaces
  $line    = str_pad($token, strlen($token)+$pad, ' ', STR_PAD_LEFT);
  $result .= $line . "\n"; // add to the cumulative result, with linefeed
  $token   = strtok("\n"); // get the next token
  $pad    += $indent; // update the pad size for subsequent lines
  endwhile;

  return $result;
}

function dpicommons_validate_xml($dom, $xsd_path) {
  set_error_handler('_dpicommons_validate_xml_error', E_WARNING);
  return $dom->schemaValidate($xsd_path);
}

function _dpicommons_validate_xml_error($errno, $errstr, $errfile = '', $errline = 0, $errcontext = array()) {
  if (strpos($errstr, 'DOMDocument::schemaValidate') === 0) {
    switch ($errno) {
      case 1:
        $mess_type = 'error';
        break;
      case 2:
        $mess_type = 'warning';
        break;
      default:
        $mess_type = 'status';
        break;
    }
    drupal_set_message(check_plain($errstr), $mess_type);
  }
}
