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
  $handler->name = 'node_view_destination_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'dpipanels_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default Node Page',
    'no_blocks' => 0,
    'pipeline' => 'dpiviewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'base-layout-article';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contenttop' => NULL,
      'mainContent' => NULL,
      'complementary' => NULL,
      'subcontentleft' => NULL,
      'subcontentright' => NULL,
      'contentbottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'dc182dfe-3d2b-4ddc-9aaf-7069ace5da9f';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b7de68b9-15ff-4d6b-a0a4-859b25c1ce8f';
    $pane->panel = 'complementary';
    $pane->type = 'block';
    $pane->subtype = 'views-most_view-block';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
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
    $pane->uuid = 'b7de68b9-15ff-4d6b-a0a4-859b25c1ce8f';
    $display->content['new-b7de68b9-15ff-4d6b-a0a4-859b25c1ce8f'] = $pane;
    $display->panels['complementary'][0] = 'new-b7de68b9-15ff-4d6b-a0a4-859b25c1ce8f';
    $pane = new stdClass();
    $pane->pid = 'new-26be826d-1238-4627-a8bb-3ac6fb41dd86';
    $pane->panel = 'mainContent';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 0,
      'override_title' => 1,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '26be826d-1238-4627-a8bb-3ac6fb41dd86';
    $display->content['new-26be826d-1238-4627-a8bb-3ac6fb41dd86'] = $pane;
    $display->panels['mainContent'][0] = 'new-26be826d-1238-4627-a8bb-3ac6fb41dd86';
    $pane = new stdClass();
    $pane->pid = 'new-fb09f358-df4c-4d2c-9053-c5bb2fa31228';
    $pane->panel = 'mainContent';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '10',
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'fb09f358-df4c-4d2c-9053-c5bb2fa31228';
    $display->content['new-fb09f358-df4c-4d2c-9053-c5bb2fa31228'] = $pane;
    $display->panels['mainContent'][1] = 'new-fb09f358-df4c-4d2c-9053-c5bb2fa31228';
    $pane = new stdClass();
    $pane->pid = 'new-bc147e8e-ae52-4550-9818-0d3fa97064e1';
    $pane->panel = 'mainContent';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 0,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'bc147e8e-ae52-4550-9818-0d3fa97064e1';
    $display->content['new-bc147e8e-ae52-4550-9818-0d3fa97064e1'] = $pane;
    $display->panels['mainContent'][2] = 'new-bc147e8e-ae52-4550-9818-0d3fa97064e1';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'sections_destination_context';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'dpisectionspanels_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default Section Page',
    'no_blocks' => 0,
    'pipeline' => 'dpiviewer',
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
