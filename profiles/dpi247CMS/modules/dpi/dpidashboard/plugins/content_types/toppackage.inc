<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Top Package'),
  'description' => t('Shows the top packages.'),
  'category' => t('Dashboard'),
  'render callback' => 'toppackage_render',
  
);

/**

* implements 

**/

function toppackage_render($subtype, $conf, $panel_args, $context, $a) {

  $element = _dpidashboard_get_element();

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Top Package';

  $result = _dpidashboard_get_toppackage($element);
  
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
