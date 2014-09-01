<?php

/**
 * Return all the types that can be handeld by one or more flow plugins
 *
 * @return
 *   An array of types
 */
function dpiblocks_api_get_available_flow_types() {
  ctools_include('plugins');

  $available_types = array();
  $plugins = ctools_get_plugins('dpiblocks', 'flow');

  foreach ($plugins as $plugin_name => $plugin) {
    $available_types[$plugin_name] = $plugin['type'];
  }

  return $available_types;
}

/**
 * Determine if a flow has a setting form and if true, return the $form object.
 *
 * Some content types simply have their content and no options. This function
 * lets a UI determine if it should display an edit link or not.
 */
function dpiblocks_api_get_settings_form_of_plugin($plugin, &$form, &$form_state) {
  ctools_include('plugins');
  if (empty($plugin['settings form']) ) {
    return FALSE;
  }

  if (!empty($plugin['settings form validate'])) {
    $form['#validate'][] = $plugin['settings form validate'];
  }

  if ($function = ctools_plugin_get_function($plugin, 'settings form')) {
    return $function($form, $form_state);
  }

  return FALSE;
}

/**
 * Get a specific flow plugin
 */
function dpiblocks_api_get_plugin_by_name($plugin_name) {
  ctools_include('plugins');
  $module = 'dpiblocks';
  $type = 'flow';

  $plugin = ctools_get_plugins($module, $type, $plugin_name);
  return $plugin;
}
