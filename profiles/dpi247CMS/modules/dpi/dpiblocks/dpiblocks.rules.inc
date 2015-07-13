<?php 

/**
 * 
 * @return multitype:array
 */
function dpiblocks_rules_action_info() {
  return array (
    'regenerate_accroche_cache' => array (
      'label' => t ( 'Refresh accroche cache' ),
      'parameter' => array (
        'node' => array (
          'type' => 'node',
          'label' => t ( 'Content' ) 
        ) 
      ),
      'group' => t ( 'Dpi Actions Rules' ),
      'callbacks' => array (
        'execute' => 'dpiblocks_accroche_regenerate_cache' 
      ) 
    ),    
  );
}

function dpiblocks_accroche_regenerate_cache($node){
  try {
    if(isset($node->nid)){
      $dest = field_get_items('node', $node, 'field_destinations');
      if(isset($dest) && $results = dpiblocks_execute_query_to_get_element_infos($node->nid, $dest)){
        foreach($results as $machine_name){
          dpicache_api_regenerate_all_cache_entry_callback('dpiblocks_accroche_feed', 'accroche_'.$machine_name->machine_name);
        }        
      }
    }
  } catch (Exception $e) {
  }
}

function dpiblocks_execute_query_to_get_element_infos($nid, $destinations){
  try {
    $conditions = dpiblocks_generate_conditions_query($nid, $destinations);
    return db_select('dpiblocks_accroche', 'da')
    ->fields('da', array('machine_name'))
    ->condition($conditions)
    ->execute()
    ->fetchAll();
  } catch (Exception $e) {}  
}

function dpiblocks_generate_conditions_query($nid, $destinations){
  $or = db_or()->condition('da.hash_cache_data', '%;'.$nid.';%', 'LIKE');
  foreach($destinations as $dest){
    if(isset($dest["tid"]) && isset($dest['target'])){
      $or->condition('da.hash_cache_data', '%|'.$dest["tid"].'_'.$dest["target"].'|%', 'LIKE');
    }
  }
  return $or;
}