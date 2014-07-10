<?php

/**
 * Create a zip containing a DPI XML and the related medias
 */
function _dpiexport_create_zip_from_dom($dom, $nid, $file_name, $files_to_zip = array()) {
  $return = array(
    'success' => FALSE,
    'message' => '',
    'zip' => '',
  );

  if ($export_folder = dpi_variable_get('dpiexport_admin_dest', FALSE)) {
    // Build real path
    $scheme = 'public';
    if (file_stream_wrapper_valid_scheme($scheme) && $wrapper = file_stream_wrapper_get_instance_by_uri($scheme.'://'.$export_folder)) {
      $export_folder = $wrapper->getDirectoryPath().'/'.$export_folder;

      // Create archive
      $zip = new ZipArchive;
      $zip_path = $export_folder.'/'.$file_name.'.zip';
      $res = $zip->open($zip_path, ZipArchive::CREATE);
      if ($res === TRUE) {
        $zip->addFromString($file_name.'.xml', $dom->saveXML());
        foreach ($files_to_zip as $file_to_zip) {
          // Convert stream path to real path
          if ($wrapper = file_stream_wrapper_get_instance_by_uri($file_to_zip->uri)) {
            $file_subpath = array_pop(explode('://', $file_to_zip->uri));
            $file_path = $wrapper->getDirectoryPath().'/'.$file_subpath;
            $zip->addFile($file_path, $file_to_zip->filename);
          }
        }
        $zip->close();

        $return['success'] = TRUE;
        $return['message'] = t('Zip success for node ID : ').$nid;
        $return['zip'] = $zip_path;
      } else {
        $return['message'] = t('Zip failure for node ID : ').$nid;
      }
    } else {
      $return['message'] = t('Unable to validate output directory.');
    }
  } else {
    $return['message'] = t('The export path is not set.');
  }

  return $return;
}

/**
 * Convert the local paths URIs to absolute paths
 */
function _dpiexport_convert_uris_to_absolute(&$dom) {
  // Replace URI elements with absolute URLs
  foreach ($dom->getElementsByTagName('URI') as $uri_node) {
    // Get local URI
    $local_uri = $uri_node->nodeValue;
    if ($wrapper = file_stream_wrapper_get_instance_by_uri($local_uri)) {
      // Set the absolute path
      $uri_node->nodeValue = $wrapper->getExternalUrl();
    }
  }

  // Replace URI tags with absolute URLs
  foreach ($dom->getElementsByTagName('Thumbnail') as $thumbnail_node) {
    // Get local URI
    if ($thumbnail_node->hasAttribute('URI')) {
      $local_uri = $thumbnail_node->getAttribute('URI');
      if ($wrapper = file_stream_wrapper_get_instance_by_uri($local_uri)) {
        // Set the absolute path
        $thumbnail_node->setAttribute('URI', $wrapper->getExternalUrl());
      }
    }
  }
}

/**
 * Convert the local paths URIs to relative paths
 */
function _dpiexport_convert_uris_to_relative(&$dom) {
  // Replace URI elements with absolute URLs
  foreach ($dom->getElementsByTagName('URI') as $uri_node) {
    $local_uri = $uri_node->nodeValue;
    // Get the file name
    $expl_uri = explode('/', $local_uri);
    if (!empty($expl_uri)) {
      if ($file_name = array_pop($expl_uri)) {
        // Set the relative path
        $uri_node->nodeValue = 'embed://'.$file_name;
      }
    }
  }

  // Replace URI tags with absolute URLs
  foreach ($dom->getElementsByTagName('Thumbnail') as $thumbnail_node) {
    if ($thumbnail_node->hasAttribute('URI')) {
      $local_uri = $thumbnail_node->getAttribute('URI');
      // Get the file name
      $expl_uri = explode('/', $local_uri);
      if (!empty($expl_uri)) {
        if ($file_name = array_pop($expl_uri)) {
          // Set the relative path
          $thumbnail_node->setAttribute('URI', 'embed://'.$file_name);
        }
      }
    }
  }
}
