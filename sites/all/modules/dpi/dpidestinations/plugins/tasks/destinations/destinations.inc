<?php

function dpidestinations_destinations_page_manager_tasks() {
 return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',
    'title' => t('All Destinations'),
    'admin title' => t('All Destinations'),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for the all blogs at <em>/blog</em>. If no variant is selected, the default Drupal most recent blog posts will be shown.'),
    'admin path' => 'destination/%destination',

    // Menu hooks so that we can alter the node/%node menu entry to point to us.
    'hook menu alter' => 'page_manager_destinations_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('page_manager_destinations_disabled', TRUE),
    'enable callback' => 'page_manager_destinations_enable',
    'access callback' => 'page_manager_destinations_access_check',
  );
}

/**
 * Callback defined by page_manager_contact_site_page_manager_tasks().
 *
 * Alter the node edit input so that node edit comes to us rather than the
 * normal node edit process.
 */
function page_manager_destinations_menu_alter(&$items, $task) {
  if (variable_get('page_manager_destinations_disabled', TRUE)) {
    return;
  }
  
  $callback = $items['destination/%destination']['page callback'];
  
  // Override the node edit handler for our purpose.
  if ($callback == 'destination_view') {
    $items['destination/%destination']['page callback'] = 'page_manager_destinations';
    $items['destination/%destination']['file path'] = $task['path'];
    $items['destination/%destination']['file'] = $task['file'];
  }
  else {
      variable_set('page_manager_destination_disabled', TRUE);
      drupal_set_message(t('Page manager module is unable to enable contact because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    return;
  }

}


/**
 * Entry point for our overridden site contact.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * node edit, which is node_page_edit().
 */
function page_manager_destinations() {
  // Load my task plugin
  $task = page_manager_get_task('destinations');

  ctools_include('context');
  ctools_include('context-task-handler');
  $output = ctools_context_handler_render($task, '', array(), array());
  if ($output !== FALSE) {
    return $output;
  }

  module_load_include('inc', 'contact', 'contact.pages');
  $function = 'contact_site_form';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('contact_site')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  if ($function == 'contact_site_form') {
    return drupal_get_form($function);
  }
  return 'rrf';
  return $function();
}



/**
 * Callback to enable/disable the page from the UI.
 */
function page_manager_destinations_enable($cache, $status) {
  variable_set('page_manager_destinations_disabled', $status);
  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  dsm('cou');
  if (!$status) {
    $GLOBALS['page_manager_enabling_destinations'] = TRUE;
  }
}

/**
 * Callback to determine if a page is accessible.
 *
 * @param $task
 *   The task plugin.
 * @param $subtask_id
 *   The subtask id
 * @param $contexts
 *   The contexts loaded for the task.
 * @return
 *   TRUE if the current user can access the page.
 */
function page_manager_contact_access_check($task, $subtask_id, $contexts) {
  return TRUE;
}

