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
  //'admin info' => 'youtube_embed_admin_info',
  //'defaults' => array(
  //    'description' => t('some working tunes.. '),
  //    'youtube_url' => 'http://www.youtube.com/watch?v=bTMZDr9RGBo',
  //  ), 
  'required context'   => array(
  	//new ctools_context_required(t('Node'), 'node'),
	//new ctools_context_required(t('Taxonomy'), 'taxonomy'),
	)
  
);

/**
 * 'Edit form' callback for the content type.
 */

function youtube_embed_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['description'] = array(
    '#title' => t('Description'),
    '#description' => t('Description (administrative)'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#default_value' => $conf['description'],
  );


  $form['youtube_url'] = array(
    '#title' => t('Youtube url'),
    '#description' => t('The url of the youtube video you want to show'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $conf['youtube_url'],
  );

  return $form;
}


/**
 * The submit form stores the data in $conf.
 */

function youtube_embed_edit_form_submit($form, &$form_state) {

  foreach (array_keys($form_state['plugin']['defaults']) as $key) {

    if (isset($form_state['values'][$key])) {

      $form_state['conf'][$key] = $form_state['values'][$key];

    }

  }

}

/**

* implements 

**/

function servertime_render($subtype, $conf, $panel_args, $context, $a) {

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
