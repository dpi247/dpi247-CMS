<?php 
/**
 * 
 * Page callback that render the list of the importer
 */
function dpimport_page_importer_list(){
  module_load_include('inc', 'dpimport', 'includes/dpimport.process');
  $importers = dpimport_getallimportersettings();
  $vars = array();
  $vars['header'] = array('Name', 'Type', 'Version', 'Dispatcher', 'Action');
  $vars['rows'] = array();
  foreach ($importers as $imp){
    $edit = '<a href="/admin/config/dpimport/import/edit/'.$imp->name.'">'.t('Edit').'</a>';
    $delete = ' <a href="/admin/config/dpimport/import/delete/'.$imp->name.'">'.t('Delete').'</a>';
    $vars['rows'][] = array($imp->name, $imp->type, $imp->version, $imp->dispatcher, $edit.$delete);
  }
  return theme('table', $vars);
}

/**
 * 
 * Importer creation/edition Form
 * @param array $form
 * @param array $form_state
 * @param String $edit_id
 */
function dpimport_page_form_importer_form($form, &$form_state, $edit_id = NULL){
  
  ctools_include('plugins');
  $form = array();
  $form = dpimport_form_importer_get_header($form, $form_state);
  
  if ($edit_id != NULL && $form_state['stage'] == 'default'){
    dpimport_form_importer_initializeform($form_state, $edit_id);
  }
  
  switch ($form_state['stage']){
    case 'importer': dpimport_form_importer_stageimporter_form($form, $form_state);break;
    case 'dispatcher': dpimport_form_importer_stagedispatcher_form($form, $form_state);break;
    default : dpimport_form_importer_stagedefault_form($form, $form_state);break;
  }
  
  if ($form_state['stage'] != 'default'){
    $form['back'] = array(
      '#type' => 'submit',
      '#value' => t('Back')
    );
  }
 
  if ($form_state['stage'] == 'dispatcher'){
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  } else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
    );
  }
  
  return $form;
}
/**
 * 
 * Validation of the importer creation/edition Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_page_form_importer_form_validate($form, &$form_state){
  if ($form_state['triggering_element']['#value'] != 'Back') {
   
    switch ($form_state['stage']){
      case 'importer': dpimport_form_importer_stageimporter_form_validate($form, $form_state);break;
      case 'dispatcher': dpimport_form_importer_stagedispatcher_form_validate($form, $form_state);break;
      default: dpimport_form_importer_stagedefault_form_validate($form, $form_state);break; 
    }
  }
}
/**
 * 
 * Submission of the importer creation/edition Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_page_form_importer_form_submit($form, &$form_state){
  switch ($form_state['stage']) {   
    case 'dispatcher':
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
        $result = dpimport_form_importer_save($form, $form_state);
        if ($result !== FALSE){
          drupal_set_message(t('The importer instance has correctly been created'));
        } else {
          drupal_set_message(t('The importer instance could not been created'), 'error');
        }
        $form_state['complete'] = TRUE;
      }
      break;
  
    default:
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      $form_state['new_stage'] = dpimport_form_importer_move_to_next_stage($form, $form_state);
    break;
  
  }
  
  if (isset($form_state['complete'])){
    drupal_goto('admin/config/dpimport/import');
  }
   
  if ($form_state['triggering_element']['#value'] == 'Back') {
    $form_state['new_stage'] = dpimport_form_importer_move_to_previous_stage($form, $form_state);
  }
   
  if (isset($form_state['multistep_values']['form_build_id'])) {
    $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
  }
  $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
  $form_state['stage'] = $form_state['new_stage'];
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * Save the Importer form forms values
 * @param Array $form
 * @param Array $form_state
 * @return FALSE if an error occurs
 */
