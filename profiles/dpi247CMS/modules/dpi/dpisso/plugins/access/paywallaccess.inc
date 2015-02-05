<?php

/**
 * @file
 * Ctools access plugin to provide access/visiblity if two user contexts are equal.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Dpisso: paywall blocked access"),
  'description' => t('Block the user if he has 0 freemium'),
  'callback' => 'dpisso_paywallaccess_ctools_access_check',
  'summary' =>  'dpisso_paywallaccess_ctools_access_summary',
    'settings form' => 'dpisso_paywallaccess_ctools_access_settings_form',
 //   'required context' => new ctools_context_required(t('Node'), 'node'),
);

function dpisso_paywall_access_ctools_access_settings_form(&$form, &$form_state, $conf) {
    $form=array();

    return $form;
}

/**
 * Check for access.
 * @param array $conf
 * @param string $context
 * @return string
 */
function dpisso_paywallaccess_ctools_access_check($conf, $context) {
    global $user;
    global $dpisso;
    $return = FALSE;
    $bypass = dpi_variable_get('dpisso_bypass_user_roles', NULL);
    
    /* Test sso freemium account */
    if(isset($dpisso['accessmanager']['freemium']) && $dpisso['accessmanager']['freemium']){
        $return = FALSE;
    }
    else{
        $return = TRUE;
    }
    
    /* Bypass by admin dpisso config */
    if(isset($bypass) && dpisso_paywall_test_bypass($bypass, $user->roles)){
      $return = FALSE;
    }
    
    /* bypass admin */
    if(user_access('administer')){
      $return = FALSE;
    }
    return $return;
}

/**
 * This function test if the user have a role who need to be bypassed
 * @param array $bypass_role
 * @param array $user_role
 * @return boolean
 */
function dpisso_paywall_test_bypass($bypass_role = array(), $user_role = array()){
  if(is_array($bypass_role)){
    foreach($user_role as $role){
      if(in_array($role, $bypass_role)){
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Describe an instance of this plugin.
 * @param array $conf
 * @param string $context
 * @return string
 */
function dpisso_paywall_access_ctools_access_summary($conf, $context) {
  return t('Match if user has no more freemium credits');
}
