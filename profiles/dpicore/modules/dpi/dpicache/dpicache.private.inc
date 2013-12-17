<?php

/**
 * Fetch cache content from ad hoc storage
 *
 * @param  String $entry_name    Name of the dpicache
 * @param  String $entry_hash    Hash of called parameters
 * @return void                  Content stored, FALSE otherwise.
 */
function dpicache_fetch_cache($entry_name, $entry_hash){
  global $conf;
  $default_cache_table = 'cache_dpicache';
  // Check if a specific storage has been defined for
  // this type of DPIcache (MemCacheDrupal).
  // By default all is stored in cache_dpicache table
  // thanks to DrupalDatabaseCache PHP class.
  // [ see /includes/cache.inc : _cache_get_object() ]
  if(isset($conf['cache_class_dpicache_'.$entry_name])){
    $bin = 'dpicache_'.$entry_name;
  }
  else {
    $bin = $default_cache_table;
  }
  $c = cache_get($entry_name.'_'.$entry_hash, $bin);
  if(!$c)
    return FALSE;
  return $c->data;
}

/**
 * Save cache content in ad hoc storage
 *
 * @param  String $entry_name       Name of the dpicache
 * @param  String $entry_hash       Hash of called parameters
 * @param  void   $content          Content to store
 * @param  Array  $cache_definition Registered definition
 * @return Boolean                  Save success, or not.
 */
function dpicache_save_cache($entry_name, $entry_hash, $content, $cache_definition){
  global $conf;
  $default_cache_table = 'cache_dpicache';
  if(isset($conf['cache_class_dpicache_'.$entry_name])){
    $bin = 'dpicache_'.$entry_name;
  }
  else {
    $bin = $default_cache_table;
  }
  cache_set($entry_name.'_'.$entry_hash, $content, $bin, CACHE_PERMANENT);
  return TRUE;
}

/**
 * Delete cache content in ad hoc storage
 *
 * @param  String $entry_name    Name of the dpicache
 * @param  String $entry_hash    Hash of called parameters
 * @return Boolean               Deletion success, or not.
 */
function dpicache_delete_cache($entry_name, $entry_hash){
  global $conf;
  $default_cache_table = 'cache_dpicache';
  if(isset($conf['cache_class_dpicache_'.$entry_name])){
    $bin = 'dpicache_'.$entry_name;
  }
  else {
    $bin = $default_cache_table;
  }
  cache_clear_all($entry_name.'_'.$entry_hash, $bin);
}


/**
 * Regenerate caches that need to be regenerated based on the time
 * @param  Integer $time Current timestamp
 * @return Boolean       Success of regenerations
 */
function dpicache_regenerate_cache_entries_since_time($time){
  if(!$time)
    $time = date();
  // Get caches that need to be regenerated
  $table_cache_entries = 'dpicache_cache_entries';
  $query = db_select($table_cache_entries, 'dce');
  $query->fields('dce',array('name','hash','data_type','creation_date','parameters'));
  $query->where('dce.expiration_date > '.$time.' AND dce.last_run+dce.update_freq < '.$time);
  dpicache_regenerate_cache_entries_process($query);
  return TRUE;

}
/**
 * Regenerate caches that need to be regenerated based on the time
 * @param  Integer $time Current timestamp
 * @return Boolean       Success of regenerations
 */
function dpicache_regenerate_cache_entries($cache_type,$category=NULL){

  $table_cache_entries = 'dpicache_cache_entries';
  $query = db_select($table_cache_entries, 'dce');
  $query->fields('dce',array('name','hash','data_type','creation_date','parameters'));

  $query->condition('dce.name', $cache_type);

  if($category!=NULL){
    $query->condition('dce.category', $category);

  }
  dpicache_regenerate_cache_entries_process($query);

  return TRUE;

}

/**
 * Process the cache generation for each result of the querry object
 *
 * @param  Ressource A querry object htat should contains dpicache_cache_entries rows
 */
