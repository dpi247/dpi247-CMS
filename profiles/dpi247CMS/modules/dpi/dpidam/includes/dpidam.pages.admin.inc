<?php

/**
 * Packages synchronization settings form.
 */
function dpidam_page_admin_packages_sync_form($form, $form_state) {
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
}

/**
 * Submit function for the settings form.
 */
function dpidam_page_admin_packages_sync_form_submit($form, $form_state) {
  drupal_set_message(t('This functionnality is not activated yet!'), 'warning');
}
