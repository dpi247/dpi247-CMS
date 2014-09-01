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

















/*

  $handler = new stdClass();
$handler->disabled = FALSE; 
$handler->api_version = 1;
$handler->name = 'sections_dpisectionspanels_context';
$handler->task = 'sections';
$handler->subtask = '';
$handler->handler = 'dpisectionspanels_context';
$handler->weight = -29;
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
$display->uuid = 'ff6125d8-acba-4d31-b063-03d525c2ea33';
$display->content = array();
$display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-0845b58e-7cf2-4ba7-a594-5b9d2f616ec9';
  $pane->panel = 'complementary';
  $pane->type = 'targetblock';
  $pane->subtype = 'secondary_content';
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
  $pane->uuid = '0845b58e-7cf2-4ba7-a594-5b9d2f616ec9';
  $display->content['new-0845b58e-7cf2-4ba7-a594-5b9d2f616ec9'] = $pane;
  $display->panels['complementary'][0] = 'new-0845b58e-7cf2-4ba7-a594-5b9d2f616ec9';
  $pane = new stdClass();
  $pane->pid = 'new-9bb950ea-cdd3-4e0c-863b-f215a3a0c812';
  $pane->panel = 'maincontentleft';
  $pane->type = 'targetblock';
  $pane->subtype = 'main_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'node_count_for_targetblockinstance' => '10',
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
  $pane->uuid = '9bb950ea-cdd3-4e0c-863b-f215a3a0c812';
  $display->content['new-9bb950ea-cdd3-4e0c-863b-f215a3a0c812'] = $pane;
  $display->panels['maincontentleft'][0] = 'new-9bb950ea-cdd3-4e0c-863b-f215a3a0c812';
  $pane = new stdClass();
  $pane->pid = 'new-7edc5004-b928-49bd-825d-32f9edb0f6fb';
  $pane->panel = 'subcontentleft';
  $pane->type = 'targetblock';
  $pane->subtype = 'extra_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'node_count_for_targetblockinstance' => '10',
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
  $pane->uuid = '7edc5004-b928-49bd-825d-32f9edb0f6fb';
  $display->content['new-7edc5004-b928-49bd-825d-32f9edb0f6fb'] = $pane;
  $display->panels['subcontentleft'][0] = 'new-7edc5004-b928-49bd-825d-32f9edb0f6fb';
$display->hide_title = PANELS_TITLE_FIXED;
$display->title_pane = 'new-9bb950ea-cdd3-4e0c-863b-f215a3a0c812';
$handler->conf['display'] = $display;
  

  $export['sections_destination_context'] = $handler;










  
  
  
  $handler = new stdClass();
  $handler->disabled = FALSE;
  $handler->api_version = 1;
  $handler->name = 'sections_destination_context';
  $handler->task = 'sections';
  $handler->subtask = '';
  $handler->handler = 'dpisectionspanels_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Politics',
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
              2 => '2',
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
  $display->uuid = '3d087145-4da0-44c0-961f-96e20cc013ba';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-7c908f29-c267-4d69-b569-2398af696305';
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
  $pane->uuid = '7c908f29-c267-4d69-b569-2398af696305';
  $display->content['new-7c908f29-c267-4d69-b569-2398af696305'] = $pane;
  $display->panels['contentbottom'][0] = 'new-7c908f29-c267-4d69-b569-2398af696305';
  $pane = new stdClass();
  $pane->pid = 'new-72f64808-6f2a-4bd8-a09b-0a01a3a23c58';
  $pane->panel = 'contenttop';
  $pane->type = 'targetblock';
  $pane->subtype = 'main_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'node_count_for_targetblockinstance' => '15',
    'use_pager' => 0,
    'use_split' => 0,
    'node_count_for_targetblock' => NULL,
    'offset' => NULL,
    'split_delta' => '',
    'hide_pager' => 0,
    'pager_id' => NULL,
    'pager_type' => 'mini',
    'rank_max_value' => '10',
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
  $pane->uuid = '72f64808-6f2a-4bd8-a09b-0a01a3a23c58';
  $display->content['new-72f64808-6f2a-4bd8-a09b-0a01a3a23c58'] = $pane;
  $display->panels['contenttop'][0] = 'new-72f64808-6f2a-4bd8-a09b-0a01a3a23c58';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  
  
  $export['sections_destination_context_politics'] = $handler;
*/

  return $export;
}
