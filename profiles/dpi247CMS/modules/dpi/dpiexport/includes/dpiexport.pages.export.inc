<?php

/**
 * Get an exported package as DPI XML
 */
function dpiexport_page_export_package($node) {
  $results = dpiexport_api_export_packages(array($node), FALSE);
  $result = array_shift($results);

  if ($result['success']) {
    // Add XML header
    drupal_add_http_header('Content-Type', 'text/xml');

    // Format output
    module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
    $dom = $result['dom'];
    print dpicommons_format_xml_string($dom->saveXML());
    return NULL;
  }
  else {
    drupal_set_message($result['message'], 'error');
    drupal_goto('node/'.$node->nid);
  }

  return '';
}

/**
 * Get an exported zipped package with its related medias
 */
function dpiexport_page_export_zipped_package($node) {
  $results = dpiexport_api_export_packages(array($node));
  $result = array_shift($results);

  // Manage output
  $output = '';
  if ($result['success']) {
    $message_type = 'status';
    // Get the node title for more readability
    $node_wrapper = entity_metadata_wrapper('node', $node);
    if (isset($node_wrapper->field_displaytitle) && $field_value = $node_wrapper->field_displaytitle->value()) {
      $output_title = filter_xss($field_value['safe_value']);
    } else {
      $output_title = $node->title;
    }
    // Direct download link. It could be nice to start a download automatically, but it's a start..
    $output .= t('Node').' '.$node->nid.' - '.$output_title.' : '.l(t('Download ZIP'), $result['zip']);
  } else {
    $message_type = 'error';
  }
  drupal_set_message($result['message'], $message_type);

  return $output;
}
