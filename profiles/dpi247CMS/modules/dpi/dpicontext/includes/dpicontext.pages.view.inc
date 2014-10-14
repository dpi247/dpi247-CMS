<?php

/**
 * @author GVD - LBA
 */

/**
 * This function is callback from menu for render page
 *
 * @return string
 */
function dpicontext_view_page() {
  $output = array ();
  $form = drupal_get_form ( 'dpicontext_add_context_form' );
  $output = drupal_render ( $form );
  $output .= dpicontext_view_table ();
  return $output;
}

/**
 * This function is use to create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return multitype:array
 */
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

/**
 * This function generate table for view informations
 *
 * @return render(table)
 */
function dpicontext_view_table() {
  $data_list = dpicontext_api_get_all_context ();
  $keys = NULL;
  
  if (! empty ( $data_list )) {
    $keys = array_keys ( $data_list [0] );
  }
  
  $header = dpicontext_generate_header ( $keys );
  dpicontext_generate_rows ( $data_list );
  
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

/**
 * This function is use to generate header for table 
 * 
 * @param array $keys
 * @return multiple:array
 */
function dpicontext_generate_header($keys) {
  if (isset ( $keys )) {
    $header = array ();
    foreach ( $keys as $key ) {
      $header [] = array (
        'data' => $key 
      );
    }
    $header [] = array (
      'data' => "operation"
    );
  }else{
    $header = array(
      array(
        'data' => "No data available !"
      )
    );
  }
  return $header;
}

/**
 * This function is use to generate rows for table
 * 
 * @param array $data_list
 */
function dpicontext_generate_rows(& $data_list = array()) {
  foreach ( $data_list as $k => $v ) {
    dpicontext_replace_context_type_from_int_to_string ( $data_list, $k );
    dpicontext_add_link_for_operation ( $data_list, $k );
    dpicontext_change_date_format ( $data_list, $v, $k );
  }
}

/**
 * This function is use to replace type from int to string
 * 
 * @param array $data_list
 * @param integer $k
 */
function dpicontext_replace_context_type_from_int_to_string(& $data_list,$k) {
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
}

/**
 * This function is use to add operation link from table
 * 
 * @param array $data_list
 * @param integer $k
 */
function dpicontext_add_link_for_operation(& $data_list, $k) {
  $link_table = array (
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
  );
  array_push ( $data_list [$k], $link_table );
}

/**
 * This is use to replace date into the right format
 * 
 * @param array $data_list
 * @param integer $v
 * @param integer $k
 */
function dpicontext_change_date_format(& $data_list, $v, $k) {
  if (is_array ( $v ['value'] )) {
    $text = "";
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