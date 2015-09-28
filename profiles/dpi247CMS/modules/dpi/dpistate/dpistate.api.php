<?php
/**
 * @file
 * Function available for external user.
 * 
 * @author  Barthélemi Laurent <lba@audaxis.com>
 * @package DPI
 * @version  1.0
 */

/**
 * This function get all states present into db
 * 
 * @return array OR false
 */
function dpistate_api_get_all_states() {
  try {
    $return = array ();
    $query = db_select ( 'dpi_state', 'ds' )->fields ( 'ds', array (
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
 * This function return one state by id
 *
 * @param integer $id          
 * @return array OR false
 */
function dpistate_api_get_one_state($id) {
  try {
    $return = array ();
    $query = db_select ( 'dpi_state', 'ds' )->condition ( 'id', $id, '=' )->fields ( 'ds', array (
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
 * This function delete one state from db by id
 *
 * @param integer $id          
 * @return Array or False
 */
function dpistate_api_delete_state_by_id($id) {
  try {
    $result = db_delete ( 'dpi_state' )->condition ( 'id', $id, '=' )->execute ();
    if (($val = dpi_variable_get ( 'dpistate_value_at_moment', FALSE )) != FALSE && isset ( $val [$id] )) {
      unset ( $val [$id] );
      dpi_variable_set ( 'dpistate_value_at_moment', $val );
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
function dpistate_api_update_state_by_id($label, $type, $unserialize_value, $id) {
  try {
    $result = db_update ( 'dpi_state' )->fields ( array (
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
 * This function add a state to the db with value from parameter
 *
 * @param String $label          
 * @param String $type          
 * @param Array $unserialize_value          
 * @return boolean
 */
function dpistate_api_add_state($label, $type, $unserialize_value) {
  try {
    $result = db_insert ( 'dpi_state' )->fields ( array (
      'label' => $label,
      'type' => $type,
      'value' => serialize ( $unserialize_value ) 
    ) )->execute ();
	if ($result) { // set default values
		$val_recorded = variable_get(VARIABLE_NAME, array());
		switch($type) {
			case 0:
				$dvalue = 0;
				break;
			case 1:
				$dvalue = $unserialize_value[0];
				break;
			case 2:
				$dvalue = 'Valeur date';
				break;
		}
		$val_recorded[$result] = array(
			'actif'	=> TRUE,
			'value' => $dvalue
		);
		variable_set(VARIABLE_NAME, $val_recorded);
	}
	
	return $result;
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
function dpistate_api_change_date_format(&$data_list, $v, $k) {
  $text = "";
  foreach ( $v ['value'] as $str ) {
    $str = date ( "d-m-Y H:i", strtotime ( $str ) );
    $text .= $str . ' - ';
  }
  $text = substr ( $text, 0, - 3 );
  $data_list [$k] ['value'] = $text;
}

/**
 * This function return all actives states
 * 
 *
 * @return array $dpistate_states_values
 *     key string $state 
 *     value multi  $value
 *       BOOL FALSE = type date inactive
 *       BOOL TRUE  = type date active
 *       string = type string value selected               
 */
function dpistate_api_get_values() {
	$dpistate_states_values = &drupal_static(__FUNCTION__);
	if (!isset($dpistate_states_values)) {
		$states = dpistate_api_get_all_states();
		$values = variable_get(VARIABLE_NAME, array());
		
		$dpistate_states_values = array();
		
		foreach($states as $state) {
			if (!$values[$state['id']]['actif']) continue; // not active
			if ($state['type']==2) { // check for dates
				$now = date('Y-m-d H:i:s');
				if (($state['value'][0]<=$now) && ($state['value'][1]>=$now)) $inDate = TRUE;
				else $inDate = FALSE;
				$dpistate_states_values[$state['label']] = $inDate;
			} else {
				$dpistate_states_values[$state['label']] = $values[$state['id']]['value'];	
			}
		}
	}
	
	return $dpistate_states_values;
}

/**
 * This function return value for given state
 * 
 *
 * @return multi  $value
 *       NULL = state no exists or inactive
 *       BOOL FALSE = type date inactive
 *       BOOL TRUE  = type date active
 *       string = type string value selected               
 */
function dpistate_api_get_value($state) {
	$states = dpistate_api_get_values();
	if (!array_key_exists($state, $states)) return NULL;
	else return $states[$state];
}