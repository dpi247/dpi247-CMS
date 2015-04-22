<?php




/**
 * Implements hook_ctools_plugin_post_alter().
 *
 * @see dpicache.esi.panel.inc
 *
 */
function dpicache_ctools_plugin_post_alter(&$plugin, &$info) {
	// Every time the configuration of a panel is saved, the default hook on the
	// panel-context is invoked.  Replace that hook, in order to check for the
	// presence of ESI panes, and switch the rendering pipeline if necessary.
	if ($plugin['name'] == 'panel_context' ||$plugin['name'] =='dpipanels_context' || $plugin['name']=='dpisectionspanels_context') {
		// Replace the default 'save' operation - panels_panel_context_save();
		$plugin['save'] = 'dpicache__panel_context_save';
	}
}


/**
 * Implements hook_form_FORM_ID_alter().
 */
function dpicache_form_panels_panel_context_edit_settings_alter(&$form, &$form_state) {
	// The 'ESI' panels renderer should automatically replace the 'standard'
	// panels renderer when there are panes on a panel which are handled by ESI.
	// It should not be selectable in its own right.
	unset($form['conf']['pipeline']['#options']['dpicacheesi']);

	if (isset($form_state['handler']) && isset($form_state['handler']->conf['original_pipeline'])) {
		$candidate = $form_state['handler']->conf['original_pipeline'];
		if (isset($form['conf']['pipeline']['#options'][$candidate])) {
			$form['conf']['pipeline']['#default_value'] = $candidate;
		}
	}
}
/**
 * Save the configuration of a panel page.
 * @see panels_panel_context_save().
 */
function dpicache__panel_context_save(&$handler, $update) {
	// Override the rendering pipeline if any pane uses ESI.
	// Only the standard rendering pipeline is supported; alternative/IPE/legacy
	// pipelines cannot be used with ESI.
	// @TODO: inform the user of this on the display, if a non-standard renderer
	// is selected.
	if (isset($handler->conf['display']) && is_a($handler->conf['display'], 'panels_display')) {
		$display = $handler->conf['display'];
	}
	else {
		// Attempt to load the display using the DID.
		$display = panels_load_display($handler->did);
	}

	if (isset($display) && _dpicache_panels__display_uses_esi($display)) {
		if ($handler->conf['pipeline'] == 'standard' || $handler->conf['pipeline'] == 'dpiviewer' || $handler->conf['pipeline'] == 'ipe') {
			$handler->conf['original_pipeline'] = $handler->conf['pipeline'];
			$handler->conf['pipeline'] = "dpicacheesi";
		}
	}
	panels_panel_context_save($handler, $update);
}


/**
 * Load the arguments which are used to populate the base context of a ctools
 * task plugin.
 *
 * @example
 * $args = esi_panels__get_base_context_arguments('node_view', array(1));
 * Returns array(node_load(1));
 *
 * @param String $task
 * The ctools task.
 * @param String $subtask
 * The subtask of the ctools task (if applicable).
 * @param Array $args
 * Arguments to pass to the argument constructor (if applicable).
 *
 * @return Array
 * Array of arguments to pass to the ctools context constructor.
 */

//@todo: prendre cela en compte car je pense que ca ne marche pas pour l'instant
function dpicacheesi__get_base_context_arguments($task, $subtask = '', $args = array()) {
	// A core bug is preventing module_invoke_all() from lazy-loading according
	// to the hook_hook_info() definitions.
	foreach (module_list(FALSE, FALSE, TRUE) as $module) {
		module_load_include('inc', $module, $module . '.esi_panels');
	}

	return module_invoke_all('esi_panels_context_arguments', $task, $subtask, $args);
}

/**
 * Check if any panes are configured to use ESI.
 *
 * @param Object $display
 * A panels_display object.
 *
 * @return Boolean
 */
function _dpicache_panels__display_uses_esi(panels_display $display) {
	// Iterate each pane.
	foreach ($display->content as $pid => $pane) {
		// Any single pane implementing ESI is enough to return TRUE.
		if (!empty($pane->cache) && $pane->cache['method'] == 'dpicacheesi') {
			return TRUE;
		}
	}

	return FALSE;
}



/**
 * Reverse the $display->cache_key encoding to get the task name.
 *
 * @param String $cache_key
 * The cache key used on a display.
 *
 * @return String
 * The task name of the task handler.
 */
function _dpicache_panels__get_taskname($cache_key) {
	// $display->cache_key = 'panel_context:' . $task_name . ':' . $handler->name;
	if (preg_match('/^panel_context:([^:]+):.*$/', $cache_key, $matches)) {
		return $matches[1];
	}
}

