<?php


function _dpidestination_api_get_all_destinations_terms_from_db($reset_cache = FALSE){
  
  $vid = dpidestinations_api_get_destinations_vocabulary_vid();
  
  $query = db_select('taxonomy_term_data', 't');
  $result = $query
  //->addTag('translatable')
  ->fields('t')
  ->condition('t.vid', $vid)
  ->orderBy('t.weight')
  ->orderBy('t.name')
  ->execute();
  
  foreach ($result as $term) {
    $terms[$term->tid] = $term;
  }
  return $terms;
}