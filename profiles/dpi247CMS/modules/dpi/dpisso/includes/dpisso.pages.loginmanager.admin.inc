<?php

function dpisso_loginmanager_admin_settings_page_form(){
  $form=array();
  
  // Get ssoClient config from file.
  $config = dpisso_get_federation();
  
  // Build Settings Table
  $headers = array('key' => t('Key'));
  $rows = array();

  $i=0;
  foreach($config as $domaineGrep => $arrayData){
    $rows[$i]['key'] = $domaineGrep;
    foreach($arrayData as $k => $v){
      $headers[$k] = $k;  
      $rows[$i][$k] = $v;
    }
    $i++;
  }
  $form=array();
  
  /**
   * The unique identifier of the dpi instance in the all SSO processes & modules
  */
  $form['dpisso_loginmanager_domain']=array(
    '#type'=>'textfield',
    '#title'=>t('Login Manager Domain'),
    '#description'=>t(" Do not indicate the scheme such as http:// neither the trailling slash"),
    '#default_value'=>dpi_variable_get('dpisso_loginmanager_domain',''),
  );
  
  $form['values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current Config Values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['values']['config'] = array(
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows))
  );
  
  return dpi_system_settings_form($form);
}

function _dpisso_loginmanager_domain_element_validate($element, &$form_state, $form) {
  if(!is_file($element["#value"])){
    form_error($element, t('File not found :').$element["#value"]);
  }
}