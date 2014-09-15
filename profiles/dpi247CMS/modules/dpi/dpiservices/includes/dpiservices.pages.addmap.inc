<?php

function dpiservices_pages_form_add_map_form(){ 
  $form = array();

  $form['info']=array(
    '#markup'=>'Enter information into the textarea. One line is one possibility.',
  );
  
  $form['map_config'] = array(
    '#title' => t('Add map possibility'),
    '#type' => 'fieldset',
  );
  
  $form['map_config']['map'] = array(
    '#title' => t('Map possibility'),
    '#type' => 'textarea',
    '#required' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value'=>'Submit'
  );
  
  return $form;  
}

function dpiservices_pages_form_add_map_form_submit($form, & $form_state){  
  if(isset($form_state['values'])){
    $table_verification = dpiservices_get_preset_in_db();
    $text = trim($form_state['values']['map']);
    $textAr = explode("\n", $text);
    $textAr = array_filter($textAr, 'trim');
    foreach ($textAr as $line){
      if(!dpiservices_preset_is_present($table_verification, $line)) dpiservices_preset_insert($line);        
    }
    drupal_set_message('All information is present to the database');
  } else {
    drupal_set_message('Information is not correctly sended', 'error');
  } 
}

function dpiservices_get_preset_in_db(){
  $value = db_select('dpiservices_preset' , 'dp')
            ->fields('dp', array('preset_map_name'))
            ->execute();
  $return = array();
  while ($row = $value->fetchAssoc()){
    $return[] = $row;
  }
  
  return $return; 
}

function dpiservices_preset_is_present($array, $element){
  foreach ($array as $val){    
    if(isset($val['preset_map_name']) && strcmp($val['preset_map_name'], trim($element)) == 0)
      return TRUE;
  }
  return FALSE;
}

function dpiservices_preset_insert($element){
  db_insert('dpiservices_preset')->fields(array(
    'preset_map_name' => $element,
  ))->execute();
}