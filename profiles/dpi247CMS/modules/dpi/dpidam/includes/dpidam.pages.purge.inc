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

/**
 * Cron task to purge old packages
 */
function dpidam_purge_cron() {
  if (dpidam_purge_cron_has_to_run()) {
    print '<strong>'.t('Nodes purge :').'</strong><br>';

    $old_nodes_age = dpi_variable_get('dpidam_purge_age', 15);

    $nids = dpidam_purge_get_nids();
    dpidam_purge_node_delete($nids);

    // Clear the page and block caches.
    cache_clear_all();
  }
}

/**
 * Check if the cron process has to run : if it is set to run all day or if we are at night
 *
 * @return boolean Whether the cron has to run or not
 */
function dpidam_purge_cron_has_to_run() {
  if (!dpi_variable_get('dpidam_historic_archive_done', FALSE)) {
    return FALSE;
  }

  switch (dpi_variable_get('dpidam_purge_run_period', 'night')) {
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

/**
 * Get an array of nids to purge
 */
function dpidam_purge_get_nids() {
  $age_limit = dpi_variable_get('dpidam_purge_age', 15);
  $changed_limit = time() - ($age_limit * 24 * 60 * 60);
  $node_limit = dpi_variable_get('dpidam_purge_limit', 20);

  $nids = array();
  $db_nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.changed', $changed_limit, '<')
    ->condition('n.type', 'package')
    ->orderBy('n.nid', 'ASC')
    ->range(0, $node_limit)
    ->execute();
  foreach ($db_nids as $db_nid) {
    $nids[] = $db_nid->nid;
  }

  return $nids;
}

/**
 * Delete nodes, the same way that node_delete but without checking permissions and without clearing the caches
 *
 * @param {int} $nids
 *   NIDs of the nodes to delete
 */
function dpidam_purge_node_delete($nids) {
  foreach ($nids as $nid) {
    // Clear the cache before the load, so if multiple nodes are deleted, the
    // memory will not fill up with nodes (possibly) already removed.
    $node = node_load($nid, NULL, TRUE);

    // Call the node-specific callback (if any):
    node_invoke($node, 'delete');
    module_invoke_all('node_delete', $node);
    module_invoke_all('entity_delete', $node, 'node');
    field_attach_delete('node', $node);

    // Remove this node from the search index if needed.
    // This code is implemented in node module rather than in search module,
    // because node module is implementing search module's API, not the other
    // way around.
    if (module_exists('search')) {
      search_reindex($nid, 'node');
    }

    print t('Purged node @nid', array('@nid' => $nid)).'<br>';
  }

  // Delete after calling hooks so that they can query node tables as needed.
  db_delete('node')
    ->condition('nid', $nids, 'IN')
    ->execute();
  db_delete('node_revision')
    ->condition('nid', $nids, 'IN')
    ->execute();
  db_delete('history')
    ->condition('nid', $nids, 'IN')
    ->execute();
  db_delete('node_access')
    ->condition('nid', $nids, 'IN')
    ->execute();

  watchdog('content', '@type: deleted %title.', array('@type' => $node->type, '%title' => $node->title));
}
