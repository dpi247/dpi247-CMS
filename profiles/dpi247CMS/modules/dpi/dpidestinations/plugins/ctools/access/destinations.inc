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
$plugin = array(
  'title' => t ( "Node: Main Destination Path" ),
  'description' => t('Only displays this pane if the Widget field on the related Home Page for this Organic Group is set to On.'),
  'callback' => 'dpidestinations_node_ctools_access_check',
  'default' => array('node' => 1),
  'summary' => 'dpidestinations_node_ctools_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'settings form' => 'dpidestinations_node_ctools_access_settings',
  'settings form submit' => 'dpidestinations_node_ctools_access_settings_submit',
);

/**
 * This function Check if element can be view
 * 
 * @param array $conf
 * @param string $state
 * @return boolean
 */
function dpidestinations_node_ctools_access_check($conf, $context = NULL){
  /* Return false if no context found */
  if(empty($context) || empty($context->data)){
   return FALSE;
  }
  
  /* Get items from node data context */
  $items = field_get_items('node', $context->data, 'field_destinations');
  $item = !empty($items) ? current($items) : array();

  /* Test presence of tid and test if he is concerne by this access */
  if(!empty($item) && in_array($item['tid'], $conf['destination_vid'])===TRUE){
    return TRUE;
  }
  
  return FALSE;
}

/**
 * This function return a simple sentence to pin up the rules
 * 
 * @param array $conf
 * @param array $state
 * @return String
 */
function dpidestinations_node_ctools_access_summary($conf, $node){
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
function dpidestinations_node_ctools_access_settings($form, &$form_state, $conf){
  /* Get all destination name with vid key */
  $destinations_tmp = dpidestinations_api_get_all_sections_term_available_as_select_options(FALSE);
  
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
function dpidestinations_node_ctools_access_settings_submit($form, &$form_state){
  $form_state['values']['settings']['destination_vid'] = $form_state['values']['destination_vid'];
}