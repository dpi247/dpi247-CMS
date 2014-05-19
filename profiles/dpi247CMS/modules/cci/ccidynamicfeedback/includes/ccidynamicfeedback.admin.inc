<?php

function ccidynamicfeedback_form_admin_form($form, $form_state) {
  $options = module_invoke_all('ccidynamicfeedback_options', 'list');

  $form['ccidynamicfeedback_display_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose informations to display'),
    '#options' => $options,
    '#default_value' => dpi_variable_get('ccidynamicfeedback_display_options', array()),
  );

  return dpi_system_settings_form($form);
}
