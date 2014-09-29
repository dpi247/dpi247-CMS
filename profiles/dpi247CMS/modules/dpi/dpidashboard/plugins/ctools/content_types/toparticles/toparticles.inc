<?php

$plugin = array(
  'title' => t('Top articles'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  
  //'admin info' => 'comments_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'toparticles_content_type_render',
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


function toparticles_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Top articles on a given period of time';
	$block->content = t('Display the top articles on a selected period for current node');
	return $block;
}

function toparticles_content_type_render($subtype, $conf, $args, $context) {
	

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Top articles';
  
  $block = new stdClass();
  $block->module = 'toparticles';
  $block->delta = $subtype;
  $block->title = $title;
  
  $values = array(
  	'select'	=> '1d',
  );
  
  $output = _toparticles_get_form($conf, $values);
  
  $block->content = $output;

  return $block;
  
}


function _toparticles_get_form($conf, $values){
  
  $form_state['conf']=$conf;
  $form_state['values'] = $values;
  return drupal_get_form("_toparticles_form_render_form",$form_state);
  
  
}
