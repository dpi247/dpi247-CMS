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
  'title' => t ( "DpiState: State" ),
  'description' => t ( 'Control access by dpistate status argument.' ),
  'callback' => 'dpistate_status_ctools_access_check',
  'settings form' => 'dpistate_status_ctools_access_settings',
  'summary' => 'dpistate_status_ctools_access_summary' 
);

/**
 * This function is use to create form about view settings
 * 
 * @param array $form
 * @param array $form_state
 * @param array $conf
 * @return multiple:array
 */
function dpistate_status_ctools_access_settings($form, &$form_state, $conf) {
  module_load_include ( '../../../dpistate/plugins/access/php', 'dpistate', 'dpistate.api' );
  $data_list = dpistate_api_get_all_states();
  $rows = array ();
  
  foreach ( $data_list as $k => $v ) {
    if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
      dpistate_api_change_date_format ( $data_list, $v, $k );
    }
    $rows ['row_' . $v ['id']] = dpistate_get_form_element_by_type ( $data_list, $v ['id'], $k, $conf, 'row_' . $v ['id'] );
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
 * This function Check if element can be view
 * 
 * @param array $conf
 * @param string $state
 * @return boolean
 */
function dpistate_status_ctools_access_check($conf, $state = NULL) {
  module_load_include ( '../../../dpistate/plugins/access/php', 'dpistate', 'dpistate.api' );
  module_load_include ( '../../../dpistate/plugins/access/inc', 'dpistate', 'plugins/access/dpistate_status' );
  $general_value = variable_get ( 'dpistate_value_at_moment', NULL );
  $access = FALSE;
  $cpt_activity = 0;
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
            if (($var = dpistate_get_access_for_numeric_type ( $general_value, $id, $elem_val )) == - 1) {
              return FALSE;
            } elseif ($var == 1) {
              $access = TRUE;
            }
            break;
          case 1 : // case of text
            if (($var = dpistate_get_access_for_text_type ( $general_value, $elem_val, $id )) == - 1) {
              return FALSE;
            } elseif ($var == 1) {
              $access = TRUE;
            }
            break;
          case 2 : // case of date
            if(($var = dpistate_get_access_for_date_type($elem_val, $id)) == -1){
              return FALSE;
            }elseif ($var == 1){
              $access = TRUE;
            }
            break;
        }
        $cpt_activity ++;
      }
    }
  }
  
  if ($cpt_activity == 0) {
    $access = TRUE;
  }
  
  return $access;
}

/**
 * This function is use to know for a numeric element if it can be view
 * 
 * @param array $general_value
 * @param integer $id
 * @param array $elem_val
 * @return number
 */
function dpistate_get_access_for_numeric_type($general_value, $id, $elem_val) {
  $access = 0;
  switch ($elem_val ['operation_' . $id . '_0']) {
    case '=' :
      if (intval ( $elem_val ['options_' . $id . '_0'] ) == intval ( $general_value [$id] ['value'] )) {
        $access = 1;
      } else {
        return - 1;
      }
      break;
    case '>' :
      if (intval ( $elem_val ['options_' . $id . '_0'] ) < intval ( $general_value [$id] ['value'] )) {
        $access = 1;
      } else {
        return - 1;
      }
      break;
    case '<' :
      if (intval ( $elem_val ['options_' . $id . '_0'] ) > intval ( $general_value [$id] ['value'] )) {
        $access = 1;
      } else {
        return - 1;
      }
      break;
  }
  return $access;
}

/**
 * This function is use to know if an element can be viewed by text condition
 * 
 * @param array $general_value
 * @param array $elem_val
 * @param integer $id
 * @return number
 */
function dpistate_get_access_for_text_type($general_value, $elem_val, $id) {
  $access = 0;
  if (intval ( $elem_val ['options_' . $id . '_1'] ) == intval ( $general_value [$id] ['index'] )) {
    $access = 1;
  } else {
    $access = - 1;
  }
  return $access;
}

/**
 * This function is use to know if an element can be viewed by date condition
 * 
 * @param array $elem_val
 * @param integer $id
 * @return number
 */
function dpistate_get_access_for_date_type($elem_val, $id) {
  $access = 0;
  $result = dpistate_api_get_one_state ( $id );
  switch ($elem_val ['operation_' . $id . '_2']) {
    case 'outside' :
      if (strtotime ( $result ['value'] [0] ) > time () || time () > strtotime ( $result ['value'] [1] )) {
        $access = 1;
      } else {
        $access = - 1;
      }
      break;
    case 'inside' :
      if (strtotime ( $result ['value'] [0] ) < time () && time () < strtotime ( $result ['value'] [1] )) {
        $access = 1;
      } else {
        $access = - 1;
      }
      break;
  }
  return $access;
}

/**
 * This function return a simple sentence to pin up the rules
 * 
 * @param array $conf
 * @param array $state
 * @return String
 */
function dpistate_status_ctools_access_summary($conf, $state) {
  return t ( 'Dpistate has defined rules' );
}

/**
 * This function return right form element necessary for the form
 *
 * @param array $data_list          
 * @param integer $v          
 * @param integer $k          
 * @return form element
 */
function dpistate_get_form_element_by_type($data_list, $v, $k, $conf, $name) {
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
          '#markup' => t ( 'Activate state : @state for date => @date', array (
            '@state' => $data_list [$k] ['label'],
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
