<?php

function dpiscald_form_group_form($form){
  	$form['dpiscald'] = array(
  	    '#type' => 'select',
  	    '#title' => t('Vimeo'),
  	);
  	
  	
    $form ['select'] = array (
        '#type' => 'submit',
        '#value' => t ( 'select' ) 
    );
    $form ['cancel'] = array (
        '#type' => 'submit',
        '#value' => t ( 'Cancel' ) 
    );
  	$form['#redirect'] = "admin/wally/wallyscheduler/dashboard";
  	 
  	return system_settings_form($form);
}

function dpiscald_form_group_form_submit($form, &$form_state){

}