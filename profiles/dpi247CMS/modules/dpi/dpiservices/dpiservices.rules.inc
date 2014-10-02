<?php
/**
 * Definition of hook_rules_action_info()
 *
 * @return array
 */
function dpiservices_rules_action_info() {
  return array (
    'regenerate_json_package_cache' => array (
      'label' => t ( 'Refresh node json cache (package)' ),
      'parameter' => array (),
      'group' => t ( 'Dpi Actions Rules' ),
      'callbacks' => array (
        'execute' => 'dpiservices_clear_cache_of_json' 
      ) 
    ),
    'regenerate_json_section_cache' => array (
      'label' => t ( 'Refresh section json cache' ),
      'parameter' => array (),
      'group' => t ( 'Dpi Actions Rules' ),
      'callbacks' => array (
        'execute' => 'dpiservices_clear_cache_of_section_json'
      )
    ),
  );
}

/**
 * Function to invalidate cash for varnish and dpicache
 *
 * @param node $node          
 */
function dpiservices_clear_cache_of_json() {
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_dpijson_render');
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_dpisection_retrieve_render');
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_dpisection_index_render');
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_wallypackages_render');
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_wallypackages_render');  
  drupal_set_message("Cache is regenerated");
}

function dpiservices_clear_cache_of_section_json(){
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_wallysection_render');
  dpicache_api_regenerate_all_cache_entry_callback('dpiservices_dpijsonexporter_render');  
  drupal_set_message("Cache is regenerated");
}

