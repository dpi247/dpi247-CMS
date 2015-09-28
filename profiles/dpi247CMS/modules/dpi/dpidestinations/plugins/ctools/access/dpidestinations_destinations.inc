<?php
/**
 * @file
 * Access plugin to test if element have main destination (First destination)
 * define with the same term that it found into the configuration.
 * 
 * @author  Barthélemi Laurent <lba@audaxis.com>
 * @package DPI
 * @version  1.0
 */
$plugin = array (
  'title' => t ( "DpiDestinations: Main Destination Path" ),
  'description' => t ( 'Control access by main destination.' ),
  'callback' => 'dpidestinations_status_ctools_access_check',
  'default' => array('field_destinations' => NULL),
  'settings form' => 'dpidestinations_status_ctools_access_settings',
  'settings form submit' => 'dpidestinations_status_ctools_access_settings_submit',
  'summary' => 'dpidestinations_status_ctools_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * This function Check if element can be view
 * 
 * @param array $conf
 * @param string $state
 * @return boolean
 */
function dpidestinations_status_ctools_access_check($conf, $context){
  dsm($conf, 'conf');
  dsm($context, 'nid');
  return TRUE;
}

/**
 * This function return a simple sentence to pin up the rules
 * 
 * @param array $conf
 * @param array $state
 * @return String
 */
function dpidestinations_status_ctools_access_summary($conf, $node){
  $destinations = dpidestinations_api_get_all_sections_term_available_as_select_options(FALSE);
  $str = "Dpidestinations : Define for ";
  foreach($conf["destination_vid"] as $v){
    $str .= $destinations[$v] . ", ";
  }
  return t ( '@str', array( '@str' => substr($str, 0, -2) ) );
}

/**
 * Form to define main destinations available for elements.
 * 
 * @param Array $form
 * @param Array $form_state
 * @param Array $conf
 * @return Array
 */
function dpidestinations_status_ctools_access_settings($form, &$form_state, $conf){
  /* Get all destination name with vid key */
  $destinations_tmp = dpidestinations_api_get_all_sections_term_available_as_select_options(FALSE);
  $form = array();
  
  /* Delete first element */
  $destinations = array();
  foreach($destinations_tmp as $k => $v){
    if($k){
      $destinations[$k] = $v;
    }
  }
  
  /* Create form element */
  $form["destination_vid"] = array(
    '#type' => 'select',
    '#title' => t('Select destinations'),
    '#options' => $destinations,
    '#default_value' => $conf['destination_vid'],        
    '#multiple' => TRUE, 
    '#size' => 10,
  );
  
  return $form;  
}

/**
 * This function replace type of element into the right part of the form at the submition
 * @param array $form
 * @param array $form_state
 */
function dpidestinations_status_ctools_access_settings_submit($form, &$form_state){
  $form_state['values']['settings']['destination_vid'] = $form_state['values']['destination_vid'];
}