function dpimport_form_importer_save($form, $form_state){
  ctools_include('plugins');
  
  $default_values = $form_state['multistep_values']['default'];
  $importer_values = $form_state['multistep_values']['importer'];
  $dispatcher_values = $form_state['multistep_values']['dispatcher'];
  
  //dispatcher
  $dispatcher_class = $importer_values['dispatcher'];
  $dispatcher_function = 'get'.$dispatcher_class;
  $importer = NULL;
  $settings = array();
  $dispatcher = $dispatcher_class::$dispatcher_function($importer, $settings);
  
  //importer
  $importer_plugin = ctools_get_plugins('dpimport', 'importer', $default_values['type']);
  $importer_class = $importer_plugin['versions'][$default_values['version']]['class'];
  $importer_settings = array();
  $importer = new $importer_class($importer_settings, $importer_plugin);
  
  //Save values in dpimport_importer
  $record = array();
  $record['type'] = $default_values['type'];
  $record['version'] = $default_values['version'];
  $record['name'] = $default_values['name'];
  $record['description'] = $default_values['description'];
  $record['dispatcher'] = $importer_values['dispatcher'];
  $record['dispatcher_settings'] = serialize($dispatcher->submitSettingsForm($dispatcher_values));
  $record['importer_settings'] = serialize($importer->submitSettingsForm($importer_values));

  if (is_numeric($default_values['iid'])){
    $result = $importer->updateImporterSettings($record, $default_values['iid']);
  } else {
    $result = $importer->insertImporterSettings($record);
  }
  return $result;
}
/**
 * 
 * Build the header of the importer creation/edition form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_get_header($form, &$form_state) {
   
  $form_state['stage'] = isset($form_state['stage']) ? $form_state['stage'] : 'default';
  
  $form_stages = array(
    'default' => 1,
    'importer' => 2,
    'dispatcher' => 3,
  );
   
  if (isset($form_stages[$form_state['stage']])) {
    $current_step = $form_stages[$form_state['stage']];
  } else {
    $current_step = 1;
  }
  
  $stages = array(
    1 => array('data' => '1. Default'),
    2 => array('data' => '2. Importer'),
    3 => array('data' => '3. Dispatcher'),
  );
   
  $stages[$current_step]['class'] = array('active');
   
  $stages_list = theme('item_list', array('items' => $stages));
   
  $form['header'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#value' => $stages_list,
  );
  return $form;
}
/**
 * 
 * Return the next stage 
 * @param Array $form
 * @param Array $form_state
 * @return String $next_stage
 */
function dpimport_form_importer_move_to_next_stage($form, &$form_state){
  $next_stage = 'default';
  switch ($form_state['stage']) {
    case 'default': $next_stage = 'importer';break;
    case 'importer': $next_stage = 'dispatcher';break;
  }
  return $next_stage;
}
/**
 * 
 * Return the previous stage
 * @param Array $form
 * @param Array $form_state
 * @return String $previous_stage
 */
