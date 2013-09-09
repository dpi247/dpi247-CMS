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

/**
* Search for taxonomy terms for a specific "path".
*
* @param $path
*   The taxonomy term path to match ( /term1/term2/term3 ).
* @param $vid
*   The taxonomy vid used for filtering on a single vocabulary.
*   If not set, will search into all vocabularies.
*
* @return
*   an array of taxonomy term object
*/
function dpicontenttypes_api_taxonomy_get_term_by_path($path, $vid = NULL) {
  $result = NULL;
  $vocabulary_name = NULL;
  
  if ($vid != NULL){
    $vocabulary = taxonomy_vocabulary_load($vid);
    $vocabulary_name = $vocabulary->machine_name;
  }
  
  $exploded_path = explode('/', trim($path, '/'));
  $term_name = $exploded_path[count($exploded_path) - 1];
  
  //Get all terms with that term_name
  $terms = taxonomy_get_term_by_name($term_name, $vocabulary_name);
  
  // Reverse building of path for founded terms. And catch the ones that match
  foreach ($terms as $term) {
    $term_path = '';
    $parents = taxonomy_get_parents_all($term->tid);
    foreach ($parents as $parent){
      $term_path = strtolower($parent->name)."/".$term_path;
    }
    if (trim($term_path, '/') == strtolower(trim($path, '/'))){
      $result[] = $term;
    }
  }
  return $result;
}
