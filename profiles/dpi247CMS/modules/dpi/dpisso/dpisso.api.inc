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
function dpisso_authentication_api_login_manager_authenticate_user($email,$password){
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