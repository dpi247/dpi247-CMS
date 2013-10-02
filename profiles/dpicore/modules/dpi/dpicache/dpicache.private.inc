<?php

/*
 * Call all the hook_dpicache_cache_entry_register 
 */
function _dpicache_get_all_cache_entry_definitions(){
  
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos=module_invoke_all('dpicache_cache_entry_register');
    
    //@todo: ajouter un drupal_alter
    
  }
  return $infos;
}


/*
 * return infos associated with a cache type
*/
function _dpicache_get_cache_entry_definition($cache_name){
  $infos = _dpicache_get_all_cache_entry_definitions();
  if(isset($infos[$cache_name])){
    return $infos[$cache_name];
  }
  return $false;
}

function _dpicache_fetch_cache_entry($hash,$infos){
  $table = 'dpicache_cache_entry';
  $cache_prefix = $infos['cache_prefix'];
  
  $result = FALSE;
  //Ce n'est pas la responsabilité de cette fonction de checker le TTL.
  // Si le hash exist, alors il considere l'entrée de cache comme valide.
  // C'est à un CRON de virer les entrée de cache dont le TTL est dépasé (
  // et d'eventuellement regénérer l'entrée de cache si nécéssaire...)
  
  $result = db_select($table, 'c')
    ->fields('c')
    ->condition('hash', $hash,'=')
    ->condition('cache_prefix', $cache_prefix,'=')
    ->execute()
    ->fetchAssoc();
  
  if($infos['data_type']=='string'){
    $data = $result['data'];
  }
  else{
    $data= serialize($result['data']);
  
  }
  
  return $data;
  
}
function _dpicache_save_cache_entry($hash,$infos,$cache){
  $table='dpicache_cache_entry';
  $record=array();
  
  $cache_prefix = $infos['cache_prefix'];
  $data_type = $infos['data_type'];
  $ttl = time()+$infos['default_ttl']*60;
  if($data_type == 'string'){
    $data = $cache;
  }
  else{
    $data = serialize($cache);
  }

  //See http://cms.dpi247.com/node/398  
  db_merge($table)
  ->key(array('hash' => $hash,'cache_prefix'=>$cache_prefix))
  ->fields(array(
  'ttl'=>$ttl,
  'data_type'=>$data_type,
  'data'=>$data,
  ))
  ->updateFields(array(
  'ttl'=>$ttl,
  'data'=>$data,
  ))
  ->execute();
 
}

function _dpicache_save_cache_cold($cache_name,$hash,$infos,$parameters){
  $table='dpicache_cache_cold';
  $record=array();

  $cache_prefix = $infos['cache_prefix'];
  $data_type = $infos['data_type'];
  $ttl = time()+$infos['time_window']*60;
  $next_run = time()+$infos['default_ttl']*60;
  $parameters = serialize($parameters);
  
  //See http://cms.dpi247.com/node/398  
  db_merge($table)
  ->key(array('cache_name' => $cache_name,'hash' => $hash,'cache_prefix'=>$cache_prefix))
  ->fields(array(
  'ttl'=>$ttl,
  'data_type'=>$data_type,
  'update_freq'=>$infos['default_ttl'],
  'parameters'=>$parameters,
  'next_run'=> $next_run,
  ))
  ->updateFields(array(
  'next_run'=> $next_run,
  ))
  ->execute();
}


function _dpicache_get_hash($cache_name,$args){
  
  $hash=sha1($cache_name.'-'.implode('-',$args));
  return $hash;

}

function _dpicache_regenerate_cache_entry($hash,$infos,$args){
  $callback=$infos["callback"];
  
  if(function_exists($callback)){
  
    
  $time=microtime();
  $cache = call_user_func_array($callback,$args);
  $time=microtime()-$time;
  dpi_log('cache_entry', "regenerate", t("Cache regeneration !cache_name for callback !callback regeneration time: !time",array("!cache_name"=>$cache_name,"!callback"=>$infos['callback'],"!time"=>$time)),$infos,$hash,WATCHDOG_INFO);
  
  return _dpicache_save_cache_entry($hash,$infos,$cache);
  }else{
    dpi_log('cache_error', "cache_error_callback", t("Callback !callback doesn't exist",array("!callback"=>$callback)),$infos,$hash,WATCHDOG_ERROR);
    //@todo: Trow an error
    return FALSE;
    
  }
  
}


function _dpicache_cron_cleanup_old_ttl_cache_entry($time){
  $table='dpicache_cache_entry';

  $num_deleted = db_delete($table)
  ->condition('ttl', $time,'<')
  ->execute();
}

function _dpicache_cron_cleanup_old_ttl_cold($time){
  $table='dpicache_cache_cold';

  $num_deleted = db_delete($table)
  ->condition('ttl', $time,'<')
  ->execute();
}
function _dpicache_cron_regenerate_old_ttl_cache_entry($time){
  
  // Jointure entre table dpicache_cache_entry et dpicache_cache_cold
  
  // Structure DB
  // dpicache_cache_cold(md5, params, last_generate_time, freq, ttl)
  
  // Je choppe toutes les entrées de la table dpicache_cache_cold dont 
  // le last_generate_time est inferieur ou égale à time()+freq.
  // Pour chacun je regénère le cache (update);
  // Et je mets dans dpicache_cache_cold pour TOUTES les 
  // entrées que je viens de traiter d'un seul coup le  last_generate à time()
  $table='dpicache_cache_cold';
  
  $result = db_select($table, 'n')
  ->fields('n')
  ->condition('next_run', time(),'<')
  ->execute();
  while($record = $result->fetchAssoc()) {
    $hash = $record["hash"];
    $cache_name = $record["cache_name"];
    $cache_prefix = $record["cache_prefix"];
    $infos = _dpicache_get_cache_entry_definition($cache_name);
    $params=unserialize($record["parameters"]);
    $hash=_dpicache_get_hash($cache_name, $params);
    
    dpi_log('cache_entry', "cache_cold", t("Cache cold generation !cache_name for callback !callback",array("!cache_name"=>$cache_name,"!callback"=>$infos['callback'])),$infos,$hash,WATCHDOG_INFO);
    
    $cache = _dpicache_regenerate_cache_entry($hash,$infos,$params);
    
    $new_next_run=time()+$record["update_freq"]*60;
    
    $num_updated = db_update($table) // Table name no longer needs {}
    ->fields(array(
      'next_run' => $new_next_run,
    ))
    ->condition('hash', $hash)
    ->condition('cache_name', $cache_name)
    ->condition('cache_prefix', $cache_prefix)
    ->execute();
    
    
  }

    
  $table='dpicache_cache_entry';

  $num_deleted = db_delete($table)
  ->condition('ttl', $time,'<')
  ->execute();
}
