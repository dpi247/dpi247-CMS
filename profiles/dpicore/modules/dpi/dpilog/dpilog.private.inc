<?php
function dpilog_get_logs_query_based_on_filters( $filters){



  $query = db_select('dpilog_logs', 'l');
  $query->join('users', 'u', 'l.user = u.uid'); //JOIN node with users
  $query->fields('l', array('lid','log_name', 'action','message','user', 'timestamp','id','severity','thread_id'));
  $query->fields('u', array('name'));
  $query = $query
  ->extend('PagerDefault')->limit(30);


  foreach($filters as $filter_name=>$filter_value){
    if($filter_value!='' && $filter_value!=array()){
      switch($filter_name){
        case 'log_name':
          $query->condition('l.log_name', $filter_value,'IN');
          break;
        case 'action':
          $query->condition('l.action', $filter_value, 'IN');
          break;
        case 'id':
          $query->condition('l.id', $filter_value,'=');
          break;
        case 'user':
          $query->condition('l.user', $filter_value,'=');
          break;
        case 'thread_id':
          $query->condition('l.thread_id', $filter_value,'=');
          break;
        case 'before_timestamp':
          $query->condition('l.timestamp', $filter_value,'<');
          break;
        case 'after_timestamp':
          $query->condition('l.timestamp', $filter_value,'>');
          break;
        case 'id':
          $query->condition('l.id', $filter_value,'IN');
          break;
        case 'severity':
          $query->condition('l.severity', $filter_value,'IN');
          break;
      }
    }
  }
  return  $query;
}

// We dont plan to use Mysql delayed because of: https://drupal.org/node/772554

function _dpilog_log($log_name,$action,$message,$variables=array(),$id=NULL,$severity=WATCHDOG_NOTICE,$uid=NULL,$keep_backtrace=FALSE){
  $granularity=dpi_variable_get('dpilog_configuration_granularity',array());
  if(isset($granularity['registrations']))
    $registrations=$granularity['registrations'];
  else
    $registrations = array();

  // defaut case
  if(!isset($registrations[$log_name][$action])){
    $registrations[$log_name][$action]['dpilog'] = 'ignore';
    $registrations[$log_name][$action]['watchdog'] = 'ignore';
    $registrations[$log_name][$action]['color'] = '';
  }
  $dpilog_method=$registrations[$log_name][$action]['dpilog'];
  $watchdog_method=$registrations[$log_name][$action]['watchdog'];

  // DPI Log
  if($dpilog_method == "save"){
    $dpilog_structure=_dpilog_format_dpilog_structure($log_name, $action, $message, $severity, $uid,$id,$variables,$keep_backtrace);
    _dpilog_delayed_save($dpilog_structure,'dpilog');
    //drupal_write_record('dpilog_logs', $dpilog_structure);
  }

  // DPI Watchdog
  switch ($watchdog_method) {
    case 'save_debug':
      watchdog($log_name.'_'.$action, $message, $variables, WATCHDOG_DEBUG);
      break;
    case 'save_info':
      watchdog($log_name.'_'.$action, $message, $variables, WATCHDOG_INFO);
      break;
    case 'save_warning':
      watchdog($log_name.'_'.$action, $message, $variables, WATCHDOG_WARNING);
      break;
    case 'save_error':
      watchdog($log_name.'_'.$action, $message, $variables, WATCHDOG_ERROR);
      break;
    case 'ignore':
    default:
      break;
  }
}



function _dpilog_format_dpilog_structure($log_name,$action,$message,$severity=DPILOG_SEVERITY_DEFAULT,$uid=NULL, $id=NULL,$variables=array(),$keep_backtrace=FALSE){
  global $dpilog_thread_id;
  global $user;

  if(!isset($uid)){
    $uid=$user->uid;
  }
  if(!isset($dpilog_log->timestamp)){
    $timestamp=time();

  }

  $dpilog_log=new stdClass();
  $dpilog_log->log_name=$log_name;
  $dpilog_log->action=$action;

  $dpilog_log->user=$uid;
  $dpilog_log->message=$message;
  $dpilog_log->id=$id;
  $dpilog_log->variables=serialize($variables);
  $dpilog_log->severity= $severity;
  $dpilog_log->timestamp=$timestamp;
  if($keep_backtrace){
    try {
      $dpilog_log->trace=serialize(debug_backtrace());
    }
    catch(Exception $e){
      $dpilog_log->trace=NULL;
    }
  }
  else{
    $dpilog_log->trace=NULL;
  }
  $dpilog_log->thread_id=$dpilog_thread_id;

  return $dpilog_log;

}

function _dpilog_get_modules_registration(){

  $definitions=array();
  $registrations=module_invoke_all('dpilog_register');
  return $registrations;

}


function _dpilog_get_watchdogs_actions(){
  return array(
    'ignore' => t('Ignore'),
    'save_debug' => t('Save as debug'),
    'save_info' => t('Save as info'),
    'save_warning' => t('Save as warning'),
    'save_error' => t('Save as error')
  );
}

