<?php
/**
 * @author LBA
 */

/**
 * @file
 * Plugin to provide access control/visibility based on length of
 * simplecontext argument (in URL).
 */
$plugin = array (
  'title' => t ( "DpiContext: Context" ),
  'description' => t ( 'Control access by dpicontext status argument.' ),
  'callback' => 'dpicontext_status_ctools_access_check',
  'settings form' => 'dpicontext_status_ctools_access_settings',
  'summary' => 'dpicontext_status_ctools_access_summary' 
);

/**
 * Settings form for the 'by role' access plugin.
 */
function dpicontext_status_ctools_access_settings($form, &$form_state, $conf) {
  module_load_include ( 'php', 'dpicontext', 'dpicontext.api' );
  $data_list = dpicontext_api_get_all_context ();
  $rows = array ();
  
  foreach ( $data_list as $k => $v ) {
    if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
      dpicontext_api_change_date_format ( $data_list, $v, $k );
    }
    $rows ['row_' . $v ['id']] = dpicontext_get_form_element_by_type ( $data_list, $v ['id'], $k, $conf, 'row_' . $v ['id'] );
  }
  
  foreach ( $rows as $k => $v ) {
    $form ['settings'] [$k] = array (
      '#type' => 'fieldset',
      '#title' => t ( 'Change parameter for :' ) 
    );
    
    foreach ( $v as $value_form => $val ) {
      $form ['settings'] [$k] [$value_form] = $val;
    }
  }
  
  return $form;
}

/**
 * Check for access.
 */
function dpicontext_status_ctools_access_check($conf, $context = NULL) {
  $general_value = dpi_variable_get ( 'dpicontext_value_at_moment', NULL );
  $access = FALSE;
  
  if (isset ( $general_value )) {
    foreach ( $conf as $elem_key => $elem_val ) {
      $id = explode ( '_', $elem_key );
      $id = $id [1];
      if (current ( $elem_val )) {
        reset ( $elem_val );
        $first_key = key ( $elem_val );
        $first_key = explode ( '_', $first_key );
        
        switch (intval ( $first_key [2] )) {
          case 0 : // case of numeric
            $second = array_slice ( $elem_val, 1, 1 );
            switch (array_slice ( $elem_val, 2, 1 )) {
              case '=' :
                  if (intval ( $second ) == intval ( $general_value [$id] ['value'] )) {
                    $access = TRUE;
                  }
                break;
              case '>' :
                  if (intval ( $second ) > intval ( $general_value [$id] ['value'] )) {
                    $access = TRUE;
                  }
                break;
              case '<' :
                  if (intval ( $second ) < intval ( $general_value [$id] ['value'] )) {
                    $access = TRUE;
                  }
                break;
            }
            break;
          case 1 : // case of text
            $second = array_slice ( $elem_val, 1, 1 );
            if (intval ( $second ) == intval ( $general_value [$id] ['index'] )) {
              $access = TRUE;
            }
            break;
          case 2 : // case of date
              
            break;
        }
      }
    }
  }
  
  return $access;
}

function dpicontext_get_access_for_numeric_type($general_value, $elem_val, & $access) {
  $access = true;
}

function dpicontext_get_access_for_text_type($general_value, $elem_val, & $access) {
  $access = true;
}

function dpicontext_get_access_for_date_type($general_value, $elem_val, & $access) {
  $access = true;
}

/**
 * Provide a summary description based upon the checked roles.
 */
function dpicontext_status_ctools_access_summary($conf, $context) {
  dsm ( $conf );
  return t ( 'Dpicontext has defined rules' );
}

/**
 * This function return right form element necessary for the form
 *
 * @param array $data_list          
 * @param integer $v          
 * @param integer $k          
 * @return form element
 */
function dpicontext_get_form_element_by_type($data_list, $v, $k, $conf, $name) {
  $return = NULL;
  switch ($data_list [$k] ['type']) {
    case 0 :
      $return = array (
        'activate_' . $v . '_0' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset ( $conf [$name] ['activate_' . $v . '_0'] )) ? $conf [$name] ['activate_' . $v . '_0'] : FALSE 
        ),
        'label_' . $v . '_0' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_0' => array (
          '#type' => 'select',
          '#options' => range ( 0, $data_list [$k] ['value'] ),
          '#default_value' => (isset ( $conf [$name] ['options_' . $v . '_0'] )) ? $conf [$name] ['options_' . $v . '_0'] : 0 
        ),
        'operation_' . $v . '_0' => array (
          '#type' => 'select',
          '#options' => array (
            '=' => '=',
            '<' => '<',
            '>' => '>' 
          ),
          '#default_value' => (isset ( $conf [$name] ['operation_' . $v . '_0'] )) ? $conf [$name] ['operation_' . $v . '_0'] : '=' 
        ) 
      );
      break;
    case 1 :
      $return = array (
        'activate_' . $v . '_1' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset ( $conf [$name] ['activate_' . $v . '_1'] )) ? $conf [$name] ['activate_' . $v . '_1'] : FALSE 
        ),
        'label_' . $v . '_1' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_1' => array (
          '#type' => 'select',
          '#options' => $data_list [$k] ['value'],
          '#default_value' => (isset ( $conf [$name] ['options_' . $v . '_1'] )) ? $conf [$name] ['options_' . $v . '_1'] : '' 
        ),
        'operation_' . $v . '_1' => array (
          '#type' => 'select',
          '#options' => array (
            '=' => '=' 
          ) 
        ) 
      );
      break;
    case 2 :
      $return = array (
        'activate_' . $v . '_2' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset ( $conf [$name] ['activate_' . $v . '_2'] )) ? $conf [$name] ['activate_' . $v . '_2'] : FALSE 
        ),
        'options_' . $v . '_2' => array (
          '#markup' => t ( 'Activate context : @context for date => @date', array (
            '@context' => $data_list [$k] ['label'],
            '@date' => $data_list [$k] ['value'] 
          ) ) 
        ),
        'operation_' . $v . '_2' => array (
          '#type' => 'select',
          '#options' => array (
            'inside' => 'inside',
            'outside' => 'outside' 
          ),
          '#default_value' => (isset ( $conf [$name] ['operation_' . $v . '_2'] )) ? $conf [$name] ['operation_' . $v . '_2'] : 'inside' 
        ) 
      );
      break;
  }
  return $return;
}
