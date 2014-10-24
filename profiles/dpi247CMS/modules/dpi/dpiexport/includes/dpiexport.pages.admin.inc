<?php

/**
 * Configure export form
 */
function dpiexport_page_admin_general_settings_form($form, $form_state) {
  $form['dpiexport_admin_dest'] = array(
    '#type' => 'textfield',
    '#title' => t('Export destination directory path'),
    '#description' => t('This is where zipped datas will be written.<br>
        This must be a writable directory, relatively to the "public" stream.'),
    '#default_value' => dpi_variable_get('dpiexport_admin_dest', ''),
    '#required' => TRUE,
  );
  return dpi_api_system_settings_form($form);
}

/**
 * Validate configure export form
 */
function dpiexport_page_admin_general_settings_form_validate($form, $form_state) {
  $values = $form_state['values'];

  $path = 'public://'.$values['dpiexport_admin_dest'];
  if (!file_prepare_directory($path)) {
    form_set_error('dpiexport_admin_dest', t('Error directory path : the path provided is not an existing writable directory'));
  }
}

/**
 * Graphical interface to export packages
 */
function dpiexport_page_admin_export_form($form, $form_state) {
  if (dpi_variable_get('dpiexport_admin_dest', FALSE)) {
    $form['dpiexport_export_nids'] = array(
      '#type'        => 'textfield',
      '#title'       => t('Packages\' nids to export'),
      '#description' => t('Comma separated nids of the packages to be exported.<br>
          The keyword "all" is accepted.<br>
          No space allowed.'),
      '#size'        => 80,
      '#maxlength'   => 512,
      '#required'    => TRUE,
    );

    $form['dpiexport_export_submit'] = array(
      '#type'  => 'submit',
      '#title' => t('Export'),
      '#value' => t('Export'),
    );

    return $form;
  } else {
    drupal_set_message(t('You have to <a href="/'.DPI_ADMIN_PATH.'/dpiexport/settings">configure this module</a> before using it!'), 'warning');
  }
}

/**
 * Validate function for the export packages form
 */
function dpiexport_page_admin_export_form_validate(&$form, &$form_state) {
  $nids_list = $form_state['values']['dpiexport_export_nids'];
  if (strtolower($nids_list) != 'all') {
    // Control numeric values
    $nids = explode(',', $nids_list);
    $controled_nids = array();
    foreach ($nids as $nid) {
      if (is_numeric($nid)) {
        $controled_nids[] = $nid;
      } else {
        form_set_error('dpiexport_export_nids', t('The ID @id isn\'t a numeric value.', array('@id' => $nid)));
      }
    }

    // Control the type of the nodes, it has to be packages
    if (!empty($controled_nids)) {
      $db_types = db_select('node', 'n')
        ->fields('n', array('nid', 'type'))
        ->condition('n.nid', $controled_nids, 'IN')
        ->execute();
      foreach ($db_types as $type) {
        if ($type->type != 'package') {
          form_set_error('dpiexport_export_nids', t('The node @nid isn\'t a package.', array('@nid' => $type->nid)));
        }
      }
    }
  }
}

/**
 * Submit function for the export packages form.
 */
function dpiexport_page_admin_export_form_submit(&$form, &$form_state) {
  $nids_list = $form_state['values']['dpiexport_export_nids'];

  // Get a node IDs list
  if (strtolower($nids_list) == 'all') {
    $nids = array();
    $db_results = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('n.type', 'package')
      ->execute();
    foreach ($db_results as $result) {
      $nids[] = $result->nid;
    }
  } else {
    $nids = explode(',', $nids_list);
  }

  // Generate ZIPs
  $results = dpiexport_api_export_packages($nids);

  // Manage output
  $output = '';
  foreach ($results as $nid => $result) {
    if ($result['success']) {
      $message_type = 'status';
      // Direct download link. It could be nice to start a download automatically, but it's a start..
      $result['message'] .= ' - '.l(t('Download ZIP'), $result['zip']).'<br>';
    } else {
      $message_type = 'error';
    }
    drupal_set_message($result['message'], $message_type);
  }
}