function _dpilog_get_dpilogs_actions(){
  return array(
    'ignore' => t("Ignore"),
    'save'   => t("Save"),
  );
}

function _dpilog_format_watchdog_structure($log_name, $action, $message, $severity, $uid,$id,$variables,$keep_backtrace,$dpilog_structure){

  $watchdog->type=$log_name.'-'.$action;
  $watchdog->message=$message;
  $watchdog->variables=$variables;
  $watchdog->severity=$severity;
  $watchdog->link=NULL;
  if(isset($dpilog_structure->lid)){
    $watchdog->link=l('dpilog','admin/dpi/dpilogs/dpilog/log/'.$dpilog_structure->lid). ' '.l('dpilog','admin/dpi/dpilogs/dpilog/log/'.$dpilog_structure->lid);
  }
  return $watchdog;
}

function _dpilog_delayed_save($dpilog_structure,$structure){

  global $dpilog_delayed_save;

  $dpilog_delayed_save[$structure][]=$dpilog_structure;


}



function _dpilog_random_color_part() {
  return str_pad( dechex( mt_rand( 0, 255 ) ), 2, '0', STR_PAD_LEFT);
}

function _dpilog_random_color() {
  return "#"._dpilog_random_color_part() . _dpilog_random_color_part() . _dpilog_random_color_part();
}

// --------------------------------------------------------------
// ------------------ Smart Watchdogs ---------------------------
// --------------------------------------------------------------


/**
 * Returns name of SQL table used for logs
 * @return String SQL table name.
 */
function _dpilog_watchdog_get_table_name(){
  return "dpilog_watchdog";
}

/**
 * Gathers a list of uniquely defined database log message types.
 *
 * @return array
 *   List of uniquely defined database log message types.
 */
function _dpilog_watchdog_get_message_types() {
  $types = array();

  $result = db_query('SELECT DISTINCT(type) FROM {'._dpilog_watchdog_get_table_name().'} ORDER BY type');
  foreach ($result as $object) {
    $types[] = $object->type;
  }
  return $types;
}

/**
 * Default rules for watchdogs event
 * @return Array modules=>watchdog levels=>action
 */
function _dpilog_watchdog_default_rules(){
  return array("DEFAULT" => array_fill_keys(array_keys(watchdog_severity_levels()), "save_simple"));
}

/**
 * Current rules for watchdog events
 * @return Array modules=>watchdog levels=>action
 */
function _dpilog_watchdog_get_rules(){
  return variable_get("dpilog_watchdog_rules_map", _dpilog_watchdog_default_rules());
}

/**
 * Get available actions for watchdogs event
 * @return Array code name => description
 */
function _dpilog_watchdog_get_possible_actions(){
  return array(
    "save_simple"    => "Save",
    "save_backtrace" => "Save with trace",
    "ignore"         => "Ignore"
  );
}

/**
 * Get the appropriate action when a
 * watchdog event occurs, given its
 * type and its severity.
 * @see _dpilog_watchdog_get_possible_actions()
 * @param  Array $log A watchdog.
 * @return String The action's machine name.
 */
function _dpilog_watchdog_get_action($log){
  if(!is_array($log))
    return "save_simple";
  $log_type = $log['type'] ?: "DEFAULT";
  $log_level = (abs(intval($log['severity'])) == $log['severity']) ? $log['severity'] : WATCHDOG_NOTICE;
  if($log_level > WATCHDOG_DEBUG)
    $log_level = WATCHDOG_NOTICE;
  $mapping = _dpilog_watchdog_get_rules();
  if(!is_array($mapping))
    return "save_simple";
  if(!isset($mapping[$log_type]))
    return $mapping["DEFAULT"][$log_level];
  return $mapping[$log_type][$log_level];
}

/**
 * Check if an PHP array of settings is valid
 * It has to be in this format:
 * array(
 *   "DEFAULT" => array(
 *      WATCHDOG_DEBUG => "save_now",
 *      WATCHDOG_NOTICE => "ignore",
 *      [...]),
 *   "module1" => array([...])
 * );
 * @param $settings {Array} An array of filters settings
 * @return {Boolean} TRUE if settings are valid, FALSE if they are not.
 */
function _dpilog_watchdog_check_settings($settings=array()){
  if(!isset($settings) || !is_array($settings) || empty($settings))
    return FALSE;
  if(!isset($settings['DEFAULT']))
    return FALSE;
  $available_levels = array_keys(watchdog_severity_levels());
  $available_actions = array_keys(_dpilog_watchdog_get_possible_actions());
  foreach($settings as $key => $levels){
    if(!is_string($key))
      return FALSE;
    if(!is_array($levels))
      return FALSE;
    foreach($levels as $level_value => $level_action){
      if(!in_array($level_value,$available_levels))
        return FALSE;
      if(!in_array($level_action,$available_actions))
        return FALSE;
    }
  }
  return TRUE;
}