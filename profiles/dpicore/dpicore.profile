<?php
/**
 * @file
 * Enables modules and site configuration for a minimal site installation.
 */

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function dpicore_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}
/**
 * 
 * Implementation of hook_ install_tasks
 * @param Array $install_state
 */
function dpicore_install_tasks($install_state) {
  
  
  $tasks = array;
  
  //Set the taxonomy vid into dpi_variable
  $tasks['set_taxonomy_variable'] = array(
    'display_name' => st('Set taxonomy variables'),
    'display' => TRUE,
    'type' => 'normal',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'dpicore_install_set_taxonomy_variables',
  );
  return $tasks;
}

/**
 * 
 * Set the taxonomy vid into dpi_variable
 */
function dpicore_install_set_taxonomy_variables(){
  //Set the vid of the vocabulary in wally_variable_set
  dpicore_install_set_a_taxonomy_variable("authors");
  dpicore_install_set_a_taxonomy_variable("concepts");
  dpicore_install_set_a_taxonomy_variable("entities");
  dpicore_install_set_a_taxonomy_variable("freetags");
  dpicore_install_set_a_taxonomy_variable("iptc");
  dpicore_install_set_a_taxonomy_variable("locations");
  dpicore_install_set_a_taxonomy_variable("packagelayout");
  dpicore_install_set_a_taxonomy_variable("persons");
  //dpicore_install_set_a_taxonomy_variable("rating");

}

/**
 * 
 * Get the vid of a vocabulary by its name and set it in a dpi_variale
 * @param String $name
 */
function dpicore_install_set_a_taxonomy_variable($name){
  $result = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = :machine_name", array(':machine_name' => $name));
  $vid = NULL;

  foreach ($result as $record){
    $vid = $record->vid;
  }

  dpi_variable_set('dpi_'.$name, $vid);
}