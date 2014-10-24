<?php


/*
 * Check authentification from an email and a passowrd
 * 
 * @param string $email
 * The email to authenticate
 * 
 * @param string $password
 * The password for the account
 * 
 * @return string|bool
 * FALSE if the autheintification fail
 * the Login Id string if succeed
 * 
 * @see dpisso_login_validate()
 * 
 */
function dpisso_authentication_api_loginmanager_authenticate_user($email,$password){
  try
  {
    $result = Call_loginmanager($accessToken);
    if($result->ssoId) {
      $ssoId = $result->ssoId;
      return $ssoId;
    }
    return false;
  }
  catch (Exception $e) {
    watchdog('prjauth', 'Error %error_message.', array('%error_message' => $e->faultstring), WATCHDOG_NOTICE);
    return false;
  }
}

function dpisso_authentication_api_loginmanager_getIdSso(){
  _dpisso_authentication_api_call_loginmanager('getIdSSO', $options);
}

function dpisso_authentication_api_loginmanager_logout(){
  
}

function dpisso_authentication_api_loginmanager_login(){
  _dpisso_authentication_api_call_loginmanager('login', $options);
  
}

function dpisso_authentication_api_get_unitid(){
  $default_value=dpi_variable_get('dpicommons_product', '')."_".dpi_variable_get('dpicommons_environment', '');
  return dpi_variable_get('dpisso_unitid',$default_value);
}

function dpisso_authentication_api_login_manager_authenticate_user($user, $pass){
  
}

function dpisso_api_get_current_url() {
  $pageURL = 'http';
  if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
  $pageURL .= "://";
  if ($_SERVER["SERVER_PORT"] != "80") {
    $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
  } else {
    $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
  }
  return $pageURL;
}
