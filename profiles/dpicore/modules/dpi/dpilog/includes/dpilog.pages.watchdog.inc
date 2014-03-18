<?php

module_load_include('inc', 'dpilog','dpilog.private');

// --------------------------------------------------------------
// ------------- Listing de tous les watchdogs ------------------
// --------------------------------------------------------------

/**
 * Page callback: Displays a listing of database log messages.
 *
 * Messages are truncated at 56 chars. Full-length messages can be viewed on the
 * message details page.
 *
 * @see dpilog_watchdog_clear_log_form()
 * @see dpilog_watchdog_event()
 * @see dpilog_watchdog_filter_form()
 * @see dpilog_watchdog_menu()
 *
 * @ingroup logging_severity_levels
 */
function dpilog_watchdog_overview() {
  $watchdog_table_name = _dpilog_watchdog_get_table_name();
  $filter = dpilog_watchdog_build_filter_query();
  $rows = array();
  $classes = array(
    WATCHDOG_DEBUG     => 'dpiwatchdog-debug',
    WATCHDOG_INFO      => 'dpiwatchdog-info',
    WATCHDOG_NOTICE    => 'dpiwatchdog-notice',
    WATCHDOG_WARNING   => 'dpiwatchdog-warning',
    WATCHDOG_ERROR     => 'dpiwatchdog-error',
    WATCHDOG_CRITICAL  => 'dpiwatchdog-critical',
    WATCHDOG_ALERT     => 'dpiwatchdog-alert',
    WATCHDOG_EMERGENCY => 'dpiwatchdog-emerg',
  );

  $build['dpilog_watchdog_filter_form'] = drupal_get_form('dpilog_watchdog_filter_form');
  $build['dpilog_watchdog_clear_log_form'] = drupal_get_form('dpilog_watchdog_clear_log_form');

  $header = array(
    '', // Icon column.
    array('data' => t('Type'), 'field' => 'w.type'),
    array('data' => t('Date'), 'field' => 'w.wid', 'sort' => 'desc'),
    t('Message'),
    array('data' => t('User'), 'field' => 'u.name'),
  );

  $query = db_select($watchdog_table_name, 'w')->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query
    ->fields('w', array('wid', 'uid', 'severity', 'type', 'timestamp', 'message', 'variables'))
    ->addField('u', 'name');
  if (!empty($filter['where'])) {
    $query->where($filter['where'], $filter['args']);
  }
  $result = $query
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  foreach ($result as $dpiwatchdog) {
    $rows[] = array('data' =>
      array(
        // Cells
        array('class' => 'icon'),
        t($dpiwatchdog->type),
        format_date($dpiwatchdog->timestamp, 'short'),
        theme('dpilog_watchdog_message', array('event' => $dpiwatchdog, 'as_link'=>TRUE, 'length' => 70)),
        theme('username', array('account' => $dpiwatchdog)),
      ),
      // Attributes for tr
      'class' => array(drupal_html_class('dpiwatchdog-' . $dpiwatchdog->type), $classes[$dpiwatchdog->severity]),
    );
  }

  $build['dpilog_watchdog_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-dpiwatchdog'),
    '#empty' => t('No log messages available.'),
  );
  $build['dpilog_watchdog_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Builds a query for database log administration filters based on session.
 *
 * @return array
 *   An associative array with keys 'where' and 'args'.
 */
function dpilog_watchdog_build_filter_query() {
  if (empty($_SESSION['dpilog_watchdog_overview_filter'])) {
    return;
  }

  $filters = dpilog_watchdog_filters();

  // Build query
  $where = $args = array();
  foreach ($_SESSION['dpilog_watchdog_overview_filter'] as $key => $filter) {
    $filter_where = array();
    foreach ($filter as $value) {
      $filter_where[] = $filters[$key]['where'];
      $args[] = $value;
    }
    if (!empty($filter_where)) {
      $where[] = '(' . implode(' OR ', $filter_where) . ')';
    }
  }
  $where = !empty($where) ? implode(' AND ', $where) : '';

  return array(
    'where' => $where,
    'args' => $args,
  );
}

/**
 * Creates a list of database log administration filters that can be applied.
 *
 * @return array
 *   Associative array of filters. The top-level keys are used as the form
 *   element names for the filters, and the values are arrays with the following
 *   elements:
 *   - title: Title of the filter.
 *   - where: The filter condition.
 *   - options: Array of options for the select list for the filter.
 */
function dpilog_watchdog_filters() {
  $filters = array();

  foreach (_dpilog_watchdog_get_message_types() as $type) {
    $types[$type] = t($type);
  }
  if (!empty($types)) {
    $filters['type'] = array(
      'title' => t('Type'),
      'where' => "w.type = ?",
      'options' => $types,
    );
  }

  $filters['severity'] = array(
    'title' => t('Severity'),
    'where' => 'w.severity = ?',
    'options' => watchdog_severity_levels(),
  );

  $filters['user'] = array(
    'title' => t('User'),
    'where' => 'w.uid = ?',
    'options' => _dpilog_watchdog_get_users_listed(),
  );

  return $filters;
}

/**
 * Get list of all users associated to an event
 * @return {Array}
 */
function _dpilog_watchdog_get_users_listed() {
  $users = array();
  $watchdog_table_name = _dpilog_watchdog_get_table_name();

  $query = db_select($watchdog_table_name, 'w');
  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query->fields('w', array('uid'));
  $query->addField('u', 'name');
  $query->condition('w.uid', 0, '>');
  $query->distinct();
  $result = $query
    ->orderBy('name')
    ->execute();

  foreach($result as $object){
    $users[$object->uid] = $object->name;
  }
  $users[0] = t('Anonymous');
  ksort($users);

  return $users;
}

/**
 * Form constructor for the database logging filter form.
 *
 * @see dpilog_watchdog_filter_form_validate()
 * @see dpilog_watchdog_filter_form_submit()
 * @see dpilog_watchdog_overview()
 *
 * @ingroup forms
 */
function dpilog_watchdog_filter_form($form) {
  $filters = dpilog_watchdog_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter log messages'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($_SESSION['dpilog_watchdog_overview_filter']),
  );

  foreach ($filters as $key => $filter) {
    $form['filters']['status'][$key] = array(
      '#title' => $filter['title'],
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 8,
      '#options' => $filter['options'],
    );
    if (!empty($_SESSION['dpilog_watchdog_overview_filter'][$key])) {
      $form['filters']['status'][$key]['#default_value'] = $_SESSION['dpilog_watchdog_overview_filter'][$key];
    }
  }

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  if (!empty($_SESSION['dpilog_watchdog_overview_filter'])) {
    $form['filters']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

/**
 * Form validation handler for dpilog_watchdog_filter_form().
 *
 * @see dpilog_watchdog_filter_form_submit()
 */
function dpilog_watchdog_filter_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Filter')){
    $filters = dpilog_watchdog_filters();
    $filters = array_keys($filters);
    foreach($filters as $filter){
      if(!empty($form_state['values'][$filter])){
        return;
      }
    }
    form_set_error('type', t('You must select something to filter by.'));
  }
}

