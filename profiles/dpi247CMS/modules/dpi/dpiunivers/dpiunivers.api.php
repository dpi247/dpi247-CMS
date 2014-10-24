<?php

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

function dpiunivers_api_get_one_univers($id) {
  try {
    $return = array ();
    $query = db_select ( 'dpi_univers', 'du' )->condition ( 'id', $id, '=' )->fields ( 'du', array (
      'label',
      'theme',
      'menu',
      'id' 
    ) )->execute ();
    while( $row = $query->fetchAssoc () ) {
      $return = $row;
    }
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

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