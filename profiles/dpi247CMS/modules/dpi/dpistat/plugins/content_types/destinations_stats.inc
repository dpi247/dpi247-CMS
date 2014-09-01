<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title'              => t('Destination\'s statistics'),
  'description'        => t('Display graphs of a destination\'s statistics.'),
  'defaults'           => dpistat_destinations_stats_get_defaults(),
  'single'             => TRUE,
  'all contexts'       => TRUE,
  'group'              => t('DPI Core'),
  'category'           => t('Dashboard'),
);

/**
 * Callback function to supply a graph of published node,article,gallery,poll.
 */
function dpistat_destinations_stats_content_type_content_types() {
  return array ('destinations_stats' => array(
    'single' => TRUE,
    'title' => t('Destination\'s statistics'),
    'category' => t('Dashboard'),
    'description' => t('Display graphs of a destination\'s statistics.'),
    'defaults' => dpistat_destinations_stats_get_defaults(),
    'all contexts' => TRUE,
    'group' => t('DPI Core'),
  ));
}

/**
 * Implementation of content_type_form()
 */
function dpistat_destinations_stats_content_type_edit_form($form, $form_state) {
  $conf = $form_state['conf'];

  $form['display_ratio'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display views in the destination vs all other destinations'),
    '#default_value' => $conf['display_ratio'],
  );

  $form['display_views'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display views in the destination vs all destinations over time'),
    '#default_value' => $conf['display_views'],
  );

  return $form;
}

/**
 * Implementation of content_type_form_submit()
 */
function dpistat_destinations_stats_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys(dpistat_destinations_stats_get_defaults()) as $key) {
    $form_state['conf'][key] = $form_state['values'][key];
  }
}

/**
 * Implementation of content_type_render()
 */
function dpistat_destinations_stats_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->content = '';

  $destination_term = 0;
  foreach ($context as $arg) {
    if (is_object($arg) && $arg->type[0] == 'entity:taxonomy_term' && $arg->data->vid == dpidestinations_api_get_sections_vocabulary_vid()) {
      $destination_term = $arg->data->tid;
    }
    if (is_object($arg) && $arg->type[0] == 'entity:node') {
      $node = $arg->data;
      $node_wrapper = entity_metadata_wrapper('node', $node);
      if (isset($node_wrapper->field_destinations) && $field_dest = $node_wrapper->field_destinations->value()) {
        $destination_term = $field_dest[0]['tid'];
      }
    }
  }

  if (!$destination_term) {
    return $block;
  }

  $block->title = t('Destination\'s statistics');

  if ($conf['display_ratio']) {
    $dest_count = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'param01', 'value' => $destination_term)));
    $all_dest_count = dpistat_api_node_get_count_by_params('weekcount', array());
    if (isset($dest_count[0]['weekcount']) && $dest_count[0]['weekcount'] && isset($all_dest_count[0]['weekcount']) && $all_dest_count[0]['weekcount']) {
      $dest_ratio = (int)(100 * $dest_count[0]['weekcount']/$all_dest_count[0]['weekcount']);
      $data = array(
        array(t('Views'), t('This destination'), t('All other destinations')),
        array(t('Percentage'), $dest_ratio, 100 - $dest_ratio),
      );
      $chart_args = array(
        'data' => $data,
        'title' => t('Views in this destination vs all other destinations'),
      );
      $block->content .= theme('dpicommons_gauge_chart', $chart_args);
    }
  }

  if ($conf['display_views']) {
    $data = array();

    $time = time();
    $hours = date('G', $time);
    $limit_tstamp_inf = $time - ($hours * 60 * 60);
    $limit_tstamp_sup = $time + (60 * 60);
    $nbr_range = 48;
    $axeXTitle = t('Hours of the day');

    $data[0][0] = 'Hours';
    for($i = 1; $i <= $nbr_range; $i++) {
      $data[$i][] = $i - 48;
      $data[$i][] = 0;
      $data[$i][] = 0;
    }

    $data[0][] = t('This destination');
    $data[0][] = t('All destinations');

    $dest_stats = dpistat_api_node_get_hourly_counts_with_params(array(array('column' => 'param01', 'value' => $destination_term)));
    foreach ($dest_stats as $timestamp => $count) {
      $hour = date('G', $time - $timestamp) - 1;
      $data[48 - $hour][1] = (int)$count;
    }
    $all_stats = dpistat_api_node_get_hourly_counts_with_params(array());
    foreach ($all_stats as $timestamp => $count) {
      $hour = date('G', $time - $timestamp) - 1;
      $data[48 - $hour][2] = (int)$count;
    }

    $chart_args = array(
      'data' => $data,
      'title' => t('This destination\'s views vs all destinations\' views'),
      'pointSize' => 3,
      'AxeXTitle' => $axeXTitle,
    );
    $block->content .= theme('dpicommons_line_chart', $chart_args);
  }

  return $block;
}

function dpistat_destinations_stats_get_defaults() {
  return array(
    'display_ratio' => 1,
    'display_views' => 1,
  );
}
