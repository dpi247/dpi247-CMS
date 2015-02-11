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
  if ($plugin['name'] == 'panel_context') {    
    // Replace the default 'save' operation - panels_panel_context_save();
    $plugin['save'] = 'dpicache__panel_context_save';
  }
}


/**
 * Implements hook_form_FORM_ID_alter().
 */
function dpicache_panels_form_panels_panel_context_edit_settings_alter(&$form, &$form_state) {
  // The 'ESI' panels renderer should automatically replace the 'standard'
  // panels renderer when there are panes on a panel which are handled by ESI.
  // It should not be selectable in its own right.
  dsm($form);
  dsm($form_state);

  unset($form['conf']['pipeline']['#options']['dpicacheesi']);
  if ($form['conf']['pipeline']['#default_value'] == 'dpicacheesi') {

    $form['conf']['pipeline']['#default_value'] = 'standard';
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

  dsm($handler->conf['pipeline'],ppeline);
  if (($handler->conf['pipeline'] == 'standard' || $handler->conf['pipeline'] == 'esi') && _dpicache_panels__display_uses_esi($handler->conf['display'])) {
    $handler->conf['pipeline'] = 'dpicacheesi';
    dsm($handler,dpicache__panel_context_save);

  }
  panels_panel_context_save($handler, $update);
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
    dsm($pane->cache['method'],CACHEMETHID );
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
  // ESI 6.x-1.x and 6.x-2.x used the URL patterns:
  // Default:                esi/panels_pane/theme:display_id:pane_id
  // With context:           esi/panels_pane/theme:display_id:pane_id/[base64($_GET['q'])]/task_name/context

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
    $task_name = _esi_panels__get_taskname($display->cache_key);

    $url .= "/{$task_name}";
  }

  // Add all the display arguments to the end of the URL.
  $url .= '/' . implode('/', $display->args);

  // Always add the current page URL.
  $url .= '/' . base64_encode($_GET['q']);

  // Allow other modules to alter the ESI URL (or respond to it).
  // @see hook_esi_block_url_alter().
  drupal_alter('esi_panels_url', $url);

  return $url;
}