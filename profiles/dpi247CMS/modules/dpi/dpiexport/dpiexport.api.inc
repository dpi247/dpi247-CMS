<?php

/**
 * Generate DPI XML for given node IDs. It is also possible to give an array of nodes.
 * By default the resulting XMLs will be zipped, with the related media contents.
 * It is also possible to return only the DOM objects and a list of path for the media contents.
 *
 * @param array $nids
 *   An array of node IDs, or nodes.
 * @param bool $zip = TRUE
 *   Return the zipped XMLs or only the DOM objects
 *
 * @param array
 *   An array of structured results logs and contents
 */
function dpiexport_api_export_packages($nids, $zip = TRUE) {
  $return = array();
  foreach ($nids as $nid) {
    if (is_numeric($nid)) {
      $node = node_load($nid);
    } else {
      $node = $nid;
    }

    // Generate DOM
    module_load_include('inc', 'dpiexport', 'includes/dpiexport.export');
    $files_to_zip = array();
    if ($dom = dpiexport_node_to_dpixml($node, $files_to_zip)) {
      if ($zip) {
        // Generate ZIP from DOM and files
        $dpiproperties = dpiproperties_load_by_type_and_id('node', $node->nid);
        $zip_name = 'DPI_export_'.$node->nid.'_'.$dpiproperties->external_reference;
        $return[$node->nid] = _dpiexport_create_zip_from_dom($dom, $node->nid, $zip_name, $files_to_zip);
      }
      else {
        // Return the DOM object and media contents
        $return[$node->nid] = array(
          'success' => TRUE,
          'message' => '',
          'dom' => $dom,
          'files' => $files_to_zip,
        );
      }
    }
    else {
      $return[$node->nid] = array(
        'success' => FALSE,
        'message' => t('DOM generation failure for node ID : ').$node->nid,
      );
    }
  }
  return $return;
}
