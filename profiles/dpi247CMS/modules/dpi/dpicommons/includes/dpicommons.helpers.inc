<?php

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
