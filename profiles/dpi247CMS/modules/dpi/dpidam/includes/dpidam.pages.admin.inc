<?php

/**
 * Packages synchronization settings form.
 */
function dpidam_page_admin_packages_sync_form($form, $form_state) {
  global $base_url;
  
  $form['dpidam_updateurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the updated alias'),
    '#description' => t('URL to send the information of the node.'),
    '#default_value' => dpi_variable_get('dpidam_updateurl', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['dpidam_updatepackageurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the updated packages'),
    '#description' => t('URL to send the updated package.'),
    '#default_value' => dpi_variable_get('dpidam_updatepackageurl', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['dpidam_cronmaxnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of packages sent in a post'),
    '#description' => t('Maximum number of packages sent in a post'),
    '#default_value' => dpi_variable_get('dpidam_cronmaxnumber', 50),
    '#size' => 80,
    '#maxlenght' => 512,
    '#required' => TRUE,
  );

  $form['dpidam_main_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Host and protocol used to DAM for URL alias (default value: %base_domain)',array("%base_domain"=>$base_url)),
    '#description' => "Use <i>protocol://host</i> format. If left blank, the default value is : <i>".$base_url."</i>",
    '#default_value' => dpi_variable_get('dpi_main_domain', ''),
    '#size' => 80,
    '#maxlenght' => 512,
    '#required' => FALSE,
  );
  
  $form['#submit'][] = 'dpidam_page_admin_packages_sync_form_submit';
  return dpi_system_settings_form($form, TRUE);
}

/**
 * Validate function for the settings form.
 */
function dpidam_page_admin_packages_sync_form_validate($form, $form_state) {
  $values = $form_state['values'];

  if (!valid_url($values['dpidam_updateurl'], TRUE)) {
    form_set_error('dpidam_updateurl', t('The given URL is not valid'));
  }

  if (!valid_url($values['dpidam_updatepackageurl'], TRUE)) {
    form_set_error('dpidam_updatepackageurl', t('The given URL is not valid'));
  }

  if (!is_numeric($values['dpidam_cronmaxnumber']) || $values['dpidam_cronmaxnumber'] < 0) {
    form_set_error('dpidam_cronmaxnumber', t('The maximum number of packages must be a numeric value greater than 0'));
  }
  
  if(!empty($form_state['values']['dpidam_main_domain'])){
    if(!preg_match('/^[a-z]+:\/\/[a-z0-9_\.-]+$/',$form_state['values']['dpidam_main_domain'])){
      form_set_error('dpidam_main_domain', t('Domain must be this format : <i>protocol://host</i>. Without trailing slash.'));
    }
  }
}

/**
 * Submit function for the settings form.
 */
function dpidam_page_admin_packages_sync_form_submit($form, $form_state) {
  drupal_set_message(t('This functionnality is not activated yet!'), 'warning');
}
