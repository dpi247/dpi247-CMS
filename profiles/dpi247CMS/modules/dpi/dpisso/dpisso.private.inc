<?php



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
    user_external_login_register( $username, 'project_authentication' );
    $account = user_external_load($username);
    $form_state['uid'] = $account->uid;
  } // else drop through to the end and return nothing - Drupal will handle the rejection for us
}
