<?php 

/**
 * Wrapper function to avoid dependancy of dpicache
 */
function _dpimport_variable_get($name, $default_value){
  if (function_exists('dpi_variable_get')){
    return dpi_variable_get($name, $default_value);
  } else {
    return variable_get($name, $default_value);
  }
}

/**
 * Wrapper function to avoid dependancy of dpicache
 */
function _dpimport_variable_set($name, $default_value){
  if (function_exists('dpi_variable_set')){
    return dpi_variable_set($name, $default_value);
  } else {
    return variable_set($name, $default_value);
  }
}

/**
 * Wrapper function to avoid dependancy of dpicache
 */
function _dpimport_variable_del( $name){
  if (function_exists('dpi_variable_del')){
    return dpi_variable_del($name);
  } else {
    return variable_del($name);
  }
}

/**
 * Check if an URL is valid and return a 200
 * Return TRUE if so False if not.
 *
 * @param String $url
 *
 * @return Boolean $response
 */
function _dpimport_checkurlresponse($url){
  $response = TRUE;
  if (!valid_url($url)){
    $response = FALSE;
  } else {
    $request = drupal_http_request($url, array('timeout' => _dpimport_variable_get('dpicommons_http_request_timeout', 3)));
    if ($request->code != '200'){
      $response = FALSE;
    }
  }
  return $response;
}

/**
 * Get an importer plugin and load related files
 */
function _dpimport_load_importer_plugin($importer_name, $version) {
  ctools_include('plugins');
  $importer_plugin = ctools_get_plugins('dpimport', 'importer', $importer_name);
  if ($importer_plugin && isset($importer_plugin['versions'][$version]['include']['module']) && isset($importer_plugin['versions'][$version]['include']['path'])) {
    module_load_include('inc', $importer_plugin['versions'][$version]['include']['module'], $importer_plugin['versions'][$version]['include']['path']);
  }
  return $importer_plugin;
}
