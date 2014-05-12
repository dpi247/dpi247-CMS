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
function _dpidam_postpackage($nid, $set_message = FALSE) {
  $result = array(
    'success' => FALSE,
    'message' => '',
  );

  // Export package
  $export_result = dpiexport_api_export_entities(array($nid));
  if ($export_result[0]['success'] == TRUE) {
    // Send package in POST
    $target_url = dpi_variable_get('dpidam_updatepackageurl', '');
    if ($target_url != '') {
      $path_to_file = $export_result[0]['zip'];
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
