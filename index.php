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

global $user;

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap ( DRUPAL_BOOTSTRAP_FULL );


//@todo: Attention au cas ou je suis logué sur le Drupal Mais je n'ai pas les cookies longterm_cookie_name et longterm_cookie_name
if(function_exists('libraries_load') && is_array(libraries_load ('ssophptoolbox'))){
  if(file_exists(DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc') && file_exists(DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module') && file_exists(DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoClient.ini')){
    require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.api.inc';
    require_once DRUPAL_ROOT . '/profiles/dpi247CMS/modules/dpi/dpisso/dpisso.module';
    $config = Config::getInstance ( DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoClient.ini' )->getConfigurationInstance();
    if (isset ( $_COOKIE [$config ['longterm_cookie_name']] ) && strcmp ( $_COOKIE [$config ['longterm_cookie_name']], 'true' ) == 0 && ! isset ( $_COOKIE [$config ['loginToken_cookie_name']] )) {
      $string = file_get_contents ( DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config/ssoFederationConfig.json' );
      $ssoFederationConfig = json_decode ( $string, true );
      $unitId = LoginManager::getUnitId ( $ssoFederationConfig );
      $redirect_url = $config ["ssoServer_url"] . '/html/login?unitId=' . $unitId . "&returnPage=" . urlencode ( dpisso_api_get_current_url () ) . "&bypassForm=true";
      header ( "Location: $redirect_url" );
    }
  }




}


menu_execute_active_handler ();
