<?php
function dpidestinations_views_default_views() {
$view = new view();
$view->name = 'defaut_targetview';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'defaut targetview';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Sort criterion: Content: Destinations (field_destinations:virtual_rank) */
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['id'] = 'field_destinations_virtual_rank';
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['table'] = 'field_data_field_destinations';
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['field'] = 'field_destinations_virtual_rank';
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['order'] = 'DESC';
/* Filter criterion: Content: Destinations (field_destinations:tid) */
$handler->display->display_options['filters']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['filters']['field_destinations_tid']['table'] = 'field_data_field_destinations';
$handler->display->display_options['filters']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['filters']['field_destinations_tid']['value']['value'] = '3';

/* Display: Target block */
$handler = $view->new_display('targetview', 'Target block', 'targetview_1');
$handler->display->display_options['pager']['type'] = 'some';

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');

$views[$view->name] = $view;
// At the end, return array of default views.


$view = new view();
$view->name = 'targetview_reorder_default';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'targetview_reorder_default';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Author uid */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'node';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
/* Field: Content: Editorial Publication date */
$handler->display->display_options['fields']['field_editorialpublicationdate']['id'] = 'field_editorialpublicationdate';
$handler->display->display_options['fields']['field_editorialpublicationdate']['table'] = 'field_data_field_editorialpublicationdate';
$handler->display->display_options['fields']['field_editorialpublicationdate']['field'] = 'field_editorialpublicationdate';
$handler->display->display_options['fields']['field_editorialpublicationdate']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
);
/* Sort criterion: Content: Destinations (field_destinations:virtual_rank) */
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['id'] = 'field_destinations_virtual_rank';
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['table'] = 'field_data_field_destinations';
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['field'] = 'field_destinations_virtual_rank';
$handler->display->display_options['sorts']['field_destinations_virtual_rank']['order'] = 'DESC';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

/* Display: Target block */
$handler = $view->new_display('targetview', 'Target block', 'targetview_1');
$handler->display->display_options['pager']['type'] = 'some';

$views[$view->name] = $view;


return $views;
}
