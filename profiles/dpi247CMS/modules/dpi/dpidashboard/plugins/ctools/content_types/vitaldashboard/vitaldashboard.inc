<?php

$plugin = array(
  'title' => t('Vital parameters'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  'defaults' => array(),
  'single' => TRUE,
  'render callback' => 'vitaldashboard_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashsystem',
  'category' => t('Dashboard'),
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
);

function vitaldashboard_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Vital dashboard ctools contenttype';
	$block->content = t('Display the vital parameters dashboard');
	return $block;
}


function vitaldashboard_content_type_render($subtype, $conf, $args, $context) {
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

