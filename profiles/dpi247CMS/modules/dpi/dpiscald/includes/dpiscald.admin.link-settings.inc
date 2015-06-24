<?php

/**
 * Implementation of Hook_form()
 * @param array $form
 * @param array $form_state
 * @return array
 */
function dpiscald_link_settings_form($form, $form_state){
  $form = array();

  $form ['dpiscald_link_pattern'] = array (
			'#type' => 'textarea',
			'#title' => t ( 'Access Manager Whitelist' ),
			'#description' => t ( " One url (or patern) per line. All theses urls will <strong>always</strong> be on free access." ),
			'#default_value' => implode('', variable_get('dpiscald_link_pattern', array())),
	);	
  
  $form ['submit'] = array (
	  '#type' => 'submit',
	  '#value' => t ( 'Save pattern' )
	);
  
  return $form;
}

function dpiscald_link_settings_form_submit($form, $form_state){
  if(isset($form_state['values']['dpiscald_link_pattern'])){
    try {
      $textAr = explode(PHP_EOL, $form_state['values']['dpiscald_link_pattern']);
      $array_sav = array();
      foreach ($textAr as $line) {
          $array_sav[] = $line;
      }
      variable_set('dpiscald_link_pattern', $array_sav);
      drupal_set_message(t('Data saved in variable'));
    }
    catch (Exception $exc) {
      drupal_set_message(t('An error occure during process'), 'error');
    }
  }else{
    drupal_set_message(t('No value into the textarea'), 'error');
  }
  
}