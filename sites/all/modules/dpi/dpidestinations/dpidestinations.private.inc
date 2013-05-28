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

function _dpidestinations_targetmachine_name_exists($value){
  $result = db_select('dpidestinations_target', 't')
  ->fields('t')
  ->condition('machine_name', $value,'=')
  ->execute()
  ->fetchAssoc();
  if(is_array($result)){
    return TRUE;
  }
  return FALSE;  
}



function _dpidestination_get_all_targetviews(){
  //@todo: Peut être interessant de mettre un cache statique ici pour performance (DDU)
  $targetviews=array();
  $views=views_get_all_views();
  foreach($views as $view_name=>$view){
    foreach($view->display as $display_name=>$display){
      if($display->display_plugin=='targetview'){
        $targetviews[$view_name]['label']=$view->human_name;
        $targetviews[$view_name]['displays'][]=array('label'=>$display->display_title,'display_name'=>$display_name);
      }
    }
  }

  return $targetviews;
}


function _target_metadata_getter($object, array $options, $property_name){
  if($property_name=="available_targetlayouts"){
    return unserialize($object->available_targetlayouts);
  }
  if($property_name=="available_templateoverrides"){
    $result=array();
    foreach(explode("\n",$object->available_templateoverrides) as $line){
      list($key,$value)=explode("|",$line);
      $result[$key]=$value;
    }
    return $object->available_templateoverrides=$result;
  }
  if($property_name=="targetview"){
    $view_name= $object->view_name;
    $view_display= $object->view_display;
    if( $view = views_get_view($view_name)){
      $view->set_display($view_display);
      return $view;
    }else{
      return NULL;
    }
  }
}

function _target_metadata_setter($object, $property_name, $wrapper){
  if($property_name=="available_targetlayouts"){
    return $object->available_targetlayouts=serialize($wrapper);
  }
  if($property_name=="available_templateoverrides"){
    $db_value="";
    foreach($wrapper as $key => $value){
      $db_value.=$key."|".$value."\n";
    }
    return $object->available_templateoverrides=$db_value;
  }
  

}