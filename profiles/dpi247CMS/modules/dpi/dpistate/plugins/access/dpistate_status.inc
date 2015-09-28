<?php
/**
 * @file
 * Access plugin to test element state.
 * 
 * @author  Barthélemi Laurent <lba@audaxis.com>
 * @package DPI
 * @version  1.0
 */
$plugin = array (
  'title' => t ( "DpiState: State" ),
  'description' => t ( 'Control access by dpistate status argument.' ),
  'callback' => 'dpistate_status_ctools_access_check',
  'settings form' => 'dpistate_status_ctools_access_settings',
  'settings form submit' => 'dpistate_status_ctools_access_settings_submit',
  'summary' => 'dpistate_status_ctools_access_summary',
  'get child' => 'dpistate_dpistate_status_ctools_access_get_child',
  'get children' => 'dpistate_dpistate_status_ctools_access_get_children',
);

/**
 * This function return one child for access plugins
 * @param array $plugin
 * @param array $parent
 * @param array $child
 * @return array
 */
function dpistate_dpistate_status_ctools_access_get_child($plugin, $parent, $child){
  $plugins = dpistate_dpistate_status_ctools_access_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

/**
 * This function return all children for the plugin access
 * @param array $plugin
 * @param array $parent
 * @return array
 */
function dpistate_dpistate_status_ctools_access_get_children($plugin, $parent){
  module_load_include ( '../../../dpistate/plugins/access/php', 'dpistate', 'dpistate.api' );
  $plugins = array();
  $data_list = dpistate_api_get_all_states();
  foreach($data_list as $data){
    $machine_name = preg_replace('@[^a-z0-9-]+@','_', strtolower($data['label']));
    $plugins[$machine_name] = array(
      'title' => t ( "DpiState : ".$data['label'] ),
      'description' => t ( 'Control access by dpistate status argument.' ),
      'keyword' => 'dpistate_status',
      'name' => $parent,
    );
  }
  return $plugins;
}

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
  if(isset($conf) && !empty($conf) && isset($conf['type'])){
    $form_state['input']['type'] = $conf['type'];
  }
  $data_list = dpistate_api_get_all_states();
  $rows = array ();
  $data = NULL;
  foreach($data_list as $data){
    $machine_name = preg_replace('@[^a-z0-9-]+@','_', strtolower($data['label']));
    if($machine_name == $form_state['input']['type']){
      $tmp_data_list = array($data);
      break;
    }
  }

  if(isset($tmp_data_list)){
    foreach ( $tmp_data_list as $k => $v ) {
      if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
        dpistate_api_change_date_format ( $tmp_data_list, $v, $k );
      }
      $info_form = dpistate_get_form_element_by_type($v, $conf, 'row_' . $v ['id'], $tmp_data_list);
    }

    $form ['type'] = array('#type' => 'hidden', '#value' => $form_state['input']['type']);
    $form ['settings'] = array(
      '#tree' => TRUE,
    );
    foreach ( $info_form as $value_form => $val ) {      
      $form ['settings'] [$value_form] = $val;
    }
  }
  return $form;
}

/**
 * This function replace type of element into the right part of the form at the submition
 * @param array $form
 * @param array $form_state
 */
