<?php

/**
 * Implements hook_entity_info().
 */
function dpicontenttypes_entity_info() {
  $entities = array();

  $entities['dpiproperties'] = array(
    'label' => t('DPI properties'),
    // Only if we use the entity API module
    'exportable' => FALSE,
    'entity class' => 'DPIPropertiesEntity',
    'controller class' => 'DPIPropertiesController',
    'base table' => 'dpicontenttypes_properties',
    'fieldable' => TRUE,  
    // Make use the class' label() and uri() implementation by default.
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'entity keys' => array(
      'id' => 'dpid',
      'bundle' => 'type',
    ),
    'bundle keys' => array(
      'bundle' => 'type',
    ),
    'bundles' => array(),
    'view modes' => array(
      'full' => array(
        'label' => t('Full'),
        'custom settings' => FALSE,
      ),
    ),
    // Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
    'module' => 'dpicontenttypes',
    'access callback' => 'dpiproperties_access_admin_callback',
  );

  $entities['dpiproperties_type'] = array(
    'label' => t('DPI properties Type'),
    'entity class' => 'DPIPropertiesTypeEntity',
    'controller class' => 'DPIPropertiesTypeController',
    'base table' => 'dpicontenttypes_properties_type',
    'fieldable' => TRUE,
    'bundle of' => 'dpiproperties',
    'exportable' => TRUE,
    'uri callback' => 'entity_class_uri',    
    'entity keys' => array(
      'id' => 'id',
      'name' => 'type',
      'label' => 'label', 
    ),
    'module' => 'dpicontenttypes',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/dpiproperties-types',
      'file' => 'dpicontenttypes.controller.class.inc',
      'controller class' => 'DPIPropertiesTypeUIController',
    ),
    'access callback' => 'dpiproperties_type_access_admin_callback',
  );

  return $entities;
}

//@todo: documenter cette partie on fait l'alter uniquement pour fournir un build mode suplémentaire...
/**
 * Implements hook_entity_info_alter().
 *
 * We need to modify the default node entity info by adding a new view mode to
 * be used in functions like node_view() or node_build_content().
 */
function dpicontenttypes_entity_info_alter(&$entity_info) {
  // Add our new view mode to the list of view modes...
  $entity_info['node']['view modes']['package'] = array(
    'label' => t('Target default'),
    'custom settings' => FALSE,
  );

  $dpiproperties_types = dpiproperties_types();
  
  $package_type_info = new stdClass();
  $package_type_info->label = 'Package';
  $dpiproperties_types['package'] = $package_type_info;
  
  $scald_atom_type_info = new stdClass();
  $scald_atom_type_info->label = 'Scald Atom';
  $dpiproperties_types['scald_atom'] = $scald_atom_type_info;
  
  $package_gallery_type_info = new stdClass();
  $package_gallery_type_info->label = 'Package_gallery';
  $dpiproperties_types['package_gallery'] = $package_gallery_type_info;
  
  foreach ($dpiproperties_types as $type => $info) {
    $entity_info['dpiproperties']['bundles'][$type] = array(
      'label' => $info->label,
      'admin' => array(
        'path' => 'admin/structure/dpiproperties-types/manage/%dpiproperties_type',
        'real path' => 'admin/structure/dpiproperties-types/manage/' . $type,
        'bundle argument' => 4,
      ),
    );
  }
  
  $entity_info['scald_atom']['save callback'] = 'scald_atom_save';  
}


function dpicontenttypes_entity_prepare_view($entity_type, $entities, $langcode = NULL){
}

/**
 * Implementation of hook_entity_property_info_alter
 * Add Dpiproperties setter/getter to set/get dpiproperties on an object
 *
 * @param Array $info
 */
