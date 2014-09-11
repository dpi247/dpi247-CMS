<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Latest created packeages'),
  'description' => t('Shows the latest created packages.'),
  'category' => t('Dashboard'),
  'render callback' => 'lastestcreated_render',
  
);

/**

* implements 

**/

function lastestcreated_render($subtype, $conf, $panel_args, $context, $a) {

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Latest created packages';

  $element = _dpidashboard_get_element();
  
  $result = _dpidashboard_get_latest_created_package(false,$element);
  
 $output = 'Element: '.$element.'<hr/>';
  
  foreach($result as $row) {
  	$output .= $row->title."<br />";
  }
  $output .= theme('pager',array('element'=>$element));

  $block = new stdClass();
  $block->module = 'servertime';
  $block->delta = $subtype;
  $block->title = $title;
  $block->content = $output;

  return $block;

}
