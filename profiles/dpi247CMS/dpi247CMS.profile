<?php
/**
 * @file
 * Enables modules and site configuration for a minimal site installation.
 */

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function dpi247CMS_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  $form['site_information']['site_mail']['#default_value'] = 'admin@'.$_SERVER['SERVER_NAME'];

  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'admin@'.$_SERVER['SERVER_NAME'];

  $form['server_settings']['site_default_country']['#default_value'] = 'BE';

  $form['update_notifications']['update_status_module']['#default_value'] = array(1, 0);
}

/**
 * This function change order and default selectbox from profil choice
 * 
 * @param array $form
 * @param array $form_state
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'dpi247CMS';
  }
  $form['profile']['dpi247CMS']['#weight'] = -40;
}

/**
 * 
 * @param unknown $form
 * @param unknown $form_state
 */
function system_form_install_configure_form_alter(&$form, $form_state) {

  $form['dpi247CMS'] = array(
    '#type' => 'fieldset',
    '#title' => t('dpi247CMS settings'),
    '#weight' => 10, // be sure we're at the end
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  
  $form['dpi247CMS']['dpi247CMSadmin_product'] = array(
    '#type' => 'textfield',
    '#title' => t('Product'),
    '#description' => t('The product of the website, it must be unique.'),
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['dpi247CMS']['dpi247CMSadmin_environment'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment'),
    '#description' => t('Stage, Prod, Dev, etc.'),
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['#submit'][] = '_dpi247CMS_install_form_submit';  
}

function _dpi247CMS_install_form_submit(&$form, $form_state){
  dpi_variable_set('dpicommons_product', $form_state['values']['dpi247CMSadmin_product']);
  dpi_variable_set('dpicommons_environment', $form_state['values']['dpi247CMSadmin_environment']);
}