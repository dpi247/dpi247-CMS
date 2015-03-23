<?php

function dpipressfilters_form_config_import_export_form($form_state) {
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'dpipressfilters').'/scripts/export_config.js' => array('type' => 'file')
  );
  
  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $filters = dpipressfilters_getfilters();
  $filters_options = array();
  foreach ($filters as $filter) {
    $filters_options[$filter->fid] = $filter->name;
  }
  $form['export']['select_filters'] = array(
    '#type' => 'radios',
    '#title' => t('Select a filter to export'),
    '#options' => $filters_options,
  );
  
  $form['export']['submit'] = array(
    '#type' => 'markup',
    '#markup' => '<input type="button" id="export-submit" value="'.t('Export').'"  class="form-submit" />',
  );
  
  $form['export']['export_area'] = array(
    '#type' => 'textarea',
    '#title' => t('Exported XML'),
    '#id' => 'export-area',
    '#prefix' => '<br><img id="export-loading" src="/'.drupal_get_path('module', 'dpipressfilters').'/images/export-loading.gif" />',
  );
  
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['import']['import_area'] = array(
    '#type' => 'textarea',
    '#title' => t('XML'),
    '#description' => t('Paste XML here.'),
    '#required' => TRUE,
  );

  $form['import']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#submit' => array('dpipressfilters_form_config_import_export_form_import_submit'),
  );
  
  return $form;
}

function dpipressfilters_form_config_import_export_form_import_submit($form, $form_state) {
  $values = $form_state['values'];
  $import_text = $values['import_area'];
  
  $dom = new DomDocument('1.0', 'UTF-8');
  if ($dom->loadXML(trim($import_text))) {
    _dpipressfilters_import_export_import_from_dom($dom);
    drupal_set_message(t('Import done'));
  } else {
    drupal_set_message(t('No XML to import or XML unreadable.'), 'error');
  }
}

/**
 * Get current settings for all formats which have
 * this filter enabled.
 * @param  {String} $filter Name of the filter.
 * @return {Array}          Keys are formats names.
 *                          Values are settings.
 */
function dpipressfilters_get_formats_config($filter){
  $all_configs = array();
  // list all all available format
  $available_formats = array_keys(filter_formats());
  foreach($available_formats as $format){
    // extract current settings for all formats where filter is enabled.
    $format_info = filter_list_format($format);
    if(empty($format_info[$filter]))
      continue;
    $filter_settings = $format_info[$filter];
    if(!isset($filter_settings->status) || !$filter_settings->status)
      continue;
    $all_configs[$format] = $filter_settings->settings;
  }
  return $all_configs;
}


/**
 * Set new settings for an input format.
 * @param  {String} $filter    Name of the filter.
 * @param  {String} $format    Name of the text/input format.
 * @param  {Array}  $settings  Settings for this filter.
 * @return {Boolean}           Success or failure.
 */
function dpipressfilters_set_format_config($filter, $format, $settings){
  if(!is_array($settings) || empty($settings))
    return FALSE;
  $available_formats = array_keys(filter_formats());
  if(!in_array($format, $available_formats))
    return FALSE;
  // Retrieve current info
  $current_format = filter_list_format($format);
  $current_filter_format = empty($current_format[$filter]) ? FALSE : $current_format[$filter];

  // Insert or update value
  $r = db_merge('filter')
      ->key(array('format' => $format, 'name' => $filter))
      ->fields(array(
          "format" => $format,
          "module" => 'dpipressfilters',
          "name" => $filter,
          "weight" => ($current_filter_format ? $current_filter_format->weight : 0),
          "status" => 1,
          "settings" => serialize($settings)
      ))
      ->execute();
  return !!$r;
}



function dpipressfilters_config_export($fid) {
  $filter = dpipressfilters_getfilterbyid($fid);
  $tags = dpipressfilters_getalltagsfromfilter($filter->name);
  $dom = _dpipressfilters_import_export_generate_dom($filter, $tags);
  
  module_load_include('inc', 'dpitoolbox', 'includes/dpitoolbox.helpers');
  print $dom->saveXML();
}

