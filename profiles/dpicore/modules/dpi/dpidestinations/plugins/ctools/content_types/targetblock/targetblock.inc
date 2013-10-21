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
function dpidestinations_targetblock_content_type_content_types($plugin) {
  $types = array();
  $targets=target_load_multiple(FALSE);
  foreach($targets as $target){
    $types[$target->machine_name]= array(
      'title' => check_plain($target->machine_name),
      'subtype'=> $target->machine_name,
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
  
  $did=$context['argument_destination_1']->data->handler->did;

  $targetBlockController=new TargetBlockController('targetblock');
  $targetblock=$targetBlockController->load_targetblock_by_did_target_and_splitdelta($did,$subtype,0);
  
  //@todo: check si on a pas de targetblock
  //si on a pas  si le user est admin afficher un block d'erreur
  // Si user est anonyme=> block vide 
  // et watchdog
  
  
  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
  $target=$targetblock_wrapper->target->value();
  $target_wrapper = entity_metadata_wrapper('target', $target);
  $targetview=$target_wrapper->targetview->value();
  
  _dpidestinations_targetblock_content_type_helper_initialize_targetview($subtype,$conf,$args,$context,$target_wrapper,$targetblock,$targetview);
  dsm($targetview);
  $block->title = _dpidestinations_targetblock_content_type_helper_get_title($subtype,$conf,$args,$context,$target_wrapper,$targetblock_wrapper,$targetview);
  $block->content = _dpidestinations_targetblock_content_type_helper_get_content($subtype,$conf,$args,$context,$target_wrapper,$targetblock_wrapper,$targetview);
  $block->admin_links = _dpidestinations_targetblock_content_type_helper_get_admin_links($subtype,$conf,$args,$context,$target_wrapper,$targetblock_wrapper,$targetview);
  
  
  
  return $block;
  
}


/**
 * Edit form.
 */
function dpidestinations_targetblock_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $targetController=new TargetBlockController('targetblock');
  
  $target_block= $targetController->load_targetblock_by_panels_panelpid($form_state['pane']->pid);
  
  
  //If the targetblock is not defined check if we have some info
  // on the conf (when adding a new block and edit the targetblock without 
  //previously save the display)
  if(!isset($target_block->targetblockid)){
    $target_block=(object)  $conf;
  }
  
  
  $form['pager_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Custom pager settings'),
  );

  

  $form['pager_settings']['node_count_for_targetblockinstance'] = array(
    '#type' => 'textfield',
    '#default_value' => $target_block->node_count_for_targetblockinstance,
    '#title' => t('Node count for this targetblock instance'),
    '#size' => 4,
    '#description' => t('The node count for this targetblock instance.'),
  );
  
  $form['pager_settings']['use_pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => $target_block->use_pager,
    
  );

  $form['pager_settings']['use_split'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use splited blocks'),
    '#default_value' => $target_block->use_split,
  
      
  );
  $form['pager_settings']['offset'] = array(
    '#type' => 'textfield',
    '#default_value' =>$target_block->offset,
    '#title' => t('Offset'),
    '#size' => 4,
    '#description' => t('The number of items to skip and not display.'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_split"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );

  
  $form['pager_settings']['split_delta'] = array(
    '#type' => 'textfield',
    '#default_value' => $target_block->split_delta,
    '#title' => t('Split index'),
    '#size' => 4,
    '#description' => t('The index of this redacblock -if this is the third splitted redabclokc it will be 3- (Only if the redac block is splitted across the page).'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_split"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );


  $form['pager_settings']['hide_pager'] = array(
    '#type' => 'checkbox',
    '#default_value' => $target_block->hide_pager,
    '#title' => t('Hide pager'),
    '#size' => 4,
    '#description' => t('Check this if you don\'t want the pager to display above this redacblock'),
  	 '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_split"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
  );

  $form['pager_settings']['pager_id'] = array(
    '#type' => 'textfield',
    '#default_value' => $target_block->pager_id,
    '#title' => t('Pager ID'),
    '#size' => 4,
    '#description' => t('This ID is use to synchronize the splitted blocks with the pager. You need to set the same one on each splitted block.'),
     '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_split"]' // element to evaluate condition on
          => array('checked' => TRUE),  // condition
        ),
      ),
    
  );
  
  $form['rank_behavior'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Rank Settings'),
    '#description' => t('Rank behavior: Beware that thoses settings could be overwrited by a specific destination term settings'),
  );
  $form['rank_behavior']['rank_max_value'] = array(
    '#type' => 'select',
    '#title' => t('Max reserved possition'),
    '#options'=> array(1,2,3,4,5,6),
  );

  $form['rank_behavior']['rank_behavior'] = array(
    '#type' => 'select',
    '#title' => t('Rank behavior'),
    '#options'=> array('top'=>t('Top'),'bottom'=>t('Bottom')),
  );
  
  $form['template'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Templates'),
    '#description' => t('Beware that thoses settings could be overwrited by a specific destination term settings'),
  );
  $form['template']['template_rule'] = array(
    '#type' => 'select',
    '#title' => t('Template rule override'),
    '#options'=> array(''=>t('No override: Use graphical chart'),'red'=>t('Red')),
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
    'node_count_for_targetblockinstance',
    'use_pager',
    'use_split',
    'node_count_for_targetblock',
    'offset',
    'split_delta',
    'hide_pager',
    'pager_id',
    'rank_max_value',
    'rank_behavior',
    'template_rule'
  );
  foreach($confs as $conf){
    $form_state['conf'][$conf] = $form_state['values'][$conf];
    
  }
  $form_state['conf'][$key] = $form_state['values'][$key];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
 
}





function   _dpidestinations_targetblock_content_type_helper_initialize_targetview($subtype, $conf,$args,$context,$target_wrapper,$targetblock,& $targetview){
  
  $term_value=$context['argument_destination_1']->data->tid;
  $target_value=$target_wrapper->machine_name->value();
  _dpidestinations_targetview_initialize_view( $targetview, $term_value,$target_value,$targetblock);
  
}




function _dpidestinations_targetblock_content_type_helper_get_title($subtype, $conf,$args,$context,$target_wrapper,$targetblock_wrapper,$targetview){
  if($conf['override_title']){
    $title = $conf['override_title_text'];
  }
  else{
    $title = t('Redacblock');
  }
  return $title;
}
function _dpidestinations_targetblock_content_type_helper_get_content($subtype, $conf,$args,$context,$target_wrapper,$targetblock_wrapper,$targetview){
  $content="";
  
  //see: https://drupal.org/node/1556174#comment-5943546 as we cannot use views_embed_view we generate dom_id :s 
  $targetview->dom_id=uniqid();
  $targetview->execute();
  dsm($targetview);
  return $targetview->render();
}


function _dpidestinations_targetblock_content_type_helper_get_admin_links($subtype, $conf,$args,$context,$target_wrapper,$targetblock_wrapper,$targetview){

  return array(
    array(
      'title'=>t('Reorder TargetBlock'),
      'href'=> "admin/targetview_reorder/".$args[0]."/".$subtype,
      'query' => drupal_get_destination(),
      
      ),
    array(
      'title' => t('Refresh Cache'),
      'href' => "admin/structure/block/manage/configure",
      'query' => drupal_get_destination(),
    ),
    array(
      'title' => t('Configure TargetBlock'),
      'href' => "admin/structure/block/manage/configure",
      'query' => drupal_get_destination(),
    ),
    
  );
}



