<?php 

function ccimapping_form_configuration_form($form, $form_state) {
  $form['ccimapping_ws_wsdluri'] = array(
    '#type' => 'textfield',
    '#title' => t('CCI Web WSDL\'s URI'),
    '#default_value' => dpi_variable_get('ccimapping_ws_wsdluri', ''),
    '#required' => TRUE,
  );

  $form['ccimapping_newsroom'] = array(
    '#type' => 'textfield',
    '#title' => t('Newsroom'),
    '#default_value' => dpi_variable_get('ccimapping_newsroom', ''),
    '#required' => TRUE,
  );

  $form['ccimapping_type'] = array(
    '#type' => 'textarea',
    '#title' => t('Type mapping between CCI and DPI'),
    '#default_value' => dpi_variable_get('ccimapping_type', ''),
    '#description' => t('One line per type, dpi_machine_name_type|cci_machine_name_type'),
  );

  return dpi_system_settings_form($form);
}