function _dpipressfilters_import_export_generate_dom($filter, $tags) {
  $dom = new DomDocument('1.0', 'UTF-8');
  $dom_filter = $dom->appendChild($dom->createElement('Filter'));
  $dom_filter->setAttribute('fid', $filter->fid);
  $dom_name = $dom_filter->appendChild($dom->createElement('Name', $filter->name));
  $dom_description = $dom_filter->appendChild($dom->createElement('Description', $filter->description));
  $dom_tags = $dom_filter->appendChild($dom->createElement('Tags'));
  $formats_settings = dpipressfilters_get_formats_config($filter->name);

  foreach ($tags as $tag) {
    $dom_tag = $dom_tags->appendChild($dom->createElement('Tag'));
    $dom_tag->setAttribute('tid', $tag->tid);
    $dom_tag->appendChild($dom->createElement('Format', is_null($tag->format) ? 0 : $tag->format));
    $dom_tag->appendChild($dom->createElement('Type', $tag->type));
    $dom_tag->appendChild($dom->createElement('TagName', $tag->tag));
    $dom_tag->appendChild($dom->createElement('TagDescription', $tag->description));
    /*$settings = $dom->createCDATASection(serialize($tag->settings));
    $dom_settings = $dom_tag->appendChild($dom->createElement('Settings'));
    $dom_settings->appendChild($settings);*/
    $dom_tag->appendChild($dom->createElement('Settings', serialize($tag->settings)));
  }
  $fsettings = $dom->createCDATASection(serialize($formats_settings));
  $dom_fsettings = $dom->createElement('FormatsSettings');
  $dom_fsettings->appendChild($fsettings);
  $dom_filter->appendChild($dom_fsettings);
  
  return $dom;
}

function _dpipressfilters_import_export_import_from_dom($dom) {
  $filter = new stdClass();
  $dom_filter = $dom->getElementsByTagName('Filter')->item(0);
  $filter->name = $dom_filter->getElementsByTagName('Name')->item(0)->nodeValue;
  $filter->description = $dom_filter->getElementsByTagName('Description')->item(0)->nodeValue;

  if ($orig_filter = dpipressfilters_getfilterbyname($filter->name)) {
    $filter->fid = $orig_filter->fid;
    drupal_write_record('dpipressfilters', $filter, array('fid'));
    db_delete('dpipressfilters_tags')
      ->condition('filter',$filter->name)
      ->execute();
  } else {
    drupal_write_record('dpipressfilters', $filter);
  }

  $mapping_tids = array();

  $dom_tags = $dom->getElementsByTagName('Tags');
  if ($dom_tags->item(0)) {
    $dom_tag = $dom_tags->item(0)->getElementsByTagName('Tag');
    if ($dom_tag->item(0)) {
      foreach ($dom_tag as $tag) {
        $new_tag = new stdClass();
        $new_tag->filter = $filter->name;
        $format = $tag->getElementsByTagName('Format')->item(0)->nodeValue;
        $new_tag->format = ($format == 0) ? NULL : $format;
        $new_tag->type = $tag->getElementsByTagName('Type')->item(0)->nodeValue;
        $new_tag->tag = $tag->getElementsByTagName('TagName')->item(0)->nodeValue;
        $new_tag->description = $tag->getElementsByTagName('TagDescription')->item(0)->nodeValue;
        $new_tag->settings = unserialize($tag->getElementsByTagName('Settings')->item(0)->nodeValue);
        drupal_write_record('dpipressfilters_tags', $new_tag);
        $mapping_tids[$new_tag->tag] = $new_tag->tid;
      }
    }
  }
  unset($dom_tags);

  $dom_fsettings = $dom->getElementsByTagName('FormatsSettings');
  if ($dom_fsettings->item(0)) {
    $fsettings_serialiazed = $dom_fsettings->item(0)->nodeValue;
    unset($dom_fsettings);
    $fsettings = unserialize($fsettings_serialiazed);
    if(!$fsettings){
      drupal_set_message("Settings for text formats can't be imported. You must ".l("configure tags behaviour for all text formats", 'admin/config/content/formats').".",'warning');
      return;
    }
    foreach($fsettings as $input_format => $settings){
      // Convert old tag ID to new tag ID
      $new_settings = array();
      foreach($settings as $old_tid => $conf){
        $new_tid = $mapping_tids[$conf['tagname']];
        $new_settings[$new_tid] = $conf;
      }
      // Save settings
      dpipressfilters_set_format_config($filter->name, $input_format, $new_settings);
    }
  }

  // Clear cache
  cache_clear_all('filter_list_format','cache');
}
