<?php

/**
 * Fork of error_handler.inc from "fail" module
 * 
 * Gestionnaire d'erreurs pour Drupal
 * Plus complet que l'actuel
 */

/**
 * Custom PHP error handler.
 * @see _drupal_error_handler();
 *
 * @param $error_level
 *   The level of the error raised.
 * @param $message
 *   The error message.
 * @param $filename
 *   The filename that the error was raised in.
 * @param $line
 *   The line number the error was raised at.
 * @param $context
 *   An array that points to the active symbol table at the point the error occurred.
 */
function _dpicommons_error_handler($error_level, $message, $filename, $line, $context) {
  if ($error_level & error_reporting()) {
    // All these constants are documented at http://php.net/manual/en/errorfunc.constants.php
    $types = array(
      E_ERROR => 'Error',
      E_WARNING => 'Warning',
      E_PARSE => 'Parse error',
      E_NOTICE => 'Notice',
      E_CORE_ERROR => 'Core error',
      E_CORE_WARNING => 'Core warning',
      E_COMPILE_ERROR => 'Compile error',
      E_COMPILE_WARNING => 'Compile warning',
      E_USER_ERROR => 'User error',
      E_USER_WARNING => 'User warning',
      E_USER_NOTICE => 'User notice',
      E_STRICT => 'Strict warning',
      E_RECOVERABLE_ERROR => 'Recoverable fatal error'
    );

    // Optimisation TAT :
    // Dès que c'est une notice ou une erreur "strict", on dégage !!!
    $log_watchdog = ($error_level != E_NOTICE && $error_level != E_USER_NOTICE && $error_level != E_STRICT);
    if ($log_watchdog == false) {
      return;
    }

    $caller = _drupal_get_last_caller(debug_backtrace());

    $error_type = isset($types[$error_level]) ? $types[$error_level] : 'Unknown error';

    // We treat recoverable errors as fatal.
    _dpicommons_log_error(array(
        '%type' => $error_type,
        '%message' => $message,
        '%function' => $caller['function'],
        '%file' => $caller['file'],
        '%line' => $caller['line'],
      ),
      $error_level == E_RECOVERABLE_ERROR,
      $log_watchdog
    );
  }
}

/**
 * Called after the main PHP process, even if this process is interrupted because of a fatal error.
 * It is actually the only way to "catch" fatal errors in PHP.
 */
function _dpicommons_shutdown_function() {
  print_r(error_get_last());
}

/**
 * Log a PHP error or exception, display an error page in fatal cases.
 * @see _drupal_log_error()
 *
 * @param $error
 *   An array with the following keys: %type, %message, %function, %file, %line.
 * @param $fatal
 *   TRUE if the error is fatal.
 */
function _dpicommons_log_error($error, $fatal = FALSE, $log_watchdog = TRUE) {
  global $user;

  $server_name = (isset($_SERVER['SERVER_NAME']))?$_SERVER['SERVER_NAME']:'unknown';

  // Initialize a maintenance theme if the boostrap was not complete.
  // Do it early because drupal_set_message() triggers a drupal_theme_initialize().
  if ($fatal && (drupal_get_bootstrap_phase() != DRUPAL_BOOTSTRAP_FULL)) {
    unset($GLOBALS['theme']);
    if (!defined('MAINTENANCE_MODE')) {
      define('MAINTENANCE_MODE', 'error');
    }
    drupal_maintenance_theme();
  }

  // When running inside the testing framework, we relay the errors
  // to the tested site by the way of HTTP headers.
  if (isset($_SERVER['HTTP_USER_AGENT']) && preg_match("/^simpletest\d+;/", $_SERVER['HTTP_USER_AGENT']) && !headers_sent() && (!defined('SIMPLETEST_COLLECT_ERRORS') || SIMPLETEST_COLLECT_ERRORS)) {
    // $number does not use drupal_static as it should not be reset
    // as it uniquely identifies each PHP error.
    static $number = 0;
    $assertion = array(
      $error['%message'],
      $error['%type'],
      array(
        'function' => $error['%function'],
        'file' => $error['%file'],
        'line' => $error['%line'],
      ),
    );
    header('X-Drupal-Assertion-' . $number . ': ' . rawurlencode(serialize($assertion)));
    $number++;
  }

  // Chargement de la backtrace...
  $backtrace_display = _dpicommons_debugtrace();

  try {
    if($log_watchdog == TRUE){
      switch($error['%type']){
        case E_ERROR:
        case E_PARSE:
        case E_CORE_ERROR:
        case E_COMPILE_ERROR:
        case E_USER_ERROR:
        case E_RECOVERABLE_ERROR:
          $type_watchdog_error = WATCHDOG_ERROR;
          break;
        case E_WARNING:
        case E_CORE_WARNING:
        case E_COMPILE_WARNING:
        case E_USER_WARNING:
          $type_watchdog_error = WATCHDOG_WARNING;
          break;
        case E_NOTICE:
        case E_USER_NOTICE:
        case E_STRICT:
          $type_watchdog_error = WATCHDOG_NOTICE;
          break;
        default:
          $type_watchdog_error = WATCHDOG_ERROR;
          break;
      }
      $error['!bt'] = $backtrace_display;
      watchdog('php', '%type: %message in %function (line %line of %file).!bt', $error, $type_watchdog_error);
    }
  }
  catch (Exception $e) {
    // Ignore any additional watchdog exception, as that probably means
    // that the database was not initialized correctly.
  }

  if ($fatal) {
    drupal_set_header('500 Service unavailable (with message)');
  }

  if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && $_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest') {
    if ($fatal) {
      // When called from JavaScript, simply output the error message.
      print t_without_theme('%type: %message in %function (line %line of %file).', $error);
      exit;
    }
  }
  else {
    // Display the message if the current error reporting level allows this type
    // of message to be displayed, and unconditionnaly in update.php.
    $error_level = variable_get('error_level', ERROR_REPORTING_DISPLAY_ALL);
    $display_error = $error_level == ERROR_REPORTING_DISPLAY_ALL || ($error_level == ERROR_REPORTING_DISPLAY_SOME && $error['%type'] != 'Notice');
    if ($display_error || (defined('MAINTENANCE_MODE') && MAINTENANCE_MODE == 'update')) {
      $class = 'error';

      // If error type is 'User notice' then treat it as debug information
      // instead of an error message, see dd().
      if ($error['%type'] == 'User notice') {
        $error['%type'] = 'Debug';
        $class = 'status';
      }

      // Ajouter ici les urls des serveurs de prod
      if(!preg_match('/(staging|www)\./',$server_name)) {
        drupal_set_message(t_without_theme('%type: %message in %function (line %line of %file).', $error).$backtrace_display, $class);
      }
    }

    if ($fatal) {
      drupal_set_title('Error');
      // We fallback to a maintenance page at this point, because the page generation
      // itself can generate errors.
      print theme('maintenance_page', 'The website encountered an unexpected error. Please try again later.');
      exit;
    }
  }
}

