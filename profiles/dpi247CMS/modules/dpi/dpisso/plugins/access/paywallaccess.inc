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
  'default' => array(
    'equality' => 1,
  ),
  'settings form' => 'dpisso_paywall_access_users_settings',
  'summary' => 'ctools_compare_users_ctools_access_summary',
  'required context' => array(
    new ctools_context_required(t('First User'), 'user'),
    new ctools_context_required(t("Second User"), 'user')
  ),
);

/**
 * Settings form for the 'by perm' access plugin
 */
function ctools_compare_users_settings($form, &$form_state, $conf) {
  $form['settings']['helptext'] = array(
    '#type' => 'markup',
    '#value' => '<div>' . t('Coucou.') . '</div>',
  );
  
  return $form;
}

/**
 * Check for access.
 */
function dpisso_compare_users_access_check($conf, $context) {
  global $user;
  return (isset($user->dpisso['accessmanager']['freemium']) && $user->dpisso['accessmanager']['freemium']) ? true : false;
}

/**
 * Describe an instance of this plugin.
 */
function dpisso_paywall_access_ctools_access_summary($conf, $context) {
  $comparison = !empty($conf['equality']) ? "is" : 'is not';
  return t('@id1 @comp @id2', array('@comp' => $comparison, '@id1' => $context[0]->identifier, '@id2' => $context[1]->identifier));
}
