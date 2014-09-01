<?php

/**
 * Simple HTML Tag formatter prepare view
 */
function dpicontenttypes_field_formatter_prepare_view_simple_html_tag_formatter($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  
  return;
  
  $tids = array();

  // Collect every possible term attached to any of the fieldable entities.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // Force the array key to prevent duplicates.
      if ($item['tid'] != 'autocreate') {
        $tids[$item['tid']] = $item['tid'];
      }
    }
  }

  if ($tids) {
    $terms = taxonomy_term_load_multiple($tids);

    // Iterate through the fieldable entities again to attach the loaded term data.
    foreach ($entities as $id => $entity) {
      $rekey = FALSE;

      foreach ($items[$id] as $delta => $item) {
        // Check whether the taxonomy term field instance value could be loaded.
        if (isset($terms[$item['tid']])) {
          // Replace the instance value with the term data.
          $items[$id][$delta]['taxonomy_term'] = $terms[$item['tid']];
        }
        // Terms to be created are not in $terms, but are still legitimate.
        elseif ($item['tid'] == 'autocreate') {
          // Leave the item in place.
        }
        // Otherwise, unset the instance value, since the term does not exist.
        else {
          unset($items[$id][$delta]);
          $rekey = TRUE;
        }
      }

      if ($rekey) {
        // Rekey the items array.
        $items[$id] = array_values($items[$id]);
      }
    }
  }
}

/**
 * Simple HTML Tag formatter view
 */
function dpicontenttypes_field_formatter_view_simple_html_tag_formatter($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array(); // Initialize the var
  $settings = $display['settings']; // Get the settings

  $tag = filter_xss($settings['tag'], array()); // The tag setting selected in the settings form
  $class = filter_xss($settings['class'], array()); // The class setting setted in the settings form

  foreach ($items as $delta => $item) {
    $element[$delta] = array(
      '#markup' => isset($item['safe_value']) ? $item['safe_value'] : filter_xss($item['value'], array()),
      '#prefix' => '<'.$tag.' class="'.$class.'">',
      '#suffix' => '</'.$tag.'>'
    );
  }

  return $element;
}

/**
 * Sclad Context formatter view
 */
function dpicontenttypes_field_formatter_view_sclad_context($entity_type, $entity, $field, $instance, $langcode, $items, $display, $render_context) {
  $element = array();

  foreach ($items as $delta => $item) {
    $sid = $item['sid'];
    $entity_ids = entity_extract_ids($entity_type, $entity);
    $options = array('entity' => array('type' => $entity_type, 'bundle' => $entity_ids[2], 'id' => $entity_ids[0], 'vid' => $entity_ids[1]));
    $element[$delta] = array('#markup' => scald_render($sid, $render_context, json_encode($options)));
  }

  return $element;
}

/**
 * Simple HTML Tag formatter settings form
 */
function dpicontenttypes_field_formatter_settings_form_simple_html_tag_formatter($field, $instance, $view_mode, $form, &$form_state) {
  // This gets the view_mode where our settings are stored
  $display = $instance['display'][$view_mode];

  // This gets the actual settings
  $settings = $display['settings'];

  // Initialize the element variable
  $element = array();

  $tag_options = array(
    'span' => 'span',
    'div' => 'div',
    'p' => 'p',
    'strong' => 'strong',
    'b' => 'b',
    'i' => 'i',
    'u' => 'u',
    'h1' => 'h1',
    'h2' => 'h2',
    'h3' => 'h3',
    'h4' => 'h4',
    'h5' => 'h5',
    'h6' => 'h5',
  );
  $element['tag'] = array(
    '#type'           => 'select', // Use a select box widget
    '#title'          => t('Tag'), // Widget label
    '#description'    => t('Select your tag'), // Helper text
    '#default_value'  => $settings['tag'], // Get the value if it's already been set
    '#options'        => $tag_options,
  );

  $element['class'] = array(
    '#type'           => 'textfield', // Use a text box widget
    '#title'          => t('Class(es)'), // Widget label
    '#description'    => t('Add class(es) to your tag, separated with spaces.'), // Helper text
    '#default_value'  => $settings['class'], // Get the value if it's already been set
  );

  return $element;
}

/**
 * Simple HTML Tag formatter summary
 */
function dpicontenttypes_field_formatter_settings_summary_simple_html_tag_formatter($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = t('Use a "@tag" tag with class(es) "@class"', array(
    '@tag' => $settings['tag'],
    '@class'  => $settings['class'],
  )); // we use t() for translation and placeholders to guard against attacks

  return $summary;
}
