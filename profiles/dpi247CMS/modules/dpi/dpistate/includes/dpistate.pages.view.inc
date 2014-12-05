<?php

/**
 * @author GVD - LBA
 */

/**
 * This function is callback from menu for render page
 *
 * @return string
 */
function dpistate_view_page() {

  $output = array ();
  $form = drupal_get_form ( 'dpistate_add_state_form' );
  $output = drupal_render ( $form );
  $output .= dpistate_view_table ();
  return $output;
}

/**
 * This function is use to create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return multitype:array
 */
function dpistate_add_state_form($form, &$form_state) {
  $form ['add'] = array (
    '#title' => t ( 'Add state' ),
    '#type' => 'fieldset' 
  );
  
  $form ['add'] ['link'] = array (
    '#type' => 'link',
    '#title' => t ( 'Add' ),
    '#href' => 'admin/config/dpistate/settings/add' 
  );
  
  return $form;
}

/**
 * This function generate table for view informations
 *
 * @return render(table)
 */
function dpistate_view_table() {
  $data_list = dpistate_api_get_all_states();
 
  $keys = NULL;
  
  if (! empty ( $data_list )) {
    $keys = array_keys ( $data_list [0] );
  }
  
  $header = dpistate_generate_header();
  $rows = dpistate_generate_rows($data_list);
  
  $variables = array (
    'header' => $header,
    //'rows' => $data_list,
	'rows' => $rows,
    'attributes' => array (),
    'caption' => NULL,
    'colgroups' => array (),
    'sticky' => FALSE,
    'empty' => "no value" 
  );
  
  return theme_table ( $variables );
}

/**
 * This function is use to generate header for table 
 * 
 * @return multiple:array
 */

function dpistate_generate_header() {
	$header = array(
		array('data'=>'Name'),
		array('data'=>'Type'),
		array('data'=>'Values'),
		array('data'=>'Actions'),
	);
	
	return $header;	
}

/**
 * This function is use to generate rows for table
 * 
 * @param array $data_list
 */

function dpistate_generate_rows($data_list = array()) {
	$rows = array();
	
	if (sizeof($data_list)==0) { // record yet
		$rows[]=array(
			t('No record yet')
		);		
	} else {
		 foreach ( $data_list as $index => $data ) {
			$rows[]=array(
				$data['label'],
				dpistate_replace_state_type_from_int_to_string($data['type']),
				dpistate_values_to_string($data['type'], $data['value']),
				dpistate_add_link_for_operation($data['id'])
			); 
		 }
	}	
	
	return $rows;
}

/**
 * This function is use to replace type from int to string
 * 
 * @param string $type
 * @return string 
 */

function dpistate_replace_state_type_from_int_to_string($type) {
	switch ($type) {
		case 0 :
		  return "Numeric";
		  break;
		case 1 :
		  return "Text";
		  break;
		case 2 :
		  return "Date";
		  break;
  }
}

/**
 * This function is use to add operation link from table
 * 
 * @param integer $id
 * @return string
 */

function dpistate_add_link_for_operation($id) {
  $array_links = array(
  	l('Edit', 'admin/config/dpistate/settings/edit/' . $id ),
	l('Delete', 'admin/config/dpistate/settings/delete/' . $id )
  );
  
  return implode(' - ',$array_links);
}

/**
 * This is use to replace date into the right format
 * 
 * @param array $data_list
 * @param integer $v
 * @param integer $k
 */
function dpistate_change_date_format(& $data_list, $v, $k) {
  if (is_array ( $v ['value'] )) {
    $text = "";
    foreach ( $v ['value'] as $str ) {
      // if date then modify date format from y-m-d to d-m-y
      if ($v ['type'] == 2) {
        $str = date ( "d-m-Y H:i", strtotime ( $str ) );
      }
      $text .= $str . ' - ';
    }
    $text = substr ( $text, 0, - 3 );
    $data_list [$k] ['value'] = $text;
    $text = "";
  }
}

function dpistate_values_to_string($type, $values) {
	switch($type) {
		case 0:
			return '0 '.t('to').' '.$values;
			break;	
		case 1:
			return implode(' | ',$values);
			break;
		case 2:
			return t('From').date(' d/m/y H:i ',strtotime($values[0])).t('to').date(' d/m/y H:i',strtotime($values[1]));
	}
}