function dpicontenttypes_entity_property_info_alter(&$info) {
  $product_id = array(
    'label' => t("ProductID"),
    'type' => 'text',
    'setter callback' => 'entity_property_dpiproperties_set_properties',
    'getter callback' => 'entity_property_dpiproperties_get_properties',
  );

  $external_reference = array(
    'label' => t("External Reference"),
    'type' => 'text',
    'setter callback' => 'entity_property_dpiproperties_set_properties',
    'getter callback' => 'entity_property_dpiproperties_get_properties',
  );

  $external_reference_2 = array(
    'label' => t("External Reference 2"),
    'type' => 'text',
    'setter callback' => 'entity_property_dpiproperties_set_properties',
    'getter callback' => 'entity_property_dpiproperties_get_properties',
  );

  $source = array(
    'label' => t("Source"),
    'type' => 'text',
    'setter callback' => 'entity_property_dpiproperties_set_properties',
    'getter callback' => 'entity_property_dpiproperties_get_properties',
  );
  
  $cciinlineobject_orig_attr = array(
    'label' => t("CCI Inline Objects original attributes"),
    'type' => 'text',
    'setter callback' => 'entity_property_dpiproperties_set_properties',
    'getter callback' => 'entity_property_dpiproperties_get_properties',
  );

  //Set to the package
  $info['node']['bundles']['package']['properties']['field_product_id'] = $product_id;
  $info['node']['bundles']['package']['properties']['field_external_reference'] = $external_reference;
  $info['node']['bundles']['package']['properties']['field_external_reference_2'] = $external_reference_2;
  $info['node']['bundles']['package']['properties']['field_source'] = $source;

  //Set to the package_gallery
  $info['node']['bundles']['package_gallery']['properties']['field_product_id'] = $product_id;
  $info['node']['bundles']['package_gallery']['properties']['field_external_reference'] = $external_reference;
  $info['node']['bundles']['package_gallery']['properties']['field_external_reference_2'] = $external_reference_2;
  $info['node']['bundles']['package_gallery']['properties']['field_source'] = $source;
  
  //Set to the atoms image
  if (isset($info['scald_atom']['bundles']['image'])){
    $info['scald_atom']['bundles']['image']['properties']['field_product_id'] = $product_id;
    $info['scald_atom']['bundles']['image']['properties']['field_external_reference'] = $external_reference;
    $info['scald_atom']['bundles']['image']['properties']['field_external_reference_2'] = $external_reference_2;
    $info['scald_atom']['bundles']['image']['properties']['field_source'] = $source;
    $info['scald_atom']['bundles']['image']['properties']['field_cciinline_orig_attr'] = $cciinlineobject_orig_attr;
  }

  //Set to the atoms video
  if (isset($info['scald_atom']['bundles']['video'])){
    $info['scald_atom']['bundles']['video']['properties']['field_product_id'] = $product_id;
    $info['scald_atom']['bundles']['video']['properties']['field_external_reference'] = $external_reference;
    $info['scald_atom']['bundles']['video']['properties']['field_external_reference_2'] = $external_reference_2;
    $info['scald_atom']['bundles']['video']['properties']['field_source'] = $source;
  }

  //Set to the atoms audio
  if (isset($info['scald_atom']['bundles']['audio'])){
    $info['scald_atom']['bundles']['audio']['properties']['field_product_id'] = $product_id;
    $info['scald_atom']['bundles']['audio']['properties']['field_external_reference'] = $external_reference;
    $info['scald_atom']['bundles']['audio']['properties']['field_external_reference_2'] = $external_reference_2;
    $info['scald_atom']['bundles']['audio']['properties']['field_source'] = $source;
  }

  //Set to the atoms flash
  if (isset($info['scald_atom']['bundles']['flash'])){
    $info['scald_atom']['bundles']['flash']['properties']['field_product_id'] = $product_id;
    $info['scald_atom']['bundles']['flash']['properties']['field_external_reference'] = $external_reference;
    $info['scald_atom']['bundles']['flash']['properties']['field_external_reference_2'] = $external_reference_2;
    $info['scald_atom']['bundles']['flash']['properties']['field_source'] = $source;
  }

  //Set to the atoms linklist
  if (isset($info['scald_atom']['bundles']['links_list'])){
    $info['scald_atom']['bundles']['links_list']['properties']['field_product_id'] = $product_id;
    $info['scald_atom']['bundles']['links_list']['properties']['field_external_reference'] = $external_reference;
    $info['scald_atom']['bundles']['links_list']['properties']['field_external_reference_2'] = $external_reference_2;
    $info['scald_atom']['bundles']['links_list']['properties']['field_source'] = $source;
  }
}

/**
 * Implementation of hook_entity_presave
 *
 * @param Object $entity
 * @param String $type
 */
