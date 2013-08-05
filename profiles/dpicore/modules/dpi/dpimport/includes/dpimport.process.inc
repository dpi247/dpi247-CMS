<?php 
/**
 * 
 * Callback function to launch an import.
 * @param string $importer_name
 * @return string
 */
function dpimport_page_process_import($importer_name){
  ctools_include('plugins');
  $importer_settings = dpimport_getimportersettings($importer_name);
  $return = NULL;
  if ($importer_settings != NULL){
    $importer_plugin = ctools_get_plugins('dpimport', 'importer', $importer_settings->type);
    if ($importer_plugin != NULL){
      //Get importer class from settings and plugin
      $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
      //Importer Instance
      $importer = new $importer_class($importer_settings, $importer_plugin);
      dpimport_process_import($importer);
      $return = $importer->render();
    } else {
      Logger::logError('Importer', 001, 2, t('There is no plugin for this importer.'));
    }
  } else {
    Logger::logError('Importer', 002, 1, t('There is no importer with these name.'));
  }
  if ($return == NULL){
    $return = Logger::printLog();
  }
  dsm(Logger::getLogError());
  return $return;
}

/**
 * 
 * Get importer settings from importer name
 * @param string $importer_name
 * @return Object $importer
 */
function dpimport_getimportersettings($importer_name){
  $result = db_query("SELECT * FROM {dpimport_importer} WHERE name = :name", array(':name' => $importer_name));
  return $result->fetchObject();
}

/**
 * 
 * Get All importer settings
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
 * 
 * Process import
 * @param Importer $importer
 */
function dpimport_process_import($importer){
  $dispatcher = $importer->getDispatcher();
  $sources = $dispatcher->extractSources();
  foreach ($sources as $source){
    $dispatcher->process($source);
  }
}


/**
 * 
 * Helping function to process an import with the postdispatcher via a form
 * 
 * @param String $importer_name
 */
function dpimport_page_test($importer_name) {
  /* If you want to test it by curl : 
  curl -X POST http://url -H "Content-Type: application/zip" --data-binary "@/local_path_to/test.xml" > /path_to_reception_folder/curl_response.html
  */
  
  $content = '';
  $content .= '<form enctype="multipart/form-data" action="/dpimport/process/'.$importer_name.'" method="POST">
        Choose a file to upload: <input name="uploadedfile" type="file" /><br />
        <input type="submit" value="Upload File" />
      </form>';

  return $content;
}