function dpicache_regenerate_cache_entries_process($query){
  foreach($query->execute() as $entry){
    $cache_definition = dpicache_get_cache_entry_definition($entry->name);
    if(!$cache_definition){
      dpi_log('dpicache', "error", t("Cron : the entry name !cache_name has no definition",array("!cache_name"=>$entry->name)),array(),$entry->hash,WATCHDOG_WARNING);
      // @TODO Modifier sa date d'expiration à '-1'
      continue;
    }
    // Get fresh content
    $fresh_content = _dpicache_cache_call_function($cache_definition, unserialize($entry->parameters));
    if(is_string($fresh_content) && strpos($fresh_content, '#ERROR#')){
      // fail to get cache
      dpi_log('dpicache', "error", t("Error while calling callback function for name = !cache_name, hash = !hash : !error",array("!cache_name"=>$entry->name,"!hash"=>$entry->hash,"!error"=>$fresh_content)),array(),$entry->hash,WATCHDOG_ERROR);
      continue;
    }
    // Save content into cache
    $saved = dpicache_save_cache($entry->name, $entry->hash, $fresh_content, $cache_definition);
    $entry_information = array(
      "name" => $entry->name,
      "hash" => $entry->hash,
      "category" => @$cache_definition["category"],
      "data_type" => @$cache_definition["data_type"] ?: "string",
      "expiration_date" => intval($entry->creation_date + $cache_definition["ttl"]),
      "update_freq" => $cache_definition["refresh_interval"]
    );
    if($saved)
      $entry_information['last_run'] = time();
    // Update cache entry data
    dpicache_save_cache_entry($entry_information, true);
    dpi_log('dpicache', "regenerate_cold", t("Cache (re)generated for name = !cache_name, hash = !hash",array("!cache_name"=>$entry->name,"!hash"=>$entry->hash)),array(),$entry->hash,WATCHDOG_DEBUG);
  }
  return TRUE;
}


/**
 * Get registered infos associated with a cache type
 * @param  String $cache_name Name of a dpicache.
 * @return Array             Infos about it. False otherwise.
 */
function dpicache_get_cache_entry_definition($cache_name){
  if(!$cache_name)
    return FALSE;
  $infos = dpicache_get_all_cache_entries_definitions();
  if(isset($infos[$cache_name])){
    return $infos[$cache_name];
  }
  return FALSE;
}

/*
 * Call all the hook_dpicache_cache_entry_register
*/
function dpicache_get_all_cache_entries_definitions(){
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos=module_invoke_all('dpicache_cache_entry_register');
    // @TODO ajouter un drupal_alter
  }
  return $infos;
}

/**
 * Call defined callbacked in registered dpicache entry
 * with parameters, and return result.
 *
 * @param  Array $dpicache_definition  A dpicache entry.
 * @param  Array $parameters           All parameters to pass to callback.
 * @return void                        Return of callback function. A string
 *               beginning with "#ERROR#" is returned if we are unable to
 *               call the callback function.
 */
function _dpicache_cache_call_function($dpicache_definition, $parameters=array()){
  if(_dpicache_validate_cache_entry($dpicache_definition) === FALSE){
    return "#ERROR# Cache entry defintion is not valid.";
  }
  $out = '';
  try {
    $out = call_user_func_array($dpicache_definition['callback'],$parameters);
  }
  catch (Exception $e) {
    watchdog('dpicache',"dpicache call function : Exception : @ex.",
    array('@ex'=>$e->getMessage()), WATCHDOG_ERROR);
    return "#ERROR# Exception occurred.";
  }
  return $out;
}

/**
 * Validate registered DPIcache definition.
 *
 * @param  Array  $dpicache_definition DPICache definition
 * @return Boolean                     Definition is valid, or not.
 */
function _dpicache_validate_cache_entry($dpicache_definition=array()){
  if(!$dpicache_definition || !is_array($dpicache_definition)){
    watchdog('dpicache',"dpicache validate : no valid dpicache definition provided",
    array(), WATCHDOG_WARNING);
    return FALSE;
  }
  if(!isset($dpicache_definition["callback"])){
    watchdog('dpicache',"dpicache validate : no valid callback function provided in !def",
    array('!def'=>'<pre>'.print_r($dpicache_definition,true).'</pre>'), WATCHDOG_WARNING);
    return FALSE;
  }
  $callback = $dpicache_definition["callback"];
  if(!$callback || !function_exists($callback)){
    watchdog('dpicache',"dpicache validate : callback function @func does not exist in !def.",
    array('!def'=>'<pre>'.print_r($dpicache_definition,true).'</pre>', '@func'=>$callback),
    WATCHDOG_WARNING);
    return FALSE;
  }
  return TRUE;
}

