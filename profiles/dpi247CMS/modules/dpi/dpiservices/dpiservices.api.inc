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
function dpiservices_packages_get_field_element_by_search_value($type, $node, $field_name, $search_value) {
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
function dpiservices_packages_get_one_element_from_item_from_node($type, $node, $field_name, $search_value, $get_item) {
  $authors = array ();
  foreach ( field_get_items ( $type, $node, $field_name ) as $items ) {
    $authors [] = taxonomy_term_load ( $items [$search_value] )->$get_item;
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
function dpiservices_packages_generate_crop_informations($sid, $nid) {
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
 * This function return info from table product
 * 
 * @param String $label
 * @return product|FALSE
 */
function dpiservices_product_get_one_product_by_label($label){
  try {
    return db_select('dpiservices_product', 'dp')
    ->condition('label', $label, '=')
    ->fields('dp', array())
    ->execute()->fetchAssoc();
  } catch (Exception $e) {
    return FALSE;
  }
}
