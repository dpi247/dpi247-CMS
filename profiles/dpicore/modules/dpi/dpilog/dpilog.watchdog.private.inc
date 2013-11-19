<?php

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