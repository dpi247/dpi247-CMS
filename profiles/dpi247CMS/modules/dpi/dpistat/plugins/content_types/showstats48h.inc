<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title'              => t('[Node] Number of views during the last 48 hours'),
  'description'        => t('Display a chart of the number of visits during the last 48 hours.'),
  'defaults'           => array(),
  'single'             => TRUE,
  'required context'   => new ctools_context_required(t('Node'), 'node'),
  'group'              => t('DPI Core'),
  'category'           => t('Dashboard'),
);

/**
 * Callback function to supply a graph of published nodes
*/
function dpistat_showstats48h_content_type_content_types() {
  return array('showstats48h' => array(
    'single' => TRUE,
    'title' => t('[Node] Number of views during the last 48 hours'),
    'category' => t('Dashboard'),
    'description' => t('Display a chart of the number of visits during the last 48 hours.'),
    'defaults' => array(),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'group' => t('DPI Core'),
  ));
}

function dpistat_showstats48h_content_type_edit_form($form, $form_state) {
  // The current configuration
  return $form;
}

function dpistat_showstats48h_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
}

/**
 * Output function for the 'shortcut' content type.
 */
function dpistat_showstats48h_content_type_render($subtype, $conf, $panel_args, $context, $a) {
  $block = new stdClass();
  $block->title = t('Number of visists during the last 48 hours');
  $theme_args = array(
    'nid' => $context->data->nid,
    'width' => 'auto',
    'height' => 'auto',
    'display_title' => FALSE,
  );
  $block->content = theme('dpistat_show_stats_48h', $theme_args);
  return $block;
}
