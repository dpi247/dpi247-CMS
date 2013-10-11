<?php

function dpidestinations_api_get_destination_from_node_for_tid($node,$tid){
  $destinations_val = field_get_items('node', $node, 'field_destinations');
  foreach($destinations_val as $index=>$destination){
    if($destination['tid']==$tid){
      return $destination;
    }
  }
  return FALSE;
  
}

function dpidestinations_api_get_destinations_vocabulary_vid(){
  return _dpidestinations_variable_get('dpidestinations_destination_vid',2);
}

function dpidestinations_api_get_all_destinations_terms_flat_struct($reset_cache=FALSE, $load_entities = FALSE){
 
  return _dpidestinations_api_get_all_destinations_terms_from_db($reset_cache);
}

function dpidestinations_api_get_all_destinations_terms_hierachical_struct($vid, $parent = 0, $reset_cache = FALSE, $load_entities = FALSE){

  $vid=dpidestinations_api_get_destinations_vocabulary_vid();
  return taxonomy_get_tree($vid, $parent, NULL, $load_entities);
  
}
function dpidestinations_api_get_all_destinations_term_available_as_select_options($is_update){
  $terms=dpidestinations_api_get_all_destinations_terms_flat_struct();
  if($is_update)
    $options=array(''=>'Remove this destination');
  else
    $options=array(''=>'Add a new destination');
    
  foreach ($terms as $term){
    $options[(int)  $term->tid]=$term->name;
    
  }
  return $options;
}


function dpidestinations_api_get_all_targets_available_for_destination_tid($tid){
  if($tid==2){
    return array('target1'=>"Default","target2"=>"BHV");
  }
  return array("target1"=>"Default","target3"=>"Football");

}

function dpidestinations_api_get_all_layouts_available_for_destination_tid_and_target($tid,$target){
  return array(1=>"layout1",2=>"layout2");

}

function dpidestinations_api_get_all_ranks_available_for_destination_tid_and_target($term_id,$target_name,$is_update=FALSE){
  

  $controller=new TargetBlockController('targetblock');
  $targetblock=$controller->load_targetblock_by_termid_and_target($term_id,$target_name);
  $rank_options=array();
  if($targetblock){
    $wrapper = entity_metadata_wrapper('targetblock', $targetblock);
    
    $rank_max_value= $wrapper->rank_max_value->value();
    $rank_options=array();
    for($i=1;$i<=$rank_max_value;$i++){
      $rank_options[$i]=$i;
    }
  }
  return $rank_options;
  
}


function dpidestination_api_get_panelsdisplayid_for_destinationterm($term_id){
  $display= _dpidestinations_get_panels_display_for_destinationterm($term_id);
  return $display->did;
}
function dpidestination_api_get_panelsdisplay_for_destinationterm($term_id){
  $display= _dpidestinations_get_panels_display_for_destinationterm($term_id);
  return $display;
}



function hook_dpidestinations_virtualrank_dispersion($term_id,$target_name,$nids){
  //Clear some cache for example
}

