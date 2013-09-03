<?php 
/**
 * 
 * Insert dpiproperties into the database
 * @param Object $object
 * @param String $entity
 * @param Interger $eid
 */
function _dpicontenttypes_entityinsertdpiproperties($object, $entity, $eid){
  //Create the dpiproperties entry corresponding to the node  
  if (!isset($object->dpiproperties)){
    $dpiproperties = new DPIPropertiesEntity(array(), 'dpiproperties');
  } else {
    $dpiproperties = $object->dpiproperties;
  }
  $dpiproperties->type = 'default';
  $dpiproperties->id = $eid;
  $dpiproperties->entity = $entity;
  $dpiproperties->external_reference = $dpiproperties->external_reference ? $dpiproperties->external_reference : dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand();
  $dpiproperties->external_reference_2 = $dpiproperties->external_reference_2 ? $dpiproperties->external_reference_2 : dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand();
  $dpiproperties->source = $dpiproperties->source ? $dpiproperties->source : 'dpi247';
  $dpiproperties->product_id = $dpiproperties->product_id ? $dpiproperties->product_id : dpi_variable_get('dpicommons_product', '');
  dpiproperties_save($dpiproperties);
}
/**
 * 
 * Delete dpiproperties from the database
 * @param Object $object
 * @param String $entity
 * @param Integer $eid
 */
function _dpicontenttypes_entitydeletedpiproperties($object, $entity, $eid){
  $condition = db_and()->condition('id', $eid)->condition('entity', $entity);
  db_delete('dpicontenttypes_properties')->condition($condition)->execute();
}