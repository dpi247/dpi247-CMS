<?php 

function hook_dpicache_cache_entry_register(){
  return array(
    'flowmix_render'=>array(
      'callback'=> 'flowmix_get_render',
      'plugin'=> 'dpicache_memcache',
      'cache_prefix'=> 'flowmix_render',
      "data_type" => "string",
      'time_window' => 24,
      'default_ttl'=> 5
    )
  );
}



function dpicache_get_cache_entry($cache_name){
  $infos=_dpicache_get_cache_entry_definition($cache_name);
  if($infos==FALSE){
    //@todo: Trow an error
    return FALSE;
  }
  
  
  $args=func_get_args();
  array_shift($args);
  $hash=_dpicache_get_hash($cache_name,$args);
    
    if(  $cache=_dpicache_fetch_cache_entry($hash,$infos)){
      //@todo:Remplacer par des dpilogs
      dpi_log('cache_entry', "cache_hit", t("Cache hit !cache_name for callback !callback",array("!cache_name"=>$cache_name,"!callback"=>$infos['callback'])),$infos,$hash,WATCHDOG_INFO);
      
     return $cache;
    }
    else{
      //@todo:Remplacer par des dpilogs
      dpi_log('cache_entry', "cache_miss", t("Cache miss !cache_name for callback !callback",array("!cache_name"=>$cache_name,"!callback"=>$infos['callback'])),$infos,$hash,WATCHDOG_INFO);
      dpi_log('cache_entry', "cache_hot", t("Cache hot generation !cache_name for callback !callback",array("!cache_name"=>$cache_name,"!callback"=>$infos['callback'])),$infos,$hash,WATCHDOG_INFO);
      
      $cache = _dpicache_regenerate_cache_entry($hash,$infos,$args);
      
      //@todo: créer une entrée dans la table de génération à froid.
      // Si j'ai du regénérer la cache manuellement alors
      // => je dois sauver également une entrée dans la table de géneration à froid
      
      // A priori je ne rafraichis ma cache a chaud que une fois sur une période de "time_window".
      // En fait: je rafraichis la cache à froid d'un nombre [division entier de time_window/freq] avant de rafraichir le cache à chaud
      _dpicache_save_cache_cold($cache_name,$hash,$infos,$args);
      return $cache;
      
    }
    return FALSE;
  
  
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