/**
 * Form submission handler for dpilog_watchdog_filter_form().
 *
 * @see dpilog_watchdog_filter_form_validate()
 */
function dpilog_watchdog_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = dpilog_watchdog_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['dpilog_watchdog_overview_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;
    case t('Reset'):
      $_SESSION['dpilog_watchdog_overview_filter'] = array();
      break;
  }
  return 'admin/dpi247/dpilogs/watchdogs';
}

/**
 * Form constructor for the form that clears out the log.
 *
 * @see dpilog_watchdog_clear_log_submit()
 * @ingroup forms
 */
function dpilog_watchdog_clear_log_form($form) {
  $form['dpilog_watchdog_clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear log messages'),
    '#description' => t('This will permanently remove the log messages from the database.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['dpilog_watchdog_clear']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear log messages'),
    '#submit' => array('dpilog_watchdog_clear_log_submit'),
  );

  return $form;
}

/**
 * Form submission handler for dpilog_watchdog_clear_log_form().
 */
function dpilog_watchdog_clear_log_submit() {
  $watchdog_table_name = _dpilog_watchdog_get_table_name();
  $_SESSION['dpilog_watchdog_overview_filter'] = array();
  db_delete($watchdog_table_name)->execute();
  drupal_set_message(t('All watchdogs have been cleared.'));
}

