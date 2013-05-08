<?php
/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('TargetBlock'),
  'description' => t('Crafted links to create content (nodes) for a given group.'),
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
  'category' => t('Target'),
  'group'=>t('DPI Core'),
  
  'all contexts' => TRUE,
  
);



/**
 * Implementation of hook_typeofcontent_content_type_content_types
 * Return all custom content types available.
 */
function dpidestinations_targetblock_content_type_content_types() {
  $types = array();
  $targets=target_load_multiple(FALSE);
  foreach($targets as $target){
    $types[$target->machine_name]= array(
      'title' => check_plain($target->label),
      'description' =>check_plain($target->description),
      //'required context' => new ctools_context_required(t('Destination'), 'destination'),
      'category' => t('Target'),
      'group'=>t('DPI Core'),
      
      //'all contexts' => TRUE,
    
    );
    
  }

  return $types;
}


/**
 * Render callback.
 */
function dpidestinations_targetblock_content_type_render($subtype, $conf, $args, $context) {
  $module = 'dpidestinations';
  $block = new stdClass();
  $block->module = $module;
  $block->title = t('Redacblock');
  $block->content = t('Redacblock Content dsfs dfsdv dsv qfv sdfvdsfv dfsv sdrversvrqz verqv eqrv
  reqsfser fqer
  fqer gfvqerfv zqerf
  eqrfv erqv
  qerv qerv
  qerfv qezrfvqe
  srgvfqe rsfvqers
  fge qrfvqer
  gvqer fvgeqr
  v');
     $block->admin_links = array(
      array(
        'title' => t('Configure block'),
        'href' => "admin/structure/block/manage/$module/$delta/configure",
        'query' => drupal_get_destination(),
      ),
    );
  return $block;
  
  if (empty($context->data)) {
    return FALSE;
  }
  
  $module = 'dpidestinations';
  $block = new stdClass();
  $block->module = $module;
  $block->title = t('Redacblock');
  
  $block->content ='rr';
  return $block;
}


/**
 * Edit form.
 */
function dpidestinations_targetblock_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  // Make the other form items dependent upon it.
  ctools_include('dependent');
  ctools_add_js('dependent');
  
  
  $form['pager_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Custom pager settings'),
  );

  $form['pager_settings']['override_pager_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overriding view settings'),
    '#default_value' => $conf['override_pager_settings'],
  );

  $form['pager_settings']['node_count_for_targetblockinstance'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['node_count_for_targetblockinstance'],
    '#title' => t('Node count for this targetblock instance'),
    '#size' => 4,
    '#description' => t('The node count for this targetblock instance.'),
    '#dependency' => array('override-pager-checkbox' => array(1)),
  );
  
  $form['pager_settings']['use_pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => $conf['use_pager'],
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="override_pager_settings"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),    
  );

  $form['pager_settings']['use_splitted'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use splited blocks'),
    '#default_value' => $conf['use_splitted'],
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="override_pager_settings"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
      
  );
  $form['pager_settings']['node_count_per_target'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['node_count_per_target'],
    '#size' => 4,
    '#title' => t('Node count for this targetblock (sum of the targetblock instance)'),
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );

  $form['pager_settings']['offset'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['offset'],
    '#title' => t('Offset'),
    '#size' => 4,
    '#description' => t('The number of items to skip and not display.'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );

  
  $form['pager_settings']['split_index_count'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['split_index_count'],
    '#title' => t('Split index'),
    '#size' => 4,
    '#description' => t('The index of this redacblock -if this is the third splitted redabclokc it will be 3- (Only if the redac block is splitted across the page).'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );

  $form['pager_settings']['split_total_count'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['split_total_count'],
    '#title' => t('# of splited blocks'),
    '#size' => 4,
    '#description' => t('The total number of splitted block. I there is three redacblock it will be 3'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
    
  );

  $form['pager_settings']['hide_pager'] = array(
    '#type' => 'checkbox',
    '#default_value' => $conf['hide_pager'],
    '#title' => t('Hide pager'),
    '#size' => 4,
    '#description' => t('Check this if you don\'t want the pager to display above this redacblock'),
  	 '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );

  $form['pager_settings']['pager_id'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['pager_id'],
    '#title' => t('Pager ID'),
    '#size' => 4,
    '#description' => t('This ID is use to synchronize the splitted blocks with the pager. You need to set the same one on each splitted block.'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
    
  );
  return $form;
}


/**
 * Edit form submit callback.
 */
function dpidestinations_targetblock_content_type_edit_form_submit($form, &$form_state) {
  //$form_state['conf']['field_name'] = $form_state['values']['field_name'];
  //$form_state['conf']['types'] = array_filter($form_state['values']['types']);
  //DEFAULT SAVE OVERRIDABLE VARIABLES:
  $confs=array(
    'override_title',
    'override_pager_settings',
    'nodes_count_for_targetblockinstance',
    'use_pager',
    'use_splitted',
    'node_count_for_target',
    'offset',
    'split_index_count',
    'split_total_count',
    'hide_pager',
    'pager_id',
  );
  foreach($confs as $conf){
    $form_state['conf'][$conf] = $form_state['values'][$conf];
    
  }
  $form_state['conf'][$key] = $form_state['values'][$key];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
