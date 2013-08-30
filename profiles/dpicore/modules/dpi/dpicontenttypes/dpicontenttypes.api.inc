<?php
/**
 * Return an array with the content types of type dpi (like article-package, photo, etc)
 */
function dpicontenttypes_api_getdpicontenttypes(){
  return array(
    'article_package',
    'gallery_package',
    'poll_package',
    'link',
    'link-list',
    'photo',
  );

}
/**
 * 
 * Return a node by its external reference. If no node exists for this external reference return NULL
 * @param String $external_reference
 * @return Object $node
 */
function dpicontenttypes_api_getnodebyexternalreference($external_reference){
  $node = NULL;
  $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
  if ($dpiproperties != NULL && $dpiproperties->id != NULL){
    $node = node_load($dpiproperties->id);
  }
  return $node;
}

/**
 * 
 * Return an atom by its external reference. If no atom exists for this external reference return NULL
 * @param String $external_reference
 * @return Object $atom
 */
function dpicontenttypes_api_getatombyexternalreference($external_reference){
  $atom = NULL;
  $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
  if ($dpiproperties != NULL && $dpiproperties->id != NULL){
    $atom = scald_atom_load($dpiproperties->id);
  }
  return $atom;
}
