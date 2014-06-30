<?php

/**
 * Settings form.
 */
function dpistat_page_admin_general_settings_form($form, $form_state) {
  module_load_include('inc', 'dpistat', 'includes/dpistat.updater_helpers');

  $form['#tree'] = TRUE;

  $form['dpistat_stats_cron_memory_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron process memory limit (in % of the PHP memory limit)'),
    '#description' => t('A number between 50 and 100'),
    '#default_value' => dpi_variable_get('dpistat_stats_cron_memory_limit', 75),
    '#size' => 80,
    '#maxlength' => 512,
  );

  $form['dpistat_stats_content_refresh_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Stats content refresh time'),
    '#description' => t('Statistics will be refreshed at this frequency'),
    '#default_value' => dpi_variable_get('dpistat_stats_content_refresh_time', 15),
    '#size' => 80,
    '#maxlength' => 512,
  );

  $form['dpistat_display_follower'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display statistics in destination blocks'),
    '#default_value' => dpi_variable_get('dpistat_display_follower', 0),
  );

  $params_functions = array('none' => t('Not used')) + dpistat_available_params_function();

  $form['dpistat_params'] = array(
    '#type' => 'fieldset',
    '#title' => 'Statistics parameters',
  );

  for ($i = 1; $i <= 10; $i++) {
    $params_callback = variable_get('dpistat_param_'.$i, array());
    $current_callback = isset($params_callback['current_callback']) ? $params_callback['current_callback'] : 'none';

    $form['dpistat_params']['dpistat_param_'.$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Parameter').' '.$i,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['dpistat_params']['dpistat_param_'.$i]['dpistat_param_name_'.$i] = array(
      '#type' => 'textfield',
      '#title' => t('Parameter name'),
      '#default_value' => isset($params_callback[$current_callback]['name']) ? $params_callback[$current_callback]['name'] : '',
      '#size' => 80,
      '#maxlength' => 512,
    );

    $form['dpistat_params']['dpistat_param_'.$i]['dpistat_callback_'.$i] = array(
      '#type' => 'select',
      '#title' => t('Value'),
      '#default_value' => $current_callback,
      '#options' => $params_functions,
    );

    foreach (dpistat_available_params_function() as $callback => $name) {
      $form_callback = $callback.'_form';
      $form_elems = $form_callback();

      for ($j = 0; $j < count($form_elems); $j++) {
        $form_elems[$j]['#default_value'] = isset($params_callback[$callback]['param_'.$j]) ? $params_callback[$callback]['param_'.$j] : NULL;
        $form_elems[$j]['#states'] = array(
          'visible' => array(
            ':input[name="dpistat_params[dpistat_param_'.$i.'][dpistat_callback_'.$i.']"]' => array('value' => $callback),
          ),
        );
        $form['dpistat_params']['dpistat_param_'.$i][$callback.'_param_'.$j] = $form_elems[$j];
      }
    }
  }

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
  );

  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults')
  );

  return $form;
}

/**
 * Validate function for the settings form.
 */
function dpistat_page_admin_general_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!is_numeric($values['dpistat_stats_cron_memory_limit']) || $values['dpistat_stats_cron_memory_limit'] < 50 || $values['dpistat_stats_cron_memory_limit'] > 100) {
    form_set_error('dpistat_stats_cron_memory_limit', t('The memory limit must be a numeric value between 50 and 100.'));
  }

  if (!is_numeric($values['dpistat_stats_content_refresh_time']) || $values['dpistat_stats_content_refresh_time'] < 0) {
    form_set_error('dpistat_stats_content_refresh_time', t('The refresh time must be a numeric value greater than 0.'));
  }

  if ($values['op'] == $values['buttons']['submit']) {
    for ($i = 1; $i <= 10; $i++) {
      $param = $values['dpistat_params']['dpistat_param_'.$i];
      if ($param['dpistat_callback_'.$i] == 'dpistat_get_node_field' && $param['dpistat_get_node_field_param_0'] == '') {
        form_set_error('dpistat_params][dpistat_param_'.$i.'][dpistat_get_node_field_param_0', t('Please enter a field name.'));
      }
    }
  }
}

/**
 * Submit function for the settings form.
 */
