<?php
/**
 * @file
 * This form allow to add or edit a univers
 * 
 * @author  Barthélemi Laurent <lba@audaxis.com>
 * @package DPI
 * @version  1.0
 */

/**
 * This function is a callback from menu to add state
 *
 * @param integer $id          
 * @return render(form)
 */
function dpiunivers_add_or_edit($id = NULL) {
  return drupal_get_form ( 'dpiunivers_add_form', $id );
}

/**
 * This function generate form to add state
 *
 * @param array $form          
 * @param array $form_state          
 * @param integer $id          
 * @return array
 */
function dpiunivers_add_form($form, &$form_state, $id) {
  $form = array ();
  
  if (isset ( $id )) {
    $form ['store'] = array (
      '#type' => 'value',
      '#value' => $id 
    );
    $univers = dpiunivers_api_get_one_univers ( $id );
    $text = "";
    
  }
  $form['dpiunivers_label'] = array (
    
    '#type' => 'textfield',
    '#title' => t ( 'Enter your univers\' name here' ),
    '#default_value' => (isset ( $univers->label )) ? $univers->label : "",
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE 
  );
  
  $themes = list_themes();
  
  $lthemes = array();
  foreach ($themes as $theme) {
		$lthemes[$theme->name] = $theme->name;  
  }
  
  $form['dpiunivers_theme'] = array (
  	'#type' => 'select',
	'#title' => t ( 'Select the associated theme' ),
    '#options' => $lthemes,
    '#default_value' => (isset ( $univers->theme )) ? $univers->theme : false,
  );
  
  $menus = menu_get_menus();
  
  $form['dpiunivers_menu'] = array (
  	'#type' => 'select',
	'#title' => t ( 'Select the associated menu' ),
    '#options' => $menus,
    '#default_value' => (isset ( $univers->menu )) ? $univers->menu : false,
  );
  
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  
  return $form;
}

/**
 * Process for updating or saving informations
 *
 * @param array $form          
 * @param array $form_state          
 */
function dpiunivers_add_form_submit($form, $form_state) {
  if (isset ( $form_state ['values'] ['store'] )) {
    if (dpiunivers_api_update_univers_by_id ( $form_state ['values'] ['dpiunivers_label'], $form_state ['values'] ['dpiunivers_theme'],  $form_state ['values'] ['dpiunivers_menu'], $form_state ['values'] ['store'] )) {
      drupal_set_message ( 'Data updated successfully' );
	  drupal_goto('admin/config/dpiunivers/settings');
    } else {
      drupal_set_message ( 'Error while updating data !', 'error' );
    }
  } else {
    if (dpiunivers_api_add_dpiunivers ( $form_state ['values'] ['dpiunivers_label'], $form_state ['values'] ['dpiunivers_theme'],  $form_state ['values'] ['dpiunivers_menu'] )) {
      drupal_set_message ( 'Data saved successfully' );
	  drupal_goto('admin/config/dpiunivers/settings');
    } else {
      drupal_set_message ( 'Error while saving data !', 'error' );
    }
  }
}