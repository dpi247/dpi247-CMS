<?php
/*
 * Check authentification from an email and a passowrd @param string $email The email to authenticate @param string $password The password for the account @return string|bool FALSE if the autheintification fail the Login Id string if succeed @see dpisso_login_validate()
 */
function dpisso_authentication_api_loginmanager_authenticate_user($email, $password) {
  try {
    $result = Call_loginmanager ( $accessToken );
    if ($result->ssoId) {
      $ssoId = $result->ssoId;
      return $ssoId;
    }
    return false;
  } catch ( Exception $e ) {
    watchdog ( 'prjauth', 'Error %error_message.', array (
      '%error_message' => $e->faultstring 
    ), WATCHDOG_NOTICE );
    return false;
  }
}

function dpisso_authentication_api_loginmanager_getIdSso() {
  _dpisso_authentication_api_call_loginmanager ( 'getIdSSO', $options );
}

function dpisso_authentication_api_loginmanager_logout() {
}

function dpisso_authentication_api_loginmanager_login() {
  _dpisso_authentication_api_call_loginmanager ( 'login', $options );
}

function dpisso_authentication_api_get_unitid() {
  $default_value = dpi_variable_get ( 'dpicommons_product', '' ) . "_" . dpi_variable_get ( 'dpicommons_environment', '' );
  return dpi_variable_get ( 'dpisso_unitid', $default_value );
}

function dpisso_authentication_api_login_manager_authenticate_user($user, $pass) {
}

function dpisso_api_get_current_url() {
  $pageURL = 'http';
  if (isset ( $_SERVER ["HTTPS"] ) && $_SERVER ["HTTPS"] == "on") {
    $pageURL .= "s";
  }
  $pageURL .= "://";
  if ($_SERVER ["SERVER_PORT"] != "80") {
    $pageURL .= $_SERVER ["SERVER_NAME"] . ":" . $_SERVER ["SERVER_PORT"] . '/' . drupal_get_path_alias($_SERVER ["REQUEST_URI"]);
  } else {
    $pageURL .= $_SERVER ["SERVER_NAME"] . '/' . drupal_get_path_alias($_SERVER ["REQUEST_URI"]);
  }
  return $pageURL;
}

function dpisso_api_parse_array_to_role_array($aSimpleArray) {
  $return = array ();
  foreach ( $aSimpleArray as $role ) {
    if (($var = user_role_load_by_name ( $role )) != FALSE)
      $return [$var->rid] = $var->name;
  }
  return $return;
}

function dpisso_api_user_logout() {
  global $user;
  watchdog ( 'user', 'Session closed for %name.', array (
    '%name' => $user->name 
  ) );
  module_invoke_all ( 'user_logout', $user );
  session_destroy ();
}

function dpisso_api_get_paywall_info() {
  libraries_load ( 'ssophptoolbox' );
  $ssoSession = new SsoSession ( DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config.json' );
  return $ssoSession->accessManager->getPaywallInfo ();
}

function dpisso_api_set_paywall_info($info) {
  libraries_load ( "ssophptoolbox" );
  $ssoSession = new SsoSession ( DRUPAL_ROOT . '/sites/all/libraries/ssophptoolbox/config.json' );
  return $ssoSession->accessManager->setPaywallInfo ( $info );
}

/**
* Provides a URL allowing to log in to SSO V2 and to redirect to the specified return page.
* @param {String} $login The domain name from which you wish to login
* @param {String} $unit_id The site identifier for the login manager
* @param {String|NULL} $return_page The page you are redirected to after logging in
* @return {String} The full login URL
**/
function dpisso_get_login_url($login, $unit_id, $return_page=NULL){
  if(!$return_page){
    $return_page = urlencode(dpisso_api_get_current_url());
  }
  return $login . '/html/login?unitId=' . $unit_id . '&returnPage=' . $return_page;
}

/**
* Provides a URL allowing to log out from SSO V2 and to redirect to the specified return page.
* @param {String} $login The domain name from which you wish to logout
* @param {String} $unit_id The site identifier for the login manager
* @param {String|NULL} $return_page The page you are redirected to after logging out
* @return {String} The full logout URL
**/
function dpisso_get_logout_url($login, $unit_id, $return_page=NULL){
  if(!$return_page){
    $return_page = urlencode(dpisso_api_get_current_url());
  }
  return $login . '/html/logout?unitId=' . $unit_id . '&returnPage=' . $return_page;
}
