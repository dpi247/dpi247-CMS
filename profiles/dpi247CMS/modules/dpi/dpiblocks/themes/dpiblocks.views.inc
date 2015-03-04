<?php

/**
 * @file
 * Views plugins implementation.
 */

/**
 * Implements hook_views_plugins().
 */
function dpiblocks_views_plugins() {
  return array (
    'row' => array(
      'dpiblock_accroche_row_plugin' => array(
        'title' => t('Accroche block Row'),
        'help' => t('Displays the node according to the layout of his destination.'),
        'handler' => 'dpiblocks_plugin_row_node_accroche',
        'path' => drupal_get_path ( 'module', 'dpiblocks' ) . '/includes/views',
        'base' => array('node'), 
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
    ),
    'style' => array (
      'dpiblocks_accroche' => array (
        'title' => t ( 'Dpiblocks accroche' ),
        'help' => t ( 'Specific style view for atom teaser' ),
        'handler' => 'accroche_teaser_plugin_style',
        'uses row plugin' => TRUE,
        'uses fields' => FALSE,
        'uses options' => FALSE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'theme' => 'accroche_custom_theme',
        'theme path' => drupal_get_path ( 'module', 'dpiblocks' ) . '/themes',
      )
    )
  );
}

function dpiblocks_views_default_views(){
  $view = new view();
  $view->name = 'dpiblocks_accroche_default';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'dpiblocks_accroche_default';
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
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'dpiblocks_accroche';
  $handler->display->display_options['row_plugin'] = 'dpiblock_accroche_row_plugin';
  $handler->display->display_options['row_options']['links'] = FALSE;
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
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
    
  $views[$view->name] = $view;
  return $views;
}
