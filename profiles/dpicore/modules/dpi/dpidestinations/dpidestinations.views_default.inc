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
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
return $views;
}
