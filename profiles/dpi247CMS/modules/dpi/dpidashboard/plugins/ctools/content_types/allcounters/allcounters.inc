<?php

$plugin = array(
  'title' => t('Every counters for this node'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  'single' => TRUE,
  
  'admin info' => 'allcounters_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'allcounters_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashboardnode',
  
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  'edit form' => 'allcounters_edit_form',
  
  
  'category' => t('Dashboard'),
  
  
  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
);

function allcounters_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Number of views ctools contenttype';
	$block->content = t('Display the number of views on a selected period for current node');
	return $block;
}

function allcounters_edit_form($form, &$form_state) {
 
 $conf=$form_state['conf'];
 $args=$form_state['args'];
 $contexts=$form_state['contexts'];

 $form["options"]=array("#tree"=>TRUE);
 
 $form['options']['nbviews']=array(
   '#type'=>'checkbox',
   '#title'=>t('Everycounters'),
   '#Description'=>t('Everycounters'),
   "#default_value"=>$conf['options']['nbviews'],
   
   
   );
   
 $form['options']['nbcomments']=array(
   '#type'=>'checkbox',
   '#title'=>t('Number of comments'),
   '#Description'=>t('Count number of comment for the node'),
   "#default_value"=>$conf['options']['nbcomments'],
   
   
   );
 
 $form['options']['facebook']=array(
   '#type'=>'checkbox',
   '#title'=>'Facebook',
   '#Description'=>t('Count number of Facebook shares'),
   "#default_value"=>$conf['options']['facebook'],
   
   
   );

 $form['options']['twitter']=array(
   '#type'=>'checkbox',
   '#title'=>'Twitter',
   '#Description'=>t('Count number of Twitter shares'),
   "#default_value"=>$conf['options']['twitter'],
    
    
 );
 
 $form['options']['googleplus']=array(
   '#type'=>'checkbox',
   '#title'=>'Google+',
   '#Description'=>t('Count number of Google+ shares'),
   "#default_value"=>$conf['options']['googleplus'],
    
    
 );
 
 
 $form['options']['linkedin']=array(
   '#type'=>'checkbox',
   '#title'=>'LinkedIn',
   '#Description'=>t('Count number of LinkedIn shares'),
   "#default_value"=>$conf['options']['linkedin'],
    
    
 );
   
  return $form;
}

function allcounters_edit_form_submit($form, &$form_state) {
  
  $value_to_save=array('facebook','twitter','googleplus','linkedin','nbcomments','nbviews',);
  
  foreach($form_state["values"]['options'] as $key=>$item){
    $form_state['conf']['options'][$key]=$form_state['values']['options'][$key];
    
  }
  
}

function allcounters_content_type_render($subtype, $conf, $args, $context) {
	
  $node = $context['argument_entity_id:node_1']->data;

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Node counters';
  
  $block = new stdClass();
  $block->module = 'viewsnode';
  $block->delta = $subtype;
  $block->title = $title;
  
  $output = '';
  $script = '';
  
  $data = array();
  
  if ($conf['options']['nbviews']==1) {
    $data['nbviews'] = dpidashboard_get_views_node($node->nid);  
	$output.='<div id="dpiDashboardSocialCountAllviews"><span id="dpiDashboardSocialCountAllviewsDisplay">'.$data['nbviews'].'</span></div>';
  }
  
  if ($conf['options']['nbcomments']==1) {
    $data['nbcomments'] = $node->comment_count;  
	$output.='<div id="dpiDashboardSocialCountAllnbcomments"><span id="dpiDashboardSocialCountAllnbcommentsDisplay">'.$data['nbcomments'].'</span></div>';
  }
  
  if ($conf['options']['facebook']==1) {
	    $data['facebook'] = true;
		$output.='<div id="dpiDashboardSocialCountAllfacebook"><span id="dpiDashboardSocialCountAllfacebookDisplay"></span></div>';
		$script.="
		jQuery.ajax({
			url: '/dpidashboard/ajax/getsocialshares?social=facebook&url='+document.URL,
			cache: false,
			success:function(result){
				if (result.success==false) {
					jQuery('#dpiDashboardSocialCountAllfacebookDisplay').html(result.error);
				} else {
					jQuery('#dpiDashboardSocialCountAllfacebookDisplay').html(result.count_str);
				}
			}
		});
		"; 
  }
  
  if ($conf['options']['twitter']==1) {
	    $data['twitter'] = true;
		$output.='<div id="dpiDashboardSocialCountAlltwitter"><span id="dpiDashboardSocialCountAlltwitterDisplay"></span></div>';
		$script.="
		jQuery.ajax({
			url: '/dpidashboard/ajax/getsocialshares?social=twitter&url='+document.URL,
			cache: false,
			success:function(result){
				if (result.success==false) {
					jQuery('#dpiDashboardSocialCountAlltwitterDisplay').html(result.error);
				} else {
					jQuery('#dpiDashboardSocialCountAlltwitterDisplay').html(result.count_str);
				}
			}
		});
		"; 
  }
  
  if ($conf['options']['linkedin']==1) {
	   $data['linkedin'] = true;
		$output.='<div id="dpiDashboardSocialCountAlllinkedin"><span id="dpiDashboardSocialCountAlllinkedinDisplay"></span></div>';
		$script.="
		jQuery.ajax({
			url: '/dpidashboard/ajax/getsocialshares?social=linkedin&url='+document.URL,
			cache: false,
			success:function(result){
				if (result.success==false) {
					jQuery('#dpiDashboardSocialCountAlllinkedinDisplay').html(result.error);
				} else {
					jQuery('#dpiDashboardSocialCountAlllinkedinDisplay').html(result.count_str);
				}
			}
		});
		"; 
  }
  
  if ($conf['options']['googleplus']==1) {
	  $data['googleplus'] = true;
		$output.='<div id="dpiDashboardSocialCountAllgoogleplus"><span id="dpiDashboardSocialCountAllgoogleplusDisplay"></span></div>';
		$script.="
		jQuery.ajax({
			url: '/dpidashboard/ajax/getsocialshares?social=googleplus&url='+document.URL,
			cache: false,
			success:function(result){
				if (result.success==false) {
					jQuery('#dpiDashboardSocialCountAllgoogleplusDisplay').html(result.error);
				} else {
					jQuery('#dpiDashboardSocialCountAllgoogleplusDisplay').html(result.count_str);
				}
			}
		});
		"; 
  }
  
  if (!empty($script)) drupal_add_js('jQuery(document).ready(function () { '.$script.' });', 'inline');
  
  drupal_add_css(drupal_get_path('module', 'dpidashboard') . '/css/allcounters.css');
  
  $block->content = array(
    '#theme' => 'allcounters',
    '#content' => $data,
  );

  return $block;
  
}

