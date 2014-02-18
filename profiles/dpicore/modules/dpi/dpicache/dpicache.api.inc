<?php 

function hook_dpicache_cache_entry_register() {
  return array(
    'flowmix_render' => array(
      'callback' => 'flowmix_get_render', // function to call
      'plugin' => 'dpicache_memcache', // which storage?
      'catagory' => 'flowmix_render', // group
      'data_type' => "string", // the callback returns a string
      'ttl' => 7*24*60*60, //(in seconds) keep a cache for one week
      'refresh_interval' => 5*60 //(in seconds) auto-refresh cache every 5 minutes
    ),
    // file key if your callback is not auto loaded (not in .module)
    //'file'=>array(
    //  "module"=>"dpidestinations",
    //  "type"=>"inc",
    //  "name"=>"plugins/ctools/content_types/targetblock/targetblock"
    //),
    // use bypass to bypass cache for debugging purpose.
    //'bypass'=>TRUE,
  );
}

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
  if(($content = dpicache_fetch_cache($cache_name, $hash)) && (!isset($cache_definition['bypass']) || $cache_definition['bypass'] !== TRUE)) {
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

    // Add or update cache entry data
    $entry_information = array(
      "name" => $cache_name,
      "hash" => $hash,
      "category" => @$cache_definition["category"],
      "data_type" => @$cache_definition["data_type"] ?: "string",
      "creation_date" => time(),
      "expiration_date" => intval(time() + $cache_definition["ttl"]),
      "last_run" => time(),
      "update_freq" => $cache_definition["refresh_interval"],
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
  $dpivariables = &drupal_static("dpivariables");

  $result = db_query('SELECT * FROM {dpi_variables}');
  while($variable = db_fetch_array($result)) {
    $dpivariables[$name] = unserialize($variable['value']);
  }

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
  $dpivariables = &drupal_static("dpivariables");

  $dpivariables[$name]=$value;

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
  drupal_static_reset($name);
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
function dpi_system_settings_form($form){
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'dpi_system_settings_form_submit';
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
function dpi_system_settings_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    dpi_variable_set($key, $value);
  }
  drupal_set_message(t('The configuration options have been saved.'));
}