/**
 * Affiche un debug de haute qualité, en HTML
 * de quoi économiser des heures de débugs.
 *
 * @param $with_display_link {Boolean}
 *   Affiche un bouton pour afficher/masquer la trace.
 *
 * @return {String}
 *   Le HTML
 */
function _dpicommons_debugtrace($with_display_link=true){
  // HACK TAT@AUDAXIS.COM
  // Cette partie du code est allergique à tout commentaire
  $backtracel = '<ol>';
  foreach(debug_backtrace() as $k => $v){
    if(strpos($v['function'],"_dpicommons") !== FALSE)
      continue;
    if(!isset($v['file'])) $v['file'] = "???";
    if(!isset($v['line'])) $v['line'] = "¿¿¿";
    $v['file_rel'] = str_replace(rtrim($_SERVER['DOCUMENT_ROOT'],'/'), '', $v['file']);
    $v['file_rel'] = preg_replace('/\/([^\/]+)$/', '/<strong>\1</strong>', $v['file_rel']);
    $v['custom_code'] = (strpos($v['file_rel'], 'sites/all') !== FALSE);
    $v['is_include'] = ($v['function'] == "include" || $v['function'] == "include_once" || $v['function'] == "require_once" || $v['function'] == "require");
    $v['include_file'] = "";
    if($v['is_include']) {
      $v['include_file_full'] = $v['args'][0];
      $v['include_file'] = str_replace(rtrim($_SERVER['DOCUMENT_ROOT'],'/'), '', $v['include_file_full']);
    }
    $v['complement'] = '';
    if($v['function'] == "mysql_query" && isset($v['args'][0])){
      $v['complement'] = 'Request : <span style="font-family:monospace">'.$v['args'][0].'</span>';
    }
    elseif($v['function'] == "array_key_exists"){
      $typearg0 = (isset($v['args'][0])) ? gettype($v['args'][0]) : "undefined";
      $typearg1 = (isset($v['args'][1])) ? gettype($v['args'][1]) : "undefined";
      $v['complement'] = 'Parameter #1 is '.$typearg0.', parameter #2 is '.$typearg0;
    }
    unset($v['args']);

    //Le bloc :
    if($v['custom_code'])
      $backtracel .= "\t<li style='background-color:GreenYellow'>";
    else
      $backtracel .= "\t<li>";

    if($v['is_include']){
      $backtracel .= "<strong>".$v['function']."</strong>(".$v['include_file'].") called at [".$v['file_rel'].":".$v['line']."]\n";
    }
    else{
      $backtracel .= "<strong>".$v['function']."</strong>() called at [".$v['file_rel'].":".$v['line']."]\n";
    }
    if($v['complement']){
      $backtracel .= "<br/>&#8618; ".$v['complement']."\n";
    }

    $backtracel .= "</li>\n";
  }
  $backtracel .= "</ol>\n";

  if($with_display_link){
    $backtrace_id = "BT".uniqid();

    $backtrace_display = "<a href='javascript:void(0);' onclick=\"javascript:document.getElementById('".$backtrace_id."').style.display=(document.getElementById('".$backtrace_id."').style.display=='block'?'none':'block');\">[TRACE]</a>";
    $backtrace_display .= "<br/><div id='".$backtrace_id."' style='display:none;font-family:sans-serif,monospace;width:70%;border:1px solid #000;background-color:#FFF;padding-left:1em'>".$backtracel."</div>";
    return $backtrace_display;
  }
  else {
    return $backtracel;
  }
}

/**
 * La fonction t(), mais sans utiliser
 * la notion de langue ni le theming.
 * 
 * @param  {String} $string
 *   La chaîne avec tokens.
 * @param  {Array} $args 
 *   Les tokens.
 * @param  {String} $langcode
 *   Inutilisé...
 *
 * @return {String}
 *   La chaîne complétée.
 */
function t_without_theme($string, $args = array(), $langcode = NULL){
  if(!is_array($args))
    return $string;
  foreach ($args as $key => $value) {
    switch ($key[0]) {
      case '@':
        // Escaped only.
        $args[$key] = check_plain($value);
        break;
      case '%':
      default:
        // Escaped and placeholder.
        $args[$key] = '<em>'.check_plain($value).'</em>';
        break;
      case '!':
        // Pass-through.
    }
  }
  return strtr($string, $args);
}
