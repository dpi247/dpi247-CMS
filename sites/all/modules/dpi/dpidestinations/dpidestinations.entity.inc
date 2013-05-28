<?php

/**
 * Implements hook_entity_info().
 */
function dpidestinations_entity_info(){
  $entities=array();

  $entities['target']=array(
    'label' => t('Target'),
    // seulement si on utilise le module entity API
    'exportable'=>TRUE,
    'entity class' => 'TargetEntity',
    'controller class' => 'TargetController',
    'base table' => 'dpidestinations_target',
    'fieldable' => TRUE,
    // Make use the class' label() and uri() implementation by default.
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'entity keys' => array(
      'id' => 'targetid',
      'name' => 'machine_name',
      'label' => 'label',
    ),

    'bundles'=>array(
      'target'=>array(
      ),
    ),
    'view modes' => array(
      'minimal' => array(
        'label' => t('Minimal'),
        'custom settings' => FALSE,
      ),
      'full' => array(
        'label' => t('Full'),
        'custom settings' => TRUE,
      ),
    ),
    //Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
    'module'=> 'dpidestinations',
    'access callback' => 'target_access_admin_callback',

    // Enable the entity API's admin UI.
    'admin ui' => array(
      'label' => 'ti',
      'path' => 'admin/structure/target',
      // OPTIONAL- See "Customize the User Interface" below if you set.
      //'file' => 'target.admin.inc',
      //'controller class' => 'EntityDefaultUIController',
    ),
    'metadata controller class' => 'DpiDestinationsTargetMetadataController',
    
  );



  $entities['targetblock']=array(
    'label' => t('Target Block'),
    // seulement si on utilise le module entity API
    'exportable'=>TRUE,
    'entity class' => 'TargetBlockEntity',
    'controller class' => 'TargetBlockController',
    'base table' => 'dpidestinations_targetblock',
    'fieldable' => TRUE,
    // Make use the class' label() and uri() implementation by default.
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',

    'entity keys' => array(
      'id' => 'targetblockid',
      // 'name' => 'machine_name',
      'label' => 'label',
    ),

    'bundles'=>array(
      'targetblock'=>array(
      ),
    ),
    'view modes' => array(
      'minimal' => array(
        'label' => t('Minimal'),
        'custom settings' => FALSE,
      ),
      'full' => array(
        'label' => t('Full'),
        'custom settings' => TRUE,
      ),
    ),
    //Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
    'module'=> 'dpidestinations',
    'access callback' => 'targetblock_access_admin_callback',

    // Enable the entity API's admin UI.
    'admin ui' => array(
      'label' => 'ti',

      'path' => 'admin/structure/targetblock',
      // OPTIONAL- See "Customize the User Interface" below if you set.
      //'file' => 'target.admin.inc',
      //'controller class' => 'EntityDefaultUIController',
    ),
    'metadata controller class' => 'DpiDestinationsTargetblockMetadataController',
    
  );
  return $entities;

}

