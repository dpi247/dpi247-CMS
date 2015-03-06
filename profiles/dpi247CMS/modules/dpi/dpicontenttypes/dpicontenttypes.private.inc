<?php

/**
 * Remove external files older than a given age (7 days by default)
 */
function _dpicontenttypes_cron_remove_external_files() {
  // Flush old files once a day
  $today = strtotime(date('Y-m-d'));
  $last_run_timestamp = dpi_variable_get('dpicontenttypes_cron_last_run', time() - 86400);
  if ($last_run_timestamp < $today) {
    dpi_variable_set('dpicontenttypes_cron_last_run', $today);

    $days_limit = dpi_variable_get('dpicontenttypes_files_days_limit', 7);
    $old_timestamp = time() - $days_limit*86400;

    $files_to_delete_query = db_select('dpicontenttypes_external_files', 'def');
    $files_to_delete_query->leftJoin('file_managed', 'fm', 'def.fid = fm.fid');
    $files_to_delete_results = $files_to_delete_query->condition('def.cron_delete', 1)
      ->condition('fm.timestamp', $old_timestamp, '<')
      ->fields('fm')
      ->execute();
    $files_to_delete_count = $files_to_delete_results->rowCount();

    if ($files_to_delete_count) {
      foreach ($files_to_delete_results as $file_to_delete) {
        file_delete($file_to_delete);
        $delete_query = db_delete('dpicontenttypes_external_files')
          ->condition('fid', $file_to_delete->fid)
          ->execute();
      }
    }
  }
}

/**
 * Create a file from URI
 *
 * @param string $file_uri
 * @param string $destination
 *
 * @return object $file
 */
function _dpicontenttypes_create_file_from_uri($file_uri, $destination) {
  if (empty($file_uri) || empty($destination)) {
    return NULL;
  }

  // @todo : Add logs

  $uri = parse_url($file_uri);
  $file = NULL;

  if ($uri && isset($uri['scheme'])) {
    $badcar = array('+', ' ', '%20', '%28', '%29');
    $destination = str_replace($badcar, '_', rtrim($destination, '/'));
    $destination .= '/';
    switch ($uri['scheme']){
      case 'http':
        module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
        if (isset($uri['path']) && !empty($uri['path']) && dpicommons_toboolean($uri['path'])) {
          $expl_path = explode('/', $uri['path']);
          $file_name = $expl_path[count($expl_path)-1];
          $data = drupal_http_request($file_uri, array('timeout' => _dpimport_variable_get('dpicommons_http_request_timeout', 3)));
          if (dpicommons_is_http_request_successfull($data)) {
            $new_headers = array();
            foreach ($data->headers as $key => $value) {
              $new_headers[strtolower($key)] = $value;
            }
            $data->headers = $new_headers;
            if (isset($data->headers['content-type'])) {
              list($file_type, $file_ext) = explode('/', $data->headers['content-type']);
              $current_ext = pathinfo($file_name, PATHINFO_EXTENSION);
              if (empty($current_ext)) {
                $file_name = $file_name.'.'.$file_ext;
              }
              $good_file_name = str_replace($badcar, '_', $file_name);
              if (_dpicontenttypes_file_check_directory_recursive($destination, 1)) {
                $file = file_save_data($data->data, $destination.$file_name);
              }
            }
          } else {
            Logger::logWarning('importer', 2, 'fileupload', t('The file (!file) can not be uploaded', array('!file' => $uri['host'])));
          }
          break;
        }
    }
  }

  return $file;
}



function _dpicontenttypes_form_validate_sync_title_with_display_title(&$element, &$form_state, $form){
  $language=$form['language']['#value'];
  if(!isset($language) or $language==''){
    $language='und';
  }
  if(isset($form_state['values']['field_displaytitle'][$language][0]['value'])){
    $strip = $form_state['values']['field_displaytitle'][$language][0]['value'];
  }else{
    $strip = $form_state['values']['field_displaytitle']['und'][0]['value'];
  }
  $form_state['values']['title']=strip_tags($strip);
}

