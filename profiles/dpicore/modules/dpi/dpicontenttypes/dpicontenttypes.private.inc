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
 
    //If no dpiproperties are defined, set default values
    $values = array(
      'entity' => $entity,
      'id' => $eid,
      'external_reference' => dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand(),
      'external_reference_2' => dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand(),
      'source' => 'dpi247',
      'product_id' => dpi_variable_get('dpicommons_product', ''),       
    );
    $dpiproperties = new DPIPropertiesEntity($values, 'dpiproperties');
    $dpiproperties->type = 'default';
    dpiproperties_save($dpiproperties);
  } else {
    //if dpiproperties are defined, save it
    $dpiproperties = $object->dpiproperties;
    $dpiproperties->entity = $entity;
    $dpiproperties->id = $eid;
    $dpiproperties->type = 'default';
    dpiproperties_save($dpiproperties);
  }
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