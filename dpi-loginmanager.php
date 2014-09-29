<?php

/**
 * @file
 * The PHP page that serves all page requests on a Drupal installation.
 *
 * The routines here dispatch control to the appropriate handler, which then
 * prints the appropriate page.
 *
 * All Drupal code is released under the GNU General Public License.
 * See COPYRIGHT.txt and LICENSE.txt.
 */

/**
 * Root directory of Drupal installation.
 */


define('DRUPAL_ROOT', getcwd());

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);




require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpicache/dpicache.api.inc';


$operation=check_plain($_GET["operation"]);
$url_arguments=$_GET;
if(!isset($operation)){
  print "ERROR No operation";
  header('HTTP/1.1 403 Forbidden', true, 403);
}
else{
  switch($operation){
    case "login":
      $return=_dpisso_controller_login($url_arguments);

      break;
    case "logout":
      $return=_dpisso_controller_logout();
      break;
    default:
      print "Invalid operation";
      header('HTTP/1.1 403 Forbidden', true, 403);
      break;
  }

  if($return===FALSE){
    print "An error occured";
    header('HTTP/1.1 403 Forbidden', true, 403);

  }

}



//LOGIN // http://dpi7-dev.drupal.dev/dpi-loginmanager.php?operation=login&hops[0]=http://lacapitale.be&hops[1]=http://lameuse.be&accessToken=23412&accessTokenValidity=2014-12-01T00:30+02:00&longTerm=TRUE&longTermValidity=2015-02-01T00:30+02:00&returnPage=http://lesoir.be/node/1
//APPAREMENT PAS BESOIN DU UNITID




function _dpisso_controller_login($url_arguments){
  //check validity of request
  if(_dpisso_controller_request_is_valid($url_arguments)){

    //setCookie    
    _dpisso_controller_set_cookies($url_arguments);
    if($login_id=_dpisso_controller_get_loginid($url_arguments)){
      _dpisso_controller_user_authentification();
      _dpisso_controller_user_get_roles_from_login_manager();
      _dpisso_controller_redirect_to_return_pager_or_another_hop_of_federation();
      RETURN TRUE;
    }
    else{
      // if accessToken do not map with a real loginId => logout
      _dpisso_controller_logout();
    }

  }
  

  RETURN FALSE;

}

function _dpisso_controller_set_cookies($url_arguments){
  setcookie(DPISSO_ACCESSTOKEN_COOKIE_NAME, $url_arguments['accessToken'], strtotime($url_arguments['accessTokenValidity']));
  if($url_arguments['longTerm']=='TRUE'){
    setcookie(DPISSO_LONTERM_COOKIE_NAME, TRUE, strtotime($url_arguments['longTermValidity']));
  }
}

function _dpisso_controller_get_loginid($url_arguments){
  $loginmanager_domain= dpi_variable_get("dpisso_loginmanager_domain", FALSE);
  if($loginmanager_domain===FALSE){
    return FALSE;
  }
  else{
    print $loginmanager_domain;
  }
}


function _dpisso_controller_request_is_valid(){
  return TRUE;
}


//Sanitize the arguments and return if the parameters are valid
function _dpisso_controller_logout(& $url_arguments){
  
  //@todo: checker le format date, que l'accessToken ne contient que certains characteres etc ...
  
  $sanitized_arguments=array();
  
  foreach($url_arguments as $key=>$item){
    $sanitized_arguments[check_plain($key)]=check_plain($item);
  }
  $url_arguments=$sanitized_arguments;
  
  return TRUE;

}
