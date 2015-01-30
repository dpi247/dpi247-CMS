<?php

/**
 * Display form admin of the selected social network plugin
 *
 * @param string $network
 *   Name of the network
 * @param string $output
 *   Ouput of the admin
 *
 * @return Nothing
 */
function dpisocial_adminplugin($network) {
	
	$breadcrumb = array();
    $breadcrumb[] = l('Home', '<front>');
    $breadcrumb[] = l('Administration', 'admin');
    $breadcrumb[] = l('DPI247', 'admin/dpi247'); 
    $breadcrumb[] = l('DPI Social', 'admin/dpi247/dpisocial'); 
		
	ctools_include('plugins');
    $social_network = ctools_get_plugins('dpisocial', 'social_network');
	
	$class = false;
	$label = '';
	  
	  foreach ($social_network as $snetwork) {
		  if ($snetwork['type']==$network) {
			  	$class=$snetwork['handler']['class'];
				$label=$snetwork['label'];
				$breadcrumb[] = l($snetwork['label'], 'admin/config/dpisocial/'.$network); 
		  }
	  }

	  if ($class) {
		
		$form_state['class'] = $class;
		$form_state['label'] = $label;
		  
		$form = drupal_get_form('social_network_settings_form',$form_state);
		
		$output = drupal_render($form); 
		
		
	  } else {
		$output = 'Plugin Not Found';  
	  }	
	  
	  // Set Breadcrumbs
  		drupal_set_breadcrumb($breadcrumb);
	  
	  return $output;
}

function dpisocial_adminplugin_others() {

  $form['dpisocial_global_settings']['dpisocial_add_print_link'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add print link"),
    '#default_value' => dpi_variable_get('dpisocial_add_print_link', 0),
  );

  $form['dpisocial_global_settings']['dpisocial_add_email_link'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add email link"),
    '#default_value' => dpi_variable_get('dpisocial_add_email_link', 0),
  );

  $form['dpisocial_global_settings']['dpisocial_add_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail default subject'),
    '#default_value' => variable_get('dpisocial_add_email_subject', ''),
    '#size' => '50',
  );

  $form['dpisocial_global_settings']['dpisocial_add_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail default subject'),
    '#descritpion' => t('Use [node:url] to place node url'),
    '#default_value' => variable_get('dpisocial_add_email_body', ''),
  );
  return system_settings_form($form);
}

function dpisocial_listplugin() {
  $output = '';
  ctools_include('plugins');
  $social_network = ctools_get_plugins('dpisocial', 'social_network');
  
  $breadcrumb = array();
  $breadcrumb[] = l('Home', '<front>');
  $breadcrumb[] = l('Administration', 'admin');
  $breadcrumb[] = l('DPI247', 'admin/dpi247'); // Link to current URL
  $breadcrumb[] = l('DPI Social', 'admin/dpi247/dpisocial'); // Link to current URL

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);
 
	  foreach ($social_network as $snetwork) {
		  $output.='<a href="/admin/dpi247/dpisocial/'.$snetwork['type'].'">'.$snetwork['label'].'</a><br />';
	  }
  
  return $output;	
}
