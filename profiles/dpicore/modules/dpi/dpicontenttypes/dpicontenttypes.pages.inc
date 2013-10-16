<?php 
/**
 * 
 * Display the dpiproperties of a content
 * @param String $type
 * @param Object $entity
 */
function dpicontenttypes_entity_properties($type, $entity){
  $content = '';
  $dpiproperties = dpiproperties_load_by_type_and_id($type, entity_id($type, $entity), TRUE);
  if ($dpiproperties != NULL){
    $dpiproperties_view = dpiproperties_view($dpiproperties);
    $content = render($dpiproperties_view);
  }
  return $content;
}