function dpistate_status_ctools_access_settings_submit($form, &$form_state){
  $form_state['values']['settings']['type'] = $form_state['values']['type'];
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
  
  /* Define id for result search */
  $array_key = array_keys($conf);
  $id = explode ( '_', current($array_key) );
  $id = $id [1];
  
  /* to know type of test that you need to do */
  $data_list = dpistate_api_get_all_states();
  foreach($data_list as $data){
    $machine_name = preg_replace('@[^a-z0-9-]+@','_', strtolower($data['label']));
    if($machine_name == $conf['type']){
      $tmp_data_list = array($data);
      break;
    }
  }

  if(isset($tmp_data_list)){
    $tmp_data_list = current($tmp_data_list);
    switch ($tmp_data_list["type"]){
      case '0':
          //test numeric
          if (($var = dpistate_get_access_for_numeric_type ( $general_value, $id, $conf )) == - 1) {
            return FALSE;
          } elseif ($var == 1) {
            $access = TRUE;
          }
          break;
        break;
      case '1':
          //test text
          if (($var = dpistate_get_access_for_text_type ( $general_value, $conf, $id )) == - 1) {
            return FALSE;
          } elseif ($var == 1) {
            $access = TRUE;
          }
        break;
      case '2':
          //test date
          if(($var = dpistate_get_access_for_date_type($general_value, $conf, $id)) == -1){
            return FALSE;
          }elseif ($var == 1){
            $access = TRUE;
          }
        break;
      default:
          $access = current(module_invoke_all('dpistate_get_access_info',$tmp_data_list, $general_value[$id], $conf));
        break;
    }        
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
function dpistate_get_access_for_date_type($general_value, $elem_val, $id) {
  $access = 0;
  switch ($elem_val ['operation_' . $id . '_2']) {
    case 'outside' :
      if (strtotime ( $general_value [$id] ['value'] ["start"] ) > time () || time () > strtotime ( $general_value [$id] ['value'] ["end"] )) {
        $access = 1;
      } else {
        $access = - 1;
      }
      break;
    case 'inside' :
      if (strtotime ( $general_value [$id] ['value'] ["start"] ) < time () && time () < strtotime ( $general_value [$id] ['value'] ["end"] )) {
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
  $tmp_conf = $conf;
  $str = "Dpistate = type : " . $tmp_conf["type"] . ", ";
  unset($tmp_conf["type"]);  
  foreach($tmp_conf as $k => $v){
    $tmp_k = explode('_', $k);    
    $str .= $tmp_k[0]." : " . $v . ", ";
  }
  return t ( '@str', array( '@str' => substr($str, 0, -2) ) );
}

/**
 * This function return right form element necessary for the form
 * @param array $v
 * @param integer $conf
 * @param integer $name
 * @return form element
 */
function dpistate_get_form_element_by_type($v, $conf, $name, $data) {
  $return = NULL;
  $general_value = variable_get ( 'dpistate_value_at_moment', NULL );
  switch ($v ['type']) {
    case '0' :
      drupal_add_css(drupal_get_path("module", "dpistate")."/css/dpistate_settings_form.css");      
      $return = array (
        'label_' . $v ['id'] . '_0' => array (
          '#markup' => $v ['label']." (Actual value is ".((isset($general_value[$v["id"]]["value"]))? $general_value[$v["id"]]["value"] : "undifined" ).")",
        ),
        'operation_' . $v ['id'] . '_0' => array (
          '#type' => 'select',
          '#options' => array (
            '=' => '=',
            '<' => '<',
            '>' => '>' 
          ),
          '#default_value' => (isset ( $conf ['operation_' . $v ['id'] . '_0'] )) ? $conf ['operation_' . $v ['id'] . '_0'] : '=',
          '#attributes' => array(
            'class' => array('inline-dpistate'),
          ),
        ), 
        'options_' . $v ['id'] . '_0' => array (
          '#type' => 'select',
          '#options' => range ( 0, $v ['value'] ),
          '#default_value' => (isset ( $conf ['options_' . $v ['id'] . '_0'] )) ? $conf ['options_' . $v ['id'] . '_0'] : 0,
          '#attributes' => array(
            'class' => array('aa-dpistate'), 
          ),
        ),
        
      );
      break;
    case '1' :
      $return = array (
        'label_' . $v ['id'] . '_1' => array (
          '#markup' => $v ['label']. " : Actual value is '". ((isset($general_value[$v["id"]]["value"]))? $general_value[$v["id"]]["value"] : "undifined" )."'",
        ),
        'options_' . $v ['id'] . '_1' => array (
          '#type' => 'select',
          '#description' => t('the selected value must be worth the present value if you want that the block is displayed'),
          '#options' => $v ['value'],
          '#default_value' => (isset ( $conf ['options_' . $v ['id'] . '_1'] )) ? $conf ['options_' . $v ['id'] . '_1'] : '' 
        ),
      );
      break;
    case '2' :
      $return = array (
        'options_' . $v ['id'] . '_2' => array (
          '#markup' => t ( '@label : Actual values is for start date @start and for end date @end', array (
            '@label' => $v ['label'],
            '@start' => ((isset($general_value[$v["id"]]["value"]["start"]))? $general_value[$v["id"]]["value"]["start"] : "undifined" ),
            '@end' => ((isset($general_value[$v["id"]]["value"]["end"]))? $general_value[$v["id"]]["value"]["end"] : "undifined" ),
          ) ) 
        ),
        'operation_' . $v ['id'] . '_2' => array (
          '#type' => 'select',
          "#description" => t('You can choose inside or outside for the date element. It\'s test with actual date.'),
          '#options' => array (
            'inside' => 'inside',
            'outside' => 'outside' 
          ),
          '#default_value' => (isset ( $conf ['operation_' . $v ['id'] . '_2'] )) ? $conf ['operation_' . $v ['id'] . '_2'] : 'inside' 
        ) 
      );
      break;
    default:
        $return = module_invoke_all('dpistate_define_access_plugins_form', $v, $general_value, $conf);
      break;
  }
  return $return;
}
