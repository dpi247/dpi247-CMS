<?php
/**
 * @file
 * unfoldcompanion.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function unfoldcompanion_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'most_view';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Most view';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Most view';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['type'] = 'ol';
  $handler->display->display_options['style_options']['class'] = 'article-group';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['path']['element_label_colon'] = FALSE;
  /* Field: Field: Display Title */
  $handler->display->display_options['fields']['field_displaytitle']['id'] = 'field_displaytitle';
  $handler->display->display_options['fields']['field_displaytitle']['table'] = 'field_data_field_displaytitle';
  $handler->display->display_options['fields']['field_displaytitle']['field'] = 'field_displaytitle';
  $handler->display->display_options['fields']['field_displaytitle']['label'] = '';
  $handler->display->display_options['fields']['field_displaytitle']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_displaytitle']['alter']['path'] = '[path]';
  $handler->display->display_options['fields']['field_displaytitle']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['field_displaytitle']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_displaytitle']['type'] = 'text_plain';
  /* Field: Content statistics: Views today */
  $handler->display->display_options['fields']['daycount']['id'] = 'daycount';
  $handler->display->display_options['fields']['daycount']['table'] = 'node_counter';
  $handler->display->display_options['fields']['daycount']['field'] = 'daycount';
  $handler->display->display_options['fields']['daycount']['label'] = '';
  $handler->display->display_options['fields']['daycount']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['daycount']['alter']['path'] = '[path]';
  $handler->display->display_options['fields']['daycount']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['daycount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['daycount']['separator'] = ' ';
  $handler->display->display_options['fields']['daycount']['prefix'] = '(';
  $handler->display->display_options['fields']['daycount']['suffix'] = ')';
  /* Sort criterion: Content statistics: Views today */
  $handler->display->display_options['sorts']['daycount']['id'] = 'daycount';
  $handler->display->display_options['sorts']['daycount']['table'] = 'node_counter';
  $handler->display->display_options['sorts']['daycount']['field'] = 'daycount';
  $handler->display->display_options['sorts']['daycount']['order'] = 'DESC';
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

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_caching'] = '-1';
  $export['most_view'] = $view;

  return $export;
}
