<?php 

/**
 * General settings for DPI247
 *
 * @param array $form_state
 */
function dpicommons_admin_settings_form($form, $form_state) {
  $form['dpicommons_product'] = array(
    '#type' => 'textfield',
    '#title' => t('ProductID'),
    '#description' => t('The product of the website, it must be unique.'),
    '#default_value' => dpi_variable_get('dpicommons_product', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['dpicommons_environment'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment'),
    '#description' => t('Stage, Prod, Dev, etc.'),
    '#default_value' => dpi_variable_get('dpicommons_environment', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['dpicommons_http_request_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP Request Timeout'),
    '#description' => t('Specifies the default timeout limit (in seconds) for every HTTP request made from this site'),
    '#default_value' => dpi_variable_get('dpicommons_http_request_timeout', 3),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#validate'][] = 'dpicommons_admin_settings_form_validate';

  return dpi_system_settings_form($form);
}

/**
 * Validation of dpicommons_admin_settings_form
 *
 * @param array $form
 * @param array $form_state
 */
function dpicommons_admin_settings_form_validate($form, &$form_state){
  if (!preg_match('/^[0-9]+$/' , $form_state['values']['dpicommons_http_request_timeout'])){
    form_set_error('dpicommons_http_request_timeout', t('HTTP Request Timeout must be an integer.'));
  }
}
