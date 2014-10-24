<?php

//SANS hops
//http://dpi7-dev.drupal.dev/dpisso-loginmanager.php?operation=login&loginToken=c083264f7a4fa40b4210e20e2d8d33342702f0863b24322835c5b83222d34392&loginTokenValidity=Coco&longTerm=tata&longTermValidity=tutu&returnPage=http://francois.be


//Avec HOP ET RETURN PAGE
//http://dpi7-dev.drupal.dev/dpisso-loginmanager.php?operation=login&loginToken=c083264f7a4fa40b4210e20e2d8d33342702f0863b24322835c5b83222d34392&hops[]=http://one.com&hops[]=http://two.com&loginTokenValidity=Coco&longTerm=tata&longTermValidity=tutu&returnPage=francois.be

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
require_once DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/Config.class.php';
require_once DRUPAL_ROOT . '/modules/user/user.pages.inc';

$config=Config::getInstance(DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoClient.ini');

if(!isset($_COOKIE['monsitedpi7'])){
  setcookie ( "monsitedpi7", md5(uniqid(rand(), true)), time()+3600*24*3);
}
$SsoSession= new SsoSession();
$redirect_url=$SsoSession->processLoginManagerUrl();
//We are on the login operation
if($login_id=$SsoSession->getLoginId()){
  global $user;
  
  $profile=$SsoSession->getProfile();
  $roles=$SsoSession->getRoles();
  
  $sso_user_infos['mail']=$profile->mail;
  $sso_user_infos['name']=$profile->cn;
  $sso_user_infos['roles'] = dpisso_api_parse_array_to_role_array($roles);

  //@todo: sync roles with existing ones
  dpisso_user_external_login_register($login_id, 'dpisso',$sso_user_infos);  
}
//We are on the logout operation
else{
   user_logout();
   //@todo: should i call the Login manager ?
}

//Process to redirect
header("Location: $redirect_url"."?status=".LoginManager::$error_code."&message=".LoginManager::$error_message);
die();
