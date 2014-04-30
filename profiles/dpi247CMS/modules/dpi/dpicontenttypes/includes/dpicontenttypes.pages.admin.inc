<?php

/**
 * Edit settings related to external files management
 */
function dpicontenttypes_form_page_external_files_form() {
  $form = array();

  $form['#prefix'] = t('Sometimes, external files provided by external services, like RSS feeds, are copied locally
      for performance issues. These files usually don\'t need to be stored for an infinite time.');

  $form['dpicontenttypes_files_days_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Remove files older than (in days)'),
    '#description' => t('The cron will remove files older than this amount of days'),
    '#default_value' => dpi_variable_get('dpicontenttypes_files_days_limit', 7),
  );

  return dpi_system_settings_form($form);
}
