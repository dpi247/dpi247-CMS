<?php 

/**
 * General settings for DPI247
 *
 * @param array $form_state
 */
function dpicommons_admin_settings_form($form, $form_state) {
  $form['dpicommons_product'] = array(
    '#type' => 'textfield',
    '#title' => t('ProductID'),
    '#description' => t('The product of the website, it must be unique.'),
    '#default_value' => dpi_variable_get('dpicommons_product', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['dpicommons_environment'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment'),
    '#description' => t('Stage, Prod, Dev, etc.'),
    '#default_value' => dpi_variable_get('dpicommons_environment', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['dpicommons_http_request_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP Request Timeout'),
    '#description' => t('Specifies the default timeout limit (in seconds) for every HTTP request made from this site'),
    '#default_value' => dpi_variable_get('dpicommons_http_request_timeout', 3),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['dpicommons_http_request_retry'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP Request retry'),
    '#description' => t('Specifies the default retry limit for every HTTP request made from this site'),
    '#default_value' => dpi_variable_get('dpicommons_http_request_retry', 1),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['#validate'][] = 'dpicommons_admin_settings_form_validate';

  return dpi_system_settings_form($form);
}

/**
 * Validation of dpicommons_admin_settings_form
 *
 * @param array $form
 * @param array $form_state
 */
function dpicommons_admin_settings_form_validate($form, &$form_state){
  if (!preg_match('/^[0-9]+$/' , $form_state['values']['dpicommons_http_request_timeout'])){
    form_set_error('dpicommons_http_request_timeout', t('HTTP Request Timeout must be an integer.'));
  }
  if (!preg_match('/^[0-9]+$/' , $form_state['values']['dpicommons_http_request_retry'])){
    form_set_error('dpicommons_http_request_retry', t('HTTP Request retry must be an integer.'));
  }
}






/**
 * Finalize install process
 * 
 * Save section variant in DB at the end of the install process
 * We need to do this here since it is not possible to do it in the install.php front controller due to
 * Some specific setup during install (DrupalFakeCache caching, mecanisme etc)
 * 
 */
function _dpicommons_finalize_install_process(){

  if(!variable_get("dpicommons_finalize_install",FALSE)){
    
    //Special treatement for pages cuz they include several object better reliable to use the "API"
    module_load_include('inc', 'page_manager', 'page_manager.admin');
    $page=_dpicommons_build_page_manager_cache_load('sections');
    foreach($page->handler_info as $key=>$value){
      $page->handler_info[$key]['changed'] = PAGE_MANAGER_CHANGED_CACHED;
    }
    page_manager_save_page_cache($page);
    $return=t("Finalize process done");
    variable_set("dpicommons_finalize_install",TRUE);
  }
  echo "info:".$return; 
}



function _dpicommons_build_page_manager_cache_load($task_name){
  $cache = new stdClass();
  $cache->task_name = $task_name;
  list($cache->task_id, $cache->subtask_id) = page_manager_get_task_id($cache->task_name);

  $cache->task = page_manager_get_task($cache->task_id);
  if (empty($cache->task)) {
    return FALSE;
  }

  if ($cache->subtask_id) {
    $cache->subtask = page_manager_get_task_subtask($cache->task, $cache->subtask_id);
    if (empty($cache->subtask)) {
      return FALSE;
    }
  }
  else {
    $cache->subtask = $cache->task;
    $cache->subtask['name'] = '';
  }

  $cache->handlers = page_manager_load_sorted_handlers($cache->task, $cache->subtask_id);
  $cache->handler_info = array();
  foreach ($cache->handlers as $id => $handler) {
    $cache->handler_info[$id] = array(
      'weight' => $handler->weight,
      'changed' => FALSE,
      'name' => $id,
    );
  }
  return $cache;
}
