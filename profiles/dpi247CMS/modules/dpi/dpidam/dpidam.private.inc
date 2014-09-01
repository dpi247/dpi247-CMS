<?php

/**
 * Process an HTTP POST
 *
 * @param {array} $postfields
 *   Fields to post (in array)
 * @param {string} $target_url
 *   URL to call
 * @param {string} $content_type
 *   Set the HTTP header "Content-Type"
 * @param {bool} $raw
 *   POST raw fields or not
 * @param {bool} $return_result
 *   Choose to return a boolean or the result of the POST
 * @param {bool} $set_message
 *   Add messages during the POST process
 * 
 * @return {string}
 *   Result of the POST
 */
function _dpidam_post($postfields, $target_url, $content_type = NULL, $raw = FALSE, $return_result = FALSE, $set_message = FALSE) {
  //Envoi un fichier en post.
  $ch = curl_init($target_url);

  if ($content_type != NULL) {
    curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: ".$content_type));
  }
  if ($raw) {
    curl_setopt($ch, CURLOPT_UPLOAD, 1);
    curl_setopt($ch, CURLOPT_INFILE, $postfields);
  } else {
    curl_setopt($ch, CURLOPT_POSTFIELDS, $postfields);
  }

  if ($return_result) {
    ob_start();
  }

  $result = curl_exec($ch);

  $http_status = curl_getinfo($ch, CURLINFO_HTTP_CODE);

  if ($http_status != 200) {
    $return = FALSE;
    if ($set_message) {
      drupal_set_message(curl_error($ch), 'error');
    }
  } else {
    if ($set_message) {
      drupal_set_message($result);
    }
    $return = TRUE;
  }

  curl_close($ch);

  if ($return && $return_result) {
    $return = ob_get_contents();
    ob_end_clean();
  }

  return $return;
}

/**
 * POST the package of the node to pandam
 *
 * @param {object} $node
 *   Node
 */
function _dpidam_post_package($nid, $set_message = FALSE) {
  $result = array(
    'success' => FALSE,
    'message' => '',
  );

  // Export package
  $export_result = dpiexport_api_export_packages(array($nid));
  if ($export_result[$nid]['success'] == TRUE) {
    // Send package in POST
    $target_url = dpi_variable_get('dpidam_updatepackageurl', '');
    if ($target_url != '') {
      $path_to_file = $export_result[$nid]['zip'];
      $fp = fopen($path_to_file, 'r');
      if ($fp) {
        $content_type = 'application/zip';

        if(_dpidam_post($fp, $target_url, $content_type, TRUE)) {
          $result['success'] = TRUE;
        } else {
          $result['message'] = t('Couldn\'t send package to DAM, POST failed');
        }
      } else {
        $result['message'] = t('Couldn\'t open zip file to produce POST stream');
      }
    } else {
      $result['message'] = t('The URL where the package is being posted is not specified');
    }
  } else {
    $result['message'] = t('The Export of the package (NID : @nid) failed. Export message : "@export_message".', array('@nid' => $nid, '@export_message' => $export_result['message']));
  }

  if (!$result['success'] && !empty($result['message']) && $set_message) {
    drupal_set_message($result['message'], 'error');
  }

  return $result;
}

/**
 * Send the informations of the node (nodeid, packageid and url) to pandam
 *
 * @param {object} $node
 *   Node
 */
function _dpidam_post_url($node, $node_url, $set_message = FALSE) {
  $result = array(
    'success' => FALSE,
    'message' => '',
  );

  // Send the URL in POST
  $target_url = dpi_variable_get('dpidam_updateurl', '');
  if ($target_url != '') {
    $dpiproperties = dpiproperties_load_by_type_and_id('node', $node->nid);
    $external_reference = $dpiproperties->external_reference;
    if ($external_reference) {
      $postfields = array('commonId' => $external_reference, 'url' => $node_url, 'date' => time());
      if (_dpidam_post($postfields, $target_url)) {
        $result['success'] = TRUE;
      } else {
        $result['message'] = t('Couldn\'t send URL to DAM, POST failed.');
      }
    } else {
      $result['message'] = t('No external reference in the given node (nid : @nid).', array('@nid' => $node->nid));
    }
  } else {
    $result['message'] = t('The URL where the package is being posted is not specified.');
  }

  if (!$result['success'] && !empty($result['message']) && $set_message) {
    drupal_set_message($result['message'], 'error');
  }

  return $result;
}

