<?php
/**
 * @author lba
 */

/**
 * This funtion return an array with information of all section
 *
 * @return array
 *
 */
function dpiservices_dpi_section_index() {
  $all_section = array ();
  foreach ( dpidestinations_api_get_all_sections_terms_hierachical_struct ( dpidestinations_api_get_sections_vocabulary_vid () ) as $section ) {
    $all_section [] = dpiservices_dpi_section_get_section_info ( $section );
  }
  return $all_section;
}

/**
 * This function return all information about a section
 *
 * @param section $section          
 * @return array
 */
function dpiservices_dpi_section_get_section_info($section) {
  return array (
    'tid' => $section->tid,
    'terme_name' => $section->name,
    'url' => drupal_get_path_alias ( 'section/' . $section->tid ),
    'uri' => dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/section/' . $section->tid,
    'targets' => array (
      dpiservices_dpi_section_get_targets ( $section->tid ) 
    ) 
  );
}

/**
 * This function return targetblock information for a section
 *
 * @param integer $tid
 * @return array
 */
function dpiservices_dpi_section_get_targets($tid) {
  $result = array ();
  foreach ( dpidestinations_api_get_all_targetblocks_for_destination ( $tid ) as $targetblock ) {
    $result [] = array (
      'target' => $targetblock->target,
      'template_rule' => $targetblock->template_rule,
      'node_count_for_targetblock' => $targetblock->node_count_for_targetblock 
    );
  }
  return $result;
}

/**
 * This function return an array with information for a secion and for a targetblocks
 *
 * @param integer $sid          
 * @param string $targetblock          
 * @return array
 */
function dpiservices_dpi_section_retrieve($sid, $product_name ,$targetblock_name) {
  module_load_include ( 'inc', 'dpiservices', 'dpiservices.private.dpi-packages' );
  module_load_include ( 'inc', 'dpidestinations', 'plugins/ctools/content_types/targetblock/targetblock' );
  module_load_include('inc', 'dpidestinations', 'dpidestinations.controller.class');
  
  global $language;
  $json_return = array ();
  $sectionterm = sectionterm_load ( $sid );
  
  $json_return ["title"] = $sectionterm->name;
  $json_return ["link"] = "http://" . $_SERVER ["SERVER_NAME"] . "/section/" . $sid;
  $json_return ["id"] = $sid;
  $json_return ["description"] = $sectionterm->vocabulary_machine_name;
  $json_return ["language"] = $language->language;
  $json_return ["copyright"] = "@todo";
  $json_return ["ttl"] = "@todo";
  $json_return ["version"] = dpiservices_dpi_section_get_version ();
  
  if (! isset ( $targetblock_name )) {
    $json_return ["packages"] = dpiservices_dpi_section_get_all_packages ( $sid , $product_name );
  } else {
    $json_return ["packages"] = dpiservices_dpi_section_get_one_packages ( $sid, $targetblock_name );
  }
  
  return $json_return;
}

/**
 * This function return all information about packages into this section id
 *
 * @param integer $sid          
 * @return multitype:multitype:array()
 */
function dpiservices_dpi_section_get_all_packages($sid, $product_name) {
  $section_info = dpidestinations_api_get_all_targetblocks_for_destination ( $sid );
  $result = array ();
  foreach ( $section_info as $section ) {
     $targetBlockController = new TargetBlockController('targetblock');
  
    $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta($section->did, $section->target, $section->split_delta);
    $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
  
    $target = $targetblock_wrapper->target->value();
    $target_wrapper = entity_metadata_wrapper('target', $target);
  
    $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview($sid,  $section->target, $targetblock);
    foreach ( $targetview->result as $node_info ) {
      $result [] = dpiservices_dpi_packages_retrieve ( $node_info->nid, $product_name );
    }
  }
  return $result;
}

/**
 * This function return all information about node into one targetblock for one section
 *
 * @param integer $sectionid          
 * @param string $targetblock_name          
 * @return multitype:multitype:array()
 */
function dpiservices_dpi_section_get_one_packages($sectionid, $targetblock_name) {
  $section_info = dpidestinations_api_get_all_targetblocks_from_target_for_destination ( $targetblock_name, $sectionid );
  $section = current ( $section_info );
  $result = array ();
  $targetBlockController = new TargetBlockController('targetblock');
  
  $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta($section->did, $section->target, $section->split_delta);
  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
  
  $target = $targetblock_wrapper->target->value();
  $target_wrapper = entity_metadata_wrapper('target', $target);
  
  $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview($sectionid,  $section->target, $targetblock);
  
  foreach ( $targetview->result as $node_info ) {
    $result [] = dpiservices_dpi_packages_retrieve ( $node_info->nid );
  }
  
  return $result;
}

/**
 * This function return the json version
 * 
 * @return string
 */
function dpiservices_dpi_section_get_version() {
  return "2.0";
}
