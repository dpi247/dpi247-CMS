<?php

function dpiservices_pages_form_add_map_form(){ 
  $form = array();

  $textarea_default_content = '';
  
  foreach(unserialize(dpi_variable_get('dpiservices_preset', '')) as $value){
    $textarea_default_content .= $value."\n";
  }
  
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
    '#default_value' => $textarea_default_content,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value'=>'Submit'
  );
  
  return $form;  
}

function dpiservices_pages_form_add_map_form_submit($form, & $form_state){  
  if(isset($form_state['values'])){
    $result = array();
    $text = trim($form_state['values']['map']);
    $textAr = explode("\n", $text);
    $textAr = array_filter($textAr, 'trim');
    foreach ($textAr as $line){
      $result[] = trim($line);        
    }
    dpi_variable_set("dpiservices_preset", serialize($result));
    drupal_set_message('All information is present to the database');
  } else {
    drupal_set_message('Information is not correctly sended', 'error');
  } 
}


