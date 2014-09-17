<?php
/**
 * @author lba
 */

/**
 * This function return an array with information for a secion and for a targetblocks
 *
 * @param integer $sid
 * @param string $targetblock
 * @return array
 */
function dpiservices_wally_section_retrieve($sid, $targetblock_name){
  module_load_include('inc', 'dpidestinations', 'plugins/ctools/content_types/targetblock/targetblock');
  module_load_include('inc', 'dpidestinations', 'dpidestinations.controller.class');
  module_load_include('inc', 'dpiservices', 'dpiservices.private.wally-packages');
  
  global $language;
  $json_return = array();
  $sectionterm = sectionterm_load($sid);

  $json_return["title"] = $sectionterm->name;
  $json_return["link"] = "http://".$_SERVER["SERVER_NAME"]."/section/".$sid;
  $json_return["id"] = $sid;
  $json_return["description"] = $sectionterm->vocabulary_machine_name;
  $json_return["language"] = $language->language;
  $json_return["copyright"] = "Copyright";
  $json_return["ttl"] = "@todo";
  $json_return["version"] = dpiservices_wally_section_get_version();

  if(!isset($targetblock_name)){
    $json_return["items"] = dpiservices_wally_section_get_all_packages($sid);
  }else{
    $json_return["items"] = dpiservices_wally_section_get_one_packages($sid, $targetblock_name);
  }

  return array("main" => $json_return);
}

/**
 * This function return all information about packages into this section id
 *
 * @param integer $sid
 * @return multitype:multitype:array()
 */
function dpiservices_wally_section_get_all_packages($sid){ 
  $section_info = dpidestinations_api_get_all_targetblocks_for_destination($sid);
  $result = array();
  foreach ($section_info as $section){
    $targetBlockController = new TargetBlockController('targetblock');

    $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta($section->did, $section->target, $section->split_delta);
    $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);

    $target = $targetblock_wrapper->target->value();
    $target_wrapper = entity_metadata_wrapper('target', $target);

    $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview($sid,  $section->target, $targetblock);

    foreach($targetview->result as $node_info){
      $result[] = dpiservices_wally_package_retrieve($node_info->nid);
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
function dpiservices_wally_section_get_one_packages($sectionid, $targetblock_name){
  $section_info = dpidestinations_api_get_all_targetblocks_from_target_for_destination($targetblock_name, $sectionid);
  $section = current($section_info);
  $result = array();
  $targetBlockController = new TargetBlockController('targetblock');

  $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta($section->did, $section->target, $section->split_delta);
  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);

  $target = $targetblock_wrapper->target->value();
  $target_wrapper = entity_metadata_wrapper('target', $target);

  $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview($sectionid,  $section->target, $targetblock);

  foreach($targetview->result as $node_info){
    $result[] = dpiservices_wally_package_retrieve($node_info->nid);
  }

  return $result;
}

/**
 * This function return the json version
 * 
 * @return string
 */
function dpiservices_wally_section_get_version(){
  return "1.0";
}
