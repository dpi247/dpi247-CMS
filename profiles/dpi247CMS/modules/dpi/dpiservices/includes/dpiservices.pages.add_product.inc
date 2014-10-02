<?php
/**
 * @author lba
 */

/**
 * This function create form to add or edit a product
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function dpiservices_pages_form_add_or_edit_product_form($form, &$form_state, $argument = ''){
  module_load_include('inc', 'dpiservices', 'dpiservices.api');
  $form=array();
  if(isset($argument) && $argument != ''){
    if(!$product_arg = dpiservices_api_product_get_one_product_by_label($argument)){
      drupal_set_message("Product cannot be loaded", 'error');
    }
  }
  
  $query = db_select('menu_custom', 'm')->fields('m',array('menu_name','title'))->orderBy('title')->execute();
  $value_select_menu = array();
  while ($row = $query->fetchAssoc()){
    $value_select_menu[$row['menu_name']] = $row['menu_name'];
  }
  
  $image_preset = array();
  foreach (image_styles() as $preset_info){
    $image_preset[$preset_info['name']] = $preset_info['name'];
  }
  
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a product'),
  );
  
  $form['fieldset']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#default_value' => (isset($product_arg)) ? $product_arg['label']: '',
  );
  
  $form['fieldset']['machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#required' => TRUE,
    '#default_value' => (isset($product_arg)) ? $product_arg['machine_name']: '',
  );
  
  $form['fieldset']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => FALSE,
    '#default_value' => (isset($product_arg)) ? $product_arg['description']: '',
  );
  
  $presets=unserialize(dpi_variable_get("dpiservices_preset", array())) ;
  if(isset($product_arg)){
    $default_preset = unserialize($product_arg['preset']);  
  }
  
  foreach ($presets as $preset){
    $form['fieldset'][$preset] = array(
      '#type' => 'select',
      '#title' => t($preset),
      '#description' => t('Select a preset.'),
      '#options' => $image_preset,
      '#default_value' => (isset($default_preset[$preset]))? $default_preset[$preset]: '',
    );
  }
  
  $form['fieldset']['select_product'] = array(
    '#type' => 'select',
    '#title' => t('Select product based on :'),
    '#options' => array(
      'Menu (Recommended)' => 'Menu (Recommended)',
      'Destination' => 'Destination'
    ),
    '#default_value' => (isset($product_arg)) ? $product_arg['product']: '',
  );
  
  $form['fieldset']['select_product_type'] = array(
    '#type' => 'select',
    '#title' => t('Select a type'),
    '#options' => $value_select_menu,
    '#default_value' => (isset($product_arg)) ? $product_arg['product_type']: '',
  );
  
  $form['fieldset']['version'] = array(
    '#type' => 'select',
    '#title' => t('Select a service to be use'),
    '#options' => dpiservices_get_services(),
    '#default_value' => (isset($product_arg)) ? $product_arg['version']: '',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value'=>'save',
  );
  
  return $form;
}

/**
 * This function merge information (insert or update)
 * 
 * @param array $form
 * @param array $form_state
 */
function dpiservices_pages_form_add_or_edit_product_form_submit($form, &$form_state){
  if(isset($form_state['values'])){
    try {
      $presets=unserialize(dpi_variable_get("dpiservices_preset", array())) ;
      $preset_db_value = array();
      
      foreach ($presets as $preset){
        if(isset($form_state['values'][$preset])) $preset_db_value[$preset] = $form_state['values'][$preset];
      }
      
      db_merge('dpiservices_product')
          ->key(array(
            'label' => $form_state['values']['label']
          ))
          ->fields(array(
            'label' => $form_state['values']['label'],
            'machine_name' => $form_state['values']['machine_name'],
            'description' => $form_state['values']['description'],
            'preset' => serialize($preset_db_value),
            'product' => $form_state['values']['select_product'],
            'product_type' => $form_state['values']['select_product_type'],
            'version' => $form_state['values']['version'],
          ))
          ->execute();
          
      drupal_set_message('Modification for '.$form_state['values']['label'].' is saved');
      drupal_goto('admin/dpi247/dpijsonexporter');
    } catch (Exception $e) {
      dsm($e);
      drupal_set_message("Une erreur sql a interrompu le traitement", 'error');
    }
  } else {
    drupal_set_message('Information is not correctly sended', 'error');
  }
}

/**
 * This function return an array with service
 * 
 * @return array
 */
function dpiservices_get_services(){
  $return_name = array();
  foreach(services_endpoint_load_all() as $service){
    $resources = $service->resources;
    if(isset($resources['dpijsonexporter']) && !isset($resources['wallyjsonexporter'])){
      $return_name[$service->name] = $service->name;
    }else{
     if(!isset($resources['dpijsonexporter']) && isset($resources['wallyjsonexporter'])){
       $return_name[$service->name] = $service->name;
     } 
    }
  }
  return $return_name;
}