<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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

function dpiunivers_dpistate_define_options_settings($row, $values){   
  switch($row["type"]){
    case 'callback' :           
        $return_form_part = array();
        $return_form_part['info'] = array (
          '#markup' => 'current univer is ' .  dpiunivers_get_current_univer(),          
        );
        return array($return_form_part);  
      break;
  }     
}

function dpiunivers_dpistate_define_info_to_save($type, $values){
  switch($type){
    case 'callback' :                
        return array($values);
      break;
  }
}

function dpiunivers_get_current_univer(){
  return "undifined";
}
