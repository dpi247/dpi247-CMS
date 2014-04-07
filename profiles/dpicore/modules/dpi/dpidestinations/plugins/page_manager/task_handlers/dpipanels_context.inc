<?php

//Need this for dependancies
ctools_get_plugins('page_manager', 'task_handlers');
/**
 * @file
 *
 * This is the task handler plugin to handle attaching a panel to any
 * task that advertises itself as a 'context' type, which all of the
 * basic page tasks provided by page_manager.module do by default.
*/

// Plugin definition
$plugin = array(
  // is a 'context' handler type, meaning it supports the API of the
  // context handlers provided by ctools context plugins.
  'handler type' => 'context',
  'visible' => TRUE, // may be added up front.

  // Administrative fields.
  'title' => t('Dpi Panels'),
  'admin summary' =>'panels_panel_context_admin_summary',
  'admin title' => 'panels_panel_context_title',
  'tab operation' => 'panels_panel_context_tab_operation',
  
  // Callback to render the data.
  'render' => 'panels_panel_context_render',
  //'render' => 'panels_panel_context_render',
  // Callback to return addressable data
  'addressable callback' => 'panels_panel_context_get_addressable',
  
  // Various callbacks for operations performed on the handler to ensure
  // related data is updated properly.
  'save' => 'panels_panel_context_save',
  'delete' => 'panels_panel_context_delete',
  'export' => 'panels_panel_context_export',
  'clone' => 'panels_panel_context_clone',
  
  'operations' => array(
    'settings' => array(
      'title' => t('General'),
      'description' => t('Change general settings about this variant.'),
      'form' => 'panels_panel_context_edit_settings',
    ),
    
    'criteria' => array(
      'title' => t('Selection rules'),
      'description' => t('Control the criteria used to decide whether or not this variant is used.'),
      'ajax' => FALSE,
      'form' => array(
      'order' => array(
        'form' => t('Selection rules'),
      ),
      'forms' => array(
        'form' => array(
          'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
          'form id' => 'ctools_context_handler_edit_criteria',
          ),
        ),
      ),
    ),
    
    'context' => array(
      'title' => t('Contexts'),
      'ajax' => FALSE,
      'description' => t('Add additional context objects to this variant that can be used by the content.'),
      'form' => array(
        'order' => array(
          'form' => t('Context'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_context',
          ),
        ),
      ),
    ),
    
    'layout' => array(
      'title' => t('Layout'),
      'description' => t('Change the layout of this panel.'),
      // No AJAX so we get our CSS loaded.
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'choose' => t('Change layout'),
          'move' => t('Move content from old layout'),
        ),
        'forms' => array(
          'choose' => array(
            'form id' => 'panels_panel_context_edit_layout',
          ),
          'move' => array(
            'include' => array(
              drupal_get_path('module', 'panels') . '/includes/display-layout.inc',
            ),
            'form id' => 'panels_panel_context_edit_move',
            'submit' => 'panels_change_layout_submit',
          ),
        ),
      ),
    ),
    
    'content' => array(
      'title' => t('Content'),
      'description' => t('Add content items and change their location with a drag and drop interface.'),
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'form' => t('Content'),
        ),
        'forms' => array(
          'form' => array(
            'include' => array(
              drupal_get_path('module', 'panels') . '/includes/display-edit.inc',
            ),
            'form id' => 'dpidestinations_dpipanels_context_edit_content',
            'no blocks' => TRUE,
          ),
        ),
      ),
    ),

    
    'preview' => array(
      'title' => t('Preview'),
      'description' => t('Get a preview of what this variant will look like.'),
      'form' => 'panels_panel_context_edit_preview',
      'ajax' => FALSE,
      'silent' => TRUE,
      'form info' => array('finish text' => t('Preview')),
      'no update and save' => TRUE,
    ),
  ),

  'add features' => array(
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
  ),
  
  // Where to go when finished.
  'add finish' => 'content',

  'required forms' => array(
    'choose' => t('Choose layout'),
    'settings' => t('Panel settings'),
    'content' => t('Panel content'),
  ),

  'edit forms' => array(
    'content' => t('Section content'),
    'criteria' => t('Selection rules'),
    'settings' => t('General'),
    'context' => t('Contexts'),
    'layout' => t('Change layout'),
    'move' => '', // no title makes it a 'hidden' edit form.
  ),
  
  'forms' => array(
    'settings' => array(
      'form id' => 'panels_panel_context_edit_settings',
    ),
    'choose' => array(
      'form id' => 'panels_panel_context_edit_choose',
      'no back validate' => TRUE,
    ),
    'layout' => array(
      'no return' => TRUE,
      'form id' => 'panels_panel_context_edit_layout',
    ),
    'move' => array(
      'include' => array(
        drupal_get_path('module', 'panels') . '/includes/display-layout.inc',
      ),
      'form id' => 'panels_panel_context_edit_move',
      'submit' => 'panels_change_layout_submit',
    ),
    'content' => array(
      'include' => array(
        drupal_get_path('module', 'panels') . '/includes/display-edit.inc',
      ),
      'form id' => 'panels_panel_context_edit_content',
      'no blocks' => TRUE,
    ),
    'context' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_context',
    ),
    'criteria' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_criteria',
     ),
  ),
  
  'default conf' => array(
    'no_blocks' => FALSE,
    'pipeline' => 'dpiviewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  ),
);

