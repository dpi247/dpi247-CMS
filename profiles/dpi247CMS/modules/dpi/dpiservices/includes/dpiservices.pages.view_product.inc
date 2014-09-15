<?php
module_load_include('inc', 'dpiservices', 'includes/dpiservices.pages.settings');


function dpiservices_pages_form_view_product_form($form, &$form_state){
  $form=array();
  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  $query = db_select('menu_custom', 'm')->fields('m',array('menu_name','title'))->orderBy('title')->execute();
  $value_select_menu = array();
  while ($row = $result->fetchAssoc()){
    $value_select_menu[$row['menu_name']] = $row['title'];
  }
  
  $form['info']=array(
    '#markup'=>'All field is required',
  );
  
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a product'),
  );
  
  $form['fieldset']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
  );
  
  $form['fieldset']['machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#required' => TRUE,
  );
  
  $form['fieldset']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => FALSE,
  );
  
  foreach (dpiservices_get_active_value_for_preset() as $preset){
    $form['fieldset'][$preset['preset_name']] = array(
      '#title' => t($preset['preset_name'].' ('.$preset['preset_map_name'].')'),
      '#type' => 'checkbox',
    );
  }
  
  $form['fieldset']['select_product'] = array(
    '#type' => 'select',
    '#title' => t('Select product based on :'),
    '#options' => array(
      'Menu (Recommended)',
      'Destination'
    )
  );
  
  $form['fieldset']['select_product_type'] = array(
    '#type' => 'select',
    '#title' => t('Select a type'),
    '#options' => $value_select_menu
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value'=>'save'
  );
  
  return $form;
}