// --------------------------------------------------------------
// ------------- Affichage du détail d'un watchdog --------------
// --------------------------------------------------------------

/**
 * Page callback: Displays details about a specific database log message.
 *
 * @param int $id
 *   Unique ID of the database log message.
 *
 * @return array|string
 *   If the ID is located in the Database Logging table, a build array in the
 *   format expected by drupal_render(); otherwise, an empty string.
 *
 * @see dpilog_watchdog_menu()
 */
function dpilog_watchdog_event($id) {
  $watchdog_table_name = _dpilog_watchdog_get_table_name();
  $result = db_query('SELECT w.*, u.name, u.uid FROM {'.$watchdog_table_name.'} w INNER JOIN {users} u ON w.uid = u.uid WHERE w.wid = :id', array(':id' => $id))->fetchObject();
  $build = array();
  if ($dpiwatchdog = $result) {
    $vtab_data = array();
    // basic details
    $vtab_data[] = array(
      "title"    => "Watchdog details",
      "subtitle" => "Basic information",
      "content"  => _dpilog_watchdog_event_main_details($dpiwatchdog)
    );

    // backtrace(s)
    if($dpiwatchdog->backtrace){
      $backtrace = json_decode(gzuncompress($dpiwatchdog->backtrace),true);
      $vtab_data[] = array(
        "title"   => "Trace",
        "subtitle"=> "Simplied functions call tree",
        "content" => theme('dpilog_wathchog_backtrace', array('backtrace'=>$backtrace))
      );
      if(function_exists("kprint_r")){
        $vtab_data[] = array(
          "title"   => "Full trace",
          "subtitle"=> "Complete PHP backtrace",
          "content" => kprint_r($backtrace, true)
        );
      }
      unset($backtrace);
    }
    else {
      $vtab_data[] = array(
        "title"    => "Trace",
        "subtitle" => "PHP backtrace",
        "content"  => "No trace available"
      );
    }

    // Other watchdogs
    $vtab_data[] = array(
      "title"    => "Other watchdogs",
      "subtitle" => "for the same process",
      "content"  => _dpilog_watchdog_other_events($dpiwatchdog->processid, $dpiwatchdog->wid)
      );
    return theme('dpi_vertical_tabs', $vtab_data);
  }
  else { // No event with this ID
    return t('No data available for this watchdog.');
  }
}

/**
 * Table with main details of a watchdog
 * event.
 * @param  Array $dpiwatchdog A watchdog.
 * @return String The HTML table with all information.
 */
