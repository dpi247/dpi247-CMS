<?php
$plugin = array(
  'title' => t('Number of views of current section'),
  'description' => t('Simple example of a ctools content type with settings .'),
  'defaults' => array(),
  'single' => TRUE,
  'admin info' => 'viewssection_admin_info',
  'render callback' => 'viewssection_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashboardsection',
  'category' => t('Dashboard'),
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
);

function viewssection_admin_info($subtype, $conf, $context = NULL) {
  	$block = new stdClass;
	$block->title = 'Number of views ctools contenttype';
	$block->content = t('Display the number of views on a selected period for current node');
	return $block;
}


function viewssection_content_type_render($subtype, $conf, $args, $context) {
  $section = $context['argument_term_1']->data;
  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';

  $block = new stdClass();
  $block->module = 'viewssection';
  $block->delta = $subtype;
  $block->title = t('Number of views');
  
  $values = array(
  	'select'	=> '1d',
	'tid'		=> $section->tid
  );
  
  $output = _viewssection_get_form($conf, $values);
  
  $block->content = $output;

  return $block;
}


function _viewssection_get_form($conf, $values){
  $form_state['conf']=$conf;
  $form_state['values'] = $values;
  return drupal_get_form("_viewssection_form_render_form",$form_state);
}
