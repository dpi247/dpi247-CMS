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
 *   The default value to use if this variable doesn't exist.
 * @return
 *   The value of the variable.
 *
 * @see _wally_variable_del(), _wally_variable_set(), _wally_variable_get_all()
 */
function _wally_variable_get($name, $default, $regenerate = FALSE) {
  
}

/**
 * Returns all persistents variables.
 *
 * @return
 *   The value of the variables.
 *
 * @see _wally_variable_get(), _wally_variable_del(), _wally_variable_set()
 */
function _wally_variable_get_all() {

}

/**
 * Sets a persistent variable.
 *
 * Case-sensitivity of the _wally_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 *
 * @see _wally_variable_del(), _wally_variable_get(), _wally_variable_get_all()
 */
function _wally_variable_set($name, $value) {

}

/**
 * Unsets a persistent variable.
 *
 * Case-sensitivity of the _wally_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see _wally_variable_get(), _wally_variable_set(), _wally_variable_get_all()
 */
function _wally_variable_del($name) {
  global $wally_vars;

  db_query("DELETE FROM {wally_variable_fast} WHERE name = '%s'", $name);

  unset($wally_vars[$name]);
}