<?php

/**
 * This function is called when a 404 is hit
 * It calls the DAM with the searched URL,
 * if the DAM return a node, the node is displayed, if the DAM return a 404, a 404 is displayed
 */
function dpidam_archive_search_url() {
  if (!dpidam_is_url_passing_through_regexes($_SERVER['REQUEST_URI'])) {
    // We don't want to perform a DAM request for this URL
    drupal_not_found();
    return;
  }

  if (!dpidam_is_archive_available_for_ip()) {
    $return = theme('dpidam_archive_failed_limit_exceeded');
    print theme('page', $return);
    return;
  }

  $url = 'http://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
  // Allow to alter the URL to handle canonical URLs
  drupal_alter('dpidam_synchro_url', $url);

  // Call the DAM
  $last_alias = NULL;
  $return = dpidam_archive_get_package_from_url($url, $last_alias);

  if (!$return['error']['error'] && $last_alias != $url && $last_alias != NULL){
    //Redirect to last alias
    drupal_goto($last_alias, array(), 301);
  }

  if (!$return['error']['error']) {
    // Set a global variable named archive to indicates in the dpidam_preprocess_page that it's an archive page
    global $dpidam_archive;
    drupal_set_header("X-Warnish-TTL: ".dpi_variable_get('dpidam_archive_ttl', '24')."h");
    $dpidam_archive = 'archive';
    print theme('page', $return['content']);
  } else {
    if (dpi_variable_get('dpidam_archive_failed_recovery', 500)) {
      // Increment the amount of failed archives recovery
      $user_ip = ip_address();
      $failed_archives = dpi_variable_get('dpidam_archive_failed_'.$user_ip, 0);
      $failed_archives++;
      dpi_variable_set('dpidam_archive_failed_'.$user_ip, $failed_archives);
    }

    // This is a real 404
    // @todo : toujours nécessaire?
    //menu_set_active_trail(array()); // Avoid fatal error due to a misformed breadcrumb
    drupal_not_found();
  }
}

/**
 * Perform an URL check according to defined regexes.
 * If the regexes are set as a white list and the URL matches, the test is validated,
 * if the regexes are set as a black list and the URL doesn't match, the test is validated.
 *
 * @param string $url
 *   The URL to check
 *
 * @return boolean The URL passes the check or not
 */
function dpidam_is_url_passing_through_regexes($url) {
  $list_type = dpi_variable_get('dpidam_archive_regex_black_or_white', 'black');
  $rough_regexes = dpi_variable_get('dpidam_archive_regexes', '');

  if (!empty($rough_regexes)) {
    $regexes = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($rough_regexes)));
    if (is_array($regexes)) {
      foreach ($regexes as $regex) {
        if (preg_match($regex, $url)) {
          if ($list_type == 'white') {
            return TRUE;
          } elseif ($list_type == 'black') {
            return FALSE;
          }
        }
      }
    }
  }

  if ($list_type == 'white') {
    return FALSE;
  }

  // Default is TRUE
  return TRUE;
}

/**
 * Check if the current user has the right to go through the archive recovery process
 *
 * @return boolean The user may go on or not
 */