function dpimport_form_importer_move_to_previous_stage($form, &$form_state) {
  $previous_stage = 'dispatcher';
  switch ($form_state['stage']) {
    case 'importer': $previous_stage = 'default';break;
    case 'dispatcher': $previous_stage = 'importer';break;
  }
  return $previous_stage;
}
/**
 * 
 * Default stage of the importer edition/creation Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_stagedefault_form(&$form, &$form_state){
  
  $values = isset($form_state['multistep_values']['default']) ? $form_state['multistep_values']['default'] : array();
  
  $form['default']['iid'] = array(
    '#type' => 'value',
    '#default_value' => isset($values['iid']) ? $values['iid'] : NULL,
  );
  
  $form['default']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The machine name of the Importer. It must be unique and alphanumerique only'),
    '#required' => TRUE,
    '#default_value' => isset($values['name']) ? $values['name'] : NULL,
    '#disabled' => isset($values['iid']) ? TRUE : FALSE,
  );
  
  $form['default']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Description of the importer'),
    '#default_value' => isset($values['description']) ? $values['description'] : NULL,
  );
  
  //Get all the importer plugin to display in the select
  $options = array();
  $importer_plugins = ctools_get_plugins('dpimport', 'importer');
  foreach ($importer_plugins as $plugin){
    $options[$plugin['type']] = $plugin['label'];
  }
  
  $form['default']['type'] = array(
    '#type' => 'select',
    '#title' => t('Importer'),
    '#description' => t('Choose the type of the importer'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => isset($values['type']) ? $values['type'] : NULL,
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'importer-version',
      'callback' => '_dpimport_ajaxcallback_page_form_importer',
      'method' => 'replace',
    ),
  );
  
  //Version : The version appears in ajax after the importer have been selected. 
  $importer = 'default';
  if (isset($values['type'])){
    $importer = $values['type'];
  } elseif (isset($form_state['values']['type'])){
    $importer = $form_state['values']['type'];  
  } 
  
  //Get all the version of the importer plugin selected to display in the select
  $version_options = array();
  if ($importer != 'default'){
    foreach ($importer_plugins[$importer]['versions'] as $version_key => $version_value){
      $version_options[$version_key] = $version_key;
    }
  }
  
  $form['default']['version'] = array(
    '#prefix' => '<div id="importer-version">', 
    '#suffix' => '</div>'
  );
  
  if ($importer != 'default'){
    $form['default']['version'] = array(
      '#type' => 'select',
      '#title' => t('Version'),
      '#options' => $version_options,
      '#default_value' => isset($values['version']) ? $values['version'] : NULL,
      '#prefix' => '<div id="importer-version">', 
      '#suffix' => '</div>'
    );
  }
}
/**
 * 
 * Ajax callback to print the version form element when an options have been choised.
 * @param Array $form
 * @param Array $form_state
 */
function _dpimport_ajaxcallback_page_form_importer($form, $form_state) {  
  return render($form['default']['version']);
}

/**
 * 
 * Validation of the Default stage of the importer edition/creation Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_stagedefault_form_validate($form, &$form_state){
  module_load_include('inc', 'dpimport','includes/dpimport.process');
  
  if (!preg_match('/^[a-z0-9]+$/' , $form_state['values']['name'])){
    form_set_error('name', t('Name must be alphanumeric only.'));
  }
  
  if (!is_numeric($form_state['values']['iid']) && dpimport_getimportersettings($form_state['values']['name']) != NULL){
    form_set_error('name', t('A importer with this name already exists'));
  }
}

/**
 * 
 * Importer stage of the importer edition/creation Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_stageimporter_form(&$form, &$form_state){
  
  $values = isset($form_state['multistep_values']['importer']) ? $form_state['multistep_values']['importer'] : array();
  
  $importer_plugin = ctools_get_plugins('dpimport', 'importer', $form_state['multistep_values']['default']['type']);
  
  $importer_class = $importer_plugin['versions'][$form_state['multistep_values']['default']['version']]['class'];
  $importer_settings = array();
  $importer = new $importer_class($importer_settings, $importer_plugin);
  
  //Get the specific form elements for this importer
  $form = $importer->getSettingsForm($form_state, $values);
  
  $options = array();
  foreach ($importer_plugin['dispatchers'] as $dispatcher){
    $options[$dispatcher] = $dispatcher;
  }
  $form['importer']['dispatcher'] = array(
    '#type' => 'select',
    '#title' => t('Dispatcher'),
    '#description' => t('Choose the dispatcher.'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => isset($values['dispatcher']) ? $values['dispatcher'] : NULL,
    '#weight' => -10,
  );
  
}
/**
 * 
 * Validation of the Importer stage of the importer edition/creation Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_stageimporter_form_validate($form, &$form_state){
  ctools_include('plugins');
  $importer_plugin = ctools_get_plugins('dpimport', 'importer', $form_state['multistep_values']['default']['type']);
  
  $importer_class = $importer_plugin['versions'][$form_state['multistep_values']['default']['version']]['class'];
  $importer_settings = array();
  $importer = new $importer_class($importer_settings, $importer_plugin);
  //Get the validation for the specific form elements of this importer
  $importer->validateSettingsForm($form_state);
}
/**
 * 
 * Dispatcher stage of the importer edition/creation Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_stagedispatcher_form(&$form, &$form_state){
  $values = isset($form_state['multistep_values']['dispatcher']) ? $form_state['multistep_values']['dispatcher'] : array();
  $dispatcher_class = $form_state['multistep_values']['importer']['dispatcher'];
  $dispatcher_function = 'get'.$dispatcher_class;
  $importer = NULL;
  $settings = array();
  $dispatcher = $dispatcher_class::$dispatcher_function($importer, $settings);
  //Get the specific form elements for this dispatcher
  $form = $dispatcher->getSettingsForm($form_state, $values);
}
/**
 * 
 * Validation of the Dispatcher stage of the importer edition/creation Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_form_importer_stagedispatcher_form_validate($form, &$form_state){
  $dispatcher_class = $form_state['multistep_values']['importer']['dispatcher'];
  $dispatcher_function = 'get'.$dispatcher_class;
  $importer = NULL;
  $settings = array();
  $dispatcher = $dispatcher_class::$dispatcher_function($importer, $settings);
  //Get the validation for the specific form elements of this dispatcher
  $dispatcher->validateSettingsForm($form_state);
}
/**
 * 
 * Initialize the form state with de actual value of the importer (only in edition)
 * @param Array $form_state
 * @param String $edit_id
 */
