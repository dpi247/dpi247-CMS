<?php
/**
 * @file
 * Provide views data and handlers for DPI Dashboard module
 */

/**
 * Implementation of hook_views_data()
 */
function dpidashboard_views_data() {

	/* ---------------
	
	latest_published_by_someone
	
	------------------ */

  	$view = new view();
	$view->name = 'latest_published_by_someone';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'Latest Published by Someone';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Latest Published by Someone';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['pager']['options']['id'] = '0';
	$handler->display->display_options['pager']['options']['quantity'] = '9';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	  'created' => 'created',
	  'path' => 'path',
	);
	$handler->display->display_options['style_options']['default'] = 'created';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
		'sortable' => 0,
		'default_sort_order' => 'asc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'created' => array(
		'sortable' => 0,
		'default_sort_order' => 'desc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'path' => array(
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	);
	/* Relationship: Content: Author */
	$handler->display->display_options['relationships']['uid']['id'] = 'uid';
	$handler->display->display_options['relationships']['uid']['table'] = 'node';
	$handler->display->display_options['relationships']['uid']['field'] = 'uid';
	/* Field: User: Name */
	$handler->display->display_options['fields']['name']['id'] = 'name';
	$handler->display->display_options['fields']['name']['table'] = 'users';
	$handler->display->display_options['fields']['name']['field'] = 'name';
	$handler->display->display_options['fields']['name']['relationship'] = 'uid';
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	/* Field: Content: Post date */
	$handler->display->display_options['fields']['created']['id'] = 'created';
	$handler->display->display_options['fields']['created']['table'] = 'node';
	$handler->display->display_options['fields']['created']['field'] = 'created';
	$handler->display->display_options['fields']['created']['date_format'] = 'short';
	/* Field: Content: Path */
	$handler->display->display_options['fields']['path']['id'] = 'path';
	$handler->display->display_options['fields']['path']['table'] = 'node';
	$handler->display->display_options['fields']['path']['field'] = 'path';
	$handler->display->display_options['fields']['path']['alter']['alter_text'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['text'] = 'Edit';
	$handler->display->display_options['fields']['path']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['path'] = '[path]/edit';
	$handler->display->display_options['fields']['path']['alter']['alt'] = 'Go';
	$handler->display->display_options['fields']['path']['absolute'] = TRUE;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Author uid */
	$handler->display->display_options['filters']['uid']['id'] = 'uid';
	$handler->display->display_options['filters']['uid']['table'] = 'node';
	$handler->display->display_options['filters']['uid']['field'] = 'uid';
	$handler->display->display_options['filters']['uid']['value'] = '';
	$handler->display->display_options['filters']['uid']['exposed'] = TRUE;
	$handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
	$handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
	$handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
	$handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
	$handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
	  2 => '2',
	  1 => 0,
	  3 => 0,
	  4 => 0,
	  6 => 0,
	  5 => 0,
	);
	
	/* Display: Content pane */
	$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
	$handler->display->display_options['inherit_panels_path'] = '1';

	// Add view to list of views to provide.
	$views[$view->name] = $view;
	
	/* ---------------
	
	latest_edited
	
	------------------ */
	
	$view = new view();
	$view->name = 'latest_edited';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'Latest Edited';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Latest Edited';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['pager']['options']['id'] = '0';
	$handler->display->display_options['pager']['options']['quantity'] = '9';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	  'created' => 'created',
	  'path' => 'path',
	);
	$handler->display->display_options['style_options']['default'] = 'created';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
		'sortable' => 0,
		'default_sort_order' => 'asc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'created' => array(
		'sortable' => 1,
		'default_sort_order' => 'desc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'path' => array(
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	);
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	/* Field: Content revision: Updated date */
	$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
	$handler->display->display_options['fields']['timestamp']['table'] = 'node_revision';
	$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
	$handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
	/* Field: Content: Path */
	$handler->display->display_options['fields']['path']['id'] = 'path';
	$handler->display->display_options['fields']['path']['table'] = 'node';
	$handler->display->display_options['fields']['path']['field'] = 'path';
	$handler->display->display_options['fields']['path']['alter']['alter_text'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['text'] = 'Edit';
	$handler->display->display_options['fields']['path']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['path'] = '[path]/edit';
	$handler->display->display_options['fields']['path']['alter']['alt'] = 'Go';
	$handler->display->display_options['fields']['path']['absolute'] = TRUE;
	/* Sort criterion: Content revision: Updated date */
	$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
	$handler->display->display_options['sorts']['timestamp']['table'] = 'node_revision';
	$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
	$handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	
	/* Display: Content pane */
	$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
	$handler->display->display_options['inherit_panels_path'] = '1';
	
	// Add view to list of views to provide.
	$views[$view->name] = $view;
	
	/* ---------------
	
	latest_edited_by_me
	
	------------------ */
	
	$view = new view();
	$view->name = 'latest_edited_by_me';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'Latest Edited by Me';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Latest Edited by Me';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['pager']['options']['id'] = '0';
	$handler->display->display_options['pager']['options']['quantity'] = '9';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	  'created' => 'created',
	  'path' => 'path',
	);
	$handler->display->display_options['style_options']['default'] = 'created';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
		'sortable' => 0,
		'default_sort_order' => 'asc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'created' => array(
		'sortable' => 1,
		'default_sort_order' => 'desc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'path' => array(
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	);
	/* Relationship: Content: Author */
	$handler->display->display_options['relationships']['uid']['id'] = 'uid';
	$handler->display->display_options['relationships']['uid']['table'] = 'node';
	$handler->display->display_options['relationships']['uid']['field'] = 'uid';
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	/* Field: Content revision: Updated date */
	$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
	$handler->display->display_options['fields']['timestamp']['table'] = 'node_revision';
	$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
	$handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
	/* Field: Content: Path */
	$handler->display->display_options['fields']['path']['id'] = 'path';
	$handler->display->display_options['fields']['path']['table'] = 'node';
	$handler->display->display_options['fields']['path']['field'] = 'path';
	$handler->display->display_options['fields']['path']['alter']['alter_text'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['text'] = 'Edit';
	$handler->display->display_options['fields']['path']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['path'] = '[path]/edit';
	$handler->display->display_options['fields']['path']['alter']['alt'] = 'Go';
	$handler->display->display_options['fields']['path']['absolute'] = TRUE;
	/* Sort criterion: Content revision: Updated date */
	$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
	$handler->display->display_options['sorts']['timestamp']['table'] = 'node_revision';
	$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
	$handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: User: Current */
	$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
	$handler->display->display_options['filters']['uid_current']['table'] = 'users';
	$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
	$handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
	$handler->display->display_options['filters']['uid_current']['value'] = '1';
	
	/* Display: Content pane */
	$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
	$handler->display->display_options['inherit_panels_path'] = '1';
	
	// Add view to list of views to provide.
	$views[$view->name] = $view;
	
	
	/* ---------------
	
	latest_published
	
	------------------ */
	
	$view = new view();
	$view->name = 'latest_published';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'Latest Published';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Latest Published';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['pager']['options']['id'] = '0';
	$handler->display->display_options['pager']['options']['quantity'] = '9';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	  'created' => 'created',
	  'path' => 'path',
	);
	$handler->display->display_options['style_options']['default'] = 'created';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
		'sortable' => 0,
		'default_sort_order' => 'asc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'created' => array(
		'sortable' => 0,
		'default_sort_order' => 'desc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'path' => array(
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	);
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	/* Field: Content: Post date */
	$handler->display->display_options['fields']['created']['id'] = 'created';
	$handler->display->display_options['fields']['created']['table'] = 'node';
	$handler->display->display_options['fields']['created']['field'] = 'created';
	$handler->display->display_options['fields']['created']['date_format'] = 'short';
	/* Field: Content: Path */
	$handler->display->display_options['fields']['path']['id'] = 'path';
	$handler->display->display_options['fields']['path']['table'] = 'node';
	$handler->display->display_options['fields']['path']['field'] = 'path';
	$handler->display->display_options['fields']['path']['alter']['alter_text'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['text'] = 'Edit';
	$handler->display->display_options['fields']['path']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['path'] = '[path]/edit';
	$handler->display->display_options['fields']['path']['alter']['alt'] = 'Go';
	$handler->display->display_options['fields']['path']['absolute'] = TRUE;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	
	/* Display: Content pane */
	$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
	$handler->display->display_options['inherit_panels_path'] = '1';
		
	// Add view to list of views to provide.
	$views[$view->name] = $view;
	
	/* ---------------
	
	latest_published_by_me
	
	------------------ */
	
	$view = new view();
	$view->name = 'latest_published_by_me';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'Latest Published by Me';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Latest Published by Me';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['pager']['options']['id'] = '0';
	$handler->display->display_options['pager']['options']['quantity'] = '9';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	  'created' => 'created',
	  'path' => 'path',
	);
	$handler->display->display_options['style_options']['default'] = 'created';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
		'sortable' => 0,
		'default_sort_order' => 'asc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'created' => array(
		'sortable' => 0,
		'default_sort_order' => 'desc',
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	  'path' => array(
		'align' => '',
		'separator' => '',
		'empty_column' => 0,
	  ),
	);
	/* Relationship: Content: Author */
	$handler->display->display_options['relationships']['uid']['id'] = 'uid';
	$handler->display->display_options['relationships']['uid']['table'] = 'node';
	$handler->display->display_options['relationships']['uid']['field'] = 'uid';
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	/* Field: Content: Post date */
	$handler->display->display_options['fields']['created']['id'] = 'created';
	$handler->display->display_options['fields']['created']['table'] = 'node';
	$handler->display->display_options['fields']['created']['field'] = 'created';
	$handler->display->display_options['fields']['created']['date_format'] = 'short';
	/* Field: Content: Path */
	$handler->display->display_options['fields']['path']['id'] = 'path';
	$handler->display->display_options['fields']['path']['table'] = 'node';
	$handler->display->display_options['fields']['path']['field'] = 'path';
	$handler->display->display_options['fields']['path']['alter']['alter_text'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['text'] = 'Edit';
	$handler->display->display_options['fields']['path']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['path'] = '[path]/edit';
	$handler->display->display_options['fields']['path']['alter']['alt'] = 'Go';
	$handler->display->display_options['fields']['path']['absolute'] = TRUE;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: User: Current */
	$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
	$handler->display->display_options['filters']['uid_current']['table'] = 'users';
	$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
	$handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
	$handler->display->display_options['filters']['uid_current']['value'] = '1';
	$handler->display->display_options['filters']['uid_current']['group'] = 1;
	
	/* Display: Content pane */
	$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
	$handler->display->display_options['inherit_panels_path'] = '1';
	
	// Add view to list of views to provide.
	$views[$view->name] = $view;
	
	
	// At the end, return array of default views.
	return $views;

}