function dpidam_is_archive_available_for_ip() {
  $failed_limit = dpi_variable_get('dpidam_archive_failed_recovery', 500);
  if ($failed_limit) {
    $user_ip = ip_address();
    $failed_archives = dpi_variable_get('dpidam_archive_failed_'.$user_ip, 0);
    if ($failed_archives >= $failed_limit) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Call the DAM, if a zip is returned, import the zip to a node and render de node.
 *
 * @param string $url
 * @param string $last_alias
 *
 * @return String or FALSE if the import fails
 */
function dpidam_archive_get_package_from_url($url, &$last_alias) {
  // Call to the DAM

  $time_start = microtime(true);
  $file_name = dpidam_archive_get_url($url, $result);
  $dpidam_archive_geturl = microtime(true) - $time_start;

  $return = array(
    'content' => '',
    'error' => array(
      'error' => FALSE,
      'message' => '',
    ),
  );

  if ($file_name) {
    if ($importer_name = dpivariable_get('dpidam_archive_importer', '')) {
      //Import the zip

      $node = dpidam_archive_import_node($importer_name, $file_name);

      $node->stat["dpipdam_archive_geturl"]=$dpidam_archive_geturl;

      if ($node) {
        //Render node
        $content = dpidam_archive_render_node($node);

        if (isset($content) && !empty($content) && $content) {
          $return['content'] = $content;

          _dpidam_save_static_node($node);

          if ($node->field_lasturlalias[0]['value'] != NULL) {
            $last_alias = $node->field_lasturlalias[0]['value'];
          }
        } else {
          $return['error']['error'] = TRUE;
          $return['error']['message'] = t('Error rendering imported node.');
        }
      } else {
        $return['error']['error'] = TRUE;
        $return['error']['message'] = t('Error importing node, see importer logs.');
      }
    } else {
      $return['error']['error'] = TRUE;
      $return['error']['message'] = t('DAM Importer not configured.');
    }
  } else {
    // @todo : mieux gérer les différents messages d'erreurs du DAM (404, 500, ..)
    // No results (file) received
    $return['error']['error'] = TRUE;
    $return['error']['message'] = t('DAM Error : ').$result->code.' - '.$result->error;;
  }

  return $return;
}

/**
 * Call the dam and save the zip in the importer folder
 *
 * @param string $url
 */
function dpidam_archive_get_url($url, &$result){
  $timeout = _dpimport_variable_get('dpicommons_http_request_timeout', 3);
  $request = dpi_variable_get('dpidam_archive_geturl', '');

  $args = '?url='.urlencode($url).'&options=origin%3Dweb';

  $headers = array();
  $data = NULL;

  // GET
  $result = drupal_http_request($request.$args, array('timeout' => $timeout));

  $file_name = FALSE;
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  if (dpicommons_is_http_request_successfull($result)) {
    if ($importer_name = dpi_variable_get('dpidam_archive_importer', '')) {
      $importer = dpimport_api_get_importer_settings($importer_name);
      $dispatcher_settings = unserialize($importer->dispatcher_settings);
      $file_name = 'dam_archive_'.rand().'.zip';
      // Save the file in the importer folder
      file_put_contents($dispatcher_settings['temp_folder'].'/'.$file_name, $result->data);
    }
  }

  // URL not found, try with the DAM ID
  if (!$file_name && arg(0) == 'archived-node') {
    // Remove the "x" at the end of the request and replace it with the ID
    $request = substr($request, 0, -1).arg(1);
    $args = '?options=origin%3Dweb';
    $headers = array();
    $data = NULL;
    // GET
    $result = drupal_http_request($request.$args, array('timeout' => $timeout));
    $file_name = FALSE;
    if (dpicommons_is_http_request_successfull($result)) {
      if ($importer_name = dpi_variable_get('dpidam_archive_importer', '')) {
        $importer = dpimport_api_get_importer_settings($importer_name);
        $dispatcher_settings = unserialize($importer->dispatcher_settings);
        $file_name = 'dam_archive_'.rand().'.zip';
        // Save the file in the importer folder
        file_put_contents($dispatcher_settings['temp_folder'].'/'.$file_name, $result->data);
      }
    }
  }

  return $file_name;
}

/**
 * Import the zip to a node
 *
 * @param string $importer_name
 * @param string $file_name
 *
 * @return A node object or FALSE if the import fails
 */
function dpidam_archive_import_node($importer_name, $file_name) {

  $time_start = microtime(true);

  $node = FALSE;

  if ($importer_name = dpi_variable_get('dpidam_archive_importer', '')) {
    $importer_settings = dpimport_api_get_importer_settings($importer_name);
    $return = NULL;
    if ($importer_settings != NULL) {
      $importer_plugin = _dpimport_load_importer_plugin($importer_settings->type, $importer_settings->version);
      if ($importer_plugin != NULL) {
        // Get importer class from settings and plugin
        $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
        // Importer Instance
        $importer = new $importer_class($importer_settings, $importer_plugin);
        $dispatcher = $importer->getDispatcher();

        module_load_include('inc', 'dpimport', 'includes/dpimport.process');

        // Try to allocate enough time to run import process.
        if (function_exists('set_time_limit')) {
          @set_time_limit(60 * $dispatcher->getLockTimeout());
        }
        $returns = array();

        if (lock_acquire('dpimport_'.$importer->getName(), $dispatcher->getLockTimeout())) {
          $importer->dpimport_preview = TRUE;
          $importer->dpimport_preview_norender = TRUE;

          if ($importer != NULL && $dispatcher != NULL) {
            $returns = dpimport_process_import_file($importer, $dispatcher, $file_name);
          } else {
            //Unknown importer
            $returns[] = array('error' => TRUE, 'error_msg' => t('Unknown importer'));
          }
        }

        //Keep the node even if it has no destination
        if (isset($returns[0]) && !$returns[0]['error']){
          $node = $returns[0]['tempnode'];
        }
      }
    }
  }

  $node->stat["dpidam_archive_importnode - Process ZIP"] = microtime(true) - $time_start;

  return $node;
}

/**
 * Render the node in the archive page.
 *
 * @param object $node
 *
 * @return string $content
 */
function dpidam_archive_render_node(&$node) {
  $content = '';

  $node->preview = TRUE;

  module_load_include('inc', 'page_manager', 'plugins/tasks/page');
  if ($page_archive = page_manager_page_load('archive')) {
    if ($page_archive->default_handlers['page_archive_panel_context']->conf['display'] != NULL){
      //If the page is in file
      $display = $page_archive->default_handlers['page_archive_panel_context']->conf['display'];
    } else {
      //if the page is in db
      $task = page_manager_get_task($page_archive->task);
      $subtask = page_manager_get_task_subtask($task, 'archive');
      $handlers = page_manager_load_sorted_handlers($task, $subtask ? $subtask['name'] : '', TRUE);
      $display = panels_load_display($handlers['page_archive_panel_context']->conf['did']);
    }

    if (isset($page_archive->arguments['damarchive']) && !empty($page_archive->arguments['damarchive'])) {
      ctools_include('context');
      $argument = $page_archive->arguments['damarchive'];
      if ($context = ctools_context_get_context_from_argument($argument, $node)) {
        //add the node to the context
        $display->context[$context->id] = $context;

        $content = panels_render_display($display);
      }
    }
  }

  return $content;
}

/**
 * Display a node as a DAM node
 *
 * @param Object $node
 */
function dpidam_archive_simulation($node) {
  $path = drupal_get_path_alias('node/'.$node->nid);
  $last_alias = $path;
  $return = dpidam_archive_getpackagefromurl($path, $last_alias);
  if ($return['error']['error']) {
    drupal_not_found();
  } else {
    global $dpidam_archive;
    $dpidam_archive = 'archive';
    print theme('page', $return['content']);
  }
}
