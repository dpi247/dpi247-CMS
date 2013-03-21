<?php

function dpidestinations_helpers_convert_flat_destinations_terms_in_options($terms){
  $options=array();
  foreach($terms as $term){
    $options[$term->tid]=$term->name;
  }
  return $options;
}