function _dpilog_watchdog_event_main_details($dpiwatchdog){
  if(!$dpiwatchdog)
    return '';
  $severity = watchdog_severity_levels();
  $rows = array(
    array(
      array('data' => t('Type'), 'header' => TRUE),
      t($dpiwatchdog->type),
    ),
    array(
      array('data' => t('Date'), 'header' => TRUE),
      format_date($dpiwatchdog->timestamp, 'custom', "l, F j, Y - H:i:s")
       .' <span class="help">[timestamp: '.$dpiwatchdog->timestamp.']</span>',
    ),
    array(
      array('data' => t('User'), 'header' => TRUE),
      theme('username', array('account' => $dpiwatchdog))
       . ' <span class="help">[user id: '.$dpiwatchdog->uid.']</span>',
    ),
    array(
      array('data' => t('Location'), 'header' => TRUE),
      l($dpiwatchdog->location, $dpiwatchdog->location, array('attributes'=>array('target'=>'_blank'))),
    ),
    array(
      array('data' => t('Process&nbsp;ID'), 'header' => TRUE),
      $dpiwatchdog->processid,
    ),
    array(
      array('data' => t('Referrer'), 'header' => TRUE),
      l($dpiwatchdog->referer, $dpiwatchdog->referer, array('attributes'=>array('target'=>'_blank'))),
    ),
    array(
      array('data' => t('Message'), 'header' => TRUE),
      theme('dpilog_watchdog_message', array('event' => $dpiwatchdog, 'as_link'=>FALSE)),
    ),
    array(
      array('data' => t('Severity'), 'header' => TRUE),
      '<meter optimum=0 low=4 high=4 min=0 max='.(count($severity)-1).' value='.(count($severity)-$dpiwatchdog->severity).'>'.$severity[$dpiwatchdog->severity].'</meter> '.$severity[$dpiwatchdog->severity],
    ),
    array(
      array('data' => t('Hostname'), 'header' => TRUE),
      check_plain($dpiwatchdog->hostname)
      ." <span class='help'>["
      .l("Lookup GeoIP","http://www.geoiptool.com/fr/?IP=".$dpiwatchdog->hostname,
          array('attributes'=>array(
            'target'=>'_blank',
            'title'=>'Resolve IP'
            )))
      ."]</span>"
    ),
  );
  $build['dpilog_watchdog_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#attributes' => array('class' => array('dpiwatchdog-event')),
  );
  $output_table = drupal_render($build);
  return $output_table;
}

/**
 * List of other events that occured
 * on the same page load.
 * @param  String $processid   ID of the process.
 * @param  Integer $current_wid ID of current displayed watchdog.
 * @return String HTML table with all associated watchdogs.
 */
function _dpilog_watchdog_other_events($processid, $current_wid){
  if(!$processid)
    return "";
  $watchdog_table_name = _dpilog_watchdog_get_table_name();
  $query = db_select($watchdog_table_name, 'w');
  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query->fields('w', array('wid', 'uid', 'severity', 'type', 'timestamp', 'message', 'variables'));
  $query->addField('u', 'name');
  $query->condition('processid',$processid,'=');
  $query->orderBy('timestamp');
  $result = $query->execute();
  $count=0;
  foreach ($result as $dpiwatchdog) {
    $rows[] = array('data' =>
      array(
        ++$count,
        t($dpiwatchdog->type),
        format_date($dpiwatchdog->timestamp, 'custom', "H:i:s"),
        theme('dpilog_watchdog_message', array('event' => $dpiwatchdog, 'as_link'=>TRUE, 'length'=>150)),
        theme('username', array('account' => $dpiwatchdog)),
      ),
      // Attributes for tr
      'class' => array(($dpiwatchdog->wid == $current_wid ? 'dpiwatchdog-highlight' : ''))
    );
  }
  $header = array(
    array('data' => '#'),
    array('data' => t('Type')),
    array('data' => t('Time')),
    t('Message'),
    array('data' => t('User')),
  );
  $build['dpilog_watchdog_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array(
      'id' => 'admin-dpiwatchdog',
      'class' => array('dpiwatchdog-otherprocesses')
      ),
    '#empty' => t('No log messages available.'),
  );
  return drupal_render($build);
}



// --------------------------------------------------------------
// ------------- Page de configuration des watchdogs ------------
// --------------------------------------------------------------

/**
 * Formulaire pour la configuration des
 * watchdogs
 * @return Array page formulaire
 */
