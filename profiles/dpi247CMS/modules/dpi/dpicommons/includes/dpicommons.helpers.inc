<?php

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