function dpistat_page_admin_general_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  dpi_variable_set('dpistat_stats_cron_memory_limit', $values['dpistat_stats_cron_memory_limit']);
  dpi_variable_set('dpistat_stats_content_refresh_time', $values['dpistat_stats_content_refresh_time']);
  dpi_variable_set('dpistat_display_follower', $values['dpistat_display_follower']);

  if ($values['op'] == $values['buttons']['submit']) {
    for ($i = 1; $i <= 10; $i++) {
      $temp_param = array();
      $param = $values['dpistat_params']['dpistat_param_'.$i];
      if ($param['dpistat_callback_'.$i] != 'none') {
        $temp_param['current_callback'] = $param['dpistat_callback_'.$i];
        $temp_param[$temp_param['current_callback']] = array();
        $temp_param[$temp_param['current_callback']]['name'] = $param['dpistat_param_name_'.$i];
        $j=0;
        while (isset($param[$temp_param['current_callback'].'_param_'.$j])) {
          $temp_param[$temp_param['current_callback']]['param_'.$j] = $param[$temp_param['current_callback'].'_param_'.$j];
          $j++;
        }
        variable_set('dpistat_param_'.$i, $temp_param);
      }
    }
    drupal_set_message(t('The configuration options have been saved.'));
  } elseif ($values['op'] == $values['buttons']['reset']) {
    dpi_variable_del_namespace('dpistat_');
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}

/**
 * Generate random statistics
 */
function dpistat_page_admin_generate_stats_form($form, $form_state) {
  $form['#tree'] = TRUE;

  $form['node_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount of node to handle in the chosen destination'),
    '#default_value' => 0,
  );

  $params = array();
  for ($i = 1; $i <= 10; $i++) {
    if (variable_get('dpistat_param_'.$i, FALSE)) {
      $form['param_'.$i] = array(
        '#type' => 'textfield',
        '#title' => t('Param @param_id values', array('@param_id' => $i)),
        '#description' => t('List of possible values for this parameter, coma separated.<br>
            Example : for a parameter returning the user role the values could be : anonymous user,authenticated user'),
        '#default_value' => '',
      );
    }
  }

  // Add the destination field
  $items = array();
  if (isset($form_state['input']['destination'])) {
    $items = $form_state['input']['destination'];
  }
  if (isset($items['add_more'])) {
    unset($items['add_more']);
  }
  $langcode = 'und';
  $field = array(
    'field_name' => 'field_destinations',
    'type' => 'destination',
    'cardinality' => 1,
    'columns' => array('tid' => '', 'target' => '', 'layout' => '', 'rank' => '', 'virtual_rank' => ''),
  );
  $instance = array(
    'label' => '',
    'description' => '',
    'widget' => array('type' => 'destination_default_widget', 'module' => 'dpidestinations'),
    'entity_type' => 'node',
    'bundle' => 'package',
    'required' => TRUE,
  );
  $form_copy = $form;
  $form_copy['#parents'] = array();
  $form_copy['#entity'] = new stdClass();
  $form_state_copy = $form_state;
  $form_state_copy['programmed'] = NULL;
  $form_state_copy['destinationsfield'] = array();
  $form['destination'] = field_multiple_value_form($field, $instance, $langcode, $items, $form_copy, $form_state_copy);
  $form['destination'][0]['#title'] = t('Generate statistics for this destination');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate statistics'),
  );

  return $form;
}

/**
 * Validation form for the generate random statistic form
 */
function dpistat_page_admin_generate_stats_form_validate($form, $form_state) {
  $values = $form_state['values'];

  if (!is_numeric($values['node_amount']) || $values['node_amount'] < 1) {
    form_set_error('node_amount', t('The nodes amount must be a numeric value greater than 1.'));
  }

  $no_destination = TRUE;
  foreach (dpicommons_element_children_numeric($values['destination']) as $dest_id) {
    if (isset($values['destination'][$dest_id]['tid']) && !empty($values['destination'][$dest_id]['tid']) && is_numeric($values['destination'][$dest_id]['tid'])
        && isset($values['destination'][$dest_id]['target']) && !empty($values['destination'][$dest_id]['target'])) {
      $no_destination = FALSE;
      break;
    }
  }
  if ($no_destination) {
    form_set_error('destination][0', t('You have to set a destination.'));
  }
}

/**
 * Submit form for the generate random statistic form
 */
function dpistat_page_admin_generate_stats_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $node_amount = $values['node_amount'];
  $options = array();
  for ($i = 1; $i <= 10; $i++) {
    if (variable_get('dpistat_param_'.$i, FALSE)) {
      $param_id = ($i == 10) ? 'param'.$i : 'param0'.$i;
      $param_values = $values['param_'.$i];
      $expl_param_values = explode(',', $param_values);
      foreach ($expl_param_values as $expl_param_value) {
        $expl_param_value = trim($expl_param_value);
        if ($expl_param_value != '') {
          if (!isset($options[$param_id])) {
            $options[$param_id] = array();
          }
          $options[$param_id][] = $expl_param_value;
        }
      }
    }
  }

  $tids = array();
  $targets = array();
  foreach (dpicommons_element_children_numeric($values['destination']) as $dest_id) {
    if (isset($values['destination'][$dest_id]['tid']) && !empty($values['destination'][$dest_id]['tid']) && is_numeric($values['destination'][$dest_id]['tid'])
        && isset($values['destination'][$dest_id]['target']) && !empty($values['destination'][$dest_id]['target'])) {
      $tid = $values['destination'][$dest_id]['tid'];
      $tids[] = $tid;
      $targets[$tid] = $values['destination'][$dest_id]['target'];
    }
  }

  dpistat_generate_stats($tids, $targets, $node_amount, $options);

  drupal_set_message(t('Generated statistics!'));
}

