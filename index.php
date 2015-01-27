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
define ( 'DRUPAL_ROOT', getcwd () );
 
global $dpisso;

if (file_exists ( $_SERVER ['DOCUMENT_ROOT'] . "/sites/all/libraries/ssophptoolbox/SsoSession.class.php" ) && file_exists ( $_SERVER ['DOCUMENT_ROOT'] . "/sites/all/libraries/ssophptoolbox/Config.class.php" )) {
    require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/libraries/ssophptoolbox/Config.class.php';
    require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/libraries/ssophptoolbox/AccessManager.class.php';
    require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/libraries/ssophptoolbox/LoginManager.class.php';
    require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/libraries/ssophptoolbox/SsoSession.class.php';
    require_once $_SERVER ['DOCUMENT_ROOT'] . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';

    $config = Config::getInstance($_SERVER ['DOCUMENT_ROOT'] . '/sites/all/libraries/ssophptoolbox/config/ssoClient.ini')->getConfigurationInstance();
    if(isset($_COOKIE[$config['loginToken_cookie_name']])){
      $loginId = LoginManager::getLoginId($_COOKIE[$config['loginToken_cookie_name']], TRUE);
      if($loginId == FALSE){
        $destroy_session = TRUE;
      }        
    }   
    
    if(isset($loginId)){
      $drupal_session_auto_connect = TRUE;
      $SsoSession = new SsoSession ( $loginId );
    }else{
      $drupal_session_auto_connect = FALSE;
      $SsoSession = new SsoSession (  );
    }

    $context = $_SERVER ["REQUEST_URI"];
    preg_match('/\badmin/i', $context, $matches);
    if(!isset($matches[0])){
      $rolesd = $SsoSession->getFreemiumInfo($context);
      $dpisso = array(
        'accessmanager' => array(
          'freemium' => (isset($rolesd->nbFreemium) && $rolesd->nbFreemium) ? true : false,
          'freemium_count' => (isset($rolesd->nbFreemium)) ? $rolesd->nbFreemium : 0,
        )
      );
    } 
}

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap ( DRUPAL_BOOTSTRAP_FULL );

if(isset($destroy_session) && $destroy_session == TRUE && user_is_logged_in()){
  dpisso_api_user_logout();
}
//drupal_set_message("Freemium count: ".$dpisso["accessmanager"]["freemium_count"]);

//@todo: Attention au cas ou je suis logué sur le Drupal Mais je n'ai pas les cookies longterm_cookie_name et longterm_cookie_name
if(function_exists('libraries_load') && is_array(libraries_load ('ssophptoolbox'))){
  if(file_exists(DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc') && file_exists(DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module') && file_exists(DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoClient.ini')){
    require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';
    require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module';
    if (isset ( $_COOKIE [$config ['longterm_cookie_name']] ) && strcmp ( $_COOKIE [$config ['longterm_cookie_name']], 'true' ) == 0 && ! isset ( $_COOKIE [$config ['loginToken_cookie_name']] )) {
      $string = file_get_contents ( DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoFederationConfig.json' );
      $ssoFederationConfig = json_decode ( $string, true );
      $unitId = LoginManager::getUnitId ( $ssoFederationConfig );
      $redirect_url = $config ["ssoServer_url"] . '/html/login?unitId=' . $unitId . "&returnPage=" . urlencode ( dpisso_api_get_current_url () ) . "&bypassForm=true";
      header ( "Location: $redirect_url" );
    }
  }
}

/* test de connexion via le login token -> co automatique */
if(isset($drupal_session_auto_connect) && $drupal_session_auto_connect == TRUE && (!isset($_COOKIE['dpisso_is_connected']) || !user_is_logged_in()) && (!isset($destroy_session) || $destroy_session != TRUE)){
  require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';
  $profile=$SsoSession->getProfile();
  $roles=$SsoSession->getRoles($_SERVER["REQUEST_URI"]);
  $sso_user_infos['mail']=$profile->mail;
  $sso_user_infos['name']=$profile->cn;
  $sso_user_infos['roles'] = dpisso_api_parse_array_to_role_array($roles);
  dpisso_user_external_login_register($loginId, 'dpisso',$sso_user_infos);  
  $federationConfig = LoginManager::getCookieInfo();
  LoginManager::setCookie ( 'dpisso_is_connected', true, Time()+3600*24*52, $federationConfig["domain"], $federationConfig["path"]);
}

menu_execute_active_handler ();
