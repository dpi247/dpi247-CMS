<?php

header('Content-Type: text/html; charset=utf-8');

/**
 * @file The PHP page for all Acess Manager bootstrap callbacks
 */
define ( 'DRUPAL_ROOT', getcwd () );
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap (DRUPAL_BOOTSTRAP_FULL);

require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpicache/dpicache.api.inc';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';
require_once DRUPAL_ROOT . '/includes/file.inc';
require_once DRUPAL_ROOT .'/modules/filter/filter.module';
require_once DRUPAL_ROOT .'/modules/user/user.module';

drupal_load('module', 'ctools');
drupal_load('module', 'dpisso');
ctools_include ( 'plugins' );

if (($politic=dpi_variable_get ( 'dpisso_paywall_paywallpolitics', null ))!=null) {
  $plugin = ctools_get_plugins ( 'dpisso', 'paywallpolitic'  , $politic);
  if ($class = ctools_plugin_get_class ( $plugin, 'handler' )) {
    $politic_instance = new $class ();
    _dpissoaccessmanager_paywall_moduleload ( $politic_instance );          
    if(isset($_GET['url']) && isset($_GET ["operation"])){
      $operation = check_plain ( $_GET ["operation"] );      
      _dpissoaccessmanager_paywall_define_return($operation, $politic_instance);
    }else{
      header ( 'HTTP/1.1 405 Internal Server Error - Invalid Arguments.', true, 405);
      echo json_encode ( null );
      die ();
    }
  } else {
    header ( 'HTTP/1.1 503 Internal Server Error - Paywall class not defined.', true, 503 );
    echo json_encode ( null );
    die ();
  }
} else {
  header ( 'HTTP/1.1 503 Internal Server Error - No paywall politic define', true, 503 );
  echo json_encode ( null );
  die ();
}

/**
 * Load the list of module for current paywall politic.
 * @param Object $politic_instance          
 */
function _dpissoaccessmanager_paywall_moduleload($politic_instance) {
  $modules = $politic_instance->getmodulelist ();
  if (is_array ( $modules )) {
    foreach ( $modules as $module ) {
      drupal_load ( 'module', $module );
    }
  }
}

/**
 * Execute the operation pass by url
 * @param String $operation
 * @param Object $politic_instance
 */
function _dpissoaccessmanager_paywall_define_return($operation, $politic_instance){  
  switch ($operation) {
    case "is_secure_page" :
        $return = $politic_instance->issecurepage ( $_GET ["url"], (isset($_GET ["services"]) )? $_GET['services']:NULL);
        header ( 'HTTP/1.1 '.$return['code'].' invalid Operation', null, $return['code'] );
        header ( 'Content-Type: application/json' );
	    echo json_encode ( $return );
	    die ();     
      break;
    default :
        header ( 'HTTP/1.1 405 invalid Operation', null, 405 );
        echo json_encode ( null );
        die ();
      break;
  }
}
