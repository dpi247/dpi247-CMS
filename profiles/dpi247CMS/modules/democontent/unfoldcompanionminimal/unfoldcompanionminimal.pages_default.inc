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
              'package_gallery' => 'package_gallery',
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
  $display->layout = 'twocol_66_33';
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
      'left' => NULL,
      'right' => NULL,
      'tag1' => NULL,
      'tag2' => NULL,
      'tag3' => NULL,
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
    $pane->panel = 'left';
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
    $display->panels['left'][0] = 'new-c1ec8cc7-5762-4e13-8daa-85dfe83d04ee';
    $pane = new stdClass();
    $pane->pid = 'new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
    $pane->panel = 'left';
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
    $display->panels['left'][1] = 'new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
    $pane = new stdClass();
    $pane->pid = 'new-e823994e-8722-4ce4-9cba-a91b76dbaed3';
    $pane->panel = 'left';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'e823994e-8722-4ce4-9cba-a91b76dbaed3';
    $display->content['new-e823994e-8722-4ce4-9cba-a91b76dbaed3'] = $pane;
    $display->panels['left'][2] = 'new-e823994e-8722-4ce4-9cba-a91b76dbaed3';
    $pane = new stdClass();
    $pane->pid = 'new-2e2d254e-a3a6-488d-b96a-47048ae4c674';
    $pane->panel = 'left';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '2e2d254e-a3a6-488d-b96a-47048ae4c674';
    $display->content['new-2e2d254e-a3a6-488d-b96a-47048ae4c674'] = $pane;
    $display->panels['left'][3] = 'new-2e2d254e-a3a6-488d-b96a-47048ae4c674';
    $pane = new stdClass();
    $pane->pid = 'new-02ccb594-0457-4441-bec1-e52d1d7eb831';
    $pane->panel = 'left';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '02ccb594-0457-4441-bec1-e52d1d7eb831';
    $display->content['new-02ccb594-0457-4441-bec1-e52d1d7eb831'] = $pane;
    $display->panels['left'][4] = 'new-02ccb594-0457-4441-bec1-e52d1d7eb831';
    $pane = new stdClass();
    $pane->pid = 'new-ca73d0ed-d6b0-46cd-9426-82e00d3d736e';
    $pane->panel = 'left';
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'ca73d0ed-d6b0-46cd-9426-82e00d3d736e';
    $display->content['new-ca73d0ed-d6b0-46cd-9426-82e00d3d736e'] = $pane;
    $display->panels['left'][5] = 'new-ca73d0ed-d6b0-46cd-9426-82e00d3d736e';
    $pane = new stdClass();
    $pane->pid = 'new-fffcb517-bc5e-478e-9841-a55353077d49';
    $pane->panel = 'left';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_byline';
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = 'fffcb517-bc5e-478e-9841-a55353077d49';
    $display->content['new-fffcb517-bc5e-478e-9841-a55353077d49'] = $pane;
    $display->panels['left'][6] = 'new-fffcb517-bc5e-478e-9841-a55353077d49';
    $pane = new stdClass();
    $pane->pid = 'new-b1680315-a3a7-4afe-90f6-60951d073ce5';
    $pane->panel = 'left';
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
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = 'b1680315-a3a7-4afe-90f6-60951d073ce5';
    $display->content['new-b1680315-a3a7-4afe-90f6-60951d073ce5'] = $pane;
    $display->panels['left'][7] = 'new-b1680315-a3a7-4afe-90f6-60951d073ce5';
    $pane = new stdClass();
    $pane->pid = 'new-2d5a2b54-dc28-4ff9-92e1-4fe46eab0051';
    $pane->panel = 'left';
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
    $pane->position = 8;
    $pane->locks = array();
    $pane->uuid = '2d5a2b54-dc28-4ff9-92e1-4fe46eab0051';
    $display->content['new-2d5a2b54-dc28-4ff9-92e1-4fe46eab0051'] = $pane;
    $display->panels['left'][8] = 'new-2d5a2b54-dc28-4ff9-92e1-4fe46eab0051';
    $pane = new stdClass();
    $pane->pid = 'new-4920be7f-f3d2-49ac-9a0e-a2fa89d414cb';
    $pane->panel = 'left';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_copyright';
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
    $pane->position = 9;
    $pane->locks = array();
    $pane->uuid = '4920be7f-f3d2-49ac-9a0e-a2fa89d414cb';
    $display->content['new-4920be7f-f3d2-49ac-9a0e-a2fa89d414cb'] = $pane;
    $display->panels['left'][9] = 'new-4920be7f-f3d2-49ac-9a0e-a2fa89d414cb';
    $pane = new stdClass();
    $pane->pid = 'new-3d85dff5-0c42-42ba-bd26-3baa98c39a15';
    $pane->panel = 'left';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_packagelayout';
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
    $pane->position = 10;
    $pane->locks = array();
    $pane->uuid = '3d85dff5-0c42-42ba-bd26-3baa98c39a15';
    $display->content['new-3d85dff5-0c42-42ba-bd26-3baa98c39a15'] = $pane;
    $display->panels['left'][10] = 'new-3d85dff5-0c42-42ba-bd26-3baa98c39a15';
    $pane = new stdClass();
    $pane->pid = 'new-e4f2ced3-001f-45d2-b93e-81a02e82518f';
    $pane->panel = 'right';
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
    $display->panels['right'][0] = 'new-e4f2ced3-001f-45d2-b93e-81a02e82518f';
    $pane = new stdClass();
    $pane->pid = 'new-f446c0c2-0570-49ed-87d2-e339391b1910';
    $pane->panel = 'right';
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
    $display->panels['right'][1] = 'new-f446c0c2-0570-49ed-87d2-e339391b1910';
    $pane = new stdClass();
    $pane->pid = 'new-bbc7c71a-bdab-4b6d-bc03-32eeb1db8d63';
    $pane->panel = 'right';
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
    $display->panels['right'][2] = 'new-bbc7c71a-bdab-4b6d-bc03-32eeb1db8d63';
    $pane = new stdClass();
    $pane->pid = 'new-da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f';
    $pane->panel = 'right';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f';
    $display->content['new-da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f'] = $pane;
    $display->panels['right'][3] = 'new-da8f53ae-8df1-4fdc-a6ed-e8062cf2e73f';
    $pane = new stdClass();
    $pane->pid = 'new-3d627dc9-7602-4441-ba6b-721cafb7336b';
    $pane->panel = 'right';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '3d627dc9-7602-4441-ba6b-721cafb7336b';
    $display->content['new-3d627dc9-7602-4441-ba6b-721cafb7336b'] = $pane;
    $display->panels['right'][4] = 'new-3d627dc9-7602-4441-ba6b-721cafb7336b';
    $pane = new stdClass();
    $pane->pid = 'new-365633df-cb4d-401a-a72e-13d4cc782ecf';
    $pane->panel = 'right';
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '365633df-cb4d-401a-a72e-13d4cc782ecf';
    $display->content['new-365633df-cb4d-401a-a72e-13d4cc782ecf'] = $pane;
    $display->panels['right'][5] = 'new-365633df-cb4d-401a-a72e-13d4cc782ecf';
    $pane = new stdClass();
    $pane->pid = 'new-14c6a0d8-71c6-4157-af89-c8bc01161d9f';
    $pane->panel = 'right';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_atompackage';
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = '14c6a0d8-71c6-4157-af89-c8bc01161d9f';
    $display->content['new-14c6a0d8-71c6-4157-af89-c8bc01161d9f'] = $pane;
    $display->panels['right'][6] = 'new-14c6a0d8-71c6-4157-af89-c8bc01161d9f';
    $pane = new stdClass();
    $pane->pid = 'new-b5ae87a7-18c2-4818-8885-deea64ac245a';
    $pane->panel = 'right';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_authors';
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
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = 'b5ae87a7-18c2-4818-8885-deea64ac245a';
    $display->content['new-b5ae87a7-18c2-4818-8885-deea64ac245a'] = $pane;
    $display->panels['right'][7] = 'new-b5ae87a7-18c2-4818-8885-deea64ac245a';
    $pane = new stdClass();
    $pane->pid = 'new-4e1e914f-3884-4afd-8866-632bdc4bf331';
    $pane->panel = 'right';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_externalurl';
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
    $pane->position = 8;
    $pane->locks = array();
    $pane->uuid = '4e1e914f-3884-4afd-8866-632bdc4bf331';
    $display->content['new-4e1e914f-3884-4afd-8866-632bdc4bf331'] = $pane;
    $display->panels['right'][8] = 'new-4e1e914f-3884-4afd-8866-632bdc4bf331';
    $pane = new stdClass();
    $pane->pid = 'new-f718833c-b57c-4d17-960e-6b4dd73eaf98';
    $pane->panel = 'right';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_restrictedaccess';
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
    $pane->position = 9;
    $pane->locks = array();
    $pane->uuid = 'f718833c-b57c-4d17-960e-6b4dd73eaf98';
    $display->content['new-f718833c-b57c-4d17-960e-6b4dd73eaf98'] = $pane;
    $display->panels['right'][9] = 'new-f718833c-b57c-4d17-960e-6b4dd73eaf98';
    $pane = new stdClass();
    $pane->pid = 'new-8ff05a76-ee30-4279-8a30-5c8777d38e6d';
    $pane->panel = 'tag1';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_concepts';
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
    $pane->uuid = '8ff05a76-ee30-4279-8a30-5c8777d38e6d';
    $display->content['new-8ff05a76-ee30-4279-8a30-5c8777d38e6d'] = $pane;
    $display->panels['tag1'][0] = 'new-8ff05a76-ee30-4279-8a30-5c8777d38e6d';
    $pane = new stdClass();
    $pane->pid = 'new-fc16fb01-102d-48e9-8550-aad586dc20bb';
    $pane->panel = 'tag1';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_entities';
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
    $pane->uuid = 'fc16fb01-102d-48e9-8550-aad586dc20bb';
    $display->content['new-fc16fb01-102d-48e9-8550-aad586dc20bb'] = $pane;
    $display->panels['tag1'][1] = 'new-fc16fb01-102d-48e9-8550-aad586dc20bb';
    $pane = new stdClass();
    $pane->pid = 'new-e35b61ff-5096-4cf0-9a0b-2dabc93b92c0';
    $pane->panel = 'tag2';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_freetags';
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
    $pane->uuid = 'e35b61ff-5096-4cf0-9a0b-2dabc93b92c0';
    $display->content['new-e35b61ff-5096-4cf0-9a0b-2dabc93b92c0'] = $pane;
    $display->panels['tag2'][0] = 'new-e35b61ff-5096-4cf0-9a0b-2dabc93b92c0';
    $pane = new stdClass();
    $pane->pid = 'new-6682db79-84a7-4378-a995-132869de376a';
    $pane->panel = 'tag2';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_iptc';
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
    $pane->uuid = '6682db79-84a7-4378-a995-132869de376a';
    $display->content['new-6682db79-84a7-4378-a995-132869de376a'] = $pane;
    $display->panels['tag2'][1] = 'new-6682db79-84a7-4378-a995-132869de376a';
    $pane = new stdClass();
    $pane->pid = 'new-9d0d1811-ef00-48e6-92c4-5dd2e2a409d8';
    $pane->panel = 'tag3';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_locations';
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
    $pane->uuid = '9d0d1811-ef00-48e6-92c4-5dd2e2a409d8';
    $display->content['new-9d0d1811-ef00-48e6-92c4-5dd2e2a409d8'] = $pane;
    $display->panels['tag3'][0] = 'new-9d0d1811-ef00-48e6-92c4-5dd2e2a409d8';
    $pane = new stdClass();
    $pane->pid = 'new-74f3eb75-e6da-481b-b280-5fdba91ec664';
    $pane->panel = 'tag3';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_persons';
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
    $pane->uuid = '74f3eb75-e6da-481b-b280-5fdba91ec664';
    $display->content['new-74f3eb75-e6da-481b-b280-5fdba91ec664'] = $pane;
    $display->panels['tag3'][1] = 'new-74f3eb75-e6da-481b-b280-5fdba91ec664';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ffa29c66-7eeb-45c5-be81-e2b9453aaefb';
  $handler->conf['display'] = $display;
  $export['node_edit_dpipanels_context'] = $handler;

  return $export;
}