/**
 * Reverse the $display->cache_key encoding to get the task name (and sub-task
 * if used).
 *
 * @param String $task_name
 * The task key, as used by a display cache_key.
 *
 * @return Array
 * - 0 => Name of the task.
 * - 1 => Name of the subtask (or '' if not set).
 */
function _dpicache_panels__get_task_identifier($task_name) {
	if (strpos('-', $task_name)) {
		list ($task, $subtask) = explode('-', $task_name, 2);
		return array($task, $subtask);
	}
	else {
		return array($task_name, '');
	}
}


/**
 * Build the URL to use for this ESI component.
 *
 * @return String
 * The internal URL. Generate a fully-qualified path by running through url().
 */
function dpicache_esi_panels_url($pane, $display) {


	$url = "esi/panels_pane/";

	global $theme;
	$url .= implode(':', array(
		$theme,
		$pane->did,
		$pane->pid,
	));

	// The did and pid are used to identify which pane content_type to load.

	// Other available data to pass into the URL:
	// - $display->args       Are *always* passed.
	// - $display->context    A pane can only accept a single context.
	// - $display->cache_key  The cache key provides the name of the task/subtask.
	if (!empty($pane->configuration['context'])) {
		// If the context originates from the *TASK* plugin (which is typical), the
		// task name is required in order to generate the task contexts
		// ($base_context in panels_panel_context_render()).
		// Additional contexts may be supplied directly by the display.
		$task_name = _dpicache_panels__get_taskname($display->cache_key);
		$url .= "/{$task_name}";


	}

	if($pane->cache['settings']["granularity"]['args']){

		// Add all the display arguments to the end of the URL.
		$url .= '/' . implode('/', $display->args);
	}

	if($pane->cache['settings']["granularity"]['page']) {

		// Always add the current page URL.
		$url .= '/' . base64_encode($_GET['q']);
	}


	switch($pane->cache['settings']['granularity']['user']){
		case DRUPAL_CACHE_GLOBAL;
			break;
		case DRUPAL_CACHE_PER_ROLE;
			$url .= '/CACHE=ROLE';
			break;
		case DRUPAL_CACHE_PER_USER;
			// Always add the current page URL.
			$url .= '/CACHE=USER';
			break;

	}
	// Allow other modules to alter the ESI URL (or respond to it).
	// @see hook_esi_block_url_alter().
	drupal_alter('esi_panels_url', $url);


	return $url;







}









/**
 * Prepare an ESI panels pane for rendering.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info().
 */
function dpicacheesi_panels__esi_pane_prepare($component_key)
{
	//remove cache granularity /Cache=ROLE

	// The URL pattern approximately corresponds to the function arguments:
	// 1. A component key, which is a colon-separated set of pieces of
	//    information: the theme name, the display ID and the pane ID.
	// 2. An optional task name.
	// 3. A variable number of arguments to the display.
	// 4. The original URL.

	// Use the parameters to reconstruct:
	// - Display
	// - Pane
	// - Contexts
	list($theme, $did, $pid) = explode(':', $component_key);

	// Check for optional args.

	$args = array_slice(func_get_args(), 1);



	if (stristr($args[count($args)+1],"CACHE")){
		array_pop($args);
	}

	// The URL is always the last argument. Check if we have a specified url or set to a fake url
	if(count($args)>>0) {
		$url = array_pop($args);
	}
	else{
		//Sometimes we need an url (for menu block for example so we simply set the url to /esi/...
		$url=base64_encode($_GET['q']);
	}
	// Task-name and context identifier are next in the URL, if context is used
	// by the pane.
	$display = panels_load_display($did);
	$pane = $display->content[$pid];

	$pane->esi_meta = array(
		'display_contexts' => array(),
	);

	// If the pane has context, attempt to load the context from the display
	// data.
	if (!empty($pane->configuration['context'])) {
		// The task name is always the first of the optional arguments.
		$task_name = array_shift($args);
		list($task, $subtask) = _dpicache_panels__get_task_identifier($task_name);


		$pane->esi_meta['task'] = $task;
		$pane->esi_meta['subtask'] = $subtask;
	}

	$pane->esi_meta += array(
		'display' => $display,
		'theme' => $theme,
		'display_args' => $args,
		'url' => base64_decode($url),
	);

	//print '<pre>'.print_r($pane,1).'</pre>';die();

	dpicacheesi_panels__restore_context($pane);


	return $pane;
}



/**
 * Restore the original context that was used when a block was displayed.
 */
