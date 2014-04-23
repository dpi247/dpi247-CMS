<?php

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
