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
    'hook menu alter' => 'dpidestinations_destinations_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'dpidestinations_destinations_get_arguments',
    'get context placeholders' => 'dpidestinations_destinations_get_contexts',
       

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('dpidestinations_destinations_disabled', TRUE),
    'enable callback' => 'dpidestinations_destinations_enable',
    'access callback' => 'dpidestinations_destinations_access_check',
  );
}

/**
 * Callback defined by page_manager_contact_site_page_manager_tasks().
 *
 * Alter the node edit input so that node edit comes to us rather than the
 * normal node edit process.
 */
function dpidestinations_destinations_menu_alter(&$items, $task) {
  if (variable_get('dpidestinations_destinations_disabled', TRUE)) {
    return;
  }
  
  $callback = $items['destination/%destinationterm']['page callback'];
  
  
  // Override the node edit handler for our purpose.
  if ($callback == 'dpidestinations_page_destination_view') {
    $items['destination/%destinationterm']['page callback'] = 'dpidestinations_destination_view_page';
    $items['destination/%destinationterm']['file path'] = $task['path'];
    $items['destination/%destinationterm']['file'] = $task['file'];
  }
  else {
      variable_set('dpidestinations_destinations_disabled', TRUE);
      drupal_set_message(t('Page manager module is unable to enable destinations page because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    return;
  }

}


/**
 * Entry point for our overridden term view.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * term view, which is term_page_view().
 */
function dpidestinations_destination_view_page($destination) {
  // Assign the term name as the page title, just as the original page
  // callback did.
  
  drupal_set_title($destination->name);

  // While we ordinarily should never actually get feeds through here,
  // just in case
  // Load my task plugin
  $task = page_manager_get_task('destinations');

  // Load the term into a context.
  ctools_include('context');
  ctools_include('context-task-handler');
  
  
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($destination));
  
  if (empty($contexts) or $destination->vid!=dpidestinations_api_get_destinations_vocabulary_vid()) {
    return drupal_not_found();
  }
  
  $output = ctools_context_handler_render($task, '', $contexts, array($destination->tid));
  
  if ($output !== FALSE) {
    return $output;
  }
  $function = 'taxonomy_term_page';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('term_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  if ($function == 'taxonomy_term_page') {
    module_load_include('inc', 'taxonomy', 'taxonomy.pages');
  }
  return '';
  return $function($term);
}



/**
 * Callback to enable/disable the page from the UI.
 */
function dpidestinations_destinations_enable($cache, $status) {
  variable_set('dpidestinations_destinations_disabled', $status);
  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['dpidestinations_enabling_destinations'] = TRUE;
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
function dpidestinations_destinations_access_check($task, $subtask_id, $contexts) {
  return TRUE;
}


function dpidestinations_destinations_get_arguments( $task, $subtask_id ){
  module_load_include('inc', 'page_manager', 'plugins/tasks/term_view');
  $args=dpidestinations_destiantions_get_arguments($task, $subtask_id);
  return $args;
  
}
function dpidestinations_destinations_get_contexts( $task, $subtask_id ){

  
  module_load_include('inc', 'page_manager', 'plugins/tasks/term_view');
  $contexts=page_manager_term_view_get_contexts($task, $subtask_id);
  return $contexts;
  
  
}

function dpidestinations_destiantions_get_arguments($task, $subtask_id){
  
  return array(
    array(
      'keyword' => 'destination',
      'identifier' => page_manager_term_view_get_type() == 'multiple' ? t('Term(s) being viewed') : t('Term being viewed'),
      'id' => 1,
      'name' => 'destination',
      'settings' => array('input_form' => 'tid', 'breadcrumb' => variable_get('page_manager_taxonomy_breadcrumb', TRUE)),
      'default' => '404',
      'toto'=> 'toto',
      'task'=>$task,
    ),
    array(
      'keyword' => 'depth',
      'identifier' => t('Depth'),
      'id' => 1,
      'name' => 'string',
      'settings' => array(),
    ),
  );  
  
}

