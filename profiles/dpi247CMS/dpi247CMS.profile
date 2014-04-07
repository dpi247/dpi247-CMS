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
