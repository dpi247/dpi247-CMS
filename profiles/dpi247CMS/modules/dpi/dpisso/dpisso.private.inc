<?php

/*
 * Get an instance of 'paywallpolitic' class & dependances.
 */
function _dpisso_get_paywall_instance($id) {
  
  if ($id==null) return null;
   
  $instances = &drupal_static(__FUNCTION__);

  if (!isset($instances[$id])) {
    ctools_include('plugins');
    $plugin = ctools_get_plugins('dpisso', 'paywallpolitic', $id);
    $class = ctools_plugin_get_class($plugin, 'handler');
    $instances[$id] = new $class($id);

    // If the class didn't extend mine, return null
    if (!is_subclass_of($instances[$id], 'PaywallPolitics')) {
      $instances[$id] = NULL;
    }
  }
  return $instances[$id];
}



function _dpisso_login_callback(){
}

function _dpisso_logout_callback(){
}

function _dpisso_authentication_api_call_loginmanager($op,$options){
  $unit_id="";
  $accesstoken="";
  
  $login_manager_domain="";
  
  switch($op){
    case 'login':
      $login_manager_url='getlogin';
      break;
    case 'logout':
      $login_manager_url='';
      break;
    case 'getLoginId':
      $login_manager_url='getloginidbyaccesstoken';
      break;
  }
}

/**
 * validation callback against the login manager for the login form.
 * 
 * @see dpisso_authentication_api_login_manager_authenticate_user()
 */
function dpisso_login_validate( $form, &$form_state )
{
  $username = $form_state['values']['name'];
  $response = dpisso_authentication_api_login_manager_authenticate_user($username, $form_state['values']['pass'] );
  if ($response != false)
  {
    user_external_login_register( $username, 'dpisso' );
    $account = user_external_load($username);
    $form_state['uid'] = $account->uid;
  } // else drop through to the end and return nothing - Drupal will handle the rejection for us
}



