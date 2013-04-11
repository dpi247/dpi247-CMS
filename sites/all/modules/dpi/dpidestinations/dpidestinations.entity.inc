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
  );



  $entities['target_block']=array(
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
  );
  return $entities;

}


function target_load($tid = NULL, $reset = FALSE){
  $tids = (isset ($tid) ? array($tid) : array());
  $target = target_load_multiple($tids, $reset);
  return $target ? reset ($target) : FALSE;
}

function target_load_multiple($tids = array(), $conditions = array(), $reset = FALSE){
  return entity_load('target', $tids, $conditions, $reset);
}





function target_access_admin_callback(){
  return TRUE;
}
function targetblock_access_admin_callback(){
  return TRUE;
}

function target_page_title($target){
  return $target->machine_name;
}

function target_view($target, $view_mode = 'full'){
  return $target->view();
}





function target_form($form, &$form_state, $target) {

  $form['targetid'] = array(
    '#type' => 'value',
    '#value'=> $target->targetid,
  );
  $form['machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine Name'),
    '#default_value'=> $target->machine_name,
    '#required' => TRUE,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value'=> $target->label,
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value'=> $target->description,
    '#required' => TRUE,
  );
  $form['view_name'] = array(
    '#type' => 'textfield',
    '#title' => t('View Name'),
    '#default_value'=> $target->view_name,
    '#required' => TRUE,
  );
  $form['view_id'] = array(
    '#type' => 'textfield',
    '#title' => t('View Id'),
    '#default_value'=> $target->view_id,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  field_attach_form('target', $target, $form, $form_state);

  return $form;
}



function target_form_submit($form, &$form_state) {
  $target_submission = (object) $form_state['values'];
  field_attach_submit('target', $target_submission, $form, $form_state);
  target_save($target_submission);
  $form_state['redirect'] = "target/".$target_submission->targetid;
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
