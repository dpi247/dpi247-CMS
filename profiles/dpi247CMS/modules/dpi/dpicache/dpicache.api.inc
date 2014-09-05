<?php

/**
 * Get cache content throught DPIcache system.
 *
 * @param  String $cache_name Cache name
 * @param  void   ???         Add all your parameters for your callback function then.
 * @return void               Your cache content, or FALSE.
 */
function dpicache_get_cache_entry($cache_name) {
  $cache_definition = dpicache_get_cache_entry_definition($cache_name);
  if(!$cache_definition) {
    dpi_log('dpicache', "error", t("dpicache_get_cache_entry : !cache_name is not a registred DPIcache", array("!cache_name" => $cache_name)), array(), '', WATCHDOG_ERROR);
    return FALSE;
  }
 
  // Get arguments for callback
  $parameters = func_get_args();
  array_shift($parameters);
  // Get hash
  $hash = _dpicache_get_hash($cache_name, $parameters);
  if(!$hash) {
    dpi_log('dpicache', "error", t("dpicache_get_cache_entry : Unable to generate hash for !cache_name",array("!cache_name"=>$cache_name)),array(),'',WATCHDOG_ERROR);
    return FALSE;
  }

  // fetch cache from ad hoc storage
  if(($content = dpicache_fetch_cache($cache_name, $hash)) && (!isset($cache_definition['bypass']) || $cache_definition['bypass'] !== TRUE) && !variable_get('dpicache_disable_dpicache_utilisation', FALSE)) {
    // cache available
    dpi_log('dpicache', "cache_hit", t("Cache hit : name = !cache_name, hash = !hash",array("!cache_name"=>$cache_name,"!hash"=>$hash)),array(),$hash,WATCHDOG_DEBUG);
    return $content;
  }
  else {
    // no cache available
    dpi_log('dpicache', "cache_miss", t("Cache miss : name = !cache_name, hash = !hash",array("!cache_name"=>$cache_name,"!hash"=>$hash)),array(),$hash,WATCHDOG_NOTICE);
    
    // Get fresh content
    $fresh_content = _dpicache_cache_call_function($cache_definition, $parameters);
    if(is_string($fresh_content) && strpos($fresh_content, '#ERROR#')){
      return FALSE;
    }

    try {
      $parameters_array = serialize($parameters);
    } catch (Exception $e) {
      // Unable to keep parameters for future use
      dpi_log('dpicache', "error", t("dpicache_get_cache_entry : Unable to serialize parameters for name = !cache_name and hash = !hash",array("!cache_name"=>$cache_name,'!hash'=>$hash)),array(),'',WATCHDOG_ERROR);
      return $fresh_content;
    }

    // Save content into adhoc cache
    $saved = dpicache_save_cache($cache_name, $hash, $fresh_content, $cache_definition);
    dpi_log('dpicache', "regenerate_hot", t("Cache (re)generated for name = !cache_name, hash = !hash",array("!cache_name"=>$cache_name,"!hash"=>$hash)),array(),$hash,WATCHDOG_DEBUG);

    $arg_callback = dpicache_get_arguments_values($cache_definition, $parameters);
    // Add or update cache entry data
    $entry_information = array(
      "name" => $cache_name,
      "hash" => $hash,
      "category" => $arg_callback["category"],
      "data_type" => @$cache_definition["data_type"] ?: "string",
      "creation_date" => time(),
      "expiration_date" => intval(time() + $arg_callback["expiration_date"]),
      "last_run" => time(),
      "update_freq" => $arg_callback['update_freq'],
      "parameters" => $parameters_array
    );
    dpicache_save_cache_entry($entry_information, false);
    return $fresh_content;
  }
}

/**
 * Get the url to invalidate a cache entry
 * The url is returned without begin or trailling slash
 *
 * @param  String $cache_name Cache name
 * @return String  tur url to call
 */
function dpicache_get_regenerate_url($cache_name){
  $args=func_get_args();
  unset($args[0]);
  foreach ($args as $key=>$value){
    if($value==NULL){
      $args[$key]=DPICACHE_NULL_URL_PARAMETER;
    }
    if($value==""){
      $args[$key]=DPICACHE_EMPTYSTRING_URL_PARAMETER;
    }
  }
  $args_url=implode('/', $args);

  $url='dpicache/regenerate/one_cache_entry/'.$cache_name.'/'.$args_url;
  return $url;
}

/**
 * Returns a persistent variable.
 *
 * Case-sensitivity of the dpi_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to return.
 * @param $default
 *   The default value to use if this variable has never been set.
 * @return
 *   The value of the variable.
 *
 * @see  dpi_variable_del(), dpi_variable_set(), dpi_variable_get_all()
 */
function dpi_variable_get($name, $default, $regenerate = FALSE) {
  $dpivariables = &drupal_static(__FUNCTION__);

  if (!isset($dpivariables[$name]) || $regenerate) {
    $result=db_query('SELECT value FROM {dpi_variables} WHERE name = :name', array(':name'=>$name));
    if($variable = $result->fetchAssoc()) {
      $dpivariables[$name] =  unserialize($variable['value']);
    }
    else{
      return $default;
    }
  }
  return $dpivariables[$name];
}

/**
 * Returns all persistents variables.
 *
 * @return
 *   The value of the variables.
 *
 * @see dpi_variable_set()
 */
function dpi_variable_get_all() {
  $dpivariables = &drupal_static('dpivariables');
  $result = db_select('dpi_variables','d')->fields('d',array('name','value'))->execute();
  foreach ($result as $row) $dpivariables[$row->name] = unserialize($row->value);
  return  $dpivariables;
}

