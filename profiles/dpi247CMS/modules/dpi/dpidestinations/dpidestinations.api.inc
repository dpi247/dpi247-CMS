<?php

/**
 * Get all destinations hierarchic tree containing sections terms, targets, layouts and ranks
 */
function dpidestinations_api_get_destinations_hierarchies($reset = FALSE) {
  static $destination_hierarchy = array();

  if (empty($destination_hierarchy) || $reset) {
    $section_vid = dpidestinations_api_get_sections_vocabulary_vid();
    $section_terms = dpidestinations_api_get_all_sections_terms_hierachical_struct($section_vid);
    foreach ($section_terms as $section_term) {
      $section_term->targetblocks = array();
      $targets = dpidestinations_api_get_all_targetblocks_for_destination($section_term->tid);
      foreach ($targets as $target) {
        $layouts = dpidestinations_api_get_all_layouts_from_target_for_destination($target->targetblockid, $target->target);
        $target->layouts = $layouts;
        $section_term->targetblocks[$target->targetblockid] = $target;
      }
      $destination_hierarchy[$section_term->tid] = $section_term;
    }
  }

  return $destination_hierarchy;
}

/**
 * Load a target from his machine name
 *
 * We use EntityFieldQuery to do the querry and return the first (and assumed result) if we found it
 * Null if no match
 *
 * @param String $target_machine_name
 * the machine name of the target
 * @return targetEntity|NULL
 *
 */
function dpidestinations_api_target_machinename_load($target_machine_name){
  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'target')
    ->propertyCondition('machine_name',$target_machine_name)
    ->execute();

  $result = $query->execute();
  if (!empty($result['target'])) {
    $entities = entity_load('target', array_keys($result['target']));
    if(count($entities)){
      return current($entities);
    }
    else{
      return NULL;
    }
  }
}

/**
 * Get the sections vocabulary ID
 */
function dpidestinations_api_get_sections_vocabulary_vid() {
  return _dpidestinations_variable_get('dpidestinations_section_vid', 2);
}

/**
 * Return all targetblock instance related to a specific target in a destination
 *
 * We take in account all targetblockinstance related to splitted targetblocks
 *
 * @param String $target_machinename
 * @param Integer $sectiontid
 *
 * @return an array of targetblockEntity
 */
function dpidestinations_api_get_all_targetblocks_for_destination($sectiontid) {
  $targetblocks = array();
  $panels_display_id = _dpidestinations_get_panels_displayid_for_sectionterm($sectiontid);

  $db_targetblocks = db_select('dpidestinations_targetblock', 'dt')
    ->fields('dt')
    ->condition('dt.did', $panels_display_id)
    ->execute();

  foreach ($db_targetblocks as $db_targetblock) {
    $targetblocks[$db_targetblock->target] = $db_targetblock;
  }

  return $targetblocks;
}

/**
 * Return all targetblock instance related to a section
 *
 * @param String $target_machinename
 * @param Integer $sectiontid
 *
 * @return an array of targetblockEntity
 */
function dpidestinations_api_get_all_targetblocks_from_target_for_destination($target_machinename, $sectiontid) {
  $targetblocks = array();
  $panels_display_id = _dpidestinations_get_panels_displayid_for_sectionterm($sectiontid);

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'targetblock')
    ->propertyCondition('target', $target_machinename)
    ->propertyCondition('did', $panels_display_id)
    ->execute();

  $result = $query->execute();
  if (!empty($result['targetblock'])) {
    $targetblocks = entity_load('targetblock', array_keys($result['targetblock']));
  }

  return $targetblocks;
}

/**
 * Get all layouts related to a given target
 */
function dpidestinations_api_get_all_layouts_from_target_for_destination($target_id, $target_machinename) {
  return __dpidestinations_api_get_all_layouts_available_for_sectiontid_and_target($target_id, $target_machinename);
}

/**
 * Return the quadruplet of information that define the destination that is used in a specific node for a specifique section
 */
function dpidestinations_api_get_destination_from_node_for_tid($node, $tid) {
  $destinations_val = field_get_items('node', $node, 'field_destinations');
  if(isset($destinations_val) && $destinations_val != FALSE) {
    foreach($destinations_val as $index => $destination) {
      if($destination['tid'] == $tid){
        return $destination;
      }
    }
  }
  return FALSE;
}