/**
 * Create/Update database with up to date data from registered dpicaches
 * @param  Array   $options dpicache_cache_entries' table's fields
 * @param  Boolean $update  If false=Creation, true=Update database
 * @return Boolean          Save success, or not.
 */
function dpicache_save_cache_entry($options=array(), $update=FALSE){
  if(!$options)
    return FALSE;
  $table_cache_entries = 'dpicache_cache_entries';
  try {
    // Insert or update
    $query = db_merge($table_cache_entries);
    $query
    ->key(array('name' => $options['name'], 'hash' => $options['hash']))
    ->insertFields(array(
      'name' => @$options['name'],
      'hash' => @$options['hash'],
      'creation_date' => @$options['creation_date'],
      'parameters' => @$options['parameters'],
      'category' => $options['category'],
      'data_type' => $options['data_type'],
      'expiration_date' => $options['expiration_date'],
      'last_run' => $options['last_run'],
      'update_freq' => $options['update_freq'],
    ))
    ->updateFields(array(
      'category' => $options['category'],
      'data_type' => $options['data_type'],
      'expiration_date' => $options['expiration_date'],
      'last_run' => $options['last_run'],
      'update_freq' => $options['update_freq'],
    ))
    ->execute();
    return TRUE;
  }
  catch (Exception $e) {
    dpi_log('dpicache', "error", t("Unable to save or update dpicache entry : !error / !options",array("!error"=>$e->getMessage(),"!options"=>print_r($options,1))),array(),@$options['hash'],WATCHDOG_ERROR);
    watchdog('dpicache', 'Save entry exception : %s', array('%s'=>$e->getMessage()), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Clean the cold cache entries table
 * by deleting out of date entries
 *
 * @param  Integer $time Current Timestamp
 * @return Boolean       Cleanup success, or not.
 */
function dpicache_cron_cleanup($time){
  if(!$time)
    $time = date();
  // Get caches that need to be deleted
  $table_cache_entries = 'dpicache_cache_entries';
  $query = db_select($table_cache_entries, 'dce');
  $query->fields('dce',array('name','hash'));
  $query->condition('dce.expiration_date',$time,'<=');
  $entries = array();
  foreach($query->execute() as $entry){
    $entries[] = array('name'=>$entry->name, 'hash'=>$entry->hash);
  }
  if(empty($entries))
    return TRUE;
  // Now, delete out of date caches
  foreach($entries as $entry){
    dpicache_delete_cache($entry['name'], $entry['hash']);
    // ... and associated entries
    $num_deleted = db_delete($table_cache_entries)
    ->condition('name',$entry['name'])
    ->condition('hash',$entry['hash'])
    ->execute();
    dpi_log('dpicache', "regenerate_cold", t("This out of date entry has been deleted : name = !cache_name, hash = !hash",array("!cache_name"=>$entry['name'],"!hash"=>$entry['hash'])),array(),$entry['hash'],WATCHDOG_DEBUG);
  }
  return TRUE;
}

/**
 * Get hash for given cache name and arguments.
 * Arguments MUST be an array of strings.
 *
 * @param  String $cache_name Name of the DPIcache.
 * @param  Array  $args       Array of callback function arguments.
 * @return String             The hash.
 */
function _dpicache_get_hash($cache_name, $args=array()){
  $hash=sha1($cache_name.'--'.implode('-',$args));
  return $hash;
}



function _dpicache_regenerate_one_cache_entry_callback($cache_name){

  $args=func_get_args();
  unset($args[0]);
  $hash=_dpicache_get_hash($cache_name, $args);
  $cache_definition=dpicache_get_cache_entry_definition($cache_name);
  $fresh_content = _dpicache_cache_call_function($cache_definition, $args);
  $saved = dpicache_save_cache($cache_name, $hash, $fresh_content, $cache_definition);
   
  return "cache regenerated";
}


function _dpicache_regenerate_all_cache_entry_callback($cache_name){

  $args=func_get_args();
  unset($args[0]);
  $hash=_dpicache_get_hash($cache_name, $args);
  $cache_definition=dpicache_get_cache_entry_definition($cache_name);
  $fresh_content = _dpicache_cache_call_function($cache_definition, $args);
  $saved = dpicache_save_cache($cache_name, $hash, $fresh_content, $cache_definition);
   
  return "cache regenerated";
}
