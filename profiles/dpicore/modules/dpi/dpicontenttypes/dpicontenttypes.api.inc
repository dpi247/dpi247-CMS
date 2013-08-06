<?php
/**
 * Return an array with the content types of type dpi (like article-package, photo, etc)
 */
function dpicontenttypes_api_getdpicontenttypes(){
  return array(
    'article-package',
    'gallery-package',
    'poll-package',
    'link',
    'link-list',
    'photo',
  );

}
/**
 * 
 * Return a node by its external reference. If no node exists for this external reference return NULL
 */
function dpicontenttypes_api_getnodebyexternalreference($external_reference){
  $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
  return $dpiproperties;
}