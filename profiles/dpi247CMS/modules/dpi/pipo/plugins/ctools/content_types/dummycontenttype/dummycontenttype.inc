<?php


/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Dummy Content types'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  'single' => TRUE,
  
  'admin info' => 'dummycontenttypes_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'dummycontenttypes_content_type_render',
  
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  'edit form' => 'dummycontenttypes_edit_form',
  
  
  'category' => t('Pipo'),
  'group' => t('Example'),
  
  
  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
  
);



 function dummycontenttypes_admin_info($subtype, $conf, $contexts) {



$block = new stdClass;
$block->title = 'Dummy ctools contenttype';
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
 * Implementation of hook_typeofcontent_content_type_content_types
 * Return all custom content types available.
 */
/*
function dpidestinations_targetblock_content_type_content_types($plugin) {
  $types = array();
  $targets = target_load_multiple(FALSE);
  foreach($targets as $target) {
    $types[$target->machine_name] = array(
      'title' => check_plain($target->machine_name),
      'subtype' => $target->machine_name,
      'description' => check_plain($target->description),
      //'required context' => new ctools_context_required(t('Destination'), 'destination'),
      'category' => t('Target'),
      'group' => t('DPI Core'),
      //'all contexts' => TRUE,
    );
  }

  return $types;
}
*/

/**
 * Edit form.
 */

function dummycontenttypes_edit_form($form, &$form_state) {
 
 $conf=$form_state['conf'];
 $args=$form_state['args'];
 $contexts=$form_state['contexts'];
dsm($conf);

$form["options"]=array("#tree"=>TRUE);
 $form['options']['option1']=array(
   '#type'=>'checkbox',
   '#title'=>'Option 1',
   '#Description'=>t('Bla bla'),
   "#default_value"=>$conf['options']['option1'],
   
   
   );

 $form['options']['option2']=array(
   '#type'=>'checkbox',
   '#title'=>'Option 2',
   '#Description'=>t('Bla bla'),
   "#default_value"=>$conf['options']['option2'],
    
    
 );
 
 $form['options']['option3']=array(
   '#type'=>'checkbox',
   '#title'=>'Option 3',
   '#Description'=>t('Bla bla'),
   "#default_value"=>$conf['options']['option3'],
    
    
 );
   
  return $form;
}


/**
 * Edit form submit callback.
 */


function dummycontenttypes_edit_form_submit($form, &$form_state) {
  
  dsm($form_state);
  $value_to_save=array('options1','options2','options3',);
  
  foreach($form_state["values"]['options'] as $key=>$item){
    $form_state['conf']['options'][$key]=$form_state['values']['options'][$key];
    
  }
  
 
  dsm($form_state['conf']);
  /*

  $//DEFAULT SAVE OVERRIDABLE VARIABLES:
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
  
  */
}





/**
 * Render callback.
*/
function dummycontenttypes_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();




  $block->title = check_plain("Dummy Ctools Content type");
  $block->content = _dummycontenttypes_get_form($conf);

  $admin_links=array(
    array(
      'title' => t('Lien 1'),
      'href' => 'node/1',
    ),
    array(
      'title' => t('Lien 2'),
      'href' => 'admin/dpi247',
    ),
  );

  $block->admin_links=$admin_links;

  return $block;
}



function _dummycontenttypes_get_form($conf){
  
  $form_state['conf']=$conf;
  return drupal_get_form("_dummycontenttypes_form_render_form",$form_state);
  
  
}