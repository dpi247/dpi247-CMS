<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for a Taxonomy term
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Destination term: ID"),
  // keyword to use for %substitution
  'keyword' => 'destination',
  'description' => t('Creates a single taxonomy term from a taxonomy ID or taxonomy term name.'),
  'context' => 'dpidestinations_destination_context',
  'default' => array('input_form' => 'tid', 'breadcrumb' => TRUE, 'transform' => FALSE),
  'settings form' => 'dpidestinations_destination_settings_form',
  'placeholder form' => 'dpidestinations_destination_ctools_argument_placeholder',
  'breadcrumb' => 'dpidestinations_destination_breadcrumb',
);

/**
 * 
 * We override the ctools_term_context to add the reference of the display handler cuz we
 * Need the did in the targetblock render
 * 
 */
function dpidestinations_destination_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  
  // Use term context code.
  module_load_include('inc', 'ctools','plugins/arguments/term');
  $context = ctools_term_context($arg , $conf , $empty);

  $task=$conf['task'];
  $handlers = page_manager_load_sorted_handlers($task, '', TRUE);
  
  //@todo: we shouldn't hard code the argument_destination_1 name 
  $id = ctools_context_handler_get_render_handler($task, NULL, $handlers,array("argument_destination_1"=> $context), array());
  
  $context->data->handler=$handlers[$id];
  
  return $context;
}

/**
 * Settings form for the argument
 */
function dpidestinations_destination_settings_form(&$form, &$form_state, $conf) {
  // @todo allow synonym use like Views does.
  $form['settings']['input_form'] = array(
    '#title' => t('Argument type'),
    '#type' => 'radios',
    '#options' => array('tid' => t('Term ID'), 'term' => t('Term name')),
    '#default_value' => $conf['input_form'],
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',
  );

  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabularies as $vid => $vocab) {
    $options[$vid] = $vocab->name;
  }
  $form['settings']['vids'] = array(
    '#title' => t('Limit to these vocabularies'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => !empty($conf['vids']) ? $conf['vids'] : array(),
    '#description' => t('If no vocabularies are checked, terms from all vocabularies will be accepted.'),
  );

  $form['settings']['breadcrumb'] = array(
    '#title' => t('Inject hierarchy into breadcrumb trail'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['breadcrumb']),
    '#description' => t('If checked, taxonomy term parents will appear in the breadcrumb trail.'),
  );

  $form['settings']['transform'] = array(
    '#title' => t('Transform dashes in URL to spaces in term name filter values'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['transform']),
  );
//  return $form;
}

/**
 * Form fragment to get an argument to convert a placeholder for preview.
 */
function dpidestinations_destination_ctools_argument_placeholder($conf) {
  switch ($conf['input_form']) {
    case 'tid':
    default:
      return array(
        '#type' => 'textfield',
        '#description' => t('Enter a taxonomy term ID.'),
      );
    case 'term':
      return array(
        '#type' => 'textfield',
        '#description' => t('Enter a taxonomy term name.'),
      );
  }
}

/**
 * Inject the breadcrumb trail if necessary.
 */
function dpidestinations_destination_breadcrumb($conf, $context) {
  if (empty($conf['breadcrumb']) || empty($context->data) || empty($context->data->tid)) {
    return;
  }

  $breadcrumb = array();
  $current = new stdClass();
  $current->tid = $context->data->tid;
  while ($parents = taxonomy_get_parents($current->tid)) {
    $current = array_shift($parents);
    $breadcrumb[] = l($current->name, 'destination/' . $current->tid);
  }

  $breadcrumb = array_merge(drupal_get_breadcrumb(), array_reverse($breadcrumb));
  drupal_set_breadcrumb($breadcrumb);
}
