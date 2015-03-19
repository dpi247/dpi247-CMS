<?php

/**
 * @author NMO
 */

/**
 * This function is callback from menu for render page
 *
 * @return string
 */
function dpiunivers_view_page() {

  $output = array ();
  $form = drupal_get_form ( 'dpiunivers_add_univers_form' );
  $output = drupal_render ( $form );
  $output .= dpiunivers_view_table ();
  return $output;
}

/**
 * This function is use to create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return multitype:array
 */
function dpiunivers_add_univers_form($form, &$form_state) {
  $form ['add'] = array (
    '#title' => t ( 'Add univers' ),
    '#type' => 'fieldset' 
  );
  
  $form ['add'] ['link'] = array (
    '#type' => 'link',
    '#title' => t ( 'Add' ),
    '#href' => 'admin/config/dpiunivers/settings/add' 
  );
  
  $form ['default'] = array (
    '#title' => t ( 'Choose a default value for dpiunivers' ),
    '#type' => 'fieldset' 
  );
  
  $univers = dpiunivers_api_get_all_universes();
  $labels = array();
  if(isset($univers)){
    foreach($univers as $univer){
      $labels[$univer["id"]] = $univer["label"];
    }
  }
  
  $form ['default'] ['select'] = array (
    '#type' => 'select',
    '#title' => t ( 'Select univers' ),
    "#options" => $labels,
    "#default_value" => variable_get("dpiunivers_default_univer", NULL),
  );
  
  $form ['default'] ["submit"] = array(    
    "#type" => "submit",
    "#value" => t('Submit'),
  );
  
  return $form;
}

function dpiunivers_add_univers_form_submit($form, & $form_state){
  if(isset($form_state["values"]["select"])){
    variable_set('dpiunivers_default_univer', $form_state["values"]["select"]);
    drupal_set_message("Settings saved");
  }else{
    drupal_set_message("No settings specified");
  }  
}

/**
 * This function generate table for view informations
 *
 * @return render(table)
 */
function dpiunivers_view_table() {
  $data_list = dpiunivers_api_get_all_universes();
 
  $keys = NULL;
  
  if (! empty ( $data_list )) {
    $keys = array_keys ( $data_list [0] );
  }
  
  $header = dpiunivers_generate_header();
  $rows = dpiunivers_generate_rows($data_list);
  
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

function dpiunivers_generate_header() {
	$header = array(
		array('data'=>'Name'),
		array('data'=>'Theme'),
		array('data'=>'Menu'),
		array('data'=>'Actions'),
	);
	
	return $header;	
}

/**
 * This function is use to generate rows for table
 * 
 * @param array $data_list
 */

function dpiunivers_generate_rows($data_list = array()) {
	$rows = array();
	
	if (sizeof($data_list)==0) { // record yet
		$rows[]=array(
			t('No record yet')
		);		
	} else {
		 foreach ( $data_list as $index => $data ) {
			$rows[]=array(
				$data['label'],
				$data['theme'],
				$data['menu'],
				dpiunivers_add_link_for_operation($data['id'])
			); 
		 }
	}	
	
	return $rows;
}


/**
 * This function is use to add operation link from table
 * 
 * @param integer $id
 * @return string
 */

function dpiunivers_add_link_for_operation($id) {
  $array_links = array(
  	l('Edit', 'admin/config/dpiunivers/settings/edit/' . $id ),
	l('Delete', 'admin/config/dpiunivers/settings/delete/' . $id )
  );
  
  return implode(' - ',$array_links);
}
