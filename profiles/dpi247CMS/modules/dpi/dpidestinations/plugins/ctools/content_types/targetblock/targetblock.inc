<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('TargetBlock'),
  'description' => t('Crafted links to create content (nodes) for a given group.'),
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
  'category' => t('Target'),
  'group' => t('DPI Core'),
  'admin info' => 'dpidestinations_targetblock_admin_info',
  
  'all contexts' => TRUE,
);

/**
 * Implementation of hook_typeofcontent_content_type_content_types
 * Return all custom content types available.
 */
function dpidestinations_targetblock_content_type_content_types($plugin) {
  $types = array();
  $targets = target_load_multiple(FALSE);
  foreach($targets as $target) {
    $types[$target->machine_name] = array(
      'title' => check_plain($target->machine_name),
      'subtype' => $target->machine_name,
      'description' => check_plain($target->description),
      'icon'  =>  drupal_get_path("module", "dpidestinations").'/images/1412086058_93156.ico',
      //'required context' => new ctools_context_required(t('Destination'), 'destination'),
      'category' => t('Target'),
      'group' => t('DPI Core'),
      //'all contexts' => TRUE,
    );
  }

  return $types;
}

function dpidestinations_targetblock_admin_info($subtype, $conf, $contexts) {
  
  if($conf['use_pager']==0){
    $pager='Pas de pager';
  }
  else{
    $pager=$conf['pager_type'];
  }
 
  if($conf['use_split']!=0){
    $split='(Split)';
  }
  else{
    $split="";
  }
  
  
    $block = new stdClass;
    $block->title = 'Target : <strong>'.$subtype.'</strong> '.$split;
    $block->content = t('Showing: <em>@row_count nodes</em><br/> 
        Nombre de position reservées: <em>@reserved_position</em><br/>
        Pager: <em>@pager</em>.<br/>
        
        ', array(
       '@reserved_position'   =>$conf['rank_max_value'],
      '@row_count' => $conf['node_count_for_targetblockinstance'],
      '@pager' => $pager,
        ));
    return $block;
  
}

/**
 * Render callback.
 */
function dpidestinations_targetblock_content_type_render($subtype, $conf, $args, $context) {
  $module = 'dpidestinations';
  $block = new stdClass();
  $block->module = $module;

  $destination_term_id = $context['argument_destination_1']->data->tid;
  $did = $context['argument_destination_1']->data->handler->did;
  
  $target_name = $subtype;
  $split_delta = $conf["split_delta"];
  $title = _dpidestinations_targetblock_content_type_helper_get_title($conf);
  return dpicache_get_cache_entry('dpidestinations_targetblock_render', $did, $destination_term_id, $target_name, $split_delta,$title);
}

function _dpidestinations_targetblock_content_type_render_process($did, $destination_term_id, $target_name, $split_delta, $title) {
  $targetBlockController = new TargetBlockController('targetblock');
  $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta($did, $target_name, $split_delta);

  // @todo: check si on a pas de targetblock
  // si on a pas  si le user est admin afficher un block d'erreur
  // Si user est anonyme=> block vide
  // et watchdog

  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
  $target = $targetblock_wrapper->target->value();
  $target_wrapper = entity_metadata_wrapper('target', $target);

  $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview($destination_term_id, $target_name, $targetblock);

  $targetview_clone = _dpidestinations_targetblock_content_type_helper_get_local_targetview($targetblock, $targetview);

  $block = new stdClass();
  $block->title = $title;
  $block->content = _dpidestinations_targetblock_content_type_helper_get_content($targetview_clone);
  $block->admin_links = _dpidestinations_targetblock_content_type_helper_get_admin_links($did, $destination_term_id, $target_name, $split_delta,$title);

  return $block;
}

function _dpidestinations_targetblock_content_type_helper_get_global_targetview($destination_term_id, $target_machine_name, $targetblock) {
  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
  $target=$targetblock_wrapper->target->value();
  $target_wrapper = entity_metadata_wrapper('target', $target);

  $targetviews = &drupal_static('dpidestinations_targetviews');
  $targetviews_values = &drupal_static('dpidestinations_targetview_valuess');

  if (!isset($targetviews[$destination_term_id][$target_machine_name])) {
    $targetview = $target_wrapper->targetview->value();
    _dpidestinations_targetview_initialize_view($targetview, $destination_term_id, $target_machine_name, $targetblock);

    //see: https://drupal.org/node/1556174#comment-5943546 as we cannot use views_embed_view we generate dom_id :s
    $targetview->dom_id = uniqid();
    $targetview->execute();

    $targetviews_values[$destination_term_id][$target_machine_name] = $targetview->result;
    $targetviews[$destination_term_id][$target_machine_name] = $targetview;
  }
  return $targetviews[$destination_term_id][$target_machine_name]  ;
}