function dpidestinations_entity_property_info(){

  $properties['target']['properties']=array(
    'targetid'=> array(
      'label'=> 'Target id',
      'type'=> 'integer',
      'schema field' => 'targetid',
      'validation callback' => 'entity_metadata_validate_integer_positive '
    ),
    'machine_name'=>array(
      'label'=> 'Machine name',
      'Description'=> 'A Target is a DPI object to define aailable slot for redactional content',
      'type'=> 'token',
      'schema field' => 'machine_name',
    ),
    'label' => array(
      'label' => 'label',
      'description' => 'The label of the target',
      'type' => 'text',
      'schema field' => 'label',
    ),
    'description' => array(
      'label' => 'Description',
      'type' => 'text',
      'description' => 'Admifffffnistrative description for this target.',
      'schema field' => 'description',
    ),
    'view_name' => array(
      'label' => 'View Name',
      'type'=>'text',
      'schema field' => 'view_name',
    ),
    'view_display' =>array(
      'label' => 'View Display',
      'type'=>'text',
      'schema field' => 'view_display',
      
    ),
    'available_targetlayouts' => array(
      'label' => 'Available Target Layouts',
      'description' => 'Wich target layout are available for this target.',
      'type' => 'text',
      'schema field' => 'available_targetlayouts',
      //'computed' => TRUE,
    ),
    'available_templateoverrides' => array(
      'label' => 'Available tempalte override',
      'description' => 'Allow Webmaster to override template logic by choosing a fixed template',
      'type' => 'text',
      'schema field' => 'available_templateoverrides',
      //'computed' => TRUE,
    ),

    'status' => array(
      'label'=> 'Status',
      'type'=> 'integer',
      'schema field' => 'status',
    ),
    'module' => array(
      'label'=> 'Module',
      'type'=> 'text',
      'schema field' => 'module',
    ),
    'url' => array(
      'label' => 'Url',
      'description' => 'The url of the entity',
      'type' => 'url',
      'computed' => TRUE,
      'entity views field' => TRUE,
    ),
  );


  return $properties;


}
function target_load($tid = NULL, $reset = FALSE){
  $tids = (isset ($tid) ? array($tid) : array());
  $target = target_load_multiple($tids, $reset);
  return $target ? reset ($target) : FALSE;
}

function target_load_multiple($tids = array(), $conditions = array(), $reset = FALSE){
  return entity_load('target', $tids, $conditions, $reset);
}


/**
 * Save target.
 */
function target_save($target) {
  entity_save('target', $target);
}

/**
 * Delete single target.
 */
function target_delete($target) {
  entity_delete('target', entity_id('target' ,$target));
}

/**
 * Delete multiple targets.
 */
function target_delete_multiple($target_ids) {
  entity_delete_multiple('target', $target_ids);
}



function target_access_admin_callback(){
  return TRUE;
}

function target_page_title($target){
  return $target->machine_name;
}

function target_view($target, $view_mode = 'full'){
  return $target->view();
}





