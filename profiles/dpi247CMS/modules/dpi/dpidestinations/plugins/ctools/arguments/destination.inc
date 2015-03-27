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
	//'keyword' => 'destination_tid',
	'keyword' => 'destination',
  'description' => t('Creates a single taxonomy term from a taxonomy ID or taxonomy term name.'),
  //'context' => 'dpidestinations_context_create_section',
	'context' => 'dpidestinations_destination_context',
	'context' => 'ctools_destination_context',

  'default' => array('input_form' => 'tid', 'breadcrumb' => TRUE, 'transform' => FALSE),
  'settings form' => 'dpidestinations_destination_settings_form',
  'placeholder form' => 'dpidestinations_destination_ctools_argument_placeholder',
  'breadcrumb' => 'dpidestinations_destination_breadcrumb',
);

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


/**
 * We override the ctools_term_context to add the reference of the display handler cuz we
 * Need the did in the targetblock render
 */
function dpidestinations_destination_context($arg = NULL, $conf = NULL, $empty = FALSE) {
	// Use term context code.
	module_load_include('inc', 'ctools','plugins/arguments/term');
	$context = ctools_destination_context($arg , $conf , $empty);



		$task = $conf['task'];
		$task = page_manager_get_task('sections');

		$handlers = page_manager_load_sorted_handlers($task, '', TRUE);

		module_load_include('inc', 'ctools','includes/context-task-handler');
		//@todo: we shouldn't hard code the argument_destination_1 name
		$id = ctools_context_handler_get_render_handler($task, NULL, $handlers,array("argument_destination_1"=> $context), array());

		if (isset($handlers[$id])) {
			if(!$context->data instanceof stdClass){
				$context->data = new stdClass();
			}
			$context->data->handler = $handlers[$id];

	}


	return $context;
}



/**
 * Discover if this argument gives us the term we crave.
 */
function ctools_destination_context($arg = NULL, $conf = NULL, $empty = FALSE) {

	// If unset it wants a generic, unfilled context.
	if ($empty) {
		return ctools_context_create_empty('section');
	}

	if (is_object($arg)) {
		$term = $arg;
	}
	else {
		switch ($conf['input_form']) {
			case 'tid':
			default:
				if (!is_numeric($arg)) {
					return FALSE;
				}
				$term = taxonomy_term_load($arg);
				break;

			case 'term':
				if (!empty($conf['transform'])) {
					$arg = strtr($arg, '-', ' ');
				}

				$terms = taxonomy_get_term_by_name($arg);

				$conf['vids'] = is_array($conf['vids']) ? array_filter($conf['vids']) : NULL;
				if ((count($terms) > 1) && isset($conf['vids'])) {
					foreach ($terms as $potential) {
						foreach ($conf['vids'] as $vid => $active) {
							if ($active && $potential->vid == $vid) {
								$term = $potential;
								// break out of the foreaches AND the case
								break 3;
							}
						}
					}
				}
				$term = array_shift($terms);
				break;
		}

		if (empty($term)) {
			return NULL;
		}
	}

	if (!empty($conf['vids']) && array_filter($conf['vids']) && empty($conf['vids'][$term->vid])) {
		return NULL;
	}

	$context = ctools_context_create('section', $term);
	$context->original_argument = $arg;




	$task = page_manager_get_task('sections');

	$handlers = page_manager_load_sorted_handlers($task, '', TRUE);

	module_load_include('inc', 'ctools','includes/context-task-handler');
	//@todo: we shouldn't hard code the argument_destination_1 name r should it be sections_dpisectionspanels_context_2 ?

	$id = ctools_context_handler_get_render_handler($task, NULL, $handlers,array("sections_dpisectionspanels_context_2"=> $context), array());


	if (isset($handlers[$id])) {

		if(!$context->data instanceof stdClass){
			$context->data = new stdClass();
		}
		$context->data->handler = $handlers[$id];

	}



	return $context;
}