function _dpidestinations_targetblock_content_type_helper_get_local_targetview($targetblock, $targetview) {
  $targetviews_values = &drupal_static('dpidestinations_targetview_valuess');

  $targetview_clone = clone($targetview) ;
  $targetview_clone->dpidestinations['targetblock'] = $targetblock;

  $term_value = $targetview_clone->dpidestinations['destination']->tid;
  $target_value = $targetview_clone->dpidestinations['target']->machine_name;
  $result_save = $targetviews_values[$term_value][$target_value];

  $result = $result_save;
  $result = array_slice($result, $targetblock->offset, $targetblock->node_count_for_targetblockinstance);

  $targetview_clone->display_handler->view->result = $result;
  $targetview_clone->result = $result;

  //We absolutly need this since the style will use cache rendering if exist
  // see $view->style_plugin->get_field($index, $field);
  // see template_preprocess_views_view_fields
  unset($targetview_clone->style_plugin->rendered_fields);
   
  return $targetview_clone;
}

/**
 * Edit form.
 */
function dpidestinations_targetblock_content_type_edit_form($form, &$form_state) {
  $targetblockController = new TargetBlockController('targetblock');
  $targetController = new TargetController('target');

  $targetblock = $targetblockController->load_targetblock_by_panels_panelpid($form_state['pane']->pid);
  //This is a new targetblock
  if($targetblock == FALSE) {
    $targetblock=_dpidestinations_targetblock_content_type_helper_initialize_targetblock($form_state);
  }
  _dpidestinations_targetblock_content_type_helper_targetblock_override_global_settings($form_state, $targetblock);

  $target=$targetController->load_target_by_name($targetblock->target);

  $target_wrapper = entity_metadata_wrapper('target', $target);
  $available_templates_override = $target_wrapper->available_templateoverrides->value();

  $form['targetblok_instance_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('TargetBlock settings'),
  );

  $form['targetblok_instance_settings']['node_count_for_targetblockinstance'] = array(
    '#type' => 'textfield',
    '#default_value' => $targetblock->node_count_for_targetblockinstance,
    '#title' => t('Number of items'),
    '#size' => 4,
    '#description' => t('Number of element displayed on this block'),
  );


  if(count($available_templates_override)){
    $form['targetblok_instance_settings']['template_rule'] = array(
      '#type' => 'select',
      '#title' => t('Template override'),
      '#description' => t('Define the template for this targetblock instance'),
      '#options'=> array(''=>t('No override: Use graphical chart'))+$available_templates_override,
    );
  }

  $form['targetblok_instance_settings']['hide_pager'] = array(
    '#type' => 'checkbox',
    '#default_value' => $targetblock->hide_pager,
    '#title' => t('Hide the pager on this targetblock'),
    '#size' => 4,
    '#description' => t('Check this if you don\'t want the pager to display above this targetblock instance'),
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_pager"]' // element to evaluate condition on
        => array('checked' => TRUE),  // condition
      ),
    ),
  );

  $form['targetblok_settings'] = array(
    '#type' => 'fieldset',
    '#description'=>t('Thoses settings will be shared across all the same targetblock instances on the page'),
    '#collapsible' => TRUE,
    "#collapsed"=>TRUE,
    '#title' => t('Advanced settings'),
  );


  $form['targetblok_settings']['rank_max_value'] = array(
    '#type' => 'select',
    '#title' => t('Number of reserved position'),
    '#options'=> range(0,_dpidestinations_variable_get('dpidestination_max_reserved_position',10)),
    '#default_value'=>isset($targetblock->rank_max_value)?$targetblock->rank_max_value:_dpidestinations_variable_get('dpidestination_default_max_reserved_position',5),
  );


  $form['targetblok_settings']['use_pager_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );
  $form['targetblok_settings']['use_pager_settings']['use_pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => $targetblock->use_pager,

  );
  $form['targetblok_settings']['use_pager_settings']['pager_type'] = array(
    '#type' => 'select',
    '#title' => t('Pager type'),
    '#options'=>array('mini'=>t("Mini Pager"),'full'=>t('full')),
    '#default_value' => 'mini',
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_pager"]' // element to evaluate condition on
        => array('checked' => TRUE),  // condition
      ),
    ),
  );

  $form['targetblok_settings']['split_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );

  $form['targetblok_settings']['split_settings']['use_split'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override split Delta'),
    '#description' => t('Manually define the order across each targetblock instances.
        The default behavior is to use the date of creation of the targetblock instance to define the order.'),
    '#default_value' => $targetblock->use_split,
  );

  $form['targetblok_settings']['split_settings']['split_delta'] = array(
    '#type' => 'textfield',
    '#default_value' => $targetblock->split_delta,
    '#title' => t('Override targetblock instance order'),
    '#size' => 4,
    '#description' => t('Define the order across splitted redacblock
        -if you want 3 targetblock instances on the page and want this targetblock as
        the third splitted targetblock the value should be 3
        (and you have to define 1 and 2 for the two others ).
        This settings will be ignored if you have o,ly one targetblock instance on the page'
    ),
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_split"]' // element to evaluate condition on
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
  //DEFAULT SAVE OVERRIDABLE VARIABLES:
  $confs = array(
    'override_title',
    'node_count_for_targetblockinstance',
    'use_pager',
    'use_split',
    'node_count_for_targetblock',
    'offset',
    'split_delta',
    'hide_pager',
    'pager_id',
    'pager_type',
    'rank_max_value',
    'template_rule'
  );
  foreach($confs as $conf) {
    $form_state['conf'][$conf] = $form_state['values'][$conf];
  }
  $form_state['conf'][$key] = $form_state['values'][$key];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }

  //We log the change to allow override when edit of another pane before page is saved
  // @see: dpidestinations_targetblock_content_type_edit_form()
  // @see: _dpidestinations_targetblock_content_type_helper_targetblock_override_global_settings()
  $form_state['display cache']->target_block_conf[$form_state['subtype_name']]['targetblock_global_settings_override'] = TRUE;
  $form_state['display cache']->target_block_conf[$form_state['subtype_name']]['targetblock_global_settings_values'] = $form_state['pane']->configuration;

  $properties_to_override = _dpidestinations_targetblock_content_type_helper_targetblock_properties_to_override();
  foreach($form_state['display']->content as $pane) {
    if($pane->type == "targetblock" &&  $pane->subtype == $form_state['subtype_name']) {
      foreach($properties_to_override as $property) {
        $property_value = $form_state['display cache']->target_block_conf[$form_state['subtype_name']]['targetblock_global_settings_values'] = $form_state['pane']->configuration[$property];
        $form_state['display']->content[$pane->pid]->configuration[$property] = $property_value;
      }
    }
  }
}


function _dpidestinations_targetblock_content_type_helper_targetblock_override_global_settings($form_state,$targetblock) {
  $override = FALSE;

  $properties_to_override = _dpidestinations_targetblock_content_type_helper_targetblock_properties_to_override();
  $max_split_delta = 0;

  if($form_state["pane"]->pid == "new") {
    //Try to find an existing similar targetblock instance
    foreach($form_state['display']->content as $pane) {
      // if we found one apply his settings
      if($pane->type == "targetblock" &&  $pane->subtype == $form_state['subtype_name']) {
        foreach($properties_to_override as $property) {
          $values[$property] = $pane->configuration[$property];
          $targetblock->{$property} = $pane->configuration[$property];

          //there is a special treatment for split_data and we need to do the entire loop :(
          if($max_split_delta<=$pane->configuration['split_delta']) {
            $max_split_delta = $pane->configuration['split_delta']+1;
            $values['split_delta'] = $max_split_delta;
            $targetblock->split_delta = $max_split_delta;
          }
        }
      }
      $form_state["pane"]->configuration = $values;
    }
  } elseif( $form_state['display cache']->target_block_conf[$form_state['subtype_name']]['targetblock_global_settings_override']) {
    $override=TRUE;

    foreach($properties_to_override as $property) {
      $values[$property] = $form_state['display cache']->target_block_conf[$form_state['subtype_name']]['targetblock_global_settings_values'][$property];
      $targetblock->{$property} = $form_state['display cache']->target_block_conf[$form_state['subtype_name']]['targetblock_global_settings_values'][$property];
    }
  }

  if($override) {
    null;
  }
}

function _dpidestinations_targetblock_content_type_helper_targetblock_properties_to_override() {
  return array("use_pager", "node_count_for_targetblock", "use_pager", "pager_type", "rank_max_value", "use_split");
}

/**
 * Initialize a new targetblock if we are creating a new ctools content type
 *
 * @param unknown $form_state
 *
 * @return TargetBlockEntity
 */
function _dpidestinations_targetblock_content_type_helper_initialize_targetblock($form_state) {
  $targetblock = new TargetBlockEntity(array(), 'TargetBlock');
  $targetblock->target = $form_state["subtype_name"];

  //If the targetblock is not defined check if we have some info
  // on the conf (when adding a new block and edit the targetblock without
  //previously save the display)
  if(!isset($targetblock->targetblockid)) {
    $targetblock = (object)array_merge((array)$targetblock, $form_state['conf']);
  }
  return $targetblock;
}

function _dpidestinations_targetblock_content_type_helper_get_title($conf) {
  if($conf['override_title']) {
    $title = $conf['override_title_text'];
  } else {
    $title = t('Redacblock');
  }
  return $title;
}

function _dpidestinations_targetblock_content_type_helper_get_content($targetview) {
  $content = "";
  $content = $targetview->render();
  return $content;
}

function _dpidestinations_targetblock_content_type_helper_get_admin_links($did, $destination_term_id, $target_name, $split_delta='',$title) {
  return array(
    array(
      'title' => t('Reorder TargetBlock'),
      'href' => "admin/targetview_reorder/".$destination_term_id."/".$target_name,
      'query' => drupal_get_destination(),
    ),
    array(
      'title' => t('Refresh Cache'),
      'href' => dpicache_get_regenerate_url('dpidestinations_targetblock_render', $did, $destination_term_id, $target_name, $split_delta,$title),
      'query' => drupal_get_destination(),
    ),
    array(
      'title' => t('Configure TargetBlock'),
      'href' => "admin/structure/block/manage/configure",
      'query' => drupal_get_destination(),
    ),
  );
}
