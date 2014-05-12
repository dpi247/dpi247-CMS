<?php

/**
 * Old nodes purge administration page
 */
function dpidam_page_admin_purge_form($form, $form_state) {
  if (dpi_variable_get('dpidam_historic_archive_done', FALSE)) {
    $form['dpidam_purge_activated'] = array(
      '#type' => 'checkbox',
      '#title' => t('Purge old nodes'),
      '#description' => t('When checked, everyday nodes older than an amount of days will be deleted.<br>
          <strong style="color:red;">Be sure that old nodes are archived before activating the purge!</strong>'),
      '#default_value' => dpi_variable_get('dpidam_purge_activated', 0),
    );

    $form['dpidam_purge_age'] = array(
      '#type' => 'textfield',
      '#title' => t('Old nodes age limit'),
      '#description' => t('Nodes older than this amount of days will be deleted'),
      '#default_value' => dpi_variable_get('dpidam_purge_age', 15),
      '#states' => array(
        'visible' => array(
          ':input[name="dpidam_purge_activated"]' => array('checked' => TRUE),
        ),
      ),
    );

    $run_period_options = array(
      'all_day' => t('All day'),
      'night' => t('Only at night (from midnight to 4 AM)'),
    );
    $form['dpidam_purge_run_period'] = array(
      '#type' => 'select',
      '#title' => t('Run the archiving process during'),
      '#description' => t('This process could add some load to your infrastructure, you may want to run it during night only.'),
      '#options' => $run_period_options,
      '#default_value' => dpi_variable_get('dpidam_purge_run_period', 'night'),
      '#states' => array(
        'visible' => array(
          ':input[name="dpidam_purge_activated"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['dpidam_purge_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Amount of nodes to delete at each cron run'),
      '#description' => t('This amount can\'t be too high for the cron to run properly.'),
      '#default_value' => dpi_variable_get('dpidam_purge_limit', 20),
      '#states' => array(
        'visible' => array(
          ':input[name="dpidam_purge_activated"]' => array('checked' => TRUE),
        ),
      ),
    );
  } else {
    drupal_set_message(t('All the old nodes have to been send to the DAM yet, you can\'t activate the purge untill it\'s over'), 'warning');
  }

  $form['#submit'][] = 'dpidam_page_admin_purge_form_submit';
  return dpi_system_settings_form($form);
}

function dpidam_page_admin_purge_form_validate($form, $form_state) {
  $values = $form_state['values'];

  if (isset($values['dpidam_purge_activated']) && $values['dpidam_purge_activated']) {
    if (empty($values['dpidam_purge_age'])) {
      form_set_error('dpidam_purge_age', t('This field is required.'));
    }

    $purge_age = $values['dpidam_purge_age'];
    if (!is_numeric($purge_age) || $purge_age < 1) {
      form_set_error('dpidam_purge_age', t('The purge age must be a numerical value greater or equal than 1 day.'));
    }
  }
}

function dpidam_page_admin_purge_form_submit($form, $form_state) {
  drupal_set_message(t('This functionnality is not activated yet!'), 'warning');
}
