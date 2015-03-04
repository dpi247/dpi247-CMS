<?php

/**
 * Get importer settings from importer name
 *
 * @param string $importer_name
 *
 * @return Object $importer
 */
function dpimport_api_get_importer_settings($importer_name) {
  $db_importer = db_select('dpimport_importer', 'di')
    ->fields('di')
    ->condition('di.name', $importer_name)
    ->execute();
  $importer = NULL;
  foreach ($db_importer as $db_importer) {
    $importer = $db_importer;
  }
  return $importer;
}

/**
 * Get All importer settings
 *
 * @return Array $importers
 */
function dpimport_api_get_all_importer_settings() {
  $importers = array();
  $db_importers = db_select('dpimport_importer', 'di')
    ->fields('di')
    ->execute();
  foreach ($db_importers as $importer){
    $importers[] = $importer;
  }
  return $importers;
}
