<?php
$plugin = array (
  'title' => 'Accroche custom content',
  'description' => 'Display a list of the newest nodes',
  'edit form' => 'dpiblocks_accroche_plugin_edit_form',
  'render callback' => 'dpiblocks_accroche_render',
  'admin info' => 'dpiblocks_accroche_admin_info',
  'category' => 'Custom Accroche',
  'group' => 'DPI Core',
  'all contexts' => TRUE 
);

function dpiblocks_reuse_accroche_content_type_content_types($plugins) {
  $types = array ();
  $layouts = _dpiblocks_reuse_accroche_get_all_teaser ();
  foreach ( $layouts as $key => $layout ) {
    $types [$key] = array (
      'title' => check_plain ( $layout [0] ),
      'subtype' => $key,
      'description' => check_plain ( $layout [0] ),
      'icon' => drupal_get_path ( "module", "dpidestinations" ) . '/images/1412086058_93156.ico',
      'category' => 'Custom Accroche',
      'group' => 'DPI Core',
      'all contexts' => TRUE
    );
  }
  return $types;
}

function _dpiblocks_reuse_accroche_get_all_teaser() {
  return variable_get ( 'dpiblocks_accroche_settings' );
}

/**
 * This function get all custom content from db
 *
 * @return $results
 * the results from db
 */
function dpiblocks_reuse_accroche_get_all(){
  // Get all custom content blocks
  $results = db_query('SELECT * FROM {dpiblocks_accroche}');
  return $results;
}