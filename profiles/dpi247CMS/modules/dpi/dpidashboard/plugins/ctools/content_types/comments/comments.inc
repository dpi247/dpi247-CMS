<?php

$plugin = array(
  'title' => t('Informations about comments'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  
  'admin info' => 'comments_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'comments_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashboardnode',
  
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  'edit form' => 'comments_edit_form',
  
  
  'category' => t('Dashboard'),
  
  
  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
);

function comments_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Infos on comments ctools contenttype';
	$block->content = t('Display various informations about the comments');
	return $block;
}

function comments_edit_form($form, &$form_state) {
 
 $conf=$form_state['conf'];
 $args=$form_state['args'];
 $contexts=$form_state['contexts'];
 
 $form['nb']=array(
   '#type'=>'checkbox',
   '#title'=>'Display number of comments',
   '#Description'=>t('Display number of comments'),
   "#default_value"=>$conf['nb'],
   
   
   );

 $form["options"]=array("#tree"=>TRUE);
 

 $form['options']['tops']=array(
   '#type'=>'checkbox',
   '#title'=>'Top comments',
   '#Description'=>t('Display top comments'),
   "#default_value"=>$conf['options']['tops'],
    
    
 );
 
 $form['options']['latest']=array(
   '#type'=>'checkbox',
   '#title'=>'Latest comments',
   '#Description'=>t('Display latest comments'),
   "#default_value"=>$conf['options']['latest'],
    
    
 );
   
  return $form;
}

function comments_edit_form_submit($form, &$form_state) {
  
  $form_state['conf']['nb'] = $form_state['values']['nb'];
  
  $value_to_save=array('tops','latest',);
  
  foreach($form_state["values"]['options'] as $key=>$item){
    $form_state['conf']['options'][$key]=$form_state['values']['options'][$key];
    
  }
  
}

function comments_content_type_render($subtype, $conf, $args, $context) {
	
  $node = $context['argument_entity_id:node_1']->data;

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Informations about the comments';

  //$output = "ICI LES SHARES POUR ".print_r($node->nid,true);//lalala
  $output = '';
  
  $values = array(
  	'select'	=> 'tops',
	'nid'		=> $node->nid,
  );

  if ($conf['nb']==1) {
	  $values['nb_comments'] = $node->comment_count;
  }  
  
  $output = _comments_get_form($conf, $values);
  

  $block = new stdClass();
  $block->module = 'comments';
  $block->delta = $subtype;
  $block->title = $title;
  $block->content = $output;

  return $block;

}

function _comments_get_form($conf, $values){
  
  $form_state['conf']=$conf;
  $form_state['values'] = $values;
  return drupal_get_form("_comments_form_render_form",$form_state);
  
  
}