/**
 * Checks whether a directory exists and is writable.
 *
 * Furthermore, the directory can optionally be created if it does not exist,
 * and/or be set to writable if it is currently not. Directories need to have
 * execute permission to be considered a directory by FTP servers.
 *
 * @param $directory
 *   A string representing the directory path.
 * @param $mode
 *   An optional bitmask containing the actions, if any, to be carried out on
 *   the directory. Any combination of the actions FILE_CREATE_DIRECTORY and
 *   FILE_MODIFY_PERMISSIONS is allowed.
 * @param $form_item
 *   An optional string containing the name of a form item that any errors
 *   will be attached to. Useful when the function validates a directory path
 *   entered as a form value. An error will consequently prevent form submit
 *   handlers from running, and instead display the form along with the
 *   error messages.
 *
 * @return
 *   FALSE if the directory does not exist or is not writable, even after
 *   any optional actions have been carried out. Otherwise, TRUE is returned.
 */
function _dpicontenttypes_file_check_directory_recursive($directory, $mode = 0, $form_item = NULL) {
  $directory = rtrim($directory, '/\\');

  if ($wrapper = file_stream_wrapper_get_instance_by_uri($directory)) {
    list($scheme, $target) = explode('://', $directory, 2);
    // Remove erroneous leading or trailing, forward-slashes and backslashes.
    $target = trim($target, '\/');
    $real_path = $wrapper->getDirectoryPath() . '/' . $target;

    // Check if directory exists.
    if (!is_dir($real_path)) {
      if (($mode & FILE_CREATE_DIRECTORY) && @mkdir($real_path, 0775, TRUE)) {
        drupal_set_message(t('The directory %directory has been created.', array('%directory' => $real_path)));
        @chmod($real_path, 0775); // Necessary for non-webserver users.
      }
      else {
        if ($form_item) {
          form_set_error($form_item, t('The directory %directory does not exist.', array('%directory' => $real_path)));
        }
        return FALSE;
      }
    }

    // Check to see if the directory is writable.
    if (!is_writable($real_path)) {
      if (($mode & FILE_MODIFY_PERMISSIONS) && @chmod($real_path, 0775)) {
        drupal_set_message(t('The permissions of directory %directory have been changed to make it writable.', array('%directory' => $real_path)));
      }
      else {
        if ($form_item) {
          form_set_error($form_item, t('The directory %directory is not writable', array('%directory' => $real_path)));
        }
        watchdog('file system', 'The directory %directory is not writable, because it does not have the correct permissions set.', array('%directory' => $real_path), WATCHDOG_ERROR);
        return FALSE;
      }
    }
  } else {
    return FALSE;
  }

  return TRUE;
}

/**
 * Insert dpiproperties into the database
 * 
 * @param Object $object
 * @param String $entity
 * 
 * @param Interger $eid
 */
function _dpicontenttypes_entityinsertdpiproperties($object, $entity, $eid) {
  //Create the dpiproperties entry corresponding to the node  
  if (!isset($object->dpiproperties)){
    $dpiproperties = new DPIPropertiesEntity(array(), 'dpiproperties');
  } else {
    $dpiproperties = $object->dpiproperties;
  }
  $dpiproperties->type = isset($dpiproperties->type) ? $dpiproperties->type : 'default';
  $dpiproperties->id = $eid;
  $dpiproperties->entity = $entity;
  $dpiproperties->external_reference = $dpiproperties->external_reference ? $dpiproperties->external_reference : dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand();
  $dpiproperties->external_reference_2 = $dpiproperties->external_reference_2 ? $dpiproperties->external_reference_2 : dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand();
  $dpiproperties->source = $dpiproperties->source ? $dpiproperties->source : 'dpi247';
  $dpiproperties->product_id = $dpiproperties->product_id ? $dpiproperties->product_id : dpi_variable_get('dpicommons_product', '');
  dpiproperties_save($dpiproperties);
}

function _dpicontenttypes_entityinsertscaldatom($node){
  switch ($node->type){
    case 'package':
        $atom = new ScaldAtom('atom_package', 'atom_package', array(          
          'title' => $node->title,
          'data' => array(
            'nid' => $node->nid,
            'author' => (isset($node->field_authors))?$node->field_authors: array(),
          ),
        ));
        if($atomId = scald_atom_save($atom)){
          drupal_set_message('Atom package '.$atomId.' created !');
        }else{
          drupal_set_message('Atom package not created !', 'error');
        }
      break;
    case 'package_gallery':
        $atom = new ScaldAtom('atom_package', 'atom_package_gallery', array(
          'title' => $node->title,
          'data' => array(
            'nid' => $node->nid,
            'author' => (isset($node->field_authors))?$node->field_authors: array(),
          ),
        ));
        if($atomId = scald_atom_save($atom)){
          drupal_set_message('Atom package gallery '.$atomId.' created !');
        }else{
          drupal_set_message('Atom package gallery not created !', 'error');
        }
      break;
  }
}

