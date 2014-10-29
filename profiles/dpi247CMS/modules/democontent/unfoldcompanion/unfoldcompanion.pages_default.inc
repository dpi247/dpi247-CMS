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
  $handler->export_type=EXPORT_IN_DATABASE;

  $handler->name = 'sections_dpisectionspanels_context';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'destination_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Finance',
    'no_blocks' => 0,
    'pipeline' => 'dpiviewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'section_default_target' => '',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'term',
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
      'mainContent' => NULL,
      'complementary' => NULL,
      'subcontentleft' => NULL,
      'subcontentright' => NULL,
      'contentbottom' => NULL,
      'maincontentleft' => NULL,
      'maincontentright' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '367d75b6-fe8b-41fb-a103-bf7f042d6473';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-e282bfa3-d2a8-46b0-9128-763def3dcb48';
  $pane->panel = 'contentbottom';
  $pane->type = 'targetblock';
  $pane->subtype = 'extra_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'node_count_for_targetblockinstance' => '5',
    'use_pager' => 0,
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
  $pane->uuid = 'e282bfa3-d2a8-46b0-9128-763def3dcb48';
  $display->content['new-e282bfa3-d2a8-46b0-9128-763def3dcb48'] = $pane;
  $display->panels['contentbottom'][0] = 'new-e282bfa3-d2a8-46b0-9128-763def3dcb48';
  $pane = new stdClass();
  $pane->pid = 'new-3dcbd178-8633-4ef6-9048-ac8f702601ef';
  $pane->panel = 'subcontentleft';
  $pane->type = 'targetblock';
  $pane->subtype = 'main_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'node_count_for_targetblockinstance' => '10',
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
    'override_title_text' => '%destination:name',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '3dcbd178-8633-4ef6-9048-ac8f702601ef';
  $display->content['new-3dcbd178-8633-4ef6-9048-ac8f702601ef'] = $pane;
  $display->panels['subcontentleft'][0] = 'new-3dcbd178-8633-4ef6-9048-ac8f702601ef';
  $pane = new stdClass();
  $pane->pid = 'new-b1fc2383-e105-48c9-af5b-6302c1c7cad9';
  $pane->panel = 'subcontentright';
  $pane->type = 'custom_content';
  $pane->subtype = 'stock_chart';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'admin_title' => '',
    'title' => '',
    'body' => '',
    'format' => 'filtered_html',
    'substitute' => TRUE,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'b1fc2383-e105-48c9-af5b-6302c1c7cad9';
  $display->content['new-b1fc2383-e105-48c9-af5b-6302c1c7cad9'] = $pane;
  $display->panels['subcontentright'][0] = 'new-b1fc2383-e105-48c9-af5b-6302c1c7cad9';
  $pane = new stdClass();
  $pane->pid = 'new-f9b2d8f0-1313-4323-a9e4-d9af4c909e5e';
  $pane->panel = 'subcontentright';
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
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'f9b2d8f0-1313-4323-a9e4-d9af4c909e5e';
  $display->content['new-f9b2d8f0-1313-4323-a9e4-d9af4c909e5e'] = $pane;
  $display->panels['subcontentright'][1] = 'new-f9b2d8f0-1313-4323-a9e4-d9af4c909e5e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['sections_dpisectionspanels_context'] = $handler;













  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'sections_dpisectionspanels_context';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'dpisectionspanels_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Dpi Section Panels',
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
  $display->uuid = 'f8fdf884-f8da-48d0-b7e4-42dc43f39d06';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-d4213a77-76db-4bd8-9804-e51a1f5db9da';
  $pane->panel = 'complementary';
  $pane->type = 'targetblock';
  $pane->subtype = 'secondary_content';
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
    'override_title_text' => 'BNP s\'implique',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'd4213a77-76db-4bd8-9804-e51a1f5db9da';
  $display->content['new-d4213a77-76db-4bd8-9804-e51a1f5db9da'] = $pane;
  $display->panels['complementary'][0] = 'new-d4213a77-76db-4bd8-9804-e51a1f5db9da';
  $pane = new stdClass();
  $pane->pid = 'new-ef72555d-9dd9-405c-ab3d-0201e6fa99f1';
  $pane->panel = 'maincontentleft';
  $pane->type = 'targetblock';
  $pane->subtype = 'main_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'node_count_for_targetblockinstance' => '10',
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
    'override_title_text' => '%destination:name',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'ef72555d-9dd9-405c-ab3d-0201e6fa99f1';
  $display->content['new-ef72555d-9dd9-405c-ab3d-0201e6fa99f1'] = $pane;
  $display->panels['maincontentleft'][0] = 'new-ef72555d-9dd9-405c-ab3d-0201e6fa99f1';
  $pane = new stdClass();
  $pane->pid = 'new-4f74a812-8068-46d4-bc26-4e3e0cb504ef';
  $pane->panel = 'maincontentright';
  $pane->type = 'custom_content';
  $pane->subtype = 'bnp_on_twitter';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'admin_title' => '',
    'title' => '',
    'body' => '',
    'format' => 'filtered_html',
    'substitute' => TRUE,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '4f74a812-8068-46d4-bc26-4e3e0cb504ef';
  $display->content['new-4f74a812-8068-46d4-bc26-4e3e0cb504ef'] = $pane;
  $display->panels['maincontentright'][0] = 'new-4f74a812-8068-46d4-bc26-4e3e0cb504ef';
  $pane = new stdClass();
  $pane->pid = 'new-642ece8d-f68b-4713-b810-426496552a1a';
  $pane->panel = 'subcontentleft';
  $pane->type = 'targetblock';
  $pane->subtype = 'extra_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'node_count_for_targetblockinstance' => '10',
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
    'override_title_text' => '%destination:name: A voir aussi',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '642ece8d-f68b-4713-b810-426496552a1a';
  $display->content['new-642ece8d-f68b-4713-b810-426496552a1a'] = $pane;
  $display->panels['subcontentleft'][0] = 'new-642ece8d-f68b-4713-b810-426496552a1a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';

  $handler->conf['display'] = $display;
  $export['sections_dpisectionspanels_context'] = $handler;

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
