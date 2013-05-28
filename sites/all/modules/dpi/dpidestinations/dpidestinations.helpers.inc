<?php

function dpidestinations_helpers_convert_flat_destinations_terms_in_options($terms){
  $options=array();
  foreach($terms as $term){
    $options[$term->tid]=$term->name;
  }
  return $options;
}


function dpidestinations_helpers_dependant_target_viewname_and_display_options_for_view_name($include_blank=TRUE){
  $target_views=_dpidestination_get_all_targetviews();
  
  $options=array();
  foreach($target_views as $target_view_name=> $target_view ){
    $options[$target_view_name]=$target_view['label'];
  }
  if($include_blank){
    $options=array_merge(array(NULL=>t('Choose a targetview')),$options);
  }
  return $options;
}

function dpidestinations_helpers_dependant_target_viewname_and_display_options_for_view_display($view_name=NULL){
  $target_views=_dpidestination_get_all_targetviews();
  
  $options=array();
  
  if($view_name==NULL or $view_name=="" or !isset($target_views[$view_name])){
    $options['']=t('Choose a view name first');
    return $options;
  }
  else{
    foreach($target_views[$view_name]['displays'] as $display){
      $options[$display['display_name']]=$display['label'];
    }
  }
  
  
  return $options;
}