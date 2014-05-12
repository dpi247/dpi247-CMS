<?php

/**
 * Manage sources
 */
function dpidam_page_admin_sources() {
  $header = array(t('Source'), t('Archive'), t('Archive from dpimport'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows = array();
  $noyes = array(0 => t('No'), 1 => t('Yes'));

  $sources = dpidam_api_get_sources();
  foreach ($sources as $source) {
    $operations = _dpidam_page_admin_sources_get_operations($source);
    $row = array(
      check_plain($source->name),
      $noyes[$source->archive],
      $noyes[$source->archive_from_dpimport],
      array('data' => $operations),
    );

    $rows[] = $row;
  }

  $table_vars = array('header' => $header, 'rows' => $rows);
  $output = !empty($rows) ? theme('table', $table_vars) : '';
  $output .= drupal_render(drupal_get_form('dpidam_page_admin_sources_add_new_source_form'));

  return $output;
}

/**
 * Add a new source form
 */
function dpidam_page_admin_sources_add_new_source_form($form, $form_state) {
  $form['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a new source'),
    '#collapsible' => FALSE,
  );

  $form['add']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('The identifier of the source used in import files (this name must be unique)'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['add']['archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Archive'),
    '#description' => t('Archive the content into the DAM'),
  );

  $form['add']['archive_from_dpimport'] = array(
    '#type' => 'checkbox',
    '#title' => t('Archive from dpimport'),
    '#description' => t('Archive the content even when it\'s coming from dpimport. (Example : Packages from NewsGate must not be archived when comming from dpimport to avoid loop. But package from FlowToNode must be archived from dpimport)'),
    '#states' => array(
      'visible' => array( // action to take.
        ':input[name="archive"]' // element to evaluate condition on
        => array('checked' => TRUE), // condition
      ),
    ),
  );

  $form['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation of the dpidam_page_admin_sources_add_new_source_form
 */
function dpidam_page_admin_sources_add_new_source_form_validate($form, $form_state) {
  if (!isset($form_state['values']['sid']) && dpidam_api_check_source_exists($form_state['values']['name'])) {
    form_set_error('name', t('A source with the name %name already exists', array('%name' => $form_state['values']['name'])));
  }
}

/**
 * Submit of the dpidam_page_admin_sources_add_new_source_form
 */
function dpidam_page_admin_sources_add_new_source_form_submit($form, &$form_state) {
  $source = new stdClass();
  $source->name = $form_state['values']['name'];
  $source->archive = $form_state['values']['archive'];
  $source->archive_from_dpimport = $form_state['values']['archive_from_dpimport'];
  $update = array();

  // Update
  if (isset($form_state['values']['sid'])) {
    $update = array('sid');
    $source->sid = $form_state['values']['sid'];
  }

  if (drupal_write_record('dpidam_source', $source, $update)) {
    drupal_set_message(t('The source "@name" has been saved.', array('@name' => $form_state['values']['name'])));
  } else {
    drupal_set_message(t('The source could not be saved.'));
  }
  $form_state['redirect'] = 'admin/dpi247/dpidam/sources';
}

/**
 * Source edition form
 */
function dpidam_page_admin_sources_edit_source_form($form, $form_state, $sid) {
  if ($source = dpidam_api_get_source($sid)) {
    $form['#validate'][] = 'dpidam_page_admin_sources_add_new_source_form_validate';
    $form['#submit'][] = 'dpidam_page_admin_sources_add_new_source_form_submit';

    $form['sid'] = array(
      '#type' => 'value',
      '#value' => $sid,
    );

    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('The identifier of the source used in import files (this name must be unique)'),
      '#default_value' => $source->name,
      '#size' => 80,
      '#maxlength' => 512,
      '#required' => TRUE,
    );

    $form['archive'] = array(
      '#type' => 'checkbox',
      '#title' => t('Archive'),
      '#description' => t('Archive the content into the DAM'),
      '#default_value' => $source->archive,
      '#required' => TRUE,
    );

    $form['archive_from_dpimport'] = array(
      '#type' => 'checkbox',
      '#title' => t('Archive from dpimport'),
      '#description' => t('Archive the content even when it\'s coming from dpimport. (Example : Packages from NewsGate must not be archived when comming from dpimport to avoid loop. But package from FlowToNode must be archived from dpimport)'),
      '#default_value' => $source->archive_from_dpimport,
      '#states' => array(
        'visible' => array( // action to take.
          ':input[name="archive"]' // element to evaluate condition on
          => array('checked' => TRUE), // condition
        ),
      ),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  } else {
    drupal_set_message(t('Unable to find source @sid', array('@sid' => $sid)), 'warning');
    drupal_goto('admin/dpi247/dpidam/sources');
  }

  return $form;
}

/**
 * Form to delete a source
 */
function dpidam_page_admin_source_delete_source_form($form, $form_state, $sid) {
  if ($source = dpidam_api_get_source($sid)) {
    $form['sid'] = array(
      '#type' => 'value',
      '#value' => $sid,
    );

    $form = confirm_form($form, t('Are you sure you want to delete this source?'), 'admin/dpi247/dpidam/sources');
  } else {
    drupal_set_message(t('Unable to find source @sid', array('@sid' => $sid)), 'warning');
    drupal_goto('admin/dpi247/dpidam/sources');
  }

  return $form;
}

/**
 * Submit of the source delete form
 */
function dpidam_page_admin_source_delete_source_form_submit($form, $form_state) {
  $result = db_delete('dpidam_source')
    ->condition('sid', $form_state['values']['sid'])
    ->execute();
  if ($result != FALSE) {
    $message = t('The source as been deleted');
  } else {
    $message = t('The source as not been deleted');
  }
  drupal_set_message(t($message));
  drupal_goto('admin/dpi247/dpidam/sources');
}

/**
 * Get links to available operations
 */
function _dpidam_page_admin_sources_get_operations($source) {
  $return = l(t('Edit'), 'admin/dpi247/dpidam/sources/edit/'.$source->sid).' - ';
  $return .= l(t('Delete'), 'admin/dpi247/dpidam/sources/delete/'.$source->sid);
  return $return;
}