/**
 * Insertion des informations d'embargo dans la table en fonction des informations présente dans le noeud.
 * @param stdClass $node
 */
function _dpicontenttypes_entityinsertembargoinformation($node){
  $useautopublishdates = field_get_items('node', $node, 'field_autopublish');
  $useautopublishdate = current($useautopublishdates);
  $autopublishdates = field_get_items('node', $node, 'field_autopublishdate');
  $autopublishdate = current($autopublishdates);
  
  if ($useautopublishdate) {        
    $infos = new stdClass();
    $infos->nid = $node->nid;
    $infos->status = $node->status;
    
    if ( $autopublishdate['value'] != NULL && $node->status == 0 && intval($autopublishdate['value']) > time()){
      $infos->date = $autopublishdate['value'];
      $infos->action = 1;
      $infos->timezone = $autopublishdate['timezone'];    
      dpicontenttypes_api_insert_autopublish($infos); 
    }

    if ($autopublishdate['value2'] != NULL && intval($autopublishdate['value2']) > time()){
      $infos->action = 0;
      $infos->date = $autopublishdate['value2'];
      dpicontenttypes_api_insert_autopublish($infos);
    }
  }
}

/**
 * Delete dpiproperties from the database
 * 
 * @param Object $object
 * @param String $entity
 * 
 * @param Integer $eid
 */
function _dpicontenttypes_entitydeletedpiproperties($object, $entity, $eid) {
  $condition = db_and()->condition('id', $eid)->condition('entity', $entity);
  db_delete('dpicontenttypes_properties')->condition($condition)->execute();
}

function _dpicontenttypes_entitydeletescaldatom($object, $entity, $nid){
  db_delete('scald_atoms')->condition('title', $object->title)->execute();
}

/**
 * Delete des informations d'embargo dans la table en fonction des informations présente dans le noeud.
 * @param stdClass $node
 */
function _dpicontenttypes_entitydeleteembargoinformation($node){  
  dpicontenttypes_api_delete_autopublish($node);
}

/**
 * Returns the URI of an image when using a style.
 *
 * Fork of the image_style_path function to change the base directory to "dpistyles" and add an entity ID as subdirectory
 *
 * The path returned by this function may not exist. The default generation
 * method only creates images when they are requested by a user's browser.
 *
 * @param $style_name
 *   The name of the style to be used with this image.
 * @param $uri
 *   The URI or path to the image.
 *
 * @return
 *   The URI to an image style image.
 *
 * @see image_style_url()
 */
function _dpicontenttypes_image_style_path($style_name, $entity_id, $atom_id, $uri) {
  $scheme = file_uri_scheme($uri);
  if ($scheme) {
    $path = file_uri_target($uri);
  }
  else {
    $path = $uri;
    $scheme = file_default_scheme();
  }
  return $scheme . '://dpistyles/' . $style_name . '/' . $entity_id . '/' . $atom_id . '/' . $scheme . '/' . $path;
}

/**
 * Get cropings values from DB for a given entity
 *
 * @param String $entity_type
 *   Entity type (i.e. "node")
 * @param Int $eid
 *   Entity ID (a node ID for example)
 *
 * @return Array
 *   The cropings values
 */
function _dpicontenttypes_get_saved_cropings_for_entity($entity_type, $eid) {
  // Get saved cropings values from DB
  $db_cropings = db_select('dpicontenttypes_cropings', 'dpic')
    ->fields('dpic')
    ->condition('dpic.etype', $entity_type)
    ->condition('dpic.eid', $eid)
    ->execute();
  $existing_cropings = array();
  foreach ($db_cropings as $db_croping) {
    $existing_cropings[$db_croping->aid] = unserialize($db_croping->cropings);
  }
  return $existing_cropings;
}

/**
 * Get cropings values from DB for a given entity and atom
 *
 * @param String $entity_type
 *   Entity type (i.e. "node")
 * @param Int $eid
 *   Entity ID (a node ID for example)
 * @param Int $aid
 *   Atom ID
 * @param String $style = ''
 *   Return only the values for a given style
 *
 * @return Array
 *   The cropings values
 */
