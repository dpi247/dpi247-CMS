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
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


require_once DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/Config.class.php';
require_once DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/SsoSession.class.php';

/*
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.private.inc';
require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpicache/dpicache.api.inc';
require_once DRUPAL_ROOT . '/modules/user/user.pages.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';
*/

$SsoSession= new SsoSession(DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config.json');

//We are on the login operation
if($login_id=$SsoSession->getLoginId()){
  LoginManager::setCookie ( 'dpisso_is_connected', true , Time() + 3600*24*52, $_SERVER['HTTP_HOST'], '/');
  $profile=$SsoSession->getProfile();
  $roles=$SsoSession->getRoles($_SERVER["REQUEST_URI"]);
  $sso_user_infos['mail']=$profile->mail;
  $sso_user_infos['name']=$profile->cn;
  $sso_user_infos['roles'] = dpisso_api_parse_array_to_role_array($roles);
  
  dpisso_user_external_login_register($login_id, 'dpisso',$sso_user_infos);
}
//We are on the logout operation
else{
  //@todo: We should remove the loginToken cookie on logout to prevent loop and no man's land.
  LoginManager::setCookie ( 'dpisso_is_connected', false , Time() - 36000, $_SERVER['HTTP_HOST'], '/');
  dpisso_api_user_logout();
   //@todo: should i call the Login manager ?
}
die();
