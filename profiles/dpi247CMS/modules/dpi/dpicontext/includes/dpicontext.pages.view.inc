<?php

function dpicontext_view_page() {
  $output = array ();
  $form = drupal_get_form ( 'dpicontext_add_context_form' );
  $output = drupal_render ( $form );
  $output .= dpicontext_view_table ();
  return $output;
}

function dpicontext_add_context_form($form, &$form_state) {
  $form ['add'] = array (
    '#title' => t ( 'Add context' ),
    '#type' => 'fieldset' 
  );
  $form ['add'] ['link'] = array (
    '#type' => 'link',
    '#title' => t ( 'Add' ),
    '#href' => 'admin/config/dpicontext/settings/add' 
  );
  
  return $form;
}

function dpicontext_view_table() {
  // TODO hide id cells ?
  $data_list = array ();
  
  $data_list = dpicontext_api_get_all_context ();
  
  if (! empty ( $data_list )) {
    $keys = array_keys ( $data_list [0] );
  }
  
  $header = array ();
  
  if (isset ( $keys )) {
    foreach ( $keys as $key ) {
      $header [] = array (
        'data' => $key 
      );
    }
  }
  
  $header [] = array (
    'data' => "operation" 
  );
  
  $text = " ";
  
  foreach ( $data_list as $k => $v ) {
    // remplace "context's type" type from int to string
    switch ($data_list [$k] ['type']) {
      case 0 :
        $data_list [$k] ['type'] = "Numeric";
        break;
      case 1 :
        $data_list [$k] ['type'] = "Text";
        break;
      case 2 :
        $data_list [$k] ['type'] = "Date";
        break;
    }
    
    // add link to edit form and set value page
    
    array_push ( $data_list [$k], array (
      'data' => array (
        array (
          '#type' => 'link',
          '#title' => t ( 'Edit' ),
          '#href' => 'admin/config/dpicontext/settings/edit/' . $data_list [$k] ["id"] 
        ),
        array (
          '#type' => 'text_format',
          '#title' => " " 
        ),
        array (
          '#type' => 'link',
          '#title' => t ( 'Delete' ),
          '#href' => 'admin/config/dpicontext/settings/delete/' . $data_list [$k] ["id"] 
        ),
        
        array (
          '#type' => 'text_format',
          '#title' => " " 
        ),
        array (
          '#type' => 'link',
          '#title' => t ( 'Set value' ),
          '#href' => 'admin/config/dpicontext/settings/settings' 
        ) 
      )
       
    ) );
    
    if (is_array ( $v ['value'] )) {
      
      foreach ( $v ['value'] as $str ) {
        // if date then modify date format from y-m-d to d-m-y
        if ($v ['type'] == 2) {
          $str = date ( "d-m-Y H:i", strtotime ( $str ) );
        }
        $text .= $str . ' - ';
      }
      $text = substr ( $text, 0, - 3 );
      $data_list [$k] ['value'] = $text;
      $text = "";
    }
  }
  
  $variables = array (
    
    'header' => $header,
    'rows' => $data_list,
    'attributes' => array (),
    'caption' => NULL,
    'colgroups' => array (),
    'sticky' => FALSE,
    'empty' => "no value" 
  );
  
  return theme_table ( $variables );
}
