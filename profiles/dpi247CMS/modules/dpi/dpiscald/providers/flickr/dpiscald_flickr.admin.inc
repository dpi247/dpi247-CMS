<?php

/**
 * @file
 * Provides admin form for Flickr's Scald Provider.
 */

/**
 * Defines the flickr settings form.
 */
function dpiscald_flickr_settings_form() {
  $form = array();
  // Flickr api key configuration.
  $dpiscald_flickr_api_key = variable_get('dpiscald_flickr_api_key', '');
  $form['dpiscald_flickr_api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr API'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['dpiscald_flickr_api']['dpiscald_flickr_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr API Key'),
    '#required' => TRUE,
    '#default_value' => $dpiscald_flickr_api_key,
  );
  if (empty($dpiscald_flickr_api_key)) {
    $form['dpiscald_flickr_api']['dpiscald_flickr_api_key']['#description'] = t('API Key from Flickr. !link', array(
      '!link' => l(t('Get one!'), 'http://www.flickr.com/services/apps/by/me'))
    );
  }
  else {
    $form['dpiscald_flickr_api']['dpiscald_flickr_api_key']['#description'] = t('API Key from Flickr.');
  }

  $form['dpiscald_flickr_api']['dpiscald_flickr_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr API Shared Secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('dpiscald_flickr_api_secret', ''),
    '#description' => t("API key's secret from Flickr."),
  );

  return system_settings_form($form);
}