function dpilog_watchdog_configure_page(){
  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['dpilog_watchdog_filters'] = array(
    '#type' => 'markup',
    '#title' => 'All filters',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#visible' => FALSE,
  );
  $settings_filters_map = _dpilog_watchdog_get_rules();

  foreach($settings_filters_map as $module_name => $module_levels_settings){
    foreach($module_levels_settings as $level_value => $level_setting){
      // Pour chaque niveau de watchdog, ajout d'une selectbox
      $form['dpilog_watchdog_filters'][$module_name][$level_value] = array(
        '#type' => 'select',
        '#title' => "Action for this event",
        '#default_value' => $level_setting,
        '#options' => _dpilog_watchdog_get_possible_actions(),
        '#description' => '',
        '#theme' => 'dpilog_watchdog_rules_settings_selectbox'
      );
    }
  }
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset everything'),
    '#description' => "You will lost all configuration setted in the table."
  );

  $form['add'] = array(
    '#type' => 'fieldset',
    '#title' => 'Add new rule',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#visible' => FALSE,
  );
  $form['add']['new_rule'] = array(
    '#type' => 'textfield',
    '#title' => t('Watchdog type / Module name'),
    '#size' => 15,
    '#required' => FALSE,
    '#description' => "Enter here a name of module or a type of watchdog. Enter XXX if you want to deal with <code>watchdog('XXX','foo...')</code>."
  );
  $form['add']['button'] = array(
    '#type' => 'submit',
    '#value' => t('Add new rule'),
    '#description' => "It will add the new type into the list."
  );
  $form["#validate"][] = 'dpilog_watchdog_rules_settings_validate';
  $form["#theme"] = 'dpilog_watchdog_rules_settings_form';
  $form["#submit"][] = 'dpilog_watchdog_rules_settings_submit';
  return $form;
}

/**
 * Hook validate for dpilog_watchdog_configure_page()
 * @param  Array $form        Defined form.
 * @param  Array $form_state  Sent form.
 * @return Boolean Form validation success.
 */
function dpilog_watchdog_rules_settings_validate($form, &$form_state){
  if(!is_array($form_state) || !isset($form_state['clicked_button']['#value'])){
    form_set_error('', 'Clicked button hasn\'t been detected');
    return FALSE;
  }
  // Different buttons = different actions
  switch($form_state['clicked_button']['#value']){
    case t("Add new rule"):
      if(!isset($form_state["values"]["add"]["new_rule"]) || trim($form_state["values"]["add"]["new_rule"]) == ''){
        form_set_error('', 'The name of the new type of watchdog is missing.');
        return FALSE;
      }
      if(trim(check_plain($form_state["values"]["add"]["new_rule"])) != $form_state["values"]["add"]["new_rule"]){
        form_set_error('', 'The new type of watchdog contains invalid characters.');
        return FALSE;
      }
      break;
    case t("Save configuration"):
    default:
      break;
  }
}

/**
 * hook submit for dpilog_watchdog_configure_page()
 * @param  Array $form        Defined form.
 * @param  Array $form_state  Sent form.
 * @return Boolean Form submit success.
 */
function dpilog_watchdog_rules_settings_submit($form, &$form_state){
  if(!is_array($form_state) || !isset($form_state['clicked_button']['#value'])){
    drupal_set_message("Form processing unavailable","error");
    return;
  }
  // Different buttons = different actions
  switch($form_state['clicked_button']['#value']){
    case t("Add new rule"):
      $new_rule = trim($form_state["values"]["add"]["new_rule"]);
      $mapping = _dpilog_watchdog_get_rules();
      if(isset($mapping[$new_rule])){
        drupal_set_message("This module name is already listed","error");
        return FALSE;
      }
      $mapping[$new_rule] = $mapping["DEFAULT"];
      variable_set("dpilog_watchdog_rules_map",$mapping);
      drupal_set_message("New module added.");
      break;
    case t("Reset everything"):
      $mapping = _dpilog_watchdog_default_rules();
      variable_set("dpilog_watchdog_rules_map",$mapping);
      break;
    case t("Save configuration"):
    default:
      if(is_array($form_state) && isset($form_state['values']['dpilog_watchdog_filters']) && is_array($form_state['values']['dpilog_watchdog_filters'])){
        variable_set("dpilog_watchdog_rules_map",$form_state['values']['dpilog_watchdog_filters']);
        drupal_set_message("The new configuration has been saved.");
      }
      else {
        drupal_set_message("An error occurred while saving wallylog configuration","error");
      }
      break;
  }
}

