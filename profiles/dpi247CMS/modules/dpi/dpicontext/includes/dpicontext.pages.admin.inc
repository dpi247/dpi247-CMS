<?php

/**
 * @author GVD
 *  
 */
function dpicontext_settings_form($form, &$form_state) {
  $form = array ();
  dsm ( 'inside form' );
  $data_list = dpicontext_api_get_all_context ();

  foreach ( $data_list as $k => $v ) {
    $text = "";
    
    if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
      
      foreach ( $v ['value'] as $str ) {
        // modify date format from y-m-d to d-m-y
        
        $str = date ( "d-m-Y H:i", strtotime ( $str ) );
        
        $text .= $str . ' - ';
      }
      $text = substr ( $text, 0, - 3 );
      $data_list [$k] ['value'] = $text;
      $text = "";
    }
    
    $form [$k]['name']  = array (
      '#markup' => $data_list [$k] ['label'] 
    );
    
    switch ($data_list [$k] ['type']) {
      case 0 :
        $form [$k] ['form_element'] = array (
          '#type' => 'select',
          '#options' => range ( 0, $data_list [$k] ['value'] ) 
        );
        
        break;
      case 1 :
        $form [$k] ['form_element'] = array (
          '#type' => 'select',
          '#options' => $data_list [$k] ['value'] 
        );
        
        break;
    }
  }
  $form ["#submit"] = "dpicontext_settings_form_submit";
  
  $form ["#theme"] = 'dpicontext_table_theme';
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ),
    '#submit' => array (
      'dpicontext_settings_form_submit' 
    ) 
  );
  
  return $form;
}

function theme_dpicontext_table_theme($vars) {
  // build header
  dsm ( $vars ["element"] );
  $element= $vars ["element"];
  $header = array (
    "Label",
    "Value" 
  );
  $rows = array ();
  dsm($element, 'ici');
  foreach ( element_children($element) as $k) {
    //$element = $form_element [$k] ['form_element'];
    $rows [] = array (
      
      // $v ['name'],
      // $v ['form_element'],
      // render ( $v )
    		array('data' => $element[$k]['name']),
    		array('data' =>render ( $element[$k]),
       ) 
       
    );
    dsm($k);
  }
  $variables = array (
    
    'header' => $header,
    'rows' => $rows,
    'attributes' => array (),
    'caption' => NULL,
    'colgroups' => array (),
    'sticky' => FALSE,
    'empty' => "no value" 
  );
  $render = theme_table ( $variables );
//   $render .= drupal_render ( $form_element ['form_id'] );
//   $render .= drupal_render ( $form_element ['form_build_id'] );
//   $render .= drupal_render ( $form_element ['form_token'] );
//   $render .= drupal_render ( $form_element ['submit'] );
  
  return $render;
}

/**
 * Process annotation settings submission.
 */
function dpicontext_settings_form_submit($form, & $form_state) {
  dsm ( 'inside submit' );
  dsm ( $form_state );
  // dpi_variable_set ( 'dpicontext_values', $form_state ['values'] ['dpicontext_defcon_level'] );
  drupal_set_message ( 'Saved successfuly ! = ' );
  // drupal_goto ( 'admin/config/dpicontext/settings' );
}








