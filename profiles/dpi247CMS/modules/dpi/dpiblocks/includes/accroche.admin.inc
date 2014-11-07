<?php

function dpiblocks_accroche_admin_add_form($form, & $form_state) {
  
  $str="";
  if($var = variable_get('dpiblocks_accroche_settings')){
    foreach ($var as $k => $v){
      $str .= $k."=".$v."\n";
    }
  }
  
  $form ["textarea"] = array (
    '#title' => t ( 'Enter settings' ),
    '#type' => 'textarea',
    '#description' => t ( 'Enter your different value in the same format from a config file. (Ex: key=value and key is accroche name, value is view mode)' ),
    "#require" => TRUE,
    "#default_value" => $str,
  );
  
  $form ["submit"] = array (
    '#type' => 'submit',
    '#value' => t ( 'Update information' ) 
  );
  
  return $form;
}


function dpiblocks_accroche_admin_add_form_submit($form, & $form_state){
  $text = trim($form_state["values"]["textarea"]);
  $textAr = explode("\n", $text);
  $textAr = array_filter($textAr, 'trim'); // remove any extra \r characters left behind
  $result = array();
  foreach ($textAr as $line) {
    $line_explode = explode("=", $line);
    $result[$line_explode[0]] = $line_explode[1];
  }
  variable_set('dpiblocks_accroche_settings', $result);
  drupal_set_message('Settings updated');
  
}