<?php

/**
 * @author NMO
 */

/**
 * This function is callback from menu for render page
 *
 * @return string
 */
function dpisocial_comments_view_page() {

  $output = array();
  $form = drupal_get_form('dpisocial_comments_form');
  $output = drupal_render($form);
  return $output;
}

/**
 * This function is use to create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return multitype:array
 */
function dpisocial_comments_form($form, &$form_state) {
  
  $socials = dpisocial_api_get_avaible_social_for_function('share url');
  
  sort($socials);
  
  $options = array();
  
  foreach ($socials as $social) {
	$options[$social] = t($social);  
  }
  
	$form['socials'] = array (
		'#type' => 'checkboxes',
		'#title' => t('Select on which network comments can be shared'),
		'#options' => $options,
		'#default_value' => dpisocial_api_get_selected_social_for_function('share comment')
	  );  
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  
  return $form;
}

function dpisocial_comments_form_submit($form, $form_state) {
	
	$socials = array();
	
	while(list($social,$value) = each($form_state['values']['socials'])) {
		if ($value!==0) $socials[] = $social;
	}
	
	variable_set('dpisocial_comments_share_networks', $socials);
}
