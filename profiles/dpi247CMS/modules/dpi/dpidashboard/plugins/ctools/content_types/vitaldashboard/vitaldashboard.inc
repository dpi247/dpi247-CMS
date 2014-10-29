<?php

$plugin = array(
  'title' => t('Vital parameters'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  'single' => TRUE,
  
  //'admin info' => 'viewsnode_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'vitaldashboard_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashsystem',
  
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  //'edit form' => 'comments_edit_form',
  
  
  'category' => t('Dashboard'),
  
  
  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
);

function vitaldashboard_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Vital dashboard ctools contenttype';
	$block->content = t('Display the vital parameters dashboard');
	return $block;
}


function vitaldashboard_content_type_render($subtype, $conf, $args, $context) {
	
  //drupal_add_css('https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css', 'external');
 // drupal_add_js('https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js');
  drupal_add_css(drupal_get_path('module', 'dpidashboard') . '/css/vitaldashboard.css');

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Vital dashboard';
  
  $block = new stdClass();
  $block->module = 'vitaldashboard';
  $block->delta = $subtype;
  $block->title = $title;
  
  $header = array(t('Title'), t('Status'), t('Description'), t('Path'));
  
  $dashboards = module_invoke_all('dpidashboard_status_item');
  
  $rows = array();
  
  foreach ($dashboards as $d) {
	$row = array(
		$d['title'],
		'<div class="dpidashboardStatus'.$d['status'].'"></div>',
		$d['message'],
		'<a href="'.$d['link'].'">'.t('Edit').'</a>'
	); 
	$rows[] = $row; 
  }
  
  $output = theme('table', array('header' => $header, 'rows' => $rows ));
  
  $block->content = $output;

  return $block;
  
}

