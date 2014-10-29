<?php

/**
 * Implementation of hook_rules_event_info().
 *
 * We talk about a package if the content type has a destination field whatever his content type
 *
 * @ingroup rules
 */
function dpidestinations_rules_event_info() {
  return array (
    'dpidestinations_rules_event_package_create' => array (
      'label' => t ( 'After a node type with field destination is created' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations',
      'variables' => rules_events_node_variables ( t ( 'created content' ) ),
      'arguments' => array (
        'acting_user' => array (
          'type' => 'user',
          'label' => t ( 'The user who create the package.' ) 
        ),
        'package' => array (
          'type' => 'package',
          'label' => t ( 'The purchased package.' ) 
        ) 
      ) 
    ),
    'dpidestinations_rules_event_package_edit' => array (
      'label' => t ( 'After a node type with field destination is edited' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations',
      'variables' => rules_events_node_variables ( t ( 'updated content' ), TRUE ) 
    ),
    'dpidestinations_rules_event_package_create_or_edit' => array (
      'label' => t ( 'Before a node type with field destination is created or edited' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations',
      'variables' => rules_events_node_variables ( t ( 'updated content' ), TRUE ) 
    )
    ,
    'dpidestinations_rules_event_package_deleted' => array (
      'label' => t ( 'After a node type with field destination is deleted' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations',
      'variables' => rules_events_node_variables ( t ( 'delete content' ), TRUE ) 
    )
    ,
    'dpidestinations_rules_event_targetblock_created_on_panel' => array (
      'label' => t ( 'After a targetblock is created on a panel' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations' 
    // @todo: define variables 'variables' => rules_events_node_variables(t('delete content'), TRUE),
        )

    ,
    'dpidestinations_rules_event_targetblock_edited_on_panel' => array (
      'label' => t ( 'After a targetblock is edited on a panel' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations' 
    // @todo: define variables 'variables' => rules_events_node_variables(t('delete content'), TRUE),
        )

    ,
    'dpidestinations_rules_event_targetblock_removed_on_panel' => array (
      'label' => t ( 'After a targetblock is removed from a panel' ),
      'module' => 'dpidestinations',
      'group' => 'DPI247 Destinations' 
    // @todo: define variables 'variables' => rules_events_node_variables(t('delete content'), TRUE),
        )

     
  )
  ;
}

/**
 * Implements hook_rules_condition_info() on behalf of the node module.
 */
function dpidestinations_rules_condition_info() {
  $defaults = array (
    'parameter' => array (
      'node' => array (
        'type' => 'node',
        'label' => t ( 'Content' ) 
      ) 
    ),
    'group' => t ( 'DPI247 Destinations' ) 
  // 'access callback' => 'rules_node_integration_access',
    );
  $items ['dpidestinations_rules_condition_node_has_main_section'] = $defaults + array (
    'label' => t ( 'Node has main section' ),
    'help' => t ( 'Evaluates to TRUE if the given content has his first destination\'s section set to a defined value.' ),
    'base' => 'dpidestinations_rules_condition_has_main_section' 
  );
  $items ['dpidestinations_rules_condition_node_has_main_section'] ['parameter'] ['section'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Section' ),
    'options list' => 'dpidestinations_api_get_all_sections_terms_flat_struct_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  
  $items ['dpidestinations_rules_condition_node_has_section'] = $defaults + array (
    'label' => t ( 'Node has a section' ),
    'help' => t ( 'Evaluates to TRUE if the given content has a destination\'s section set to a defined value.' ),
    'base' => 'dpidestinations_rules_condition_has_section' 
  );
  $items ['dpidestinations_rules_condition_node_has_section'] ['parameter'] ['section'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Section' ),
    'options list' => 'dpidestinations_api_get_all_sections_terms_flat_struct_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  
  $items ['dpidestinations_rules_condition_node_has_main_target'] = $defaults + array (
    'label' => t ( 'Node has main target' ),
    'help' => t ( 'Evaluates to TRUE if the given content has a main destination\'s target set to a defined value.' ),
    'base' => 'dpidestinations_rules_condition_has_main_target' 
  );
  $items ['dpidestinations_rules_condition_node_has_main_target'] ['parameter'] ['target'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Target' ),
    'options list' => 'dpidestinations_api_get_all_targets_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  
  $items ['dpidestinations_rules_condition_node_has_target'] = $defaults + array (
    'label' => t ( 'Node has a target' ),
    'help' => t ( 'Evaluates to TRUE if the given content has a destination\'s target set to a defined value.' ),
    'base' => 'dpidestinations_rules_condition_has_target' 
  );
  $items ['dpidestinations_rules_condition_node_has_target'] ['parameter'] ['target'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Target' ),
    'options list' => 'dpidestinations_api_get_all_targets_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  
  $items ['dpidestinations_rules_condition_node_has_main_section_and_main_target'] = $defaults + array (
    'label' => t ( 'Node has main section and main target' ),
    'help' => t ( 'Evaluates to TRUE if the given content has a main destination\'s section and a destination\'s target set to a defined value.' ),
    'base' => 'dpidestinations_rules_condition_has_main_section_and_main_target' 
  );
  $items ['dpidestinations_rules_condition_node_has_main_section_and_main_target'] ['parameter'] ['section'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Section' ),
    'options list' => 'dpidestinations_api_get_all_sections_terms_flat_struct_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  $items ['dpidestinations_rules_condition_node_has_main_section_and_main_target'] ['parameter'] ['target'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Target' ),
    'options list' => 'dpidestinations_api_get_all_targets_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  
  $items ['dpidestinations_rules_condition_node_has_section_and_target'] = $defaults + array (
    'label' => t ( 'Node has a section and a target' ),
    'help' => t ( 'Evaluates to TRUE if the given content has a main destination\'s section and a destination\'s target set to a defined value.' ),
    'base' => 'dpidestinations_rules_condition_has_section_and_target' 
  );
  $items ['dpidestinations_rules_condition_node_has_section_and_target'] ['parameter'] ['section'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Section' ),
    'options list' => 'dpidestinations_api_get_all_sections_terms_flat_struct_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  $items ['dpidestinations_rules_condition_node_has_section_and_target'] ['parameter'] ['target'] = array (
    'type' => 'list<text>',
    'label' => t ( 'Target' ),
    'options list' => 'dpidestinations_api_get_all_targets_as_options',
    'description' => t ( 'The content main section to check for.' ),
    'restriction' => 'input' 
  );
  
  return $items;
}
function dpidestinations_rules_condition_has_section($node, $sections) {
  $destinations = field_get_items ( 'node', $node, 'field_destinations', $langcode );
  $return = FALSE;
  foreach ( $destinations as $destination ) {
    if (in_array ( $destination ['tid'], $sections )) {
      $return = TRUE;
    }
  }
  
  return $return;
}
function dpidestinations_rules_condition_has_main_section($node, $sections) {
  $destinations = field_get_items ( 'node', $node, 'field_destinations', $langcode );
  $main_section = $destinations ['0'] ['tid'];
  
  return in_array ( $main_section, $sections );
}
function dpidestinations_rules_condition_has_main_target($node, $targets) {
  $destinations = field_get_items ( 'node', $node, 'field_destinations', $langcode );
  $main_section = $destinations ['0'] ['target'];
  return in_array ( $main_section, $sections );
}
function dpidestinations_rules_condition_has_target($node, $targets) {
  $destinations = field_get_items ( 'node', $node, 'field_destinations', $langcode );
  $return = FALSE;
  foreach ( $destinations as $destination ) {
    if (in_array ( $destination ['target'], $sections )) {
      $return = TRUE;
    }
  }
  return $return;
}
function dpidestinations_rules_condition_has_main_section_and_main_target($node, $sections, $targets) {
  return dpidestinations_rules_condition_has_main_section ( $node, $sections ) & dpidestinations_rules_condition_has_main_target ( $node, $targets );
}
function dpidestinations_rules_condition_has_section_and_target($node, $sections, $targets) {
  return dpidestinations_rules_condition_has_section ( $node, $sections ) & dpidestinations_rules_condition_has_target ( $node, $targets );
}

/**
 * Definition of hook_rules_action_info()
 *
 * @return array
 */
function dpidestinations_rules_action_info() {
  return array (
    'regenerate_node' => array (
      'label' => t ( 'Refresh content' ),
      'parameter' => array (
        'node' => array (
          'type' => 'node',
          'label' => t ( 'Content' ) 
        ) 
      ),
      'group' => t ( 'Dpi Actions Rules' ),
      'callbacks' => array (
        'execute' => 'dpidestinations_clear_cache_of_targetbloks_for_a_node' 
      ) 
    ),
    'refresh_node_cash' => array (
      'label' => t ( 'Refresh node content' ),
      'parameter' => array (
        'node' => array (
          'type' => 'node',
          'label' => t ( 'Content' ) 
        ) 
      ),
      'group' => t ( 'Dpi Actions Rules' ),
      'callbacks' => array (
        'execute' => 'dpidestinations_clear_cache_of_node_content' 
      ) 
    ) 
  );
}

/**
 * Function to invalidate cash for varnish and dpicache
 *
 * @param node $node          
 */
function dpidestinations_clear_cache_of_targetbloks_for_a_node($node) {
  /**
   * Invalidation cache for this node because the pre_render of
   * dpidestinations_plugin_row_node_targetview give the precedent node
   */
  $_GET ['q'] = $_GET ['destination'];
  drupal_static ( 'drupal_get_destination', '', TRUE );
  unset ( $_GET ['destination'] );
  
  entity_get_controller ( 'node' )->resetCache ( array (
    $node->nid 
  ) );
  foreach ( field_get_items ( 'node', $node, 'field_destinations' ) as $destination ) {
    if (isset ( $destination ['tid'] ) && is_numeric ( $destination ['tid'] ) && isset ( $destination ['target'] )) {
      $category = dpidestination_api_get_category ( $destination );
      // Clean dpicache for targetblocks
      dpicache_api_regenerate_all_cache_entry_callback ( 'dpidestinations_targetblock_render', $category );
      
      // clean varnish for argetblocks
      foreach ( dpicommons_api_get_all_path_alias_for_taxonomy ( $destination ['tid'] ) as $url_alias ) {
        dpicache_api_varnish_purge_pattern ( '/' . $url_alias );
      }
    } else {
      drupal_set_message ( 'Cache entry is not refresh for this content', 'error' );
    }
  }
}

/**
 * Invalide node section for varnish cache
 *
 * @param node $node          
 */
function dpidestinations_clear_cache_of_node_content($node) {
  // Clean varnish for node
  foreach ( dpicommons_api_get_all_path_alias_for_node ( $node ) as $url_alias ) {
    dpicache_api_varnish_purge_pattern ( '/' . $url_alias );
  }
}

