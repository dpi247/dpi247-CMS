<?php
/**
 * @author lba
 */

/**
 * This function create a form for configure jsonexporter
 * 
 * @return array
 */
function dpiservices_pages_form_settings_form(){
  $form = array();
  
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  
  $form['fieldset']['links'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#required' => TRUE,
    '#default_value' => dpi_variable_get("dpiservices_settings_link_url", ""),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value'=>'save'
  );
  
  return $form;
}

/**
 * This function set information to dpi_variable
 * 
 * @param array $form
 * @param array $form_state
 */
function dpiservices_pages_form_settings_form_submit($form, &$form_state){
  if(isset($form_state['values'])){
    dpi_variable_set('dpiservices_settings_link_url', $form_state["values"]['links']);
    drupal_set_message("Information is saved");
  } else {
    drupal_set_message('Information is not correctly sended', 'error');
  }
}