/**
 * Present the panels drag & drop editor to edit the display attached
 * to the task handler.
*/
function dpidestinations_dpipanels_context_edit_content($form, &$form_state) {
  ctools_include('ajax');
  ctools_include('plugins', 'panels');
  ctools_include('common', 'panels');
  ctools_include('context');
  ctools_include('context-task-handler');

  $cache = panels_edit_cache_get('panel_context:' . $form_state['task_name'] . '::' . $form_state['handler_id'] . '::::');
  $form_state['renderer'] = panels_get_renderer_handler('dpieditor', $cache->display);
  $form_state['renderer']->cache = &$cache;

  $form_state['display'] = &$cache->display;
  $form_state['content_types'] = $cache->content_types;
  // Tell the Panels form not to display save & cancel buttons.
  $form_state['no buttons'] = TRUE;

  $form_state['display_title'] = !empty($cache->display_title);
  $form_state['no preview'] = TRUE;
  $form_state['page']->display_cache[$form_state['handler_id']] = $cache;

  $form = panels_edit_display_form($form, $form_state);

  if (!isset($form_state['type']) || $form_state['type'] != 'add' && !empty($form_state['handler_id']) && !empty($form['buttons'])) {
    $form['buttons']['preview'] = $form['buttons']['return'];
    $form['buttons']['preview']['#value'] = t('Update and preview');
  };
  return $form;
}

/**
 * Validate changes to the panel content form.
 */
function dpidestinations_dpipanels_context_edit_content_validate(&$form, &$form_state) {
  panels_panel_context_edit_content_validate($form, $form_state);
}

function dpidestinations_dpipanels_context_edit_content_submit(&$form, &$form_state) {
  panels_panel_context_edit_content_submit($form, $form_state);
}

function dpidestinations_dpipanels_renderer_editor($handler, $base_contexts, $args, $test = TRUE) {
  // Go through arguments and see if they match.
  ctools_include('context');
  ctools_include('context-task-handler');
  ctools_include('plugins', 'panels');

  // Add my contexts
  $contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);

  // Test.
  if ($test && !ctools_context_handler_select($handler, $contexts)) {
    return;
  }

  if (isset($handler->handler)) {
    ctools_context_handler_pre_render($handler, $contexts, $args);
  }

  // Load the display
  $display = panels_panel_context_get_display($handler);
  $display->context = $contexts;
  $display->args = $args;
  $display->css_id = $handler->conf['css_id'];
  $task_name = page_manager_make_task_name($handler->task, $handler->subtask);

  $display->cache_key = 'panel_context:' . $task_name . ':' . $handler->name;

  // Check to see if there is any CSS.
  if (!empty($handler->conf['css'])) {
    ctools_include('css');
    $css_id = 'panel_context:' . $handler->name;
    $filename = ctools_css_retrieve($css_id);
    if (!$filename) {
      $filename = ctools_css_store($css_id, $handler->conf['css']);
    }
    drupal_add_css($filename);
  }

  // With an argument, this actually sets the display.
  panels_get_current_page_display($display);

  $renderer = panels_get_renderer($handler->conf['pipeline'], $display);

  // Remove and add body element classes
  $panel_body_css = &drupal_static('panel_body_css');

  if (isset($handler->conf['body_classes_to_remove']) && !isset($panel_body_css['body_classes_to_remove'])) {
    $panel_body_css['body_classes_to_remove'] = $handler->conf['body_classes_to_remove'];
  }
  if (isset($handler->conf['body_classes_to_add']) && !isset($panel_body_css['body_classes_to_add'])) {
    $panel_body_css['body_classes_to_add'] = $handler->conf['body_classes_to_add'];
  }

  $info = array(
    'content' => panels_render_display($display, $renderer),
    'no_blocks' => !empty($handler->conf['no_blocks']),
  );

  $info['title'] = $display->get_title();

  return $info;
}

