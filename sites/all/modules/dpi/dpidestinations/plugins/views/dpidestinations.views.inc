<?php

/**
 * Implementation of hook_views_plugins
 */
function dpidestinations_views_plugins() {
  $path = drupal_get_path('module', 'dpidestinations');
  $views_path = drupal_get_path('module', 'views');
  //require_once "./$path/theme/theme.inc";

  return array(
    'display' => array(
      'targetblock' => array(
        'title' => t('Target block'),
        'help' => t('View display used for the default target, use this as a basis for custom destination views and targetss.'),
        'handler' => 'dpidestinations_plugin_display_targetblock',
       // 'path' => drupal_get_path('module', 'dpidestinations') . '/plugins/views',
      //  'theme path' => drupal_get_path('module', 'views') . '/theme',
      //  'theme' => 'views_view',
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => TRUE,
        'help topic' => 'targetblock',
        'admin' => t('Target Content pane'),
        //'base' => array('TargetBlock'), // only works with 'TargetBlock' as base.
        
      ),
    ),
/*
    'style' => array( //declare the style plugin
      'target_style' => array(
        'path' =>  drupal_get_path('module', 'dpidestinations') . '/plugins/views',
        'title' => t('Target Style'),
        'help' => t('Used for WallyEdit previews'),
        'handler' => 'dpidestinations_views_plugin_style_targetblock',
        'theme' => 'dpidestinations_style_targetblock',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'dpidestinations') . "/theme",
        'uses row plugin' => TRUE,
        'uses fields' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),

    'row' => array( //declare the unformatted row plugin
      'targetblock_row' => array(
        'title' => t('Target block Row'),
        'help' => t('(Displays the node according to the layout of his destination.'),
        'path' =>  drupal_get_path('module', 'dpidestinations') . '/plugins/views',
        'handler' => 'dpidestinations_views_plugin_row_targetblock',
        'theme' => 'dpidestinations_row_targetblock',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'dpidestinations') . "/theme",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      )
    )
  */
  )
  ;
  
}
