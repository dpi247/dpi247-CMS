<?php
/**
 * @file
 * unfoldcompanion.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function unfoldcompanion_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'sections_destination_context';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'destination_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default Section Page',
    'no_blocks' => 0,
    'pipeline' => 'dpidestinations_viewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'section_default_target' => '',
  );
  $display = new panels_display();
  $display->layout = 'base-layout-brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contenttop' => NULL,
      'maincontentleft' => NULL,
      'maincontentright' => NULL,
      'complementary' => NULL,
      'subcontentleft' => NULL,
      'subcontentright' => NULL,
      'contentbottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1';
    $pane->panel = 'maincontentleft';
    $pane->type = 'targetblock';
    $pane->subtype = 'main';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'node_count_for_targetblockinstance' => '5',
      'use_pager' => 0,
      'use_split' => 0,
      'node_count_for_targetblock' => NULL,
      'offset' => NULL,
      'split_delta' => '0',
      'hide_pager' => 0,
      'pager_id' => NULL,
      'pager_type' => 'mini',
      'rank_max_value' => '5',
      'template_rule' => NULL,
      '' => NULL,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $display->content['new-1'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-1';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-1';
  $handler->conf['display'] = $display;
  $export['sections_destination_context'] = $handler;

  return $export;
}
