<?php

/**
 * This function get all context present into db
 * 
 * @return array OR false
 */
function dpicontext_api_get_all_context() {
  try {
    $return = array ();
    $query = db_select ( 'dpi_context', 'dc' )->fields ( 'dc', array (
      'label',
      'type',
      'value',
      'id' 
    ) )->execute ();
    
    while( $row = $query->fetchAssoc () ) {
      $row ['value'] = unserialize ( $row ['value'] );
      $return [] = $row;
    }
    
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function return one context by id
 *
 * @param integer $id          
 * @return array OR false
 */
function dpicontext_api_get_one_context($id) {
  try {
    $return = array ();
    $query = db_select ( 'dpi_context', 'dc' )->condition ( 'id', $id, '=' )->fields ( 'dc', array (
      'label',
      'type',
      'value',
      'id' 
    ) )->execute ();
    while( $row = $query->fetchAssoc () ) {
      $row ['value'] = unserialize ( $row ['value'] );
      $return = $row;
    }
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function delete one context from db by id
 *
 * @param integer $id          
 * @return Array or False
 */
function dpicontext_api_delete_context_by_id($id) {
  try {
    $result = db_delete ( 'dpi_context' )->condition ( 'id', $id, '=' )->execute ();
    if(($val=dpi_variable_get('dpicontext_value_at_moment', FALSE))!=FALSE && isset($val[$id])){
      unset($val[$id]);
      dpi_variable_set('dpicontext_value_at_moment', $val);
    }
    return $result;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function update information for $condition_val condition
 *
 * @param String $label          
 * @param String $type          
 * @param Array $unserialize_value          
 * @param Integer $condition_val          
 * @return boolean
 */
function dpicontext_api_update_context_by_id($label, $type, $unserialize_value, $id) {
  try {
    $result = db_update ( 'dpi_context' )->fields ( array (
      'label' => $label,
      'type' => $type,
      'value' => serialize ( $unserialize_value ) 
    ) )->condition ( 'id', $id, '=' )->execute ();
    return TRUE;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function add a context to the db with value from parameter
 *
 * @param String $label          
 * @param String $type          
 * @param Array $unserialize_value          
 * @return boolean
 */
function dpicontext_api_add_context($label, $type, $unserialize_value) {
  try {
    $result = db_insert ( 'dpi_context' )->fields ( array (
      'label' => $label,
      'type' => $type,
      'value' => serialize ( $unserialize_value ) 
    ) )->execute ();
    return TRUE;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

/**
 * This function create right render for the date
 * Modify date format from y-m-d to d-m-y
 *
 * @param array $data_list
 * @param integer $v
 * @param integer $k
 */
function dpicontext_api_change_date_format(&$data_list, $v, $k) {
  $text = "";
  foreach ( $v ['value'] as $str ) {
    $str = date ( "d-m-Y H:i", strtotime ( $str ) );
    $text .= $str . ' - ';
  }
  $text = substr ( $text, 0, - 3 );
  $data_list [$k] ['value'] = $text;
}
