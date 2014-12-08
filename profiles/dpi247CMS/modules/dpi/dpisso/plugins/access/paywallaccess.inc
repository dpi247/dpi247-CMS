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
  'title' => t("Dpisso: paywall access"),
  'description' => t('Test if the user have more than 0 freemium'),
  'callback' => 'dpisso_check_if_user_has_freemium',
  'summary' => 'dpisso_paywall_access_ctools_access_summary',
    'settings form' => 'dpisso_paywall_access_ctools_access_settings_form',
    'all contexts' => TRUE,
);

function dpisso_paywall_access_ctools_access_settings_form()&$form, &$form_state, $conf) {
    $form=array();

    return $form;
}
/**
 * Check for access.
 */
function dpisso_check_if_user_has_freemium($conf, $context) {
    die();
return TRUE;
    global $dpisso;
    drupal_set_message('ooo');
    drupal_set_message(print_r($dpisso));
    if(isset($dpisso['accessmanager']['freemium']) && $dpisso['accessmanager']['freemium']){
        dsm("TRUE DPISSOACCESS");
        drupal_set_message('rr');
        return TRUE;
    }
    else{
        dsm("FALSE DPISSOACCESS");
        drupal_set_message('aaa');


        return FALSE;
    }
}

/**
 * Describe an instance of this plugin.
 */
function dpisso_paywall_access_ctools_access_summary($conf, $context) {

  return t('Match if user has no more freemium credits');
}
