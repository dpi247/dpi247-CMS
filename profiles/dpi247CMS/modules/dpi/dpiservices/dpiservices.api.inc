<?php
/**
 * @author lba
 */

/**
 * This function return one element from a field
 * 
 * @param String $type type of element (node,scald_atom,...)
 * @param object $node Element where you search info
 * @param string $field_name Name of your field
 * @param string $search_value name of the value in your field
 * @return (true)?string:false 
 */
function dpiservices_packages_get_field_element_by_search_value($type = null, $node = null, $field_name = null, $search_value = null) {
  if($info = field_get_items ( $type, $node, $field_name )){
    $element = current($info);
  }
  return (isset($element[$search_value])) ? $element[$search_value] : FALSE;
}

/**
 * This function return all element from a field
 * 
 * @param String $type type of element (node,scald_atom,...)
 * @param object $node Element where you search info
 * @param string $field_name Name of your field
 * @param string $search_value name of the value in your field
 * @return (true)?array:null
 */
function dpiservices_packages_get_one_element_from_item_from_node($type = null, $node = null, $field_name = null, $search_value = null, $get_item = null) {
  $authors = array ();
  if($var = field_get_items ( $type, $node, $field_name )){
    foreach ($var  as $items ) {
      $authors [] = taxonomy_term_load ( $items [$search_value] )->$get_item;
    } 
  }
  return $authors;
}

/**
 * This function return all value necessery for a crop
 *
 * @param integer $sid
 * @param integer $nid
 * @return array
 */
function dpiservices_packages_generate_crop_informations($sid=null, $nid=null) {
  $gen_value = array ();
  $crop_value = array ();
  $crop_definition_value = array ();

  if($info =  _dpicontenttypes_get_saved_cropings_for_entity_and_atom ( 'node', $nid, $sid )){
    foreach ($info as $key => $value ) {
      $crop_value ['_' . $value [0] . '_' . $value [1] . '_' . $value [2] . '_' . $value [3]] = $key;
      $crop_definition_value [$key] = array (
        'start_width' => $value [0],
        'start_height' => $value [1],
        'width' => $value [2],
        'height' => $value [3]
      );
    }
  }
  $gen_value [] = $crop_value;
  $gen_value [] = $crop_definition_value;

  return $gen_value;
}

/**
 * This function return all value for crop definition with url
 * 
 * @param string $sid
 * @param string $nid
 * @return array
 */
function dpiservices_packages_generate_crop_links($sid=null, $nid=null, $atom=null){
  module_load_include("inc", 'dpicommons', "dpicommons.api");
  $crop_link_definition = array ();
  if($info =  _dpicontenttypes_get_saved_cropings_for_entity_and_atom ( 'node', $nid, $sid )){
    foreach ($info as $key => $value ) {
      $crop_link_definition [$key] = array (
        'url' => dpiflexiscald_api_get_path_for_atom($atom, $key),
      );
    }
  }
  return $crop_link_definition;
}

/**
 * This function return info from table product
 * 
 * @param String $label
 * @return product|FALSE
 */
function dpiservices_product_get_one_product_by_label($label = null){
  if(!isset($label) || $label== "")
      return FALSE;
  
  try {
    return db_select('dpiservices_product', 'dp')
    ->condition('label', $label, '=')
    ->fields('dp', array())
    ->execute()->fetchAssoc();
  } catch (Exception $e) {
    return FALSE;
  }
}

/**
 * This function get row from dpisrevices_product.
 * Parameter limite number of row.
 * No parameter return all informations from table.
 *
 * @param integer $nb
 * @return array
 */
function dpiservices_product_get_all_product_info($nb = 0) {
  if(!is_numeric($nb))
    return FALSE;
  $return = array ();
  if($nb <= 0){
    $query = db_select('dpiservices_product', 'dp')->fields('dp', array ())->execute();
  }else{
    $query = db_select('dpiservices_product', 'dp')->fields('dp', array ())->range(0, $nb)->execute();
  }
  while( $row = $query->fetchAssoc () ) {
    $return [] = $row;
  }
  return $return;
}