function _dpicontenttypes_get_saved_cropings_for_entity_and_atom($entity_type, $eid, $aid, $style_name = '') {
  // Get saved cropings values from DB
  $db_cropings = db_select('dpicontenttypes_cropings', 'dpic')
    ->fields('dpic')
    ->condition('dpic.etype', $entity_type)
    ->condition('dpic.eid', $eid)
    ->condition('dpic.aid', $aid)
    ->execute();
  $existing_cropings = FALSE;
  foreach ($db_cropings as $db_croping) {
    $existing_cropings = unserialize($db_croping->cropings);
    break;
  }
  $existing_cropings = $existing_cropings ? $existing_cropings : array();

  if ($style_name) {
    return (isset($existing_cropings[$style_name]) && is_array($existing_cropings[$style_name]) && !empty($existing_cropings[$style_name])) ?
      $existing_cropings[$style_name] : FALSE;
  } else {
    return (is_array($existing_cropings) && !empty($existing_cropings)) ? $existing_cropings : FALSE;
  }
}

/**
 * Update croping informations.
 *
 * If cropings are given, set these values for the given entity_type, eid and aid.
 * If cropings aren't given, flag existing croping informations as permanent or temporary, following the temp parameter.
 *
 * The form_token parameter allows to identify a croping using the form_token column, instead of the eid column.
 */
function _dpicontenttypes_cropings_update_cropings($entity_type, $eid, $aid, $cropings = array(), $temp = FALSE, $form_token = '') {
  if (empty($cropings)) {
    $update_query = db_update('dpicontenttypes_cropings');
    $fields = array(
      'temp' => $temp ? 1 : 0,
    );
    if ($form_token) {
      // If a form_token is given, we have to replace the fake eid with the real one
      $fields['eid'] = $eid;
      $fields['form_token'] = NULL;
    }
    $update_query->fields($fields)
      ->condition('etype', $entity_type)
      ->condition('aid', $aid);
    if ($form_token) {
      $update_query->condition('form_token', $form_token);
    } else {
      $update_query->condition('eid', $eid);
    }
    $update_query->execute();
  } else {
    // Get existing cropings values from DB
    $select_query = db_select('dpicontenttypes_cropings', 'dpic')
      ->fields('dpic')
      ->condition('dpic.etype', $entity_type)
      ->condition('dpic.aid', $aid);
    if ($form_token) {
      $select_query->condition('dpic.form_token', $form_token);
    } else {
      $select_query->condition('dpic.eid', $eid);
    }
    $db_cropings = $select_query->execute();
    $existing_croping = FALSE;
    foreach ($db_cropings as $db_croping) {
      $existing_croping = $db_croping;
      break;
    }

    if ($existing_croping) {
      $new_croping = $existing_croping;
    } else {
      $new_croping = new stdClass();
      $new_croping->etype = $entity_type;
      $new_croping->eid = $eid;
      $new_croping->aid = $aid;
      if ($form_token) {
        $new_croping->form_token = $form_token;
      } else {
        $new_croping->form_token = NULL;
      }
    }

    foreach ($cropings as $style_name => $croping) {
      for ($i = 0; $i < 4; $i++) {
        $cropings[$style_name][$i] = (int)floor((int)$croping[$i]);
      }
    }

    $new_croping->cropings = serialize($cropings);
    $new_croping->temp = $temp ? 1 : 0;
    if ($existing_croping) {
      drupal_write_record('dpicontenttypes_cropings', $new_croping, 'cid');
    } else {
      drupal_write_record('dpicontenttypes_cropings', $new_croping);
    }
  }
  return;
}

/**
 * Delete croping informations, each of the parameters giving an additionnal selection information.
 */
function _dpicontenttypes_cropings_delete_cropings($cid = NULL, $entity_type = NULL, $eid = NULL, $aid = NULL, $temp = NULL, $form_token = NULL) {
  $delete_query = db_delete('dpicontenttypes_cropings');
  if ($cid) {
    $delete_query->condition('cid', $cid);
  }
  if ($entity_type) {
    $delete_query->condition('etype', $entity_type);
  }
  if ($eid) {
    $delete_query->condition('eid', $eid);
  }
  if ($aid) {
    $delete_query->condition('aid', $aid);
  }
  if ($temp) {
    $delete_query->condition('temp', 1);
  } else {
    $delete_query->condition(db_or()->condition('temp', 0)->isNull('temp'));
  }
  if ($form_token) {
    $delete_query->condition('form_token', $form_token);
  }
  $delete_query->execute();
}