function dpimport_form_importer_initializeform(&$form_state, $edit_id){
  module_load_include('inc', 'dpimport','includes/dpimport.process');
  $importer_settings = dpimport_getimportersettings($edit_id);
  
  //importer
  $importer_plugin = ctools_get_plugins('dpimport', 'importer', $importer_settings->type);
  $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
  $importer = new $importer_class($importer_settings, $importer_plugin);
  
  //dispatcher
  $dispatcher = $importer->getDispatcher();
  
  $form_state['multistep_values']['default']['iid'] = $importer_settings->iid;
  $form_state['multistep_values']['default']['name'] = $importer_settings->name;
  $form_state['multistep_values']['default']['description'] = $importer_settings->description;
  $form_state['multistep_values']['default']['type'] = $importer_settings->type;
  $form_state['multistep_values']['default']['version'] = $importer_settings->version;
  $form_state['multistep_values']['importer'] = $importer->initializeForm();
  $form_state['multistep_values']['importer']['dispatcher'] = $importer_settings->dispatcher;
  $form_state['multistep_values']['dispatcher'] = $dispatcher->initializeForm();
}
/**
 * 
 * Importer deletion Form
 * @param Array $form
 * @param Array $form_state
 * @param String $delete_id
 */
function dpimport_page_form_importer_delete_form($form, &$form_state, $delete_id){
  
  $form = array();
  
  $form['message'] = array(
    '#markup' => '<p>'.t('Are you sure you want to delete the importer "@name"', array('@name' => $delete_id)).'?</p>',
  );
  
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $delete_id,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  return $form;
}

/**
 * 
 * Submition of the Importer deletion Form
 * @param Array $form
 * @param Array $form_state
 */
function dpimport_page_form_importer_delete_form_submit($form, &$form_state){

  ctools_include('plugins');
  module_load_include('inc', 'dpimport','includes/dpimport.process');

  $importer_settings = dpimport_getimportersettings($form_state['values']['name']);
  $importer_plugin = ctools_get_plugins('dpimport', 'importer', $importer_settings->type);
  $importer_class = $importer_plugin['versions'][$importer_settings->version]['class'];
  $importer = new $importer_class($importer_settings, $importer_plugin);

  $result = $importer->deleteImporterSettings($importer_settings->iid);
  if ($result !== FALSE){
    drupal_set_message(t('The importer instance has correctly been deleted'));
  } else {
    drupal_set_message(t('The importer instance could not been deleted'), 'error');
  }
  
  drupal_goto('admin/config/dpimport/import');
}