function dpicontenttypes_entity_presave($entity, $type) {
  //@todo Pour tous les objets de type field, vérifier si l'ancien fichier = le nouveau, si non, mettre un timeout sur l'ancien pour qu'il soit supprimer dans x temps (voir nodeapi de wallymport en drupal 6)
}

/**
 * Implementation of hook_entity_insert
 *
 * Flag cropings as permanent and delete unused cropings
 */
function dpicontenttypes_entity_insert($entity, $entity_type) {
  module_load_include('inc', 'dpicontenttypes', 'includes/dpicontenttypes.cropings');
  _dpicontenttypes_cropings_entity_form_confirm_new_cropings($entity, $entity_type);
}

/**
 * Implementation of hook_entity_update
 *
 * Flag cropings as permanent and delete unused cropings
 */
function dpicontenttypes_entity_update($entity, $entity_type) {
  module_load_include('inc', 'dpicontenttypes', 'includes/dpicontenttypes.cropings');
  _dpicontenttypes_cropings_entity_form_confirm_new_cropings($entity, $entity_type);
}

/**
 * Implementation of hook_entity_delete
 *
 * Delete cropings related to this entity
 */
function dpicontenttypes_entity_delete($entity, $entity_type) {
  module_load_include('inc', 'dpicontenttypes', 'includes/dpicontenttypes.cropings');
  list($eid, $revision, $bundle) = entity_extract_ids($entity_type, $entity);
  _dpicontenttypes_cropings_delete_cropings(NULL, $entity_type, $eid);
}

/**
 * Load a DPI properties
 *
 * @param integer $tid
 * @param boolean $reset
 *
 * @return boolean
 */
function dpiproperties_load($dpid = NULL, $reset = FALSE) {
  $dpiproperties = dpiproperties_load_multiple(array($dpid), array(), $reset);
  return reset($dpiproperties);
}

/**
 * Load multiple dpiproperties
 *
 * @param array $dpids
 * @param array $conditions
 *
 * @param boolean $reset
 */
function dpiproperties_load_multiple($dpids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('dpiproperties', $dpids, $conditions, $reset);
}

/**
 * Load a DPI properties by external reference
 *
 * @param String $external_reference
 * @param boolean $reset
 *
 * @return DPIProperties
 */
function dpiproperties_load_by_external_reference($external_reference, $reset = FALSE) {
  $dpiproperties = entity_load('dpiproperties', FALSE, array('external_reference' => $external_reference), $reset);
  return reset($dpiproperties);
}

/**
 * Load a DPI properties by entity type and entity id
 *
 * @param String $type
 * @param Integer $id
 * @param boolean $reset
 *
 * @return DPIProperties
 */
function dpiproperties_load_by_type_and_id($type, $id, $reset = FALSE) {
  $dpiproperties = entity_load('dpiproperties', FALSE, array('entity' => $type, 'id' => $id), $reset);
  return reset($dpiproperties);
}

/**
 * Save dpiproperties.
 */
function dpiproperties_save($dpiproperties) {
  
  entity_save('dpiproperties', $dpiproperties);
  
}

/**
 * Delete single dpiproperties.
 */
function dpiproperties_delete($dpiproperties) {
  entity_delete('dpiproperties', entity_id('dpiproperties' ,$dpiproperties));
}

/**
 * Delete multiple dpiproperties.
 */
function dpiproperties_delete_multiple($dpiproperties_ids) {
  entity_delete_multiple('dpiproperties', $dpiproperties_ids);
}

/**
 * Access callback to manage DPIProperties
 *
 * @param String $op
 * @param unknown_type $task
 * @param Object $account
 * @param String $entity_type
 */
function dpiproperties_access_admin_callback($op, $task, $account = NULL, $entity_type = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }
  switch ($op) {
    case 'create':
      return user_access('administer dpiproperties', $account)
      || user_access('create dpiproperties', $account);
    case 'view':
      return user_access('administer dpiproperties', $account)
      || user_access('view dpiproperties', $account);
    case 'edit':
      return user_access('administer dpiproperties')
      || user_access('edit dpiproperties');
  }
  return TRUE;
}

/**
 * Return the title of a DPIProperties page
 *
 * @param Object $target
 */
