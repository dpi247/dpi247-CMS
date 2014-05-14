<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for a DAM archive ID
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('DAM Archive: ID'),
  'keyword' => 'damarchive',
  'description' => t('Creates a node context from a DAM Archive ID argument.'),
  'context' => 'dpidam_argument_damarchive_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the DAM Archive ID of a package for this argument'),
  ),
);

/**
 * Discover if this argument gives us the node we crave.
*/
function dpidam_argument_damarchive_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('node');
  }

  // We can accept either a node object or a pure nid.
  if (is_object($arg)) {
    return ctools_context_create('node', $arg);
  }

  static $dam_nodes = array();

  if (isset($dam_nodes[$arg])) {
    $node = $dam_nodes[$arg];

    if (!$node) {
      return FALSE;
    }

    return ctools_context_create('node', $node);
  }

  // @todo : encore nécessaire?
  //menu_set_active_trail(array()); // Avoid multiple parallel calls

  $timeout = _dpimport_variable_get('dpicommons_http_request_timeout', 3);
  $request = dpi_variable_get('dpidam_archive_geturl', '');
  // Remove the "x" at the end of the request and replace it with the ID
  $request = substr($request, 0, -1).$arg;
  $request_args = '?options=origin%3Dweb';
  $headers = array();
  $data = NULL;

  // GET
  $result = drupal_http_request($this->getSourceUrl(), array('timeout' => $timeout));
  $file_name = FALSE;
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  if (dpicommons_is_http_request_successfull($result)) {
    if ($importer_name = dpi_variable_get('dpidam_archive_importer', '')) {
      $importer = dpimport_api_get_importer_settings($importer_name);
      $dispatcher_settings = unserialize($importer->dispatcher_settings);
      $file_name = 'dam_archive_'.rand().'.zip';
      // Save the file in the importer folder
      file_put_contents($dispatcher_settings['temp_folder'].'/'.$file_name, $result->data);
    }
  }

  $node = NULL;
  if ($file_name) {
    if ($importer_name = dpi_variable_get('dpidam_archive_importer', '')) {
      module_load_include('inc', 'dpidam', 'includes/dpidam.archive');
      // Import the zip
      if ($node = dpidam_archive_import_node($importer_name, $file_name)) {
        $node->preview = TRUE;
        $dam_nodes[$arg] = $node;
      }
    }
  }

  if (!$node) {
    return FALSE;
  }

  return ctools_context_create('node', $node);
}
