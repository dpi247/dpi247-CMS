<?php
/**
 * Implement the hook_form
 */
function dpisso_loginmanager_admin_settings_page_form(){
  // Get ssoClient config from file.
  $config = dpisso_get_config(); 
  $array_conf = (array) $config;

  // Build Settings Table
  $headers = array(t('Key'), t('Value'));
  $rows = array();
  foreach ($array_conf as $k=>$v) {
  	$rows[]= array($k,$v);
  }

  $form=array();
  
  /**
   * The unique identifier of the dpi instance in the all SSO processes & modules
   */
  $form['ssoClienInitFile']=array(
    '#type'=>'textfield',
    '#title'=>t('ssoClientInitFile'),
    '#description'=>t("ssoClient configuration file path."),
    '#default_value'=> dpi_variable_get('ssoClienInitFile',''),
  	'#required' => TRUE,
  	'#element_validate' => array('_ssoClienInitFile_element_validate'),
  );

  $form['values'] = array(
  		'#type' => 'fieldset',
  		'#title' => t('Current Config Values'),
  		'#collapsible' => TRUE,
  		'#collapsed' => FALSE,
  );
  
  $form['values']['config'] = array(
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows))
  );
  
  return dpi_system_settings_form($form);
}

/*
 * Validate the  ssoClienInitFile form element.
 * 
 */
function _ssoClienInitFile_element_validate($element, &$form_state, $form) {
	if(!is_file($element["#value"])) {
		form_error($element, t('File not found :').$element["#value"]);
	}
}