// --------------------------------------------------------------
// ------------- Supression d'un type de watchdog ---------------
// --------------------------------------------------------------

/**
 * Delete configuration of $module
 * @param $module {String} Module name
 */
function dpilog_watchdog_delete_rule($module=""){
  $url_config_page = 'admin/dpi247/dpilogs/configure/watchdog';
  if(trim($module) == '' || $module == "DEFAULT")
    drupal_goto($url_config_page);
  $mapping = _dpilog_watchdog_get_rules();
  unset($mapping[$module]);
  variable_set("dpilog_watchdog_rules_map",$mapping);
  drupal_set_message("The configuration for module <em>$module</em> has been deleted.");
  drupal_goto($url_config_page);
}


// --------------------------------------------------------------
// ------------- Page de configuration export -------------------
// --------------------------------------------------------------

/**
 * Form for import/export configuration.
 * @return Array The export configuration form.
 */
function dpilog_watchdog_configure_export_page(){
  $settings_filters_map = _dpilog_watchdog_get_rules();
  $settings_filters_map = json_encode($settings_filters_map);
  $form['dpilog_watchdog_message'] = array(
    '#type' => 'markup',
    '#markup' => "The serialized contents below can be copied and pasted into another DPI7 website to export its configuration.",
  );
  $form['dpilog_watchdog_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Import a watchdog configuration',
    '#required' => TRUE,
    '#tree' => TRUE,
    '#default_value' => $settings_filters_map,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import configuration'),
  );
  $form['#validate'][] = "dpilog_watchdog_configure_export_validate";
  $form['#submit'][] = "dpilog_watchdog_configure_export_submit";
  return $form;
}

/**
 * Hook validate for dpilog_watchdog_configure_export_page().
 * @param  Array $form        Defined form.
 * @param  Array $form_state  Sent form.
 * @return Boolean Form validation success.
 */
function dpilog_watchdog_configure_export_validate($form, &$form_state){
  if(!isset($form_state["values"]["dpilog_watchdog_export"])){
    form_set_error('dpilog_watchdog_export','Please, fulfill the field before submit.');
    return FALSE;
  }
  $input = $form_state["values"]["dpilog_watchdog_export"];
  if(strlen(trim($input)) < 5){
    form_set_error('dpilog_watchdog_export','Please, fulfill the field before submit.');
    return FALSE;
  }
  $input = json_decode(trim($input),TRUE);
  if(!_dpilog_watchdog_check_settings($input)){
    form_set_error('dpilog_watchdog_export','The specified field does not contain valid data.');
    return FALSE;
  }
  return TRUE;
}

/**
 * Hook submit for dpilog_watchdog_configure_export_page().
 * @param  Array $form        Defined form.
 * @param  Array $form_state  Sent form.
 * @return Boolean Form submit success.
 */
function dpilog_watchdog_configure_export_submit($form, &$form_state){
  $input = trim($form_state["values"]["dpilog_watchdog_export"]);
  $input = json_decode(trim($input),TRUE);
  variable_set("dpilog_watchdog_rules_map",$input);
  drupal_set_message("The new configuration has been imported.");
}


// --------------------------------------------------------------
// ------------- Page de configuration purge -------------------
// --------------------------------------------------------------

/**
 * Form for purge watchdogs configuration.
 * @return Array The export configuration form.
 */
function dpilog_watchdog_configure_purge_form($form=array()){
  // Clear all watchdogs button
  $form = dpilog_watchdog_clear_log_form($form);
  // Select minimal watchdogs to keep
  $form['dpilog_watchdog_row_limit'] = array(
    '#type' => 'select',
    '#title' => t('Database log messages to keep'),
    '#default_value' => dpi_variable_get('dpilog_watchdog_row_limit', 100000),
    '#options' => array(0 => t('All')) + drupal_map_assoc(array(100, 1000, 10000, 100000, 1000000))
  );
  $form['actions']['#weight'] = 1;
  return dpi_system_settings_form($form);
}