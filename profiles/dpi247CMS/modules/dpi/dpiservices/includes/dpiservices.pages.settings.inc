<?php

function dpiservices_pages_form_map_preset_form($form, &$form_state){
  $form=array();
  $dpiservices_preset_value = dpiservices_get_active_value_for_preset();
  $image_preset = array();
  
  foreach (image_styles() as $preset_info){
    $image_preset[$preset_info['name']] = $preset_info['label'];
  }
  
  $form['info']=array(
    '#markup'=>'This form is use to create a mapping between preset and a preset title. It\'s important for export of json.',
  );
  
  foreach ($dpiservices_preset_value as $preset){    
    $form['preset_select'][$preset['preset_map_name']] = array(
      '#type' => 'select',
      '#title' => t($preset['preset_map_name']),
      '#default_value' => (isset($preset['preset_name'])) ? $preset['preset_name'] : '',
      '#description' => t('Select your preset.'),
      '#options' => $image_preset
    );
  }
  
  if(isset($dpiservices_preset_value)){
    $form['submit'] = array(
      '#type' => 'submit',
     '#value'=>'Submit'
    );
  }
  
  return $form;
}

function dpiservices_pages_form_map_preset_form_submit($form, &$form_state){
  if(isset($form_state['values'])){
    $dpiservices_preset_value = dpiservices_get_active_value_for_preset();
    foreach ($dpiservices_preset_value as $preset){
      if(isset($preset['preset_map_name']) && isset($form_state['values'][$preset['preset_map_name']])){
        dpiservices_insert_preset_name($form_state['values'][$preset['preset_map_name']], $preset['preset_map_name']);
      }
    }
    drupal_set_message("Update information success");
  }else{
    drupal_set_message("Service is not available" , 'error');
  }
}

function dpiservices_get_active_value_for_preset(){  
  $return = array();
  
  $value = db_select('dpiservices_preset', 'dp')
            ->fields('dp', array('preset_name','preset_map_name'))
            ->execute();
  while($row = $value->fetchAssoc()){
    $return[] = $row;
  }  
  
  return $return;
}

function dpiservices_insert_preset_name($preset_name, $preset_map_name){
   db_update('dpiservices_preset')
      ->fields(array(
        'preset_name' => $preset_name
      ))
      ->condition('preset_map_name' , $preset_map_name, '=')
      ->execute();
}