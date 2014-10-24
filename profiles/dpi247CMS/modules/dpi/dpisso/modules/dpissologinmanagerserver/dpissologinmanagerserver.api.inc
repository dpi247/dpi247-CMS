<?php

function dpissologinmanagerserver_api_check_email_unicity($mail) {
  $query = db_select ( 'dpissologinmanager_users', 'd' )->condition ( 'd.mail', $mail, '=' );
  $count = $query->countQuery ()->execute ()->fetchField ();
  if ($count == 0)
    return TRUE;
  return FALSE;
}

function dpissologinmanagerserver_api_check_loginid_unicity($loginId) {
  $query = db_select ( 'dpissologinmanager_users', 'd' )->condition ( 'd.loginId', $loginId, '=' );
  $count = $query->countQuery ()->execute ()->fetchField ();
  if ($count == 0)
    return TRUE;
  return FALSE;
}

function dpissologinmanagerserver_api_validate_connexion($mail, $mdp) {
  require_once DRUPAL_ROOT . '/' . variable_get ( 'password_inc', 'includes/password.inc' );
  try {
    $retour = FALSE;
    $query = db_select ( 'dpissologinmanager_users', 'd' )->condition ( 'mail', $mail, '=' )->fields ( 'd', array () )->execute ();
    $result = $query->fetchAssoc ();
    if (isset ( $result )) {
      $user = new stdClass ();
      if(is_array($result)){
        foreach ($result as $k => $v){
          $user->$k = $v;
        }
      }
      
      if(user_check_password($mdp, $user)){
        $retour = $user;
      }
    }
    return $retour;
  } catch ( Exception $e ) {
    return FALSE;
  }
}