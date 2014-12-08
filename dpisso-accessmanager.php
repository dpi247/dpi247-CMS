<?php
/**
 * @file
 * The PHP page for all Acess Manager bootstrap callbacks
 */

/**
 * Root directory of Drupal installation.
 */
define ( 'DRUPAL_ROOT', getcwd () );

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap ( DRUPAL_BOOTSTRAP_SESSION );

require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpicache/dpicache.api.inc';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';
require_once DRUPAL_ROOT . '/includes/file.inc';
require_once DRUPAL_ROOT .'/modules/filter/filter.module';
require_once DRUPAL_ROOT .'/modules/user/user.module';

drupal_load ( 'module', 'ctools' );
ctools_include ( 'plugins' );


$cctools = ctools_get_plugins ( 'ctools', 'content_types'  );



/*
 * Load requiered modules for current paywall politics.
 */
$politic = dpi_variable_get ( 'dpisso_paywall_paywallpolitics', null );

if ($politic) {
  $plugin = ctools_get_plugins ( 'dpisso', 'paywallpolitic'  , $politic);
  if ($class = ctools_plugin_get_class ( $plugin, 'handler' )) {
    $politic_instance = new $class ();
    _dpissoaccessmanager_paywall_moduleload ( $politic_instance );
  } else {
    _dpissoaccessmanager_return_json ( FALSE, 503, "Internal Server Error - Paywall class not defined." );
  }
} else {
  _dpissoaccessmanager_return_json ( FALSE, 503, "Internal Server Error - No paywall politic define" );
}

$operation = check_plain ( $_GET ["operation"] );
$url_arguments = $_GET;
if (! isset ( $operation )) {
  print "ERROR No operation";

  header ( 'HTTP/1.1 403 Forbidden', NULL, 403 );
} else {
  switch ($operation) {
    
    case "is_secure_page" :
      if (isset ( $_GET ["url"] ) ) {
        $return = $politic_instance->issecurepage ( $_GET ["url"], $_GET ["services"] );
      }
      else{
        print "Missing parameters";
        header ( 'HTTP/1.1 405 invalid Operation', null, 405 );
        die ();
      }
        
      break;
    
    default :
      print "Invalid operation";
      header ( 'HTTP/1.1 405 invalid Operation', null, 405 );
      die ();
      break;
  }
  
  // Print the results of a valid operation & die.
  _dpissoaccessmanager_return_json ( $return ["data"], $return ["code"] );
}

/**
 * Print an error page OR the data in JSON form.
 */
function _dpissoaccessmanager_return_json($data, $httperror, $httperrormessage = "") {
  if ($httperror != 200) {
    header ( 'HTTP/1.1 ' . $httperror . ' ' . $httperrormessage, true, $httperror );
    echo json_encode ( $data );
    die ();
  } else {
    header ( 'Content-Type: application/json' );
    echo json_encode ( $data );
    die ();
  }
}

/**
 * Load the list of module for current paywall politic.
 *
 * @param unknown $politic_instance          
 */
function _dpissoaccessmanager_paywall_moduleload($politic_instance) {
  $modules = $politic_instance->getmodulelist ();
  if (is_array ( $modules )) {
    foreach ( $modules as $module ) {
      drupal_load ( 'module', $module );
    }
  }
}
