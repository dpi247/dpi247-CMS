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
  $settings = $display['settings'];
  $nb = dpicontenttypes_define_nb_items($render_context, count($items), $settings);
  $count = 1;
  foreach ($items as $delta => $item) {
    if(dpicontenttypes_test_scald_item_validity($item, $settings)){
      $sid = $item['sid'];
      $entity_ids = entity_extract_ids($entity_type, $entity);
      $options = array('entity' => array('type' => $entity_type, 'bundle' => $entity_ids[2], 'id' => $entity_ids[0], 'vid' => $entity_ids[1]));
      $element[$delta] = array('#markup' => scald_render($sid, $settings['context'], json_encode($options)));
      if(isset($nb) && $nb==$count){
        break;
      }
      $count++;
    }
  }

  return $element;
}

/**
 * Mediabox formatter view
 */
function dpicontenttypes_field_formatter_view_mediabox($entity_type, $entity, $field, $instance, $langcode, $items, $display, $render_context) {
  $element = array();
  $settings = $display['settings'];
  $nb = dpicontenttypes_define_nb_items($render_context, count($items), $settings);
  $count = 1;
  $thumb = 0;
  foreach ($items as $delta => $item) {
    if(dpicontenttypes_test_scald_item_validity($item, $settings)){
      $sid = $item['sid'];
      $entity_ids = entity_extract_ids($entity_type, $entity);
      $options = array('entity' => array('type' => $entity_type, 'bundle' => $entity_ids[2], 'id' => $entity_ids[0], 'vid' => $entity_ids[1]));
//      $element[$delta] = array('#markup' => scald_render($sid, $settings['context'], json_encode($options)));
      $atom = scald_atom_load($sid);

      if ($count == 1) {
        $principale = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('thumbnail embed-responsive embed-responsive-16by9 '),
            'id' => 'image-gallery-launch',
          ),
        );
        $principale[$delta]['photo'] = array(
          '#theme' => 'image_style',
          '#path' => $atom->file_source,
          '#style_name' => 'ena_16_9_extra_big',
          '#attributes' => array(
            'class' => 'embed-responsive-item gr-content-image',
            'alt' => 'Responsive image',
            'title' => $atom->field_caption['und'][0]['safe_value'],
          ),
        );
        $principale[$delta]['caption'] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('gr-caption'),
          ),

          'content-caption' => array(
            '#type' => 'markup',
            '#markup' => '<p>'.$atom->field_caption['und'][0]['safe_value'].'</p>',
          ),
        );

      } else {

        if ($count == 2) {
          $secondaires = array(
            '#type' => 'container',
            '#attributes' => array(
              'class' => array('gr-gallery-thumbnails'),
              'id' => 'gr-gallery-thumbnails',
            ),
          );
        }
        $secondaires[$thumb] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('col-xs-6 col-md-3 thumbnail gr-gallery-thumbnail embed-responsive-16by9'),
          ),
        );

        $secondaires[$thumb][0] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('gr-caption'),
          ),
          'content-caption' => array(
            '#type' => 'markup',
            '#markup' => '<p>'.$atom->field_caption['und'][0]['safe_value'].'</p>',
          ),
        );

        $secondaires[$thumb][1] = array(
          '#type' => 'markup',
          '#markup' => '<a href="'.file_create_url($atom->file_source).'" title="'.$atom->title.'" data-gallery="">' . theme_image_style(array('style_name' => 'ena_16_9_medium', 'path' => $atom->file_source)) . '</a>',
        );

        $thumb++;
      }

      $element = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('media'),
          ),
      );

      $element[0] = $principale;
      $element[1] = $secondaires;

      if(isset($nb) && $nb==$count){
        break;
      }
      $count++;
    }
  }
  return $element;
}

function dpicontenttypes_test_scald_item_validity($item, $settings){
  $return = FALSE;
  if(!isset($item['type']) || $item['type']==""){
    $atom = scald_atom_load($item['sid']);
    $item['type']=$atom->type;
  }
  foreach(dpicontenttypes_get_scald_types() as $type){
    if(isset($settings['type_'.$type]) && $settings['type_'.$type]==TRUE && $item['type']==$type)
      return TRUE;
  }
  return $return;
}

function dpicontenttypes_define_nb_items($render_context, $total_nb_items, $settings){
  $return = 1;
  if($render_context == "media_box"){
     if($settings['type_number']=='all'){
       $return = $total_nb_items;
     }else{
       if(intval($settings['value_number'])>$total_nb_items){
         $return = $total_nb_items;
       }else{
         $return = intval($settings['value_number']);
       }
     }
  }
  return $return;
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

function dpicontenttypes_field_formatter_settings_form_first_element_formatter($field, $instance, $view_mode, $form, &$form_state){
  $display = $instance['display'][$view_mode];
  $formatter_type = $form_state['values']['fields'][$instance['field_name']]["type"];
  // This gets the actual settings
  $settings = $display['settings'];
  
  // Initialize the element variable
  $element = array();
  
  foreach(dpicontenttypes_get_scald_types() as $k => $v){
    $element['type_'.$k] = array(
      '#type' => 'checkbox',
      '#title' => $v,
      '#default_value' => $settings['type_'.$k],
    );
  }
  
  $contexts = dpicontenttypes_field_formatter_get_context();

  $element['context'] = array(
    '#type'           => 'select', // Use a text box widget
    '#title'          => t('Context'), // Widget label
    '#options'         => $contexts,
    '#default_value'  => $settings['context'], // Get the value if it's already been set
  );
  
  if(strcmp($formatter_type, "media_box") == 0){
    $radio_object = array(
      'all' => t('Tous'),
      'custom' => t('Custom'),
    );
  
    $element['type_number'] = array(
      '#type'           => 'radios',
      '#title'          => t('Number of elements'),
      '#default_value'  => $settings['type_number'],
      '#options'        => $radio_object,
    );
  
    $element['value_number'] = array(
      '#type' => 'textfield',
      '#title' => t('From 1 to '),
      '#default_value' => $settings['value_number'],
      '#size' => 5,
      '#maxlength' => 5,
      '#prefix' => '<div id="textfieldFloatLeft">',
      '#suffix' => '</div>',
      '#description' => t('Complétez uniquement si vous avez choisi "custom"')
    );
  }
  
  return $element;
}

function dpicontenttypes_get_scald_types(){
  $return = array();
  foreach(scald_types() as $type){
    $return[$type->type] = $type->type;
  }
  return $return;
}

function dpicontenttypes_field_formatter_get_context(){
  $contexts = scald_contexts();
  $return = array();
  if(is_array($contexts)){
    foreach($contexts as $key => $context){
      $return[$key] = $context['title'];
    }
  }
  return $return;
}

function dpicontenttypes_field_formatter_settings_summary_first_element_formatter($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $elem_type = "";
  foreach(dpicontenttypes_get_scald_types() as $type){
    if(isset($settings['type_'.$type]) && $settings['type_'.$type] == TRUE)
      $elem_type .= ", ".$type;
  }
  $elem_type = substr($elem_type, 2);
  
  $summary = t('Use a type "@type" with context "@context"', array(
    '@type' => $elem_type,
    '@context'  => $settings['context'],
  )); // we use t() for translation and placeholders to guard against attacks

  if(isset($settings['value_number'])){
    if($settings['type_number'] == 'all'){
      $summary .= t(' for all element');
    }else{
      $summary .= t(' for element from 1 to @number', array('@number' => $settings['value_number']));
    }
  }
  
  return $summary;
}