<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Server Time'),
  'description' => t('Shows the time of the server.'),
  'category' => t('Dashboard'),
  //'edit form' => 'youtube_embed_edit_form',
  'render callback' => 'servertime_render',
  'admin info' => 'servertime_admin_info',
  //'defaults' => array(
  //    'description' => t('some working tunes.. '),
  //    'youtube_url' => 'http://www.youtube.com/watch?v=bTMZDr9RGBo',
  //  ), 
  'required context'   => array(
  	//new ctools_context_required(t('Node'), 'node'),
	//new ctools_context_required(t('Taxonomy'), 'taxonomy'),
	)
  
);

function servertime_admin_infoo($subtype, $conf, $context = NULL) {
  $context = new stdClass();
  $context->data = new stdClass();
  $context->data->description = t("no real context");
  $block = simplecontext_content_type_render($subtype, $conf, array("Example"), $context);
  return $block;
}

function servertime_render($subtype, $conf, $panel_args, $context, $a) {
	
	// dsm;

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Servertime';

  $output = "<div>".date('Y-m-d H:i:s')."</div>";//lalala

  $block = new stdClass();
  $block->module = 'servertime';
  $block->delta = $subtype;
  $block->title = $title;
  $block->content = $output;

  return $block;

}
