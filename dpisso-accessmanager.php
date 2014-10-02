<?php

/**
 * @file
 * The PHP page for all Acess Manager bootstrap callbacks
 */

/**
 * Root directory of Drupal installation.
 */
define('DRUPAL_ROOT', getcwd());

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpicache/dpicache.api.inc';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';

$operation=check_plain($_GET["operation"]);
$url_arguments=$_GET;
if(!isset($operation)){
  print "ERROR No operation";
  header('HTTP/1.1 403 Forbidden', true, 403);
} else {
  $return = FALSE;
  switch($operation){
  	case "getlists" :
  	  $list=dpi_variable_get('dpisso_accessmanager_whitelist',"");
  	  $return["whitelist"]=explode("\r\n", $list);
  	  $list=dpi_variable_get('dpisso_accessmanager_blacklist',"");
  	  $return["blacklist"]=explode("\r\n", $list);
  	  break;
    case "getwhitelist" :
      $list=dpi_variable_get('dpisso_accessmanager_whitelist',"");
      $return["whitelist"]=explode("\r\n", $list);
      break;
    case "getblacklist" :
      $list=dpi_variable_get('dpisso_accessmanager_blacklist',"");
      $return["blacklist"]=explode("\r\n", $list);
      break;
    default:
      print "Invalid operation";
      header('HTTP/1.1 403 Forbidden', true, 403);
      break;
  }

  // Print the results.
  if ($return===FALSE || $return===NULL) {
    _dpissoaccessmanager_return_json(FALSE, 503, "Internal Server Error");
  } else {
    $return["unitId"]=dpisso_authentication_api_get_unitid();
    _dpissoaccessmanager_return_json($return, 200);
    
  }
}

/**
 * Print an error page OR the data in JSON form. 
 */
function _dpissoaccessmanager_return_json($data, $httperror, $httperrormessage="") {
  if ($httperror!=200) {
    header('HTTP/1.1 '.$httperror.' '.$httperrormessage, true, $httperror);
  } else {
    header('Content-Type: application/json');
    echo json_encode($data);
  }
}
