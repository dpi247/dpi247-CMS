<?php

/**
 * Returns HTML for a log message.
 *
 * @param array $variables
 *   An associative array containing:
 *   - event: An object with at least the message and variables properties.
 *   - link: (optional) Format message as link, event->wid is required.
 *
 * @ingroup themeable
 */
function theme_dpilog_watchdog_message($variables) {
  $output = '';
  $event = $variables['event'];
  $length = isset($variables['length']) ? intval($variables['length']) : 56;
  // Check for required properties.
  if (isset($event->message) && isset($event->variables)) {
    // Messages without variables or user specified text.
    if ($event->variables === 'N;') {
      $output = $event->message;
    }
    // Message to translate with injected variables.
    else {
      $output = format_string($event->message, unserialize($event->variables));
    }
    $output = filter_xss($output, array());
    $id = FALSE;
    $link = FALSE;
    if (isset($event->wid)) {
      $id = $event->wid;
      $link = 'admin/dpi247/dpilogs/watchdog/event/';
    }
    if (isset($event->lid)) {
      $id = $event->lid;
      $link = 'admin/dpi247/dpilogs/dpilog/view/log/';
    }
    if ($variables['as_link'] && $id && $link) {
      // Truncate message to 56 chars.
      $output = truncate_utf8($output, $length, TRUE, TRUE);
      $output = l($output, $link . $id, array('html' => TRUE));
    }
  }
  return $output;
}

/**
 * Theming for selectbox element
 * displayed in the dpilog watchdog
 * configuration.
 * @param  Array $variables Element properties.
 * @return String The HTML rendered element.
 */
function theme_dpilog_watchdog_rules_settings_selectbox($variables){
  $select = '';
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-select'));
  $multiple = $element['#multiple'];
  $output = '<select name="' . $element['#name'] . '' . ($multiple ? '[]' : '') . '"' . ($multiple ? ' multiple="multiple" ' : '') . drupal_attributes($element['#attributes']) . ' id="' . $element['#id'] . '">' . form_select_options($element) . '</select>';
  $output .= "\n<script type=\"text/javascript\">insertButton(\"".$element['#id']."\")</script>\n";
  return $output;
}

/**
 * Theming for configuration form.
 * @param  Array $variables Element properties.
 * @return String The HTML rendered form.
 */
function theme_dpilog_watchdog_rules_settings_form($variables){
  // The table
  $header = array_merge(array('','Watchdog type'), array_map("ucfirst",array_values(array_reverse(watchdog_severity_levels()))));
  $rows = array();
  foreach(element_children($variables['form']['dpilog_watchdog_filters']) as $key){
    $row = array();
    // Add operations
    if($key != "DEFAULT")
      $row[] = l("Delete","admin/dpi247/dpilogs/configure/watchdog/delete".$key,array("attributes"=>array("class"=>"dpiwatchdogdelete","onclick"=>"return confirm('Do you really want to delete configuration for module $key ?');")));
    else
      $row[] = "&nbsp;";
    // Add module name
    $row[] = "<strong>".$key."</strong>";
    $levels = count(watchdog_severity_levels());
    foreach(element_children($variables['form']['dpilog_watchdog_filters'][$key]) as $level_value){
      // Add elements for each choice
      if(is_int($level_value)){
        $levels--;
        $row[] = drupal_render($variables['form']['dpilog_watchdog_filters'][$key][$levels]);
      }
    }
    $rows[] = $row;
  }

  $output = theme('table', array('header'=>$header, 'rows'=>$rows, 'attributes'=>array('class' => array('fixedtable'))));
  return $output . drupal_render_children($variables["form"]);
}

/**
 * Display PHP backtrace (simple
 * and full mode)
 * @param  Array $variables Theme argument.
 * @return String The HTML rendered backtrace.
 */
function theme_dpilog_wathchog_backtrace($variables){
  if(!$variables || !is_array($variables))
    return "";
  if(!isset($variables['backtrace']) || !is_array($variables['backtrace']))
    return "";
  // simplify backtrace
  $functions_stack = array_map(function($l){return $l['function'];},$variables['backtrace']);
  
  $skip_lines = TRUE;
  $backtracel = '<ol>';
  foreach($variables['backtrace'] as $k => $v){
    if($skip_lines){
      if(in_array("_drupal_error_handler", $functions_stack)){
        if($v['function'] == '_drupal_error_handler'){ // You can't retrieve the current error handler in PHP
          $skip_lines = FALSE;
        }
        continue;
      }
      elseif(in_array("watchdog", $functions_stack)){
        if($v['function'] == 'watchdog'){ // Everything after watchdog() is useless
          $skip_lines = FALSE;
        }
        else {
          continue;
        }
      }
      else {
        $skip_lines = FALSE;
      }
    }
    if(!isset($v['file'])) $v['file'] = "";
    if(!isset($v['line'])) $v['line'] = "";
    $v['file_rel'] = str_replace(rtrim($_SERVER['DOCUMENT_ROOT'],'/'), '', $v['file']);
    $v['file_rel'] = preg_replace('/\/([^\/]+)$/', '/<strong>\1</strong>', $v['file_rel']);
    $v['custom_code'] = (strpos($v['file_rel'], 'sites/all') !== FALSE);
    $v['is_include'] = ($v['function'] == "include" || $v['function'] == "include_once" || $v['function'] == "require_once" || $v['function'] == "require");
    $v['include_file'] = "";
    if($v['is_include']) {
      $v['include_file_full'] = $v['args'][0];
      $v['include_file'] = str_replace(rtrim($_SERVER['DOCUMENT_ROOT'],'/'), '', $v['include_file_full']);
    }
    $v['complement'] = '';
    if($v['function'] == "mysql_query" && isset($v['args'][0])){
      $v['complement'] = 'Request : <span style="font-family:monospace">'.$v['args'][0].'</span>';
    }
    elseif($v['function'] == "array_key_exists"){
      $typearg0 = (isset($v['args'][0])) ? gettype($v['args'][0]) : "undefined";
      $typearg1 = (isset($v['args'][1])) ? gettype($v['args'][1]) : "undefined";
      $v['complement'] = 'Parameter #1 is '.$typearg0.', parameter #2 is '.$typearg0;
    }
    unset($v['args']);

    //Affichage du bloc :
    if($v['custom_code'])
      $backtracel .= "\t<li style='background-color:GreenYellow'>";
    else
      $backtracel .= "\t<li>";

    if($v['is_include']){
      $backtracel .= "<strong>".$v['function']."</strong>(".$v['include_file'].") called at [".$v['file_rel'].":".$v['line']."]\n";
    }
    else{
      $backtracel .= "<strong>".$v['function']."</strong>() called at [".$v['file_rel'].":".$v['line']."]\n";
    }
    if($v['complement']){
      $backtracel .= "<br/>&#8618; ".$v['complement']."\n";
    }
    $backtracel .= "</li>\n";
  }
  $backtracel .= "</ol><br/>\n";
  return $backtracel;
}