function dpiproperties_page_title($target) {
  return $target->external_reference;
}

/**
 * Callback to view a dpiproperties
 *
 * @param DPIProperties $dpiproperties
 * @param String $view_mode
 */
function dpiproperties_view($dpiproperties, $view_mode = 'full') {
  return $dpiproperties->view();
}

/**
 * Load DPIProperties Type.
 */
function dpiproperties_type_load($dpiproperties_type) {
  return dpiproperties_types($dpiproperties_type);
}

/**
 * List of dpiproperties Types.
 */
function dpiproperties_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('dpiproperties_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save dpiproperties type entity.
 */
function dpiproperties_type_save($dpiproperties_type) {
  entity_save('dpiproperties_type', $dpiproperties_type);
}

/**
 * Delete single case type.
 */
function dpiproperties_type_delete($dpiproperties_type) {
  entity_delete('dpiproperties_type', entity_id('dpiproperties_type' ,$dpiproperties_type));
}

/**
 * Delete multiple case types.
 */
function dpiproperties_type_delete_multiple($dpiproperties_ids) {
  entity_delete_multiple('dpiproperties_type', $dpiproperties_ids);
}

/**
 * Access callback
 */
function dpiproperties_type_access_admin_callback() {
  return TRUE;
}

/**
 * Generates the dpiproperties type editing form.
 */
function dpiproperties_type_form($form, &$form_state, $dpiproperties_type, $op = 'edit') {
  if ($op == 'clone') {
    $dpiproperties_type->label .= ' (cloned)';
    $dpiproperties_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $dpiproperties_type->label,
    '#description' => t('The human-readable name of this dpiproperties type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($dpiproperties_type->type) ? $dpiproperties_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $dpiproperties_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'dpiproperties_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this dpiproperties type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($dpiproperties_type->description) ? $dpiproperties_type->description : '',
    '#description' => t('Description about the dpiproperties type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save dpiproperties type'),
    '#weight' => 40,
  );

  if (!$dpiproperties_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete dpiproperties type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('dpiproperties_type_form_submit_delete')
    );
  }

  return $form;
}

/**
 * Submit handler for creating/editing dpiproperties_type.
 */
function dpiproperties_type_form_submit(&$form, &$form_state) {
  $dpiproperties_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  dpiproperties_type_save($dpiproperties_type);

  // Redirect user back to list of dpiproperties types.
  $form_state['redirect'] = 'admin/structure/dpiproperties-types';
}

function dpiproperties_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/dpiproperties-types/' . $form_state['dpiproperties_type']->type . '/delete';
}

/**
 * dpiproperties type delete form.
 */
