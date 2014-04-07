<?php
/**
 * @file
 * unfoldcompanionminimal.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function unfoldcompanionminimal_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_dpipanels_context';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'dpipanels_context';
  $handler->weight = 0;
  $handler->conf = array(
    'no_blocks' => 0,
    'pipeline' => 'dpiviewer',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'title' => 'Package edition',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'package' => 'package',
            ),
          ),
          'context' => 'argument_node_edit_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'twocol_bricks';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left_above' => NULL,
      'right_above' => NULL,
      'middle' => NULL,
      'left_below' => NULL,
      'right_below' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '1fb85fbd-93a3-4aa7-960b-61e64c2e6ecb';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-6ca4822a-2131-42ca-8b06-45900916b4f8';
    $pane->panel = 'bottom';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '6ca4822a-2131-42ca-8b06-45900916b4f8';
    $display->content['new-6ca4822a-2131-42ca-8b06-45900916b4f8'] = $pane;
    $display->panels['bottom'][0] = 'new-6ca4822a-2131-42ca-8b06-45900916b4f8';
    $pane = new stdClass();
    $pane->pid = 'new-c1ec8cc7-5762-4e13-8daa-85dfe83d04ee';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_displaytitle';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'c1ec8cc7-5762-4e13-8daa-85dfe83d04ee';
    $display->content['new-c1ec8cc7-5762-4e13-8daa-85dfe83d04ee'] = $pane;
    $display->panels['left_above'][0] = 'new-c1ec8cc7-5762-4e13-8daa-85dfe83d04ee';
    $pane = new stdClass();
    $pane->pid = 'new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
    $pane->panel = 'left_above';
    $pane->type = 'node_form_title';
    $pane->subtype = 'node_form_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
    $display->content['new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb'] = $pane;
    $display->panels['left_above'][1] = 'new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
    $pane = new stdClass();
    $pane->pid = 'new-ca73d0ed-d6b0-46cd-9426-82e00d3d736e';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_textbarette';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
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
    $pane->uuid = 'ca73d0ed-d6b0-46cd-9426-82e00d3d736e';
    $display->content['new-ca73d0ed-d6b0-46cd-9426-82e00d3d736e'] = $pane;
    $display->panels['left_above'][2] = 'new-ca73d0ed-d6b0-46cd-9426-82e00d3d736e';
    $pane = new stdClass();
    $pane->pid = 'new-e823994e-8722-4ce4-9cba-a91b76dbaed3';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_textchapo';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'e823994e-8722-4ce4-9cba-a91b76dbaed3';
    $display->content['new-e823994e-8722-4ce4-9cba-a91b76dbaed3'] = $pane;
    $display->panels['left_above'][3] = 'new-e823994e-8722-4ce4-9cba-a91b76dbaed3';
    $pane = new stdClass();
    $pane->pid = 'new-2e2d254e-a3a6-488d-b96a-47048ae4c674';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_textbody';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '2e2d254e-a3a6-488d-b96a-47048ae4c674';
    $display->content['new-2e2d254e-a3a6-488d-b96a-47048ae4c674'] = $pane;
    $display->panels['left_above'][4] = 'new-2e2d254e-a3a6-488d-b96a-47048ae4c674';
    $pane = new stdClass();
    $pane->pid = 'new-02ccb594-0457-4441-bec1-e52d1d7eb831';
    $pane->panel = 'left_below';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_embededobjects';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '02ccb594-0457-4441-bec1-e52d1d7eb831';
    $display->content['new-02ccb594-0457-4441-bec1-e52d1d7eb831'] = $pane;
    $display->panels['left_below'][0] = 'new-02ccb594-0457-4441-bec1-e52d1d7eb831';
    $pane = new stdClass();
    $pane->pid = 'new-b1680315-a3a7-4afe-90f6-60951d073ce5';
    $pane->panel = 'left_below';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_textforetitle';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'b1680315-a3a7-4afe-90f6-60951d073ce5';
    $display->content['new-b1680315-a3a7-4afe-90f6-60951d073ce5'] = $pane;
    $display->panels['left_below'][1] = 'new-b1680315-a3a7-4afe-90f6-60951d073ce5';
    $pane = new stdClass();
    $pane->pid = 'new-e4f2ced3-001f-45d2-b93e-81a02e82518f';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_destinations';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'e4f2ced3-001f-45d2-b93e-81a02e82518f';
    $display->content['new-e4f2ced3-001f-45d2-b93e-81a02e82518f'] = $pane;
    $display->panels['right_above'][0] = 'new-e4f2ced3-001f-45d2-b93e-81a02e82518f';
    $pane = new stdClass();
    $pane->pid = 'new-f446c0c2-0570-49ed-87d2-e339391b1910';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_autopublish';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'f446c0c2-0570-49ed-87d2-e339391b1910';
    $display->content['new-f446c0c2-0570-49ed-87d2-e339391b1910'] = $pane;
    $display->panels['right_above'][1] = 'new-f446c0c2-0570-49ed-87d2-e339391b1910';
    $pane = new stdClass();
    $pane->pid = 'new-bbc7c71a-bdab-4b6d-bc03-32eeb1db8d63';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_autopublishdate';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
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
    $pane->uuid = 'bbc7c71a-bdab-4b6d-bc03-32eeb1db8d63';
    $display->content['new-bbc7c71a-bdab-4b6d-bc03-32eeb1db8d63'] = $pane;
    $display->panels['right_above'][2] = 'new-bbc7c71a-bdab-4b6d-bc03-32eeb1db8d63';
    $pane = new stdClass();
    $pane->pid = 'new-3d627dc9-7602-4441-ba6b-721cafb7336b';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_editorialpublicationdate';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '3d627dc9-7602-4441-ba6b-721cafb7336b';
    $display->content['new-3d627dc9-7602-4441-ba6b-721cafb7336b'] = $pane;
    $display->panels['right_above'][3] = 'new-3d627dc9-7602-4441-ba6b-721cafb7336b';
    $pane = new stdClass();
    $pane->pid = 'new-da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_editorialupdatedate';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f';
    $display->content['new-da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f'] = $pane;
    $display->panels['right_above'][4] = 'new-da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f';
    $pane = new stdClass();
    $pane->pid = 'new-365633df-cb4d-401a-a72e-13d4cc782ecf';
    $pane->panel = 'right_below';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_linkslists';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '365633df-cb4d-401a-a72e-13d4cc782ecf';
    $display->content['new-365633df-cb4d-401a-a72e-13d4cc782ecf'] = $pane;
    $display->panels['right_below'][0] = 'new-365633df-cb4d-401a-a72e-13d4cc782ecf';
    $pane = new stdClass();
    $pane->pid = 'new-2d5a2b54-dc28-4ff9-92e1-4fe46eab0051';
    $pane->panel = 'right_below';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_textsubtitle';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '2d5a2b54-dc28-4ff9-92e1-4fe46eab0051';
    $display->content['new-2d5a2b54-dc28-4ff9-92e1-4fe46eab0051'] = $pane;
    $display->panels['right_below'][1] = 'new-2d5a2b54-dc28-4ff9-92e1-4fe46eab0051';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
  $handler->conf['display'] = $display;
  $export['node_edit_dpipanels_context'] = $handler;

  return $export;
}
