<?php

function dpidestinations_api_get_destinations_vocabulary_vid(){

  return 2;
}

function dpidestinations_api_get_all_destinations_terms_flat_struct($reset_cache=FALSE, $load_entities = FALSE){
 
  return _dpidestination_api_get_all_destinations_terms_from_db($reset_cache);
}

function dpidestinations_api_get_all_destinations_terms_hierachical_struct($vid, $parent = 0, $reset_cache = FALSE, $load_entities = FALSE){

  $vid=dpidestinations_api_get_destinations_vocabulary_vid();
  return taxonomy_get_tree($vid, $parent, NULL, $load_entities);
  
}
function dpidestinations_api_get_all_destinations_term_available_as_select_options(){
  $terms=dpidestinations_api_get_all_destinations_terms_flat_struct();
  $options=array();
  foreach ($terms as $term){
    $options[(int)  $term->tid]=$term->name;
    
  }
  return $options;
}

function dpidestinations_api_get_all_targets_available_for_destination_tid($tid){
  if($tid==2){
    return array(1=>"Default",2=>"BHV");
  }
  return array(1=>"Actu",2=>"Football");
  
}

function dpidestinations_api_get_all_layouts_available_for_destination_tid_and_target($tid,$target){
  return array(1=>"layout1",2=>"layout2");

}

function dpidestinations_api_get_all_ranks_available_for_destination_tid_and_target($tid,$target){
  return array(1,2);

}