/**
 * Generate random statistics
 * 
 * @param {array|int} $tids
 *   
 * @param unknown $targets
 * @param number $node_amout
 * @param unknown $options
 */
function dpistat_generate_stats($tids, $targets, $node_amount = 10, $options = array()) {
  if (is_numeric($tids)) {
    $tids = array($tids);
  }
  if (is_string($targets)) {
    $target = $targets;
    $targets = array();
    foreach ($tids as $tid) {
      $targets[$tid] = $target;
    }
  }

  $default_options = array(
    'daycount' => array('min' => 300, 'max' => 1000),
    'weekcount' => array('min' => 3000, 'max' => 5000),
    'monthcount' => array('min' => 5000, 'max' => 7000),
    'yearcount' => array('min' => 7000, 'max' => 9000),
    'totalcount' => array('min' => 9000, 'max' => 10000),
    'hourly_count' => array('min' => 200, 'max' => 500),
  );
  $timestamp = time();
  $current_hour = $timestamp - (date('i', $timestamp) * 60 + date('s', $timestamp));

  foreach ($tids as $tid) {
    $target = $targets[$tid];
    $db_query = db_select('field_data_field_destinations', 'f');
    $db_query->innerJoin('node', 'n', 'f.entity_id = n.nid');
    $db_query->addExpression('DISTINCT(n.nid)', 'nid');
    $db_query->addExpression('n.title', 'title');
    $db_res = $db_query
      ->condition('f.field_destinations_tid', $tid)
      ->condition('f.field_destinations_target', $target)
      ->condition('f.entity_type', 'node')
      ->condition('n.status', 1)
      ->orderBy('f.field_destinations_virtual_rank', 'DESC')
      ->range(0, $node_amount)
      ->execute();
    foreach ($db_res as $res) {
      db_delete('dpistat_node_counter')
        ->condition('nid', $res->nid)
        ->execute();
      db_delete('dpistat_node_hourly_counter')
        ->condition('nid', $res->nid)
        ->execute();
      /*db_delete('dpistat_term_counter')
        ->condition('nid', $res->nid)
        ->execute();*/

      for ($i = 1; $i < 10; $i++) {
        $res->{'param0'.$i} = '';
      }
      $res->param10 = '';

      $options_copy = $options;
      dpistat_generate_stats_recursively_add_param($res, $options_copy, $default_options, $current_hour);
    }
  }

  // Clear the live statistics cache
  cache_clear_all('dpistat_stats_content_', 'cache', TRUE);
}

function dpistat_generate_stats_recursively_add_param(&$res, &$options, $default_options, $current_hour) {
  $param_found = FALSE;
  $option_keys = array_keys($options);
  while(!$param_found && !empty($options)) {
    $option_key = array_shift($option_keys);
    $option = array_shift($options);
    if (strstr($option_key, 'param')) {
      // It is a param option
      $param_found = TRUE;
      foreach ($option as $param) {
        $res->{$option_key} = $param;
        if (empty($options)) {
          // No options left, generate stats
          dpistat_generate_stats_write_records($res, $default_options, $current_hour);
        } else {
          // There are still options, go to next recursive loop
          $options_copy = $options;
          dpistat_generate_stats_recursively_add_param($res, $options_copy, $default_options, $current_hour);
        }
      }
      return;
    }
  }

  // If no param found for the loop and no options left, generate stats
  if (empty($options)) {
    dpistat_generate_stats_write_records($res, $default_options, $current_hour);
  }
}

function dpistat_generate_stats_write_records($res, $default_options, $current_hour) {
  $res->timestamp = $current_hour;
  // Node counter
  foreach ($default_options as $option_key => $option) {
    $min = isset($option['min']) ? $option['min'] : $default_options[$option_key]['min'];
    $max = isset($option['max']) ? $option['max'] : $default_options[$option_key]['max'];
    $res->{$option_key} = rand($min, $max) / rand(1, 3);
  }
  drupal_write_record('dpistat_node_counter', $res);

  // Node hourly counter
  for ($i = 0; $i <= 48; $i++) {
    $res->timestamp = $current_hour - $i * 3600;
    $min = isset($options['hourly_count']['min']) ? $options['hourly_count']['min'] : $default_options['hourly_count']['min'];
    $max = isset($options['hourly_count']['max']) ? $options['hourly_count']['max'] : $default_options['hourly_count']['max'];
    $res->count = rand($min, $max) / rand(1, 3);
    drupal_write_record('dpistat_node_hourly_counter', $res);
  }
}
