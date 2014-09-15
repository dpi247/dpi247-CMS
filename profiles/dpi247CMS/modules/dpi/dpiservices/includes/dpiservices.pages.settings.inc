<?php

function dpiservices_pages_form_map_preset_form($form, &$form_state){
  $form=array();
  
  $form['info']=array(
    '#markup'=>'This form is use to create a mapping between preset and a preset title. It\'s important for export of json.',
  );
  
  $form[''] = array(
    
  );
  
  return $form;
}