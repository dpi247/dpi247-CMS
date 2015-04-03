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
  $handler->export_type=EXPORT_IN_DATABASE;
  $handler->handler = 'dpisectionspanels_context';
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
  $pane->pid = 'new-78c5a068-4dec-4d2a-a2f3-42fad0850e50';
  $pane->panel = 'complementary';
  $pane->type = 'views';
  $pane->subtype = 'most_view';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_pager_settings' => 0,
    'use_pager' => 0,
    'nodes_per_page' => '5',
    'pager_id' => '0',
    'offset' => '0',
    'more_link' => 0,
    'feed_icons' => 0,
    'panel_args' => 0,
    'link_to_view' => 0,
    'args' => '',
    'url' => '',
    'display' => 'default',
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
  $pane->uuid = '78c5a068-4dec-4d2a-a2f3-42fad0850e50';
  $display->content['new-78c5a068-4dec-4d2a-a2f3-42fad0850e50'] = $pane;
  $display->panels['complementary'][0] = 'new-78c5a068-4dec-4d2a-a2f3-42fad0850e50';
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
  $export['node_view_destination_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'sections__news_section';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'dpipanels_context';
  $handler->weight = -26;
  $handler->conf = array(
    'no_blocks' => 0,
    'pipeline' => 'dpiviewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'title' => 'News section',
    'name' => 'news_section',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'section',
          'settings' => array(
            'vid' => '3',
            1 => array(),
            2 => array(),
            5 => array(),
            4 => array(),
            6 => array(),
            7 => array(),
            8 => array(),
            9 => array(),
            11 => array(),
            10 => array(),
            12 => array(),
            13 => array(),
            3 => array(
              1 => '1',
            ),
          ),
          'context' => 'argument_destination_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
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
  $display->uuid = 'b0e4c035-a820-4e54-97bf-53e7777382d9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f44ebb06-f9a1-44f3-9e34-753c03304be5';
    $pane->panel = 'maincontentleft';
    $pane->type = 'targetblock';
    $pane->subtype = 'main_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'node_count_for_targetblockinstance' => '10',
      'use_pager' => 1,
      'use_split' => 0,
      'node_count_for_targetblock' => NULL,
      'offset' => NULL,
      'split_delta' => '',
      'hide_pager' => 0,
      'pager_id' => NULL,
      'pager_type' => 'mini',
      'rank_max_value' => '5',
      'template_rule' => NULL,
      '' => NULL,
      'override_by_rank' => 0,
      'context' => 'argument_destination_1',
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f44ebb06-f9a1-44f3-9e34-753c03304be5';
    $display->content['new-f44ebb06-f9a1-44f3-9e34-753c03304be5'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-f44ebb06-f9a1-44f3-9e34-753c03304be5';
    $pane = new stdClass();
    $pane->pid = 'new-ceb459ad-e61d-49a7-a371-56b8b055e80b';
    $pane->panel = 'subcontentleft';
    $pane->type = 'targetblock';
    $pane->subtype = 'extra_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'node_count_for_targetblockinstance' => '10',
      'use_pager' => 1,
      'use_split' => 0,
      'node_count_for_targetblock' => NULL,
      'offset' => NULL,
      'split_delta' => '',
      'hide_pager' => 0,
      'pager_id' => NULL,
      'pager_type' => 'mini',
      'rank_max_value' => '5',
      'template_rule' => NULL,
      '' => NULL,
      'override_by_rank' => 0,
      'context' => 'argument_destination_1',
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'ceb459ad-e61d-49a7-a371-56b8b055e80b';
    $display->content['new-ceb459ad-e61d-49a7-a371-56b8b055e80b'] = $pane;
    $display->panels['subcontentleft'][0] = 'new-ceb459ad-e61d-49a7-a371-56b8b055e80b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-f44ebb06-f9a1-44f3-9e34-753c03304be5';
  $handler->conf['display'] = $display;
  $export['sections_dpisectionspanels_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'sections__dpi_sections_default';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'dpipanels_context';
  $handler->weight = -25;
  $handler->conf = array(
    'no_blocks' => 0,
    'pipeline' => 'dpiviewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'title' => 'Dpi Sections Default',
    'name' => 'dpi_sections_default',
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
  $display->uuid = '14d1fa9e-c2be-4dc4-bf9e-ed833dc6c15d';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-45a362a4-478c-467f-97d3-8e851ac28c95';
    $pane->panel = 'complementary';
    $pane->type = 'targetblock';
    $pane->subtype = 'secondary_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'node_count_for_targetblockinstance' => '10',
      'use_pager' => 1,
      'use_split' => 0,
      'node_count_for_targetblock' => NULL,
      'offset' => NULL,
      'split_delta' => '',
      'hide_pager' => 0,
      'pager_id' => NULL,
      'pager_type' => 'mini',
      'rank_max_value' => '5',
      'template_rule' => NULL,
      '' => NULL,
      'override_by_rank' => 0,
      'context' => 'argument_destination_1',
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '45a362a4-478c-467f-97d3-8e851ac28c95';
    $display->content['new-45a362a4-478c-467f-97d3-8e851ac28c95'] = $pane;
    $display->panels['complementary'][0] = 'new-45a362a4-478c-467f-97d3-8e851ac28c95';
    $pane = new stdClass();
    $pane->pid = 'new-93932246-6cb6-4de4-bd20-8dddb97c4f76';
    $pane->panel = 'maincontentleft';
    $pane->type = 'targetblock';
    $pane->subtype = 'main_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'node_count_for_targetblockinstance' => '10',
      'use_pager' => 1,
      'use_split' => 0,
      'node_count_for_targetblock' => NULL,
      'offset' => NULL,
      'split_delta' => '',
      'hide_pager' => 0,
      'pager_id' => NULL,
      'pager_type' => 'mini',
      'rank_max_value' => '5',
      'template_rule' => NULL,
      '' => NULL,
      'override_by_rank' => 0,
      'context' => 'argument_destination_1',
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '93932246-6cb6-4de4-bd20-8dddb97c4f76';
    $display->content['new-93932246-6cb6-4de4-bd20-8dddb97c4f76'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-93932246-6cb6-4de4-bd20-8dddb97c4f76';
    $pane = new stdClass();
    $pane->pid = 'new-f96bad65-cf74-4565-9de1-e7a2b7b67ca7';
    $pane->panel = 'subcontentleft';
    $pane->type = 'targetblock';
    $pane->subtype = 'extra_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'node_count_for_targetblockinstance' => '10',
      'use_pager' => 1,
      'use_split' => 0,
      'node_count_for_targetblock' => NULL,
      'offset' => NULL,
      'split_delta' => '',
      'hide_pager' => 0,
      'pager_id' => NULL,
      'pager_type' => 'mini',
      'rank_max_value' => '5',
      'template_rule' => NULL,
      '' => NULL,
      'override_by_rank' => 0,
      'context' => 'argument_destination_1',
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f96bad65-cf74-4565-9de1-e7a2b7b67ca7';
    $display->content['new-f96bad65-cf74-4565-9de1-e7a2b7b67ca7'] = $pane;
    $display->panels['subcontentleft'][0] = 'new-f96bad65-cf74-4565-9de1-e7a2b7b67ca7';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-93932246-6cb6-4de4-bd20-8dddb97c4f76';
  $handler->conf['display'] = $display;
  $export['sections_dpisectionspanels_context_2'] = $handler;
  
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context';
  $handler->task = 'term_view';
  $handler->export_type=EXPORT_IN_DATABASE;

  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
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
  $display->title = '%term:name';
  $display->uuid = '27e39610-f388-4227-9ce6-2051cb4c698f';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-ce0b7711-0084-4389-b3e9-163fb25d8acd';
  $pane->panel = 'contenttop';
  $pane->type = 'views';
  $pane->subtype = 'taxonomy_term';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_pager_settings' => 0,
    'use_pager' => 1,
    'nodes_per_page' => '0',
    'pager_id' => '0',
    'offset' => '0',
    'more_link' => 0,
    'feed_icons' => 0,
    'panel_args' => 0,
    'link_to_view' => 0,
    'args' => '',
    'url' => '',
    'display' => 'default',
    'context' => array(
      0 => 'argument_term_1.tid',
      1 => '',
    ),
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
  $pane->uuid = 'ce0b7711-0084-4389-b3e9-163fb25d8acd';
  $display->content['new-ce0b7711-0084-4389-b3e9-163fb25d8acd'] = $pane;
  $display->panels['contenttop'][0] = 'new-ce0b7711-0084-4389-b3e9-163fb25d8acd';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ce0b7711-0084-4389-b3e9-163fb25d8acd';
  $handler->conf['display'] = $display;
  $export['term_view_panel_context'] = $handler;

  return $export;
}