/**
 * Return the quadruplet of information that define the main destination that is used in a specific node
 */
function dpidestinations_api_get_main_destination_from_node($node) {
  $destinations_val = field_get_items('node', $node, 'field_destinations');
  if (isset($destinations_val) && $destinations_val != FALSE) {
    return array_shift($destinations_val);
  }
  return FALSE;
}

function dpidestinations_api_get_all_sections_terms_flat_struct($reset_cache = FALSE, $load_entities = FALSE) {
  return _dpidestinations_api_get_all_sections_terms_from_db($reset_cache);
}
function dpidestinations_api_get_all_sections_terms_flat_struct_as_options($reset_cache=FALSE, $load_entities = FALSE){
  $options=array();
  $terms= _dpidestinations_api_get_all_sections_terms_from_db($reset_cache);
  foreach($terms as$term){
    $options[$term->tid]=$term->name;
  }
  return $options;
}

function dpidestinations_api_get_all_targets_as_options(){
  
  $options=array();
  
  $targets=dpidestinations_api_get_all_targets();
  foreach($targets as $target){
    $options[$target->machine_name]=$target->label;
  }
  return $options;
  
}


function dpidestinations_api_get_all_sections_terms_hierachical_struct($vid, $parent = 0, $reset_cache = FALSE, $load_entities = FALSE) {
  $vid = dpidestinations_api_get_sections_vocabulary_vid();
  return taxonomy_get_tree($vid, $parent, NULL, $load_entities);
}

function dpidestinations_api_get_all_sections_term_available_as_select_options($is_update) {
  if($is_update) {
    $options = array('' => t('Remove this destination'));
  }
  else {
    $options = array('' => t('Add a new destination'));
  }


  $vid = dpidestinations_api_get_sections_vocabulary_vid();
  $vocabularies=taxonomy_get_vocabularies();
  $vocabulary=$vocabularies[$vid];$vocabulary;
  if ($vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary->name)) {
    if ($terms = taxonomy_get_tree($vid, 0)) {
      
      foreach ($terms as $term) {
        $options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
      }
    }
  }

  return $options;
}

function dpidestinations_api_get_all_targets() {
  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'target')
    ->execute();

  $targets = entity_load('target', array_keys($result['target']));
  return $targets;
}

function dpidestinations_api_get_all_targetblocks() {
  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'targetblock')
    ->execute();

  $targets = array();
  if (!empty($result) && isset($result['targetblock'])) {
    $targets = entity_load('targetblock', array_keys($result['targetblock']));
  }

  return $targets;
}

function dpidestinations_api_get_all_ranks_available_for_sectiontid_and_target($term_id, $target_name, $is_update = FALSE) {
  $controller = new TargetBlockController('targetblock');
  $targetblock = $controller->load_targetblock_by_termid_and_target($term_id, $target_name);

  $rank_options = array();
  if($targetblock) {
    $wrapper = entity_metadata_wrapper('targetblock', $targetblock);

    $rank_max_value = $wrapper->rank_max_value->value();
    $rank_options = array();
    for($i = 1; $i <= $rank_max_value; $i++) {
      $rank_options[$i] = $i;
    }
  }
  return $rank_options;
}

function dpidestinations_api_get_panelsdisplayid_for_sectionterm($term_id) {
  $display = _dpidestinations_get_panels_display_for_sectionterm($term_id);
  return $display->did;
}

function dpidestinations_api_get_panelsdisplay_for_sectionterm($term_id) {
  $display = _dpidestinations_get_panels_display_for_sectionterm($term_id);
  return $display;
}

/*
 * Return all node type that have a field_destination
 */
function dpidestinations_api_get_node_type_with_destination_field(){
  return array(
    'package'=>'package'
  );
}





function   dpidestination_api_get_category($destination){
  $did = dpidestinations_api_get_panelsdisplay_for_sectionterm($destination['tid']);
  $category = $did->did.'_'.$destination['tid'].'_'.$destination['target'];
  return $category;

}