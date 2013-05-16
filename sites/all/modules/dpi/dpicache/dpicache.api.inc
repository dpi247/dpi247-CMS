<?php 

/**
 * Returns a persistent variable.
 *
 * Case-sensitivity of the _wally_variable_* functions depends on the database
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
  }
  return $dpivariables[$name];
  
}

/**
 * Alias to _dpi_variable_get_all
 *
 * @return
 *   The value of the variables.
 *
 * @see dpi_variable_get_all(), dpi_variable_get(), dpi_variable_del(), dpi_variable_set()
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
 * Alias to _dpi_variable_set
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 *
 * @see _dpi_variable_set(), dpi_variable_del(), dpi_variable_get(), dpi_variable_get_all()
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
 * Alias to _dpi_variable_del
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see _dpi_variable_del(), dpi_variable_get(), dpi_variable_set(), dpi_variable_get_all()
 */
function dpi_variable_del($name) {
  db_delete('dpi_variables')
  ->condition('name', $name)
  ->execute();
  drupal_static_reset($name);
}
