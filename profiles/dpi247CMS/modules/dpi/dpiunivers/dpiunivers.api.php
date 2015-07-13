<?php

/**
 * This function get all universes present into db
 * @return array OR false
 */
function dpiunivers_api_get_all_universes() {
  try {
    $return = array ();
    $query = db_select ( 'dpi_univers', 'du' )->fields ( 'du', array (
      'label',
      'theme',
      'menu',
      'id' 
    ) )->execute ();    
    while( $row = $query->fetchAssoc () ) {     
      $return [] = $row;
    }    
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function get specific univers by id
 * @param integer $id
 * @return array OR false
 */
function dpiunivers_api_get_one_univers($id) {
  try {
    $return = array ();
    $query = db_select ( 'dpi_univers', 'du' )->condition ( 'id', $id, '=' )->fields ( 'du', array (
      'label',
      'theme',
      'menu',
      'id' 
    ) )->execute ();
    while( $row = $query->fetchObject () ) {
      $return = $row;
    }
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function add a univers into db
 * @param string $label
 * @param string $theme
 * @param string $menu
 * @return boolean
 */
function dpiunivers_api_add_dpiunivers($label, $theme, $menu) {
	try {
    $result = db_insert ( 'dpi_univers' )->fields ( array (
      'label' => $label,
      'theme' => $theme,
      'menu' => $menu
    ) )->execute ();
	
	return $result;
  } catch ( Exception $e ) {
    return FALSE;
  }	
}

/**
 * This function update an existing univers into db from id
 * @param string $label
 * @param string $theme
 * @param string $menu
 * @param integer $id
 * @return boolean
 */
function dpiunivers_api_update_univers_by_id($label, $theme, $menu, $id) {
  try {
    $result = db_update ( 'dpi_univers' )->fields ( array (
      'label' => $label,
      'theme' => $theme,
      'menu' => $menu
    ) )->condition ( 'id', $id, '=' )->execute ();
    return TRUE;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function delete specific univers by id
 * @param integer $id
 * @return array OR false
 */
function dpiunivers_api_delete_univers_by_id($id) {
  try {
    $result = db_delete( 'dpi_univers' )->condition( 'id', $id, '=' )->execute();
    return $result;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * Get informations just for univers from dpistate database
 */
function dpiunivers_api_get_univers(){
  module_load_include('php', 'dpistate', 'dpistate.api');
  $all_states = dpistate_api_get_all_states();
  $all_values = variable_get("dpistate_value_at_moment", array());
  $univers = array();
  foreach($all_states as $state){
    if($state['type'] == 'univers'){
      $state["value"] = (isset($all_values[$state["id"]]))? $all_values[$state["id"]] : NULL;
      $univers[] = $state;
    }
  }
  return $univers;
}