/**
 * Sets a persistent variable.
 *
 * Case-sensitivity of the dpi_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 *
 * @see dpi_variable_del(), dpi_variable_get(), dpi_variable_get_all()
 */
function dpi_variable_set($name, $value) {
  $dpivariables = &drupal_static('dpivariables');

  $dpivariables[$name] = $value;

  $serialized_value = serialize($value);

  db_merge('dpi_variables')
    ->key(array('name' => $name))
    ->fields(array(
      'name' => $name,
      'value' => $serialized_value,
    ))
    ->execute();
}

/**
 * Unsets a persistent variable.
 *
 * Case-sensitivity of the dpi_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see dpi_variable_del(), dpi_variable_get(), dpi_variable_get_all()
 */
function dpi_variable_del($name) {
  db_delete('dpi_variables')
    ->condition('name', $name)
    ->execute();
  drupal_static_reset('dpivariables');
}

/**
 * Unsets persistent variables starting with a given namespace
 *
 * Case-sensitivity of the dpi_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 * @param $type
 *   Type of variables to delete, "system" (classic variable_set variables),
 *   "dpi" (dpi_variable_set variables) or "both".
 *
 * @see dpi_variable_get(), dpi_variable_set(), dpi_variable_del()
 */
function dpi_variable_del_namespace($namespace, $type = 'both') {
  if ($type == 'system' || $type == 'both') {
    global $conf;

    $db_vars = db_select('variable', 'v')
      ->fields('v', array('name'))
      ->condition('v.name', $namespace.'%', 'LIKE')
      ->execute();
    foreach ($db_vars as $db_var) {
      unset($conf[$db_var->name]);
    }
    db_delete('variable')
      ->condition('name', $namespace.'%', 'LIKE')
      ->execute();

    cache_clear_all('variables', 'cache_bootstrap');
  }

  if ($type == 'dpi' || $type == 'both') {
    db_delete('dpi_variables')
      ->condition('name', $namespace.'%', 'LIKE')
      ->execute();
    drupal_static_reset('dpivariables');
  }
}

/**
 * Sets up a form to save information automatically
 * into dpi_variable structure.
 *
 * @param $form
 * An associative array containing the structure of the form.
 *
 * @return
 * The form structure.
 *
 * @see system_settings_form()
 *
 */
function dpi_api_system_settings_form($form) {
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'dpi_api_system_settings_form_submit';
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }
  return $form;
}

/**
 * Form submission handler for dpi_system_settings_form().
 *
 * @see system_settings_form_submit()
 */
function dpi_api_system_settings_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    dpi_api_variable_set($key, $value);
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Regenerate one caches that need to be regenerated based on the cache's name
 * 
 * @param String $cache_name
 */
function dpicache_api_regenerate_one_cache_entry_callback($cache_name){

  $args=func_get_args();
  
  foreach ($args as $key=>$value){
    if($value==DPICACHE_NULL_URL_PARAMETER){
      $args[$key]=NULL;
    }
    if($value==DPICACHE_EMPTYSTRING_URL_PARAMETER){
      $args[$key]="";
    }
  }
  
  unset($args[0]);
  
  $hash=_dpicache_get_hash($cache_name, $args);  

  dpi_log('dpicache', "manual_regeneration_of_cache_entry", t("Manual cache regeneration: @cache_name (@args)",array('@cache_name'=>$cache_name,'@args'=>implode(',', $args))),array(),$hash,WATCHDOG_INFO);
  $cache_definition=dpicache_get_cache_entry_definition($cache_name);  
  if($cache_definition!=FALSE){
    $fresh_content = _dpicache_cache_call_function($cache_definition, $args);
    $saved = dpicache_save_cache($cache_name, $hash, $fresh_content, $cache_definition);
    drupal_set_message(t("Manual cache regeneration success: @cache_name (@args)",array('@cache_name'=>$cache_name,'@args'=>implode(',', $args))));    
  }else{
    drupal_set_message(t("Manual cache regeneration failed: @cache_name (@args) : No cache",array('@cache_name'=>$cache_name,'@args'=>implode(',', $args))),'error');
  }
  drupal_goto();
}

/**
 * Regenerate caches that need to be regenerated based on cachetype and category
 * 
 * @param  String $time cache name
 * @param String $categoroy the category of cache
 */
function dpicache_api_regenerate_all_cache_entry_callback($cache_type,$category=NULL){
  
  drupal_set_message(t("Manual cache regeneration: @cache_name, category: @category",array('@cache_name'=>$cache_type,'@category'=>$category)));
  dpi_log('dpicache', "manual_regeneration_of_cache_name", t("Manual cache regeneration: @cache_name category: @category",array('@cache_name'=>$cache_type,'@category'=>$category)),array(),NULL,WATCHDOG_INFO);

  $query = db_select('dpicache_cache_entries', 'dce');
  $query->fields('dce',array('name','hash','data_type','creation_date','parameters'));
  $query->condition('dce.name', $cache_type);

  if($category!=NULL){
    $query->condition('dce.category', $category);
  }
  dpicache_regenerate_cache_entries_process($query);
}



function dpicache_api_varnish_purge_pattern($pattern){
  $host=$_SERVER['SERVER_NAME'];
  varnish_purge($host, $pattern);  
}

function dpicache_api_varnish_purge_all_page(){
  varnish_purge_all_pages();
}