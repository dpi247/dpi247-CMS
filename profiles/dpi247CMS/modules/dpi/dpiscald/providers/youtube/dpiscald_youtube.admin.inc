<?php
function dpiscald_youtube_configuration_form() {
  $form = array();
  $form['dpiscald_kaltura']['dpiscald_kaltura_partner_id'] = array(
    '#title' => t('Youtube API Key'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpiscald_youtube_api_key', ""),
  );

  return system_settings_form($form);
}