<?php
/**
 * @file
 * Access plugin to test if element have parents term for main destinations
 * define with the same term that it found into the configuration.
 * 
 * @author  Barthélemi Laurent <lba@audaxis.com>
 * @package DPI
 * @version  1.0
 */
$plugin = array(
  'title' => t ( "Node: Main Destination Path has parents" ),
  'description' => t('Only displays this pane if the Widget field on the related Home Page for this Organic Group is set to On.'),
  'callback' => 'dpidestinations_has_parents_node_ctools_access_check',
  'default' => array('node' => 1),
  'summary' => 'dpidestinations_has_parents_node_ctools_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'settings form' => 'dpidestinations_has_parents_node_ctools_access_settings',
);

/**
 * This function Check if element can be view
 * 
 * @param array $conf
 * @param string $state
 * @return boolean
 */
function dpidestinations_has_parents_node_ctools_access_check($conf, $context = NULL){  
  /* Return false if no context found */
  if(empty($context) || empty($context->data)){
   return FALSE;
  }
  
  /* Get items from node data context */
  $items = field_get_items('node', $context->data, 'field_destinations');
  $item = !empty($items) ? current($items) : array();

  // Get the $vid.
  if (!isset($conf['vid'])) {
    return FALSE;
  }
  $vid = $conf['vid'];  
  $tree = dpidestinations_has_parents_get_taxonomy_tree($vid, $item['tid']);  

  // Get the terms.
  if (!isset($conf[$vid])) {
    return FALSE;
  }
  
  $return = FALSE;  
  $terms = array_filter($conf[$vid]);
  
  if(is_array($tree)){
    return (bool) array_intersect($tree, $terms);
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
function dpidestinations_has_parents_node_ctools_access_summary($conf, $node){
  return t ( 'Destination path have parent.' );
}

/**
 * Form to define main destinations available for elements.
 * 
 * @param Array $form
 * @param Array $form_state
 * @param Array $conf
 * @return Array
 */
function dpidestinations_has_parents_node_ctools_access_settings($form, &$form_state, $conf){
  // If no configuration was saved before, set some defaults.
  if (empty($conf)) {
    $conf = array(
      'vid' => 0,
    );
  }
  if (!isset($conf['vid'])) {
    $conf['vid'] = 0;
  }

  $form['settings']['vid'] = array(
    '#title' => t('Vocabulary'),
    '#type' => 'select',
    '#options' => array(),
    '#description' => t('Select the vocabulary for this form.'),
    '#id' => 'ctools-select-vid',
    '#default_value' => $conf['vid'],
    '#required' => TRUE,
  );

  ctools_include('dependent');
  $options = array();

  // A note: Dependency works strangely on these forms as they have never been
  // updated to a more modern system so they are not individual forms of their
  // own like the content types.

  $form['settings']['#tree'] = TRUE;

  // Loop over each of the configured vocabularies.
  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $options[$vid] = $vocabulary->name;
    $form['settings'][$vocabulary->vid] = array(
      '#title' => t('Terms'),
      '#description' => t('Select a term or terms from @vocabulary.', array('@vocabulary' => $vocabulary->name)), //. $description,
      '#dependency' => array('ctools-select-vid' => array($vocabulary->vid)),
      '#default_value' => !empty($conf[$vid]) ? $conf[$vid] : '',
      '#multiple' => TRUE,
    );

    $terms = array();
    foreach (taxonomy_get_tree($vocabulary->vid) as $tid => $term) {
      $terms[$term->tid] = str_repeat('-', $term->depth) . ($term->depth ? ' ' : '') . $term->name;
    }
    $form['settings'][$vocabulary->vid]['#type'] = 'select';
    $form['settings'][$vocabulary->vid]['#options'] = $terms;
    unset($terms);
  }
  $form['settings']['vid']['#options'] = $options;
  return $form;
}

/**
 * This function return parents term from a tid term.
 * @param int $vid
 * @param int $tid
 * @return Array
 */
function dpidestinations_has_parents_get_taxonomy_tree($vid, $tid){
  $result = array();  
  foreach (taxonomy_get_parents_all($tid) as $v){
    $result[] = $v->tid;
  }  
  return $result;
}