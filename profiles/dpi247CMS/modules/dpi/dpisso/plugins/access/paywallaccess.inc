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
  'callback' => 'dpisso_compare_users_access_check',
  'default' => array('operator' => '='),
  'summary' => 'dpisso_paywall_access_ctools_access_summary',
  'required context' => new ctools_context_required(t('Context'), 'any', TRUE),
);


/**
 * Check for access.
 */
function dpisso_compare_users_access_check($conf, $context) {
  global $user;

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
