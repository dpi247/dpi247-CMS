<?php

//http://dpi7-dev.drupal.dev/dpisso-loginmanager.php?operation=login&loginTokenValidity=%22Coco%22&longTerm=tata&longTermValidity=tutu&returnPage=francois.be&unitId=dpi_audaxis&loginToken=c094c2e2abcc7d24c63dd4fb8db643e47c28455708cc8b4f6c3ee8e1e9a8aedb
function pprint($toPrint){
  
  
  print "<pre>".print_r($toPrint,1)."</pre>";
}

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
//drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpicache/dpicache.api.inc';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/third/libraries/libraries.module';


require_once DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/Config.class.php';

$config=Config::getInstance(DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoClient.ini');

if(!isset($_COOKIE['monsitedpi7'])){
  setcookie ( "monsitedpi7", md5(uniqid(rand(), true)), time()+3600*24*3);
}

pprint($_GET);
$SsoSession= new SsoSession();
$redirect_url=$SsoSession->processLoginManagerUrl();

//We are on the login operation
if($login_id=$SsoSession->getLoginId()){
  global $user;
  
  $profile=$SsoSession->getProfile();
  $roles=$SsoSession->getRoles();
  
  
  $sso_user_infos['mail']=$profile['mail'];
  $sso_user_infos['name']=$profile['cn'];

  
  //@todo: sync roles with existing ones
  $user->roles=$roles;
  dpisso_user_external_login_register($login_id, 'dpisso',$sso_user_infos);
  
}
//We are on the logout operation
else{
  user_logout();
  //@todo: should i call the Login manager ?
  $redirect_url=LoginManager::logout($SsoSession);
}

//Process to redirect
header("Location: $redirect_url"."?status=".LoginManager::$error_code."&message=".LoginManager::$error_message);
die();
