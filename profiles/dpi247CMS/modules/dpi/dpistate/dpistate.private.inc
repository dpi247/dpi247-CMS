<?php

/**
 * This function define a new type of dpistate
 * @return array
 */
function dpistate_dpistate_get_options(){
  return array(
     'weekdays' => t('Days of the week')
  );
}

/**
 * Return form elements for the type of dpistate
 * @param array $type
 * @param array $state
 * @return array
 */
function dpistate_dpistate_get_form_element($type, $state){
  switch($type){
    case 'weekdays':
        return array(
          '#markup' => t('Element is defined into the settings element'),
        );
      break;
  }  
}

/**
 * This function return string present into the view table in dpistate menu
 * @param array $type
 * @param array $values
 * @return array
 */
function dpistate_dpistate_get_value_table_view($type, $values){
  switch ($type) {
    case 'weekdays':
       return t('Represente un état pour les jours de la semaine');
      break;
    default:
      break;
  }
}

/**
 * Return form for dpistate settings
 * @param array $row
 * @param array $values
 * @return array
 */
function dpistate_dpistate_define_options_settings($row, $values){  
  switch($row["type"]){
    case 'weekdays' :    
        $return_form_part = array();
        
        $return_form_part['daysofweek_days'] = array (
          '#type' => 'select',
          '#title' => t ( 'Select the associated theme' ),
          '#options' => array(
            t('Lundi'),
            t('Mardi'),
            t('Mercredi'),
            t('Jeudi'),
            t('Vendredi'),
            t('Samedi'),
            t('Dimanche'),
          ),
          '#default_value' => (isset ( $values ['daysofweek_days'] )) ? $values ['daysofweek_days'] : NULL,
        );
        
        return array($return_form_part);     
      break;
  }  
}

/**
 * Return value from the dpistate form to be saved
 * @param String $type
 * @param array $values
 * @return array
 */
function dpistate_dpistate_define_info_to_save($type, $values){  
  switch($type){
    case 'weekdays' :                
        return array($values);
      break;
  }
}

/**
 * This function return label, operation, options for the current dpistate (present when you add access rules)
 * @param array $v
 * @param array $general_value
 * @param array $conf
 * @return array
 */
function dpistate_dpistate_define_access_plugins_form($v, $general_value, $conf){
  switch($v["type"]){
    case "weekdays":
      return array (
        'label_' . $v ['id'] . '_' . $v['type'] => array (
          '#markup' => $v ['label']." (Actual value is ".((isset($general_value[$v["id"]]["daysofweek_days"]))? $general_value[$v["id"]]["daysofweek_days"] : "undifined" ).")",
        ),
        'operation_' . $v ['id'] . '_' . $v['type'] => array (
          '#type' => 'select',
          '#options' => array (
            '=' => '=',
            '<' => '<',
            '>' => '>' 
          ),
          '#default_value' => (isset ( $conf ['operation_' . $v ['id'] . '_' . $v['type']] )) ? $conf ['operation_' . $v ['id'] . '_' . $v['type']] : '=',
          '#attributes' => array(
            'class' => array('inline-dpistate'),
          ),
        ), 
        'options_' . $v ['id'] . '_' . $v['type'] => array (
          '#type' => 'select',
          '#options' => array(
            t('Lundi'),
            t('Mardi'),
            t('Mercredi'),
            t('Jeudi'),
            t('Vendredi'),
            t('Samedi'),
            t('Dimanche'),
          ),
          '#default_value' => (isset ( $conf ['options_' . $v ['id'] . '_' . $v['type']] )) ? $conf ['options_' . $v ['id'] . '_' . $v['type']] : 0,
          '#attributes' => array(
            'class' => array('aa-dpistate'), 
          ),
        ),        
      );
  }
}

/**
 * This function return the boolean access for the current element
 * @param array $v
 * @param array $general_value
 * @param array $conf
 * @return int
 */
function dpistate_dpistate_get_access_info($v, $general_value, $conf){  
  switch($v["type"]){
    case "weekdays":
        $id = $v["id"];
        $access = 0;
        switch ($conf ['operation_' . $id . '_weekdays']) {
          case '=' :
            if (intval ( $conf ['options_' . $id . '_weekdays'] ) == intval ( $general_value ['daysofweek_days'] )) {
              $access = 1;
            }
            break;
          case '>' :
            if (intval ( $conf ['options_' . $id . '_weekdays'] ) < intval ( $general_value ['daysofweek_days'] )) {
              $access = 1;
            }
            break;
          case '<' :
            if (intval ( $conf ['options_' . $id . '_weekdays'] ) > intval ( $general_value ['daysofweek_days'] )) {
              $access = 1;
            }
            break;
        }
        return $access;
      break;
  }
}