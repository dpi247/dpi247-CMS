<?php

/**
 * Old nodes archiving settings
 */
function dpidam_page_admin_historic_form($form, $form_state) {
  $form['dpidam_historic_activated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate old nodes archiving'),
    '#default_value' => dpi_variable_get('dpidam_historic_activated', 0),
  );

  $run_period_options = array(
    'all_day' => t('All day'),
    'night' => t('Only at night (from midnight to 4 AM)'),
  );
  $form['dpidam_historic_run_period'] = array(
    '#type' => 'select',
    '#title' => t('Run the archiving process during'),
    '#description' => t('This process could add some load to your infrastructure, you may want to run it during night only.'),
    '#options' => $run_period_options,
    '#default_value' => dpi_variable_get('dpidam_historic_run_period', 'night'),
    '#states' => array(
      'visible' => array(
        ':input[name="dpidam_historic_activated"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['dpidam_historic_node_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount of nodes to archive at each cron run'),
    '#description' => t('This amount can\'t be too high for the cron to run properly.'),
    '#default_value' => dpi_variable_get('dpidam_historic_node_limit', 20),
    '#states' => array(
      'visible' => array(
        ':input[name="dpidam_historic_activated"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['#submit'][] = 'dpidam_page_admin_historic_form_submit';
  return dpi_system_settings_form($form);
}

function dpidam_page_admin_historic_form_validate($form, $form_state) {
  $values = $form_state['values'];

  if ($values['dpidam_historic_activated']) {
    if (empty($values['dpidam_historic_node_limit'])) {
      form_set_error('dpidam_historic_node_limit', t('This field is required.'));
    }

    $node_limit = $values['dpidam_historic_node_limit'];
    if (!is_numeric($node_limit) || $node_limit < 1) {
      form_set_error('dpidam_historic_node_limit', t('The node limit must be a numerical value greater or equal than 1.'));
    }
  }
}

function dpidam_page_admin_historic_form_submit($form, $form_state) {
  drupal_set_message(t('This functionnality is not activated yet!'), 'warning');
}

/**
 * Send a fixed amount of packages to the DAM at each cron run
 */
function dpidam_historic_cron() {
  if (_dpidam_historic_cron_has_to_run()) {
    print '<strong>'.t('Old nodes archiving :').'</strong><br>';

    $nids = _dpidam_historic_get_nids();

    if (empty($nids)) {
      // If not nodes left to archive, stop the process and flag the archive process as done
      dpi_variable_set('dpidam_historic_activated', FALSE);
      dpi_variable_set('dpidam_historic_archive_done', TRUE);
      return;
    }

    foreach ($nids as $nid) {
      $result = _dpidam_post_package($nid);
      dpi_variable_set('dpidam_historic_last_node', $nid);
      if ($result['success']) {
        print t('Archived node @nid', array('@nid' => $nid));
      } else {
        print t('Error while archiving node @nid. Error message : @message', array('@nid' => $nid, '@message' => $result['message']));
      }
      print '<br>';
    }
  }
}

/**
 * Get an array of nids to process
 */
function _dpidam_historic_get_nids() {
  // Put an upper limit to the nodes to process,
  // we only want the old nodes, not the nodes created after this process started
  $most_recent_nid_limit = dpi_variable_get('dpidam_historic_most_recent_nid', FALSE);
  if (!$most_recent_nid_limit) {
    $db_max_nid = db_select('node', 'n');
    $db_max_nid->addExpression('MAX(n.nid)', 'max_nid');
    $res_max_nid = $db_max_nid
      ->condition('n.type', 'package')
      ->execute();
    foreach ($res_max_nid as $max_nid) {
      $most_recent_nid_limit = $max_nid->max_nid;
    }
    dpi_variable_set('dpidam_historic_most_recent_nid', $most_recent_nid_limit);
  }

  $last_node = dpi_variable_get('dpidam_historic_last_node', 0);
  $node_limit = dpi_variable_get('dpidam_historic_node_limit', 20);

  $db_nodes = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.nid', $last_node, '>')
    ->condition('n.nid', $most_recent_nid_limit, '<')
    ->condition('n.type', 'package')
    ->orderBy('n.nid', 'ASC')
    ->range(0, $node_limit)
    ->execute();
  $nids = array();
  foreach ($db_nodes as $node) {
    $nids[] = $node->nid;
  }

  return $nids;
}

/**
 * Check if the cron process has to run : if it is set to run all day or if we are at night
 *
 * @return boolean Whether the cron has to run or not
 */
function _dpidam_historic_cron_has_to_run() {
  if (!dpi_variable_get('dpidam_historic_activated', FALSE)) {
    return FALSE;
  }

  switch (dpi_variable_get('dpidam_historic_run_period', 'night')) {
    case 'all_day':
      return TRUE;
      break;

    case 'night':
      $current_hour = (int)date('G');
      if ($current_hour >= 0 && $current_hour < 4) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}
