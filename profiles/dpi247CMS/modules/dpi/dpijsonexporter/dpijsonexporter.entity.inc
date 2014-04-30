<?php

/**
 * Implements hook_entity_info().
 */
function dpijsonexporter_entity_info(){
  $entities = array();

  $entities['jsonproduct'] = array(
    'label' => t('JsonExporter Product'),
    // seulement si on utilise le module entity API
    'exportable' => TRUE,
    'entity class' => 'JsonproductEntity',
    'controller class' => 'JsonproductController',
    'base table' => 'dpijsonexporter_jsonproduct',
    'fieldable' => TRUE,
    'module'=>"dpijsonexporter",
    // Make use the class' label() and uri() implementation by default.
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'entity keys' => array(
      'id' => 'id',
      'name' => 'machine_name',
      'label' => 'label',
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
    'module' => 'dpijsonexporter',
    'access callback' => 'jsonproduct_access_admin_callback',
    // Enable the entity API's admin UI.
    'metadata controller class' => 'DpiDestinationsTargetMetadataController',
  );
    $entities['jsonproduct']['admin ui'] = array(
      // Enable the entity API's admin UI.
      'path' => 'admin/dpi247/dpijsonexporter',
      'file' => 'includes/dpijsonexporter.pages.admin.inc',
      'controller class' => 'DpiJsonexporterJsonproductUIController',
    );


  return $entities;
}


function jsonproduct_load($tid = NULL, $reset = FALSE){
  $tids = (isset ($tid) ? array($tid) : array());
  $jsonproduct = jsonproduct_load_multiple($tids, $reset);
  return $jsonproduct ? reset ($jsonproduct) : FALSE;
}

function jsonproduct_load_multiple($tids = array(), $conditions = array(), $reset = FALSE){
  return entity_load('jsonproduct', $tids, $conditions, $reset);
}


/**
 * Save jsonproduct.
 */
function jsonproduct_save($jsonproduct) {
  entity_save('jsonproduct', $jsonproduct);
}

/**
 * Delete single jsonproduct.
 */
function jsonproduct_delete($jsonproduct) {
  entity_delete('jsonproduct', entity_id('jsonproduct' ,$jsonproduct));
}

/**
 * Delete multiple jsonproducts.
 */
function jsonproduct_delete_multiple($jsonproduct_ids) {
  entity_delete_multiple('jsonproduct', $jsonproduct_ids);
}


function jsonproduct_access_admin_callback(){
  return TRUE;
}

function jsonproduct_page_title($jsonproduct){
  return $jsonproduct->machine_name;
}

function jsonproduct_view($jsonproduct, $view_mode = 'full'){
  return $jsonproduct->view();
}

function jsonproduct_form($form, &$form_state, $jsonproduct) {
  $menus = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  foreach($menus as$menu){
    $menu_name_options[$menu['menu_name']]=$menu['title'];
  }
  dsm(image_styles());
  $presets_options=  image_styles();
  foreach(image_styles() as $style_id=>$style){
    $presets_options[$style_id]=$style['label'];
  }
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $jsonproduct->id,
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $jsonproduct->label,
    '#required' => TRUE,
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $jsonproduct->machine_name,
    '#machine_name' => array(
      'source' => array('label'),
      'label' => t('JsonProduct Machine name'),
      'exists' => '_dpijsonexporter_jsonproduct_machine_name_exists',
    ),
    '#disabled' => !empty($jsonproduct->machine_name),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $jsonproduct->description,
    '#required' => TRUE,
  );

  $form['menu_name'] = array(
    '#type' => 'select',
    '#title' => t('Menu'),
    '#options' => $menu_name_options,
    '#default_value' => $jsonproduct->menu_name,
    '#required' => TRUE,
    
  );
  $form['presets'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available Presets'),
    '#options' => $presets_options,
    '#default_value' => $jsonproduct->presets,
    '#required' => TRUE,
    
  );



  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  field_attach_form('jsonproduct', $jsonproduct, $form, $form_state);
  return $form;
}


function jsonproduct_form_submit($form, &$form_state) {
  $jsonproduct = entity_ui_form_submit_build_entity($form, $form_state);
  $jsonproduct->available_jsonproductlayouts = serialize($jsonproduct->available_jsonproductlayouts);
  $jsonproduct->save();
  $form_state['redirect'] = "admin/dpi247/dpijsonexporter/jsonproduct";
}

