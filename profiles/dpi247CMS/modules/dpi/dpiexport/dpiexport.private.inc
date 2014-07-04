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
          $zip->addFile($file_to_zip['filepath'], $file_to_zip['filename']);
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