function target_form($form, &$form_state, $target) {
  $view_name_options=dpidestinations_helpers_dependant_target_viewname_and_display_options_for_view_name();
  
  $selected_view_name = isset($form_state['values']['view_name']) ? $form_state['values']['view_name'] : $target->view_name;
  if($selected_view_name=="" or $selected_view_name==NULL){
    $selected_view_name=key($view_name_options);
  }
  $selected_view_display = isset($form_state['values']['view_display']) ? $form_state['values']['view_display'] : $target->view_display;
  
  $form['targetid'] = array(
    '#type' => 'value',
    '#value'=> $target->targetid,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value'=> $target->label,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value'=> $target->machine_name,
    '#machine_name'=> array(
      'source'=>array('label'),
      'label' => t('Target Machine name'),
      'exists' => '_dpidestinations_targetmachine_name_exists',


    ),
    '#disabled' => !empty($target->machine_name),

  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value'=> $target->description,
    '#required' => TRUE,
  );
  $form['view_name'] = array(
    '#type' => 'select',
    '#title' => t('View Name'),
    '#options' => $view_name_options,
    '#default_value'=> $selected_view_name,
    '#required' => TRUE,
    '#ajax' => array(
      // When 'event' occurs, Drupal will perform an ajax request in the
      // background. Usually the default value is sufficient (eg. change for
      // select elements), but valid values include any jQuery event,
          // most notably 'mousedown', 'blur', and 'submit'.
          // 'event' => 'change',
          'callback' => '_dpidestinations_ajaxcallback_viewname_viewdisplay',
          'wrapper' => 'dropdown-view-display',
      ),
  );
  $form['view_display'] = array(
    '#type' => 'select',
    '#title' => t('View Display'),
    '#options' => dpidestinations_helpers_dependant_target_viewname_and_display_options_for_view_display($selected_view_name),
    '#default_value'=> $selected_view_display,
    '#required' => TRUE,
    '#prefix' => '<div id="dropdown-view-display">',
    '#suffix' => '</div>',
    
  );
  $form['available_targetlayouts'] = array(
    '#type' => 'checkboxes',
    '#options' => dpi_variable_get('dpidestinations_available_targetlayouts'),
    '#default_value'=> unserialize($target->available_targetlayouts),
    '#title' => t('Available target\'s layout'),
  );
  $form['available_templateoverrides'] = array(
    '#type' => 'textarea',
    '#title' => t('Template override'),
    '#default_value'=> $target->available_templateoverrides,
    '#required' => False,
    '#description'=> t('One template override per line in the following format: machine_name|label')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
 
  field_attach_form('target', $target, $form, $form_state);

  return $form;
}


function target_form_submit($form, &$form_state) {
  $target = entity_ui_form_submit_build_entity($form, $form_state);
  $target->available_targetlayouts=serialize($target->available_targetlayouts);
  $target->save();
  $form_state['redirect'] = "target/".$target_submission->tid;
}











function targetblock_load($tid = NULL, $reset = FALSE){
  $tids = (isset ($tid) ? array($tid) : array());
  $targetblock = targetblock_load_multiple($tids, $reset);
  return $targetblock ? reset ($targetblock) : FALSE;
}

//tids set to FALSE to load all entities
function targetblock_load_multiple($tids = array(), $conditions = array(), $reset = FALSE){
  return entity_load('targetblock', $tids, $conditions, $reset);
}


/**
 * Save target.
 */
function targetblock_save($targetblock) {
  entity_save('targetblock', $targetblock);
}

/**
 * Delete single target.
 */
function targetblock_delete($targetblock) {
  entity_delete('targetblock', entity_id('targetblock' ,$targetblock));
}

/**
 * Delete multiple targets.
 */
function targetblock_delete_multiple($targetblock_ids) {
  entity_delete_multiple('targetblock', $targetblock_ids);
}


function targetblock_access_admin_callback(){
  return TRUE;
}

function targetblock_page_title($targetblock){
  return $targetblock->id;
}

function targetblock_view($targetblock, $view_mode = 'full'){
  return $targetblock->view();
}



function targetblock_form($form, &$form_state, $targetblock) {

  $form['pager_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Custom pager settings'),
  );

  $form['pager_settings']['override_pager_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overriding view settings'),
    '#default_value' => $targetblock->override_pager_settings,
  );

  $form['pager_settings']['node_count_for_targetblockinstance'] = array(
    '#type' => 'textfield',
    '#default_value' => $targetblock->node_count_for_targetblockinstance,
    '#title' => t('Total nodes per pages'),
    '#size' => 4,
    '#description' => t('The total number of nodes on the page.'),
    '#dependency' => array('override-pager-checkbox' => array(1)),
  );
  $form['pager_settings']['use_pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => $targetblock->use_pager,
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="override_pager_settings"]' // element to evaluate condition on
        => array('checked' => TRUE),  // condition
      ),
    ),
  );

  $form['pager_settings']['split_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use splited blocks'),
    '#default_value' => $targetblock->split_flag,
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="override_pager_settings"]' // element to evaluate condition on
        => array('checked' => TRUE),  // condition
      ),
    ),

  );
  $form['pager_settings']['node_count_per_target'] = array(
    '#type' => 'textfield',
    '#default_value' => $targetblock->node_count_per_target,
    '#size' => 4,
    '#title' => t('Number of posts displayed on the page for this target'),
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="use_splitted"]' // element to evaluate condition on
        => array('checked' => TRUE),  // condition
      ),
    ),
  );

  $form['pager_settings']['offset'] = array(
    '#type' => 'textfield',
    '#default_value' => $targetblock->offset,
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
    '#default_value' => $targetblock->split_index_count,
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
    '#default_value' => $targetblock->split_total_count,
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
    '#default_value' => $targetblock->hide_pager,
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
    '#default_value' => $targetblock->pager_id,
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

  $form['template'] = array(
    '#type' => 'select',
    '#default_value' => $targetblock->template,
    '#title' => t('Template'),
    '#options'=>array('default'=>'Use templates rules')+$target->available_templateoverrides,


  );
  return $form;

}
