<?php

/**
 * Alter an entity edition form to add the cropings edition links and flush temporary cropings values
 */
function _dpicontenttypes_cropings_entity_form_add_cropings_alter(&$form, &$form_state, $form_id) {
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];

  $fields_infos = field_info_instances($entity_type, $bundle);
  $fields_to_alter = array(); // Build an array with all the atom fields that will be altered
  foreach ($fields_infos as $field_infos) {
    if ($field_infos['widget']['type'] == 'dpiatom_reference_textfield') { // The field is an atom reference
      $fields_to_alter[] = $field_infos['field_name'];
    }
  }

  if (!empty($fields_to_alter)) {
    $entity = $form['#entity'];
    list($eid, $revision_id, $bundle) = entity_extract_ids($entity_type, $entity);

    if (!$eid) {
      // This an entity creation, we have to set a fake eid
      $eid = $form['form_token']['#default_value'];
    }

    // Delete temporary cropings linked to this entity
    _dpicontenttypes_cropings_delete_cropings(NULL, $entity_type, $eid, NULL, TRUE);

    // Load the ajax and modal libraries and add the modal javascript.
    ctools_include('ajax');
    ctools_include('modal');
    ctools_modal_add_js();
    $croping_link_settings = array(
      'entityType' => $entity_type,
      'eid' => $eid,
    );
    drupal_add_js(array('dpicontenttypesCropingsLinks' => $croping_link_settings), 'setting');

    foreach (element_children($form) as $element) {
      if (in_array($element, $fields_to_alter)) {
        $items = &$form[$element][$form[$element]['#language']];
        foreach (dpicommons_element_children_numeric($items) as $item_id) {
          $item = $items[$item_id];
          if (isset($item['type']['#value']) && $item['type']['#value'] == 'image'
              && isset($item['provider']['#value']) && $item['provider']['#value'] == 'scald_image') {
            // Add "edit cropings" link
            $aid = $item['sid']['#default_value'];
            _dpicontenttypes_cropings_generate_edition_link($item, $entity_type, $eid, $aid);
            $items[$item_id] = $item;
          }
        }
      }
    }
  }
}

/**
 * Add an "edit croping" link on an atom in an entity edition form
 */
function _dpicontenttypes_cropings_generate_edition_link(&$item, $entity_type, $eid, $aid) {
  $callback = 'admin/editdpicrop/ajax/'.$entity_type.'/'.$eid.'/'.$aid;

  $edit_crop_link = ctools_modal_text_button(t('Click here to edit croping informations'), $callback, '');
  $item['editcrops'] = array(
    '#markup' => $edit_crop_link,
    '#weight' => 100,
  );
}

/**
 * When an entity is saved, its temporary cropings have to be flaged as permanent (i.e. unflagged as temporary)
 *
 * We also remove croping informations for any atom not linked to the entity anymore
 */
function _dpicontenttypes_cropings_entity_form_confirm_new_cropings($entity, $entity_type) {
  list($eid, $revision_id, $bundle) = entity_extract_ids($entity_type, $entity);

  $fields_infos = field_info_instances($entity_type, $bundle);
  $fields_to_alter = array(); // Build an array with all the atom fields that will be altered
  foreach ($fields_infos as $field_infos) {
    if ($field_infos['widget']['type'] == 'dpiatom_reference_textfield') { // The field is an atom reference
      $fields_to_alter[] = $field_infos['field_name'];
    }
  }

  if (!empty($fields_to_alter)) {
    $existing_crops = _dpicontenttypes_get_saved_cropings_for_entity($entity_type, $eid);

    if ($entity_wrapper = entity_metadata_wrapper($entity_type, $entity)) {
      foreach ($entity as $element => $field) {
        if (in_array($element, $fields_to_alter) && isset($entity_wrapper->{$element})) {
          $items = $entity_wrapper->{$element}->value(array('identifier' => TRUE));
          foreach (dpicommons_element_children_numeric($items) as $item_id) {
            $item = $items[$item_id];
            if (isset($item['type']) && $item['type'] == 'image'
                && isset($item['provider']) && $item['provider'] == 'scald_image') {
              // Set temporary cropings as permanent
              $aid = $item['sid'];
              if ($entity->is_new && isset($entity->form_token)) {
                $form_token = $entity->form_token;
              } else {
                $form_token = '';
              }
              _dpicontenttypes_cropings_update_cropings($entity_type, $eid, $aid, array(), FALSE, $form_token);

              // Keep only ununsed atoms in the existing cropings
              if (isset($existing_crops[$aid])) {
                unset($existing_crops[$aid]);
              }
            }
          }
        }
      }
    }

    if (!empty($existing_crops)) {
      // There are ununsed cropings, remove them from the DB
      foreach ($existing_crops as $aid => $cropings) {
        _dpicontenttypes_cropings_delete_cropings(NULL, $entity_type, $eid, $aid);
      }
    }
  }
}
