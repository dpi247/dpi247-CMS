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
  $handler->did = 1;
  $display = new panels_display();
  $display->did = 1;
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
  $display->uuid = '48741434-d0e4-4452-9568-6ab2c19bdef7';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-be16bf6c-aa77-4e12-8cc4-719c0f7cef1a';
    $pane->panel = 'complementary';
    $pane->type = 'block';
    $pane->subtype = 'views-most_view-block';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
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
    $pane->uuid = 'be16bf6c-aa77-4e12-8cc4-719c0f7cef1a';
    $display->content['new-be16bf6c-aa77-4e12-8cc4-719c0f7cef1a'] = $pane;
    $display->panels['complementary'][0] = 'new-be16bf6c-aa77-4e12-8cc4-719c0f7cef1a';
    $pane = new stdClass();
    $pane->pid = 'new-7916302a-c84c-4258-b4ae-fe91f7b0c9e4';
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
    $pane->uuid = '7916302a-c84c-4258-b4ae-fe91f7b0c9e4';
    $display->content['new-7916302a-c84c-4258-b4ae-fe91f7b0c9e4'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-7916302a-c84c-4258-b4ae-fe91f7b0c9e4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-7916302a-c84c-4258-b4ae-fe91f7b0c9e4';
  $handler->conf['display'] = $display;
  $export['sections_destination_context'] = $handler;

  return $export;
}
