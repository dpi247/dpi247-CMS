<?php

$plugin = array(
  'title' => t('Number of views of current node'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  'single' => TRUE,
  
  'admin info' => 'viewsnode_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'viewsnode_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashboardnode',
  
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  //'edit form' => 'comments_edit_form',
  
  
  'category' => t('Dashboard'),
  
  
  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
);

function viewsnode_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Number of views ctools contenttype';
	$block->content = t('Display the number of views on a selected period for current node');
	return $block;
}


function viewsnode_content_type_render($subtype, $conf, $args, $context) {
	
  $node = $context['argument_entity_id:node_1']->data;

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Number of views';
  
  $block = new stdClass();
  $block->module = 'viewsnode';
  $block->delta = $subtype;
  $block->title = $title;
  
  $values = array(
  	'select'	=> '1d',
	'nid'		=> $node->nid
  );
  
  $output = _viewsnode_get_form($conf, $values);
  
  $block->content = $output;

  return $block;
  
}


function _viewsnode_get_form($conf, $values){
  
  $form_state['conf']=$conf;
  $form_state['values'] = $values;
  return drupal_get_form("_viewsnode_form_render_form",$form_state);
  
  
}
