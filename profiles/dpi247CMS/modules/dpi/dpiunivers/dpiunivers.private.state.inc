<?php

/**
 * Define default dpistate settings
 * @return array
 */
function dpiunivers_default_dpistate(){
  return array(
    'dpiunivers' => array(
      'callback' => "dpiunivers_get_current_univer",
      'label' => "Univers",
      'type' => "callback",
      'id' => 'dpiunivers',
      'value' => array(),
    ),
  );
}

/**
 * This function define element into the table settings
 * @param array $row
 * @param array $values
 * @return array|form
 */
function dpiunivers_dpistate_define_options_settings($row, $values){   
  switch($row["type"]){
    case 'callback' :           
        $return_form_part = array();
        $val = dpiunivers_get_current_univer();
        $return_form_part['info'] = array (
          '#markup' => 'current univer is ' . ((isset($val['label'])) ? $val['label'] : "undifined") ,          
        );
        return array($return_form_part);  
      break;
  }     
}

/**
 * This function return the value that we need to save
 * @param array $type
 * @param array $values
 * @return array
 */
function dpiunivers_dpistate_define_info_to_save($type, $values){
  switch($type){
    case 'callback' :                
        return array($values);
      break;
  }
}

/**
 * This function return the univers value from condition
 * @return array
 */
function dpiunivers_get_current_univer(){
  $univers = dpiunivers_api_get_all_universes();
  $return = "undefined";
  if(!empty($univers) && is_array($univers)){
    $default_value = variable_get('dpiunivers_default_univer', NULL);
    if(isset($default_value)){      
      $return = (array) dpiunivers_api_get_one_univers(intval($default_value));
    }else{
      $key = array_rand($univers);
      $return = $univers[$key];
    }
    drupal_alter('dpiunivers_alter_current_univers', $return, $univers);
  }
  return $return;
}
