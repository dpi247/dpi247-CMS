<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title'              => t('Viewers\' statistics'),
  'description'        => t('Display graphs of a node or a destination viewers\' statistics.'),
  'defaults'           => dpistat_viewers_stats_get_defaults(),
  'single'             => TRUE,
  'all contexts'       => TRUE,
  'group'              => t('DPI Core'),
  'category'           => t('Dashboard'),
);

/**
 * Callback function to supply a graph of published node,article,gallery,poll.
*/
function dpistat_viewers_stats_content_type_content_types() {
  return array('viewers_stats' => array(
    'single' => TRUE,
    'title' => t('Viewers\' statistics'),
    'category' => t('Dashboard'),
    'description' => t('Display graphs of a node or a destination viewers\' statistics.'),
    'defaults' => dpistat_viewers_stats_get_defaults(),
    'all contexts' => TRUE,
    'group' => t('DPI Core'),
  ));
}

/**
 * Implementation of content_type_form()
 */
function dpistat_viewers_stats_content_type_edit_form($form, $form_state) {
  $conf = $form_state['conf'];

  $form['display_authenticated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display authenticated vs anonymous'),
    '#default_value' => $conf['display_authenticated'],
  );

  $form['display_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display authenticated users with special roles vs no special roles'),
    '#default_value' => $conf['display_roles'],
  );

  return $form;
}

/**
 * Implementation of content_type_form_submit()
 */
function dpistat_viewers_stats_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys(dpistat_viewers_stats_get_defaults()) as $key) {
    $form_state['conf'][key] = $form_state['values'][key];
  }
}

/**
 * Implementation of content_type_render()
 */
function dpistat_viewers_stats_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->title = 'Viewers\' statistics';
  $block->content = '';

  if ($conf['display_authenticated']) {
    $data = array();
    if (arg(0) == 'node') {
      $authenticated = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'nid', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 1)));
      $anonymous = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'nid', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 0)));
      if (!empty($authenticated) && !empty($anonymous)) {
        $data = array(
          array(t('Authenticated'), (int)$authenticated[0]['weekcount']),
          array(t('Anonymous'), (int)$anonymous[0]['weekcount']),
        );
      }
    } else {
      $authenticated = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'param01', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 1)));
      $anonymous = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'param01', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 0)));
      if (!empty($authenticated) && !empty($anonymous)) {
        $data = array(
          array(t('Authenticated'), (int)$authenticated[0]['weekcount']),
          array(t('Anonymous'), (int)$anonymous[0]['weekcount']),
        );
      }
    }
    if (!empty($data)) {
      $theme_args = array(
        'data' => $data,
        'title' => t('Authenticated vs Anonymous'),
        'header_name' => 'authenticated vs anonymous',
        'header_unit' => 'views count',
      );
      $block->content .= theme('dpicommons_pie_chart', $theme_args);
    }
  }

  if ($conf['display_roles']) {
    $data = array();
    if (arg(0) == 'node') {
      $subscribed_user = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'nid', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 1), array('column' => 'param03', 'value' => 1, 'op' => '>')));
      $not_subscribed_user = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'nid', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 1), array('column' => 'param03', 'value' => 1)));
      if (!empty($subscribed_user) && !empty($not_subscribed_user)) {
        $data = array(
          array(t('User type'), t('Users count')),
          array(t('Subscribed user'), (int)$subscribed_user[0]['weekcount']),
          array(t('Not subscribed user'), (int)$not_subscribed_user[0]['weekcount']),
        );
      }
    } else {
      $subscribed_user = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'param01', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 1), array('column' => 'param03', 'value' => 1, 'op' => '>')));
      $not_subscribed_user = dpistat_api_node_get_count_by_params('weekcount', array(array('column' => 'param01', 'value' => $panel_args[0]), array('column' => 'param02', 'value' => 1), array('column' => 'param03', 'value' => 1)));
      if (!empty($subscribed_user) && !empty($not_subscribed_user)) {
        $data = array(
          array(t('User type'), t('Users count')),
          array(t('Subscribed user'), (int)$subscribed_user[0]['weekcount']),
          array(t('Not subscribed user'), (int)$not_subscribed_user[0]['weekcount']),
        );
      }
    }
    if (!empty($data)) {
      $theme_args = array(
        'data' => $data,
        'title' => t('Users with special(s) role(s) vs Users without any special role'),
        'hAxis' => '',
        'vAxis' => '',
      );
      $block->content .= theme('dpicommons_bar_chart', $theme_args);
    }
  }

  return $block;
}

function dpistat_viewers_stats_get_defaults() {
  return array(
    'display_authenticated' => 1,
    'display_roles' => 1,
  );
}
