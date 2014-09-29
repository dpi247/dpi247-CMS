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
	
  drupal_add_css('https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css', 'external');
  //drupal_add_css('https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap-theme.min.css', 'external');
  drupal_add_js('https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js');

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Number of views';
  
  $block = new stdClass();
  $block->module = 'viewsnode';
  $block->delta = $subtype;
  $block->title = $title;
  
  $header = array(t('title'), t('status'), t('description'), t('link'));
  $rows = array(
  	array('11','<span class="glyphicon glyphicon-ok-sign"></span>','13','14'),
	array('21','<span class="glyphicon glyphicon-remove-sign"></span>','23','24'),
	array('31','<span class="glyphicon glyphicon-info-sign"></span>','33','34'),
	array('41','<span class="glyphicon glyphicon-question-sign"></span>','43','44'),
  );
  
  $output = theme('table', array('header' => $header, 'rows' => $rows ));
  
  $block->content = $output;

  return $block;
  
}

