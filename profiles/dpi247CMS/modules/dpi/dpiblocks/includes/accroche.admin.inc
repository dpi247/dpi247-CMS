<?php

/**
 * @author LBA
 */
/**
 * 
 * @return Array|multiple|render
 */
function dpiblocks_get_accroche_info() {
  drupal_add_css(drupal_get_path('module', 'dpiblocks')."/css/dpiblocks.css");
  
  $render = l('Add accroche', 'admin/dpi247/accroche/add', array('attributes' => array('class' => 'dpiblocks_buton')));
  $rows = array();
  $header = array (
    array ('data' => t ( 'Readable name' )),
    array ('data' => t ( 'Machine name' )),
    array ('data' => t ( 'View mode' )),
    array ('data' => t ( 'Operation' ))
  );

  foreach(variable_get("dpiblocks_accroche_settings", array()) as $k => $array_val){
    $rows[] = array(
      array('data' => $array_val[0]),
      array('data' => $k),
      array('data' => $array_val[1]),
      array('data' => l('Edit', 'admin/dpi247/accroche/add/'.$k)." - ".l('Delete', 'admin/dpi247/accroche/del/'.$k)),
    );
  }
  
  $render .= theme('table',array('header' => $header,'rows'=> $rows));
  return $render;
}

/**
 *
 * @param Array $form          
 * @param Array $form_state          
 * @return Array
 */
function dpiblocks_accroche_admin_add_form($form, & $form_state, $data = NULL) {
  drupal_add_css(drupal_get_path('module', 'dpiblocks')."/css/dpiblocks.css");
  if (isset($data)){
    $result = variable_get ( 'dpiblocks_accroche_settings', array () );
    $default_value = array(
      'machine_name' => $data,
      'name' => $result[$data][0],
      'view' => $result[$data][1],
    );
  }
  
  $form ["name"] = array (
    '#type' => 'textfield',
    '#title' => t ( 'Name value' ),
    '#default_value' => (isset($default_value["name"]))? $default_value["name"] : "",
    '#required' => TRUE 
  );
  
  $form ["machine_name"] = array (
    '#type' => 'machine_name',
    '#title' => t ( 'View machine name' ),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t ( 'A unique name to construct the machine name of view. It must only contain lowercase letters, numbers and hyphens.' ),
    '#machine_name' => array (
      'exists' => '_dpiblocks_validate_machine_name',
      'source' => array (
        'name' 
      ) 
    ),
    "#default_value" => (isset($default_value["machine_name"]))? $default_value["machine_name"] : "",
    '#disabled' => isset($default_value["machine_name"]),
  );

  $form ["view_mode"] = array (
    '#type' => 'select',
    '#title' => t ( 'View mode' ),
    "#options" => dpiblocks_get_all_view_mode_name(),
    '#default_value' => (isset($default_value["view"]))? $default_value["view"] : "",
    '#required' => TRUE 
  );
  
  $form ["submit"] = array (
    '#type' => 'submit',
    '#value' => (isset($default_value))? t ( 'Update accroche' )  : t ( 'Add new accroche' ),
  );
  
  return $form;
}

function _dpiblocks_validate_machine_name() {
  return 0;
}

function dpiblocks_get_all_view_mode_name(){
  $entity_info = entity_get_info ( 'node' );
  $arrayKey = array_keys ( $entity_info ['view modes'] );
  $return = array();
  foreach ($arrayKey as $view_name){
    $return[$view_name] = $view_name;
  }
  return $return;
}

/**
 *
 * @param Array $form          
 * @param Array $form_state          
 */
function dpiblocks_accroche_admin_add_form_submit($form, & $form_state) {
  $result = variable_get ( 'dpiblocks_accroche_settings', array () );
  $result [$form_state ["values"] ["machine_name"]] = array (
    $form_state ["values"] ["name"],
    $form_state ["values"] ["view_mode"], 
  );
  variable_set ( 'dpiblocks_accroche_settings', $result );
  drupal_set_message ( 'element successfully record' );
  drupal_goto ( 'admin/dpi247/accroche' );
}

function dpiblocks_accroche_admin_del($machine_name){
  if(isset($machine_name) && $machine_name!=""){
    $result = variable_get ( 'dpiblocks_accroche_settings', array () );
    if(isset($result[$machine_name])){
      unset($result[$machine_name]);
      variable_set('dpiblocks_accroche_settings', $result);
      drupal_set_message('Delete successful');
      drupal_goto('admin/dpi247/accroche');
    }else{
      drupal_set_message('Element selected is not avalaible', 'error');
    }
  }else{
    drupal_set_message('Element selected is not avalaible', 'error');
  }
}

