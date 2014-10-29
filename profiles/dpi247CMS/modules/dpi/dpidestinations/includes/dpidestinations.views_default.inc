<?php

/**
 * Implementation of hook_views_default_views
 */
function dpidestinations_views_default_views() {
  $view = new view();
  $view->name = 'default_targetview';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'default targetview';
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
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['row_class'] = 'article-inline';
  $handler->display->display_options['style_options']['class'] = 'article-group';
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

  /* Display: Target block */
  $handler = $view->new_display('targetview', 'Target block', 'targetview_1');
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'targetblock_node_row';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');

  $views[$view->name] = $view;
  // At the end, return array of default views.
  
  $view = new view();
  $view->name = 'targetview_reorder_default';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'targetview reorder default';
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
  /* Field: Content: Authors */
  $handler->display->display_options['fields']['field_authors']['id'] = 'field_authors';
  $handler->display->display_options['fields']['field_authors']['table'] = 'field_data_field_authors';
  $handler->display->display_options['fields']['field_authors']['field'] = 'field_authors';
  $handler->display->display_options['fields']['field_authors']['label'] = '';
  $handler->display->display_options['fields']['field_authors']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_authors']['delta_offset'] = '0';
  /* Field: Content: Editorial Publication date */
  $handler->display->display_options['fields']['field_editorialpublicationdate']['id'] = 'field_editorialpublicationdate';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['table'] = 'field_data_field_editorialpublicationdate';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['field'] = 'field_editorialpublicationdate';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['label'] = '';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_editorialpublicationdate']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Sort criterion: Content: Post date */
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
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'package' => 'package',
  );
  
  $views[$view->name] = $view;

  return $views;
}