function dpiproperties_type_form_delete_confirm($form, &$form_state, $dpiproperties_type) {
  $form_state['dpiproperties_type'] = $dpiproperties_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['dpiproperties_type_id'] = array('#type' => 'value', '#value' => entity_id('dpiproperties_type' ,$dpiproperties_type));
  return confirm_form($form,
      t('Are you sure you want to delete dpiproperties type %title?', array('%title' => entity_label('dpiproperties_type', $dpiproperties_type))),
      'dpiproperties/' . entity_id('dpiproperties_type' ,$dpiproperties_type),
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * dpiproperties type delete form submit handler.
 */
function dpiproperties_type_form_delete_confirm_submit($form, &$form_state) {
  $dpiproperties_type = $form_state['dpiproperties_type'];
  dpiproperties_type_delete($dpiproperties_type);

  watchdog('dpiproperties_type', '@type: deleted %title.', array('@type' => $dpiproperties_type->type, '%title' => $dpiproperties_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $dpiproperties_type->type, '%title' => $dpiproperties_type->label)));

  $form_state['redirect'] = 'admin/structure/dpiproperties-types';
}

/**
 * Page to select dpiproperties Type to add new dpiproperties.
 */
function dpiproperties_admin_add_page() {
  $items = array();
  foreach (dpiproperties_types() as $dpiproperties_type_key => $dpiproperties_type) {
    $items[] = l(entity_label('dpiproperties_type', $dpiproperties_type), 'dpiproperties/add/' . $dpiproperties_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of dpiproperties to create.')));
}

/**
 * Add new dpiproperties page callback.
 */
function dpiproperties_add($type) {
  $dpiproperties_type = dpiproperties_types($type);

  $dpiproperties = entity_create('dpiproperties', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('dpiproperties_type', $dpiproperties_type))));

  $output = drupal_get_form('dpiproperties_form', $dpiproperties);

  return $output;
}

/**
 * dpiproperties Form.
 */
function dpiproperties_form($form, &$form_state, $dpiproperties) {
  $form_state['dpiproperties'] = $dpiproperties;

  $form['external_reference'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('External reference'),
    '#default_value' => $dpiproperties->external_reference,
  );

  $form['external_reference_2'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('External reference 2'),
    '#default_value' => $dpiproperties->external_reference_2,
  );

  $form['source'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Source'),
    '#default_value' => $dpiproperties->source,
  );

  $form['product_id'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Product ID'),
    '#default_value' => $dpiproperties->product_id,
  );

  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $dpiproperties->entity,
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $dpiproperties->id,
  );

  field_attach_form('dpiproperties', $dpiproperties, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save dpiproperties'),
    '#submit' => $submit + array('dpiproperties_form_submit'),
  );

  // Show Delete button if we edit dpiproperties.
  $dpiproperties_id = entity_id('dpiproperties', $dpiproperties);
  if (!empty($dpiproperties_id) && dpiproperties_access_admin_callback('edit', $dpiproperties)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('dpiproperties_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'dpiproperties_form_validate';

  return $form;
}

function dpiproperties_form_validate($form, &$form_state) {

}

/**
 * dpiproperties submit handler.
 */
function dpiproperties_form_submit($form, &$form_state) {
  $dpiproperties = $form_state['dpiproperties'];

  entity_form_submit_build_entity('dpiproperties', $dpiproperties, $form, $form_state);

  dpiproperties_save($dpiproperties);

  $dpiproperties_uri = entity_uri('dpiproperties', $dpiproperties);

  $form_state['redirect'] = $dpiproperties_uri['path'];

  drupal_set_message(t('dpiproperties %title saved.', array('%title' => entity_label('dpiproperties', $dpiproperties))));
}

function dpiproperties_form_submit_delete($form, &$form_state) {
  $dpiproperties = $form_state['dpiproperties'];
  $dpiproperties_uri = entity_uri('dpiproperties', $dpiproperties);
  $form_state['redirect'] = $dpiproperties_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function dpiproperties_delete_form($form, &$form_state, $dpiproperties) {
  $form_state['dpiproperties'] = $dpiproperties;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['dpiproperties_type_id'] = array('#type' => 'value', '#value' => entity_id('dpiproperties' ,$dpiproperties));
  $dpiproperties_uri = entity_uri('dpiproperties', $dpiproperties);
  return confirm_form($form,
      t('Are you sure you want to delete dpiproperties %title?', array('%title' => entity_label('dpiproperties', $dpiproperties))),
      $dpiproperties_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function dpiproperties_delete_form_submit($form, &$form_state) {
  $dpiproperties = $form_state['dpiproperties'];
  dpiproperties_delete($dpiproperties);

  drupal_set_message(t('dpiproperties %title deleted.', array('%title' => entity_label('dpiproperties', $dpiproperties))));

  $form_state['redirect'] = '<front>';
}

/**
 * Set DPI Properties on an object
 *
 * @param Object $object
 * @param String $property_name
 * @param String $wrapper
 */
function entity_property_dpiproperties_set_properties($object, $property_name, $wrapper) {
  $name = str_replace('field_', '', $property_name);
  if (!isset($object->dpiproperties)){
    $object->dpiproperties = new DPIPropertiesEntity(array(), 'dpiproperties');
  }
  $object->dpiproperties->$name = $wrapper;
}

/**
 * Set DPI Properties on a object
 *
 * @param Object $object
 * @param String $property_name
 * @param String $wrapper
 */
function entity_property_dpiproperties_get_properties($object, $options, $property_name, $type, $info) {
  $name = str_replace('field_', '', $property_name);
  if (!isset($object->dpiproperties)){
    $object->dpiproperties = new DPIPropertiesEntity(array(), 'dpiproperties');
  }
  return $object->dpiproperties->$name;
}
