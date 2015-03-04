<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Dpi Esi Test'),
  'description' => t('Test Esi behavior for pane.'),
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
  'category' => t('Dpi Esi Test'),
  'group' => t('DPI Devel'),
  'all contexts' => TRUE,
);

/**
 * Implementation of hook_typeofcontent_content_type_content_types
 * Return all custom content types available.
 */

/*
function dpiesitest_dpiesitestpane_content_type_content_types($plugin) {
  $types = array();
  $targets = target_load_multiple(FALSE);
  foreach($targets as $target) {
    $types[$target->machine_name] = array(
      'title' => check_plain($target->machine_name),
      'subtype' => $target->machine_name,
      'description' => check_plain($target->description),
      //'required context' => new ctools_context_required(t('Destination'), 'destination'),
      'category' => t('Target'),
      'group' => t('DPI Core'),
      //'all contexts' => TRUE,
    );
  }

  return $types;
}
*/


function dpiesitest_dpiesitestpane_content_type_render($subtype, $conf, $panel_args,$context) {
  
  global $user;
  drupal_add_css(".dpiesitest dt{font-weight:bold;}",array('type'=>"inline"));
  
  $module = 'dpiesitest';
  
  $block = new stdClass();
  $block->module = $module;
  
  $username=$user?$user->name:"Anonymous";
  
  $block->title = "DPI ESI test pane";
  
  $content = '<div class="dpiesitest"><dl>';
  $content .="<dt>Build time</dt><dd>".date("Y-m-d h:i:s")."</dd>";
  $content .="<dt>Page</dt><dd>".$_GET['q']."</dd>";
  $content .="<dt>User that generate content</dt><dd>".$username."</dd>";
  $content .="<dt>User role from original user</dt><dd>".print_r($user->roles,1)."</dd>";
  $content .="<dt>Context</dt><dd>".kprint_r($context,1)."</dd>";
  $content .="<dt>Args</dt><dd>".kprint_r($panel_args,1)."</dd>";
  $content .="<dt>Conf</dt><dd>".kprint_r($conf,1)."</dd>";
  $content .= "</dl></div>";
  
  $block->content =$content;
  

  return $block;
}
