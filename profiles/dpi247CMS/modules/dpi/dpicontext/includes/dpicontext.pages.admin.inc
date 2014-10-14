<?php

/**
 * @author LBA 
 */

/**
 * This function create form
 * The render of the form table is create by the theme
 * into the function dpicontext_table_theme
 *
 * @param array $form          
 * @param array $form_state          
 * @return multitype:array
 */
function dpicontext_settings_form($form, &$form_state) {
  $form = array ();
  $data_list = dpicontext_api_get_all_context ();
  $rows = array ();
  
  foreach ( $data_list as $k => $v ) {
    if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
      dpicontext_change_date_format ( $data_list, $v, $k );
    }
    $rows ['row_' . $v ['id']] = dpicontext_get_form_element_by_type ( $data_list, $v ['id'], $k );
  }
  
  $form ['table'] = array (
    '#theme' => 'dpicontext_table_theme',
    '#header' => array (
      t ( 'Activation' ),
      t ( 'Name' ),
      t ( 'Options' ) 
    ),
    'rows' => $rows 
  );
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  
  return $form;
}

/**
 * This function render the part of the form name table
 *
 * @param array:form $variables          
 * @return render
 */
function theme_dpicontext_table_theme($variables) {
  $form = $variables ['element'];
  $rows = $form ['rows'];
  $header = $form ['#header'];
  
  $content = array (
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array () 
  );
  
  foreach ( element_children ( $rows ) as $row_index ) {
    $row = array ();
    foreach ( element_children ( $rows [$row_index] ) as $col_index ) {
      $row [] = drupal_render ( $rows [$row_index] [$col_index] );
    }
    $content ['#rows'] [] = $row;
  }
  
  // Render the table and return it.
  return drupal_render ( $content );
}

/**
 * Save informations to dpi_variable after submitting
 *
 * @param array $form          
 * @param array $form_state          
 */
function dpicontext_settings_form_submit($form, & $form_state) {
  try {
    $temp_form_state = $form_state ['values'];
    unset ( $temp_form_state ['submit'] );
    unset ( $temp_form_state ['form_build_id'] );
    unset ( $temp_form_state ['form_token'] );
    unset ( $temp_form_state ['form_id'] );
    unset ( $temp_form_state ['op'] );
    
    $val = dpicontext_reorder_informations_to_save ( $temp_form_state, $form );
    
    dpi_variable_set('dpicontext_value_at_moment', $val);
    drupal_set_message ( 'Saved successfuly !' );
  } catch (Exception $e) {
    drupal_set_message ( 'An error is happened to create values' );
  }
}

/**
 * This function réorder informations to create a table
 * that we can use to compare with view context
 * 
 * @param array $infos
 * @param array $form
 * @return multiple:array
 */
function dpicontext_reorder_informations_to_save($infos, $form) {
  $return = array ();
  foreach ( $infos as $k => $v ) {
    $val_info = explode ( '_', $k );
    if (isset ( $val_info [1] ) && isset ( $val_info [0] ) && $val_info [0] == 'activate') {
      
      switch ($val_info [2]) {
        case 0 :
          $return [$val_info [1]] = array (
            'actif' => $v,
            'value' => $infos ['options_' . $val_info [1] . '_0'], 
          );
          break;
        case 1 :
          $return [$val_info [1]] = array (
            'actif' => $v,
            'value' => $form['table']['rows']['row_'.$val_info [1]]['options_'.$val_info [1].'_1']['#options'][$infos ['options_' . $val_info [1] . '_1']],  
          );
          break;
        case 2 :
          $return [$val_info [1]] = array (
            'actif' => $v,
            'value' => 'Valeur date',
          );
          break;
      }
    }
  }
  return $return;
}

/**
 * This function create right render for the date
 * Modify date format from y-m-d to d-m-y
 * 
 * @param array $data_list          
 * @param integer $v          
 * @param integer $k          
 */
function dpicontext_change_date_format(&$data_list, $v, $k) {
  $text = "";
  foreach ( $v ['value'] as $str ) {
    $str = date ( "d-m-Y H:i", strtotime ( $str ) );
    $text .= $str . ' - ';
  }
  $text = substr ( $text, 0, - 3 );
  $data_list [$k] ['value'] = $text;
}

/**
 * This function return right form element necessary for the form
 *
 * @param array $data_list          
 * @param integer $v          
 * @param integer $k          
 * @return form element
 */
function dpicontext_get_form_element_by_type($data_list, $v, $k) {
  $return = NULL;
  switch ($data_list [$k] ['type']) {
    case 0 :
      $return = array (
        'activate_' . $v . '_0' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ) 
        ),
        'label_' . $v . '_0' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_0' => array (
          '#type' => 'select',
          '#options' => range ( 0, $data_list [$k] ['value'] ) 
        ) 
      );
      break;
    case 1 :
      $return = array (
        'activate_' . $v . '_1' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ) 
        ),
        'label_' . $v . '_1' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_1' => array (
          '#type' => 'select',
          '#options' => $data_list [$k] ['value'] 
        ) 
      );
      break;
    case 2 :
      $return = array (
        'activate_' . $v . '_2' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ) 
        ),
        'label_' . $v . '_2' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_2' => array (
          '#markup' => t ( 'Activate context : @context for date => @date', array (
            '@context' => $data_list [$k] ['label'],
            '@date' => $data_list [$k] ['value'] 
          ) ) 
        ) 
      );
      break;
  }
  return $return;
}

