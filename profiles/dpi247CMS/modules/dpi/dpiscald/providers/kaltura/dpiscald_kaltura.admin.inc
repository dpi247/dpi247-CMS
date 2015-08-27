<?php
function dpiscald_kaltura_configuration_form() {
  $form = array();
  $form['dpiscald_kaltura']['dpiscald_kaltura_partner_id'] = array(
    '#title' => t('Partner ID'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpiscald_kaltura_partner_id', "1986501"),
  );
  $form['dpiscald_kaltura']['dpiscald_kaltura_user'] = array(
    '#title' => t('User'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpiscald_kaltura_user', "xdecock"),
  );
  $form['dpiscald_kaltura']['dpiscald_kaltura_admin_secret'] = array(
    '#title' => t('Administrator Secret'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpiscald_kaltura_admin_secret', "5fcd1d63c31c748600ed23e0e6415164"),
  );
  $form['dpiscald_kaltura']['dpiscald_kaltura_user_secret'] = array(
    '#title' => t('User Secret'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpiscald_kaltura_user_secret', "7fc8fe4f93a8690cbc73af9c368658a7"),
  );

  return system_settings_form($form);
}