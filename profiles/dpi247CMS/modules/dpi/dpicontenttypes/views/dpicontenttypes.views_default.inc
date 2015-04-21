<?php

/**
 * 
 */
function dpicontenttypes_views_default_views() {  
  $export = array();
  
  $view = new view();
  $view->name = 'custom_admin_content';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Custom content management';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Custom content management';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Rechercher';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'nid' => 'nid',
    'field_displaytitle' => 'field_displaytitle',
    'field_editorialpublicationdate' => 'field_editorialpublicationdate',
    'type' => 'type',
    'field_destinations' => 'field_destinations',
    'field_packagelayout' => 'field_packagelayout',
    'field_packagetype' => 'field_packagetype',
    'comment' => 'comment',
    'status' => 'status',
    'sticky' => 'sticky',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'field_editorialpublicationdate';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_displaytitle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_editorialpublicationdate' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_destinations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_packagelayout' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_packagetype' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'comment' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sticky' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => ' - ',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => ' - ',
      'empty_column' => 0,
    ),
  );
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::12' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Field: Display Title */
  $handler->display->display_options['fields']['field_displaytitle']['id'] = 'field_displaytitle';
  $handler->display->display_options['fields']['field_displaytitle']['table'] = 'field_data_field_displaytitle';
  $handler->display->display_options['fields']['field_displaytitle']['field'] = 'field_displaytitle';
  $handler->display->display_options['fields']['field_displaytitle']['label'] = 'Titre du package';
  $handler->display->display_options['fields']['field_displaytitle']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_displaytitle']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['field_displaytitle']['alter']['target'] = '_blank';
  /* Field: Content: Editorial Publication date */
  $handler->display->display_options['fields']['field_editorialpublicationdate']['id'] = 'field_editorialpublicationdate';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['table'] = 'field_data_field_editorialpublicationdate';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['field'] = 'field_editorialpublicationdate';
  $handler->display->display_options['fields']['field_editorialpublicationdate']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Content: Destinations */
  $handler->display->display_options['fields']['field_destinations']['id'] = 'field_destinations';
  $handler->display->display_options['fields']['field_destinations']['table'] = 'field_data_field_destinations';
  $handler->display->display_options['fields']['field_destinations']['field'] = 'field_destinations';
  $handler->display->display_options['fields']['field_destinations']['click_sort_column'] = 'tid';
  $handler->display->display_options['fields']['field_destinations']['type'] = 'ena_destinations_display';
  $handler->display->display_options['fields']['field_destinations']['settings'] = array(
    'wrapper_html_tag' => '- None -',
    'wrapper_html_tag_bootstrap_class' => '- None -',
    'wrapper_html_tag_custom_class' => '',
    'element_html_tag' => '- None -',
    'element_html_tag_bootstrap_class' => '- None -',
    'element_html_tag_custom_class' => '',
    'element_html_link' => 'yes',
    'element_prefixe' => '',
    'element_suffixe' => '',
    'separator_option' => '',
  );
  $handler->display->display_options['fields']['field_destinations']['delta_offset'] = '0';
  /* Field: Content: Package Layout */
  $handler->display->display_options['fields']['field_packagelayout']['id'] = 'field_packagelayout';
  $handler->display->display_options['fields']['field_packagelayout']['table'] = 'field_data_field_packagelayout';
  $handler->display->display_options['fields']['field_packagelayout']['field'] = 'field_packagelayout';
  /* Field: Content: Package Type */
  $handler->display->display_options['fields']['field_packagetype']['id'] = 'field_packagetype';
  $handler->display->display_options['fields']['field_packagetype']['table'] = 'field_data_field_packagetype';
  $handler->display->display_options['fields']['field_packagetype']['field'] = 'field_packagetype';
  $handler->display->display_options['fields']['field_packagetype']['label'] = 'Package type';
  /* Field: Content: Comment status */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'node';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Sticky */
  $handler->display->display_options['fields']['sticky']['id'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['table'] = 'node';
  $handler->display->display_options['fields']['sticky']['field'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['not'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Actions';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
  );
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['group'] = 1;
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['operator_id'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Nid';
  $handler->display->display_options['filters']['nid']['expose']['operator'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
  );
  /* Filter criterion: Field: Display Title (field_displaytitle) */
  $handler->display->display_options['filters']['field_displaytitle_value']['id'] = 'field_displaytitle_value';
  $handler->display->display_options['filters']['field_displaytitle_value']['table'] = 'field_data_field_displaytitle';
  $handler->display->display_options['filters']['field_displaytitle_value']['field'] = 'field_displaytitle_value';
  $handler->display->display_options['filters']['field_displaytitle_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_displaytitle_value']['group'] = 1;
  $handler->display->display_options['filters']['field_displaytitle_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_displaytitle_value']['expose']['operator_id'] = 'field_displaytitle_value_op';
  $handler->display->display_options['filters']['field_displaytitle_value']['expose']['label'] = 'Titre';
  $handler->display->display_options['filters']['field_displaytitle_value']['expose']['operator'] = 'field_displaytitle_value_op';
  $handler->display->display_options['filters']['field_displaytitle_value']['expose']['identifier'] = 'field_displaytitle_value';
  $handler->display->display_options['filters']['field_displaytitle_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['filters']['field_displaytitle_value']['group_info']['label'] = 'Display Title (field_displaytitle)';
  $handler->display->display_options['filters']['field_displaytitle_value']['group_info']['identifier'] = 'field_displaytitle_value';
  $handler->display->display_options['filters']['field_displaytitle_value']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['field_displaytitle_value']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type de contenu';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
  );
  /* Filter criterion: Content: Package Layout (field_packagelayout) */
  $handler->display->display_options['filters']['field_packagelayout_tid']['id'] = 'field_packagelayout_tid';
  $handler->display->display_options['filters']['field_packagelayout_tid']['table'] = 'field_data_field_packagelayout';
  $handler->display->display_options['filters']['field_packagelayout_tid']['field'] = 'field_packagelayout_tid';
  $handler->display->display_options['filters']['field_packagelayout_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_packagelayout_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_packagelayout_tid']['expose']['operator_id'] = 'field_packagelayout_tid_op';
  $handler->display->display_options['filters']['field_packagelayout_tid']['expose']['label'] = 'Package Layout';
  $handler->display->display_options['filters']['field_packagelayout_tid']['expose']['operator'] = 'field_packagelayout_tid_op';
  $handler->display->display_options['filters']['field_packagelayout_tid']['expose']['identifier'] = 'field_packagelayout_tid';
  $handler->display->display_options['filters']['field_packagelayout_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
  );
  $handler->display->display_options['filters']['field_packagelayout_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_packagelayout_tid']['vocabulary'] = 'packagelayout';
  /* Filter criterion: Content: Package Type (field_packagetype) */
  $handler->display->display_options['filters']['field_packagetype_tid']['id'] = 'field_packagetype_tid';
  $handler->display->display_options['filters']['field_packagetype_tid']['table'] = 'field_data_field_packagetype';
  $handler->display->display_options['filters']['field_packagetype_tid']['field'] = 'field_packagetype_tid';
  $handler->display->display_options['filters']['field_packagetype_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_packagetype_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_packagetype_tid']['expose']['operator_id'] = 'field_packagetype_tid_op';
  $handler->display->display_options['filters']['field_packagetype_tid']['expose']['label'] = 'Package Type';
  $handler->display->display_options['filters']['field_packagetype_tid']['expose']['operator'] = 'field_packagetype_tid_op';
  $handler->display->display_options['filters']['field_packagetype_tid']['expose']['identifier'] = 'field_packagetype_tid';
  $handler->display->display_options['filters']['field_packagetype_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
  );
  $handler->display->display_options['filters']['field_packagetype_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_packagetype_tid']['vocabulary'] = 'package_type';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content';
  
  $export['custom_admin_content'] = $view;
  
  return $export;
}
