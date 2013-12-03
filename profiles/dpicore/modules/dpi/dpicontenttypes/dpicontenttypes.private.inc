<?php 

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
  $dpiproperties->type = 'default';
  $dpiproperties->id = $eid;
  $dpiproperties->entity = $entity;
  $dpiproperties->external_reference = $dpiproperties->external_reference ? $dpiproperties->external_reference : dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand();
  $dpiproperties->external_reference_2 = $dpiproperties->external_reference_2 ? $dpiproperties->external_reference_2 : dpi_variable_get('dpicommons_product', '').'-'.dpi_variable_get('dpicommons_environment', '').'-'.time().'-'.rand();
  $dpiproperties->source = $dpiproperties->source ? $dpiproperties->source : 'dpi247';
  $dpiproperties->product_id = $dpiproperties->product_id ? $dpiproperties->product_id : dpi_variable_get('dpicommons_product', '');
  dpiproperties_save($dpiproperties);
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
