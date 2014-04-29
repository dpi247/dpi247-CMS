<?php

/**
 * Callback function to launch an import.
 *
 * @param string $importer_name
 * @return string
 */
function dpimport_page_process_import($importer_name){
  $importer_settings = dpimport_getimportersettings($importer_name);
  $return = NULL;
  if ($importer_settings != NULL) {
    $importer_plugin = _dpimport_load_importer_plugin($importer_settings->type, $importer_settings->version);
    if ($importer_plugin != NULL) {
      // Get importer class from settings and plugin
      $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
      // Importer Instance
      $importer = new $importer_class($importer_settings, $importer_plugin);
      $dispatcher = $importer->getDispatcher();
      if (lock_acquire('dpimport_'.$importer->getName(), $dispatcher->getLockTimeout())) {
        dpimport_process_import($importer, $dispatcher);
        $return = $importer->render();
        lock_release('dpimport_'.$importer->getName());
      } else {
        Logger::logError('importer', 0, 'semaphore', t('This importer is currently working. Please try again later'));
      }
    } else {
      Logger::logError('importer', 0, 'importer_001', t('There is no plugin for this importer.'));
    }
  } else {
    Logger::logError('importer', 0, 'importer_002', t('There is no importer with this name.'));
  }
  if ($return == NULL){
    $return = Logger::printLog();
  }
  
  print $return;
}

/**
 * Get importer settings from importer name
 *
 * @param string $importer_name
 * @return Object $importer
 */
function dpimport_getimportersettings($importer_name){
  $result = db_query("SELECT * FROM {dpimport_importer} WHERE name = :name", array(':name' => $importer_name));
  return $result->fetchObject();
}

/**
 * Get All importer settings
 *
 * @return Array $importers
 */
function dpimport_getallimportersettings(){
  $importers = array();
  $result = db_query("SELECT * FROM {dpimport_importer} ");
  foreach ($result as $record){
    $importers[] = $record;
  }
  return $importers;
}

/**
 * Process import
 *
 * @param Importer $importer
 */
function dpimport_process_import($importer, $dispatcher) {
  $sources = $dispatcher->extractSources();
  if ($sources != NULL){
    foreach ($sources as $source){
      if ($source['temp_source'] != NULL){
        $dispatcher->process($source['temp_source']);
      }
      $dispatcher->moveSource($source);
    }
  }
}

/**
 * Delete old files
 */
function dpimport_deleteoldfiles($importers) {
  foreach ($importers as $importer_settings){
    $importer_plugin = _dpimport_load_importer_plugin($importer_settings->type, $importer_settings->version);
    if ($importer_plugin != NULL){
      // Get importer class from settings and plugin
      $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
      // Importer Instance
      $importer = new $importer_class($importer_settings, $importer_plugin);
      $dispatcher = $importer->getDispatcher();
      $dispatcher->cleanDirectories();
    }
  }
}

/**
 * Process Cron driven imports
 */
function dpimport_process_cron_imports($importers) {
  if (function_exists('dpi_variable_get')) {
    $importer_uid = dpi_variable_get('dpimport_importer_user', 0);
  } else {
    $importer_uid = variable_get('dpimport_importer_user', 0);
  }
  if ($importer_uid) {
    // Use a fake user for entities creation/update
    global $user;
    $old_user = $user;
    $user = user_load($importer_uid);

    foreach ($importers as $importer_settings) {
      $dispatcher_settings = unserialize($importer_settings->dispatcher_settings);
      if (isset($dispatcher_settings['cron']) && $dispatcher_settings['cron'] && dpimport_cron_has_to_run($importer_settings, $dispatcher_settings)) {
        $importer_plugin = _dpimport_load_importer_plugin($importer_settings->type, $importer_settings->version);
        if ($importer_plugin != NULL) {
          // Get importer class from settings and plugin
          $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
          // Importer Instance
          $importer = new $importer_class($importer_settings, $importer_plugin);
          $dispatcher = $importer->getDispatcher();
          if (lock_acquire('dpimport_'.$importer->getName(), $dispatcher->getLockTimeout())) {
            dpimport_process_import($importer, $dispatcher);
            lock_release('dpimport_'.$importer->getName());
            db_update('dpimport_importer')
              ->fields(array('last_run' => time()))
              ->condition('iid', $importer_settings->iid)
              ->execute();
          } else {
            Logger::logError('importer', 0, 'semaphore', t('This importer is currently working. Please try again later'));
          }
        }
      }
    }

    $user = $old_user;
  } else {
    // @todo : No importer user defined, add an error
  }
}

/**
 * Check if an importer cron has to run
 *
 * @param Object $importer_settings
 */
function dpimport_cron_has_to_run($importer_settings, $dispatcher_settings) {
  $run_frequency = (isset($dispatcher_settings['cron_frequency']) && is_numeric($dispatcher_settings['cron_frequency'])) ? $dispatcher_settings['cron_frequency'] : 10;
  $run_frequency *= 60;
  if (time() > ($importer_settings->last_run + $run_frequency)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Helping function to process an import with the postdispatcher via a form
 * 
 * @param String $importer_name
 */
function dpimport_page_test($importer_name) {
  $content = '';

  $importer_settings = dpimport_getimportersettings($importer_name);
  if ($importer_settings != NULL) {
    /* If you want to test it by curl :
     curl -X POST http://url -H "Content-Type: application/zip" --data-binary "@/local_path_to/test.xml" > /path_to_reception_folder/curl_response.html
    */

    $content = drupal_get_form('dpimport_page_test_form', $importer_name);
  }
  else {
    drupal_set_message(t('The chosen importer doesn\'t exist.'), 'error');
  }

  return $content;
}

/**
 * Form to upload a test zip to be imported
 */
function dpimport_page_test_form($form, $form_state, $importer_name) {
  $form['#action'] = '/dpimport/process/'.$importer_name;

  $form['uploadedfile'] = array(
    '#name' => 'uploadedfile',
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#title_display' => 'invisible',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Upload File',
  );

  return $form;
}