/**
 * Add an URL to the buffer, each time the cron is running, the URL of the NID who are in the buffer will be sent to the DAM
 *
 * @param {int} $nid
 *   Nid of the node
 */
function _dpidam_add_url_to_buffer($nid, $url) {
  $row = new stdClass();
  $row->nid = $nid;
  $url = url($url, array('alias' => TRUE, 'absolute' => TRUE));
  // Allow to alter the URL to handle canonical URLs
  drupal_alter('dpidam_synchro_url', $url);
  $row->url = $url;
  $row->timestamp = time();
  drupal_write_record('dpidam_urltosend', $row);
}

/**
 * Return the source configuration form source name
 *
 * @param {string} $source
 */
function _dpidam_get_source_by_name($source) {
  $return = NULL;
  $db_result = db_select('dpidam_source', 'ds')
    ->fields('ds')
    ->condition('ds.name', $source)
    ->execute();
  foreach ($db_result as $obj) {
    $return = new stdClass;
    $return->sid = (int)$obj->sid;
    $return->name = $obj->name;
    $return->archive = (int)$obj->archive;
    $return->archive_from_dpimport = (int)$obj->archive_from_dpimport;
    break;
  }
  return $return;
}

/**
 * Add a package to the buffer, each time the cron is running, the packages who are in the buffer will be sent to the DAM
 *
 * @param {int} $nid
 *   Nid of the node
 */
function _dpidam_add_package_to_buffer($nid) {
  $result = db_select('dpidam_packagestosend', 'dp')
    ->fields('dp', array('nid'))
    ->condition('dp.nid', $nid)
    ->execute();
  if (!$result) {
    $row = new stdClass();
    $row->nid = (int)$nid;
    $row->timestamp = time();
    drupal_write_record('dpidam_packagestosend', $row);
  }
}

/**
 * Remove a package from the buffer
 *
 * @param {int} $nid
 *   Nid of the node
 */
function _dpidam_remove_package_from_buffer($nid) {
  db_delete('dpidam_packagestosend')
    ->condition('nid', $nid)
    ->execute();
}

/**
 * Remove an url from the buffer
 *
 * @param {int} $nid
 *   Nid of the node
 */
function _dpidam_remove_url_from_buffer($nid) {
  db_delete('dpidam_urltosend')
    ->condition('nid', $nid)
    ->execute();
}

/**
 * Save a static node to be able to add it as a variable in the page template preprocess
 */
function _dpidam_save_static_node($node = NULL) {
  static $saved_node = NULL;

  if ($node) {
    $saved_node = $node;
  }

  return $saved_node;
}

/**
 * Calls the DAM module to get an image ressource and stores it locally
 *
 * @param {string} $id
 *   ressource dam id (pattern : d-yyyymmdd-aaaaaa)
 * @param {string} $size
 *   size of the ressource : high or thS (thumbnail)
 */
function _dpidam_get_dam_image($id, $size) {
  $dam_url = dpi_variable_get('dpidam_search_thumbnails_URL', '');
  $dam_image_subdirectory = dpi_variable_get('dpidam_image_subdirectory', '');
  $ressource = $id.'_'.$size;

  $dam_file = NULL;

  if(preg_match('/^[d]-(\d{8})-[A-Z0-9]{6}_(high|thS|thM)/', $ressource)) {
    $dam_url .= '/'.$ressource.'.jpg';
    $dam_file = dpicontenttypes_api_create_or_update_file($dam_url, $dam_image_subdirectory, 'public', 'dpidam');
  }

  return $dam_file;
}