function dpicacheesi_panels__restore_context(&$pane) {
	// Restore the theme.

	global $theme;
	$theme = $pane->esi_meta['theme'];


	$_GET['q'] = $pane->esi_meta['url'];
	$_SERVER['REQUEST_URI'] = $pane->esi_meta['url'];
	drupal_static_reset('drupal_get_destination');

	// Load up contexts. Owch.
	if (!empty($pane->configuration['context'])) {
		ctools_include('context');
		ctools_include('context-task-handler');

		// Load the task/subtask plugins.
		$task = page_manager_get_task($pane->esi_meta['task']);
		$subtask = (empty($pane->esi_meta['subtask'])) ? '' : page_manager_get_subtask($task, $pane->esi_meta['subtask']);

		// Use the task-name and the original display arguments to generate the
		// arguments that were passed to the context constructor.
		// E.g. The node_view task takes "1" and returns node_load(1).

		$base_context_arguments = dpicacheesi__get_base_context_arguments($pane->esi_meta['task'], $pane->esi_meta['subtask'], $pane->esi_meta['display_args']);

		$base_contexts = ctools_context_handler_get_task_contexts($task, $subtask, $base_context_arguments);


		// The base contexts are then typically used in the task plugin as render
		// information:
		// $output = ctools_context_handler_render($task, '', $contexts, array($node->nid));

		// Other contexts are usually loaded by the panel_context task_handler
		// plugin in the render function panels_panel_context_render():
		// $contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);

		// Load the relevant handler
		$handlers = page_manager_load_sorted_handlers($task, $subtask ? $subtask['name'] : '', TRUE);
		$id = ctools_context_handler_get_render_handler($task, $subtask, $handlers, $base_contexts, $pane->esi_meta['display_args']);



		$pane->esi_meta['display_contexts'] =array();

		if ($id) {

			$handler = $handlers[$id];
			//kprint_r($handler);
			//kprint_r($base_contexts);

			$contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);
			$pane->esi_meta['display_contexts'] = $contexts;

		}

	}
}


/**
 * Render the HTML for a single block.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function dpicacheesi_panels__esi_pane_render($pane) {


	module_load_include('inc','panels','includes/plugins');

	// Much of this is from the "standard" display renderer:
	// see panels_renderer_standard::prepare().
	ctools_include('plugins');

	ctools_include('content');
	$content_type = ctools_get_content_type($pane->type);

	// Check access control; if the user doesn't have access, simply return an
	// empty string.
	ctools_include('context');

	if ($pane->access && !ctools_access($pane->access, $pane->esi_meta['display_contexts'])) {
		return '<!-- ESI no access -->';
	}




	$content = ctools_content_render($pane->type, $pane->subtype, $pane->configuration, array(), $pane->esi_meta['display_args'], $pane->esi_meta['display_contexts']);


	//kprint_r($pane);
	//kprint_r($content);
	//print $content;
	if (empty($content)) {
		return '<!-- ESI no content -->';
	}

	foreach (module_implements('panels_pane_content_alter') as $module) {
		$function = $module . '_panels_pane_content_alter';
		$function($content, $pane, $pane->esi_meta['display_args'], $pane->esi_meta['display_contexts']);
	}

	dpicacheesi_panels_set_http_headers($pane);

	// Pass long the css_id that is usually available.
	if (!empty($pane->css['css_id'])) {
		$content->css_id = check_plain($pane->css['css_id']);
	}

	// Pass long the css_class that is usually available.
	if (!empty($pane->css['css_class'])) {
		$content->css_class = check_plain($pane->css['css_class']);
	}
	if (!empty($content->content)) {

		if (!empty($pane->style['style'])) {
			$style = panels_get_style($pane->style['style']);

			if (isset($style) && isset($style['render pane'])) {

				$output = theme($style['render pane'], array('content' => $content, 'pane' => $pane, 'display' => $this->display, 'style' => $style, 'settings' => $pane->style['settings']));

				// This could be null if no theme function existed.
				if (isset($output)) {
					$output.='<!-- ESI content -->';
					return $output;
				}
			}
		}

		// fallback
		return "<!-- ESI pane with content -->".theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $pane->esi_meta['display']));
	}

	return "<!-- ESI blank content pane -->".theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $pane->esi_meta['display']));

}

/**
 * Set HTTP headers to control caching of ESI fragments.
 */
function dpicacheesi_panels_set_http_headers($pane) {
	$ttl = $pane->cache['settings']['esi_ttl'];

	$headers = array();
	$headers[] = array('Cache-Control', "private, max-age=$ttl");

	// Allow other modules to alter the headers.
	// @see hook_esi_block_cache_headers_alter().
	drupal_alter('esi_panels_cache_headers', $headers);

	foreach($headers as $header) {
		drupal_add_http_header($header[0], $header[1]);
	}
}

/**
 * Flush the ESI block caches.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function dpicacheesi_panels__esi_pane_flush() {
	// @TODO.

}



