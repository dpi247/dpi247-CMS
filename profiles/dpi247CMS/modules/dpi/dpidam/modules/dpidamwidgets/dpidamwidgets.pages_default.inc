<?php

/**
 * Implementation of hook_default_page_manager_pages().
 */
function dpidamwidgets_default_page_manager_pages() {
  $page = new stdClass;
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'dam_archives_search';
  $page->task = 'page';
  $page->admin_title = 'DAM Archives Search';
  $page->admin_description = '';
  $page->path = 'archives/damsearch';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array();
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_dam_archives_search_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'dam_archives_search';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'base-layout-brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'ed85c775-5c4b-4659-8937-e8c50bd82c2f';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1a794efc-2dde-4326-92a6-0280b5373838';
    $pane->panel = 'maincontentleft';
    $pane->type = 'damsearch';
    $pane->subtype = 'damsearch';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'profile' => 'default',
      'override_title' => 0,
      'override_title_text' => '',
      'display_results' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = '';
    $pane->uuid = '1a794efc-2dde-4326-92a6-0280b5373838';
    $display->content['new-1a794efc-2dde-4326-92a6-0280b5373838'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-1a794efc-2dde-4326-92a6-0280b5373838';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-1a794efc-2dde-4326-92a6-0280b5373838';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['dam_archives_search'] = $page;

 return $pages;
}
