<?php


/**
 * Implements hook_field_info().
 */
function dpicontenttypes_field_info() {
  return array(
    'dpiatom_reference' => array(
      'label' => t('DPIAtom Reference'),
      'description' => t('This field stores the ID of a related atom as an integer value.'),
      'instance_settings' => array(
        'referencable_types' => array()
      ),
      'default_widget' => 'textfield',
      'default_formatter' => 'title',
      'property_type' => 'scald_atom',
    )
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function dpicontenttypes_field_instance_settings_form($field, $instance) {
  $options = array();
  foreach(scald_types() as $name => $type) {
    $options[$name] = $type->title;
  }
  $form = array();
  $form['referencable_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Atom types that can be referenced'),
    '#multiple' => TRUE,
    '#options' => $options,
    '#default_value' => $instance['settings']['referencable_types']
  );

  $analyse_options=array();
  $fields = field_info_instances($instance['entity_type'], $instance['bundle']);
  foreach ($fields as $name => $field) {
    if (!empty($field['settings']['dnd_enabled'])) {
      $analyse_options[$name]=$field['label'];
    }
  }

  $form['field_to_analyse'] = array(
    '#type' => 'select',
    '#title' => t('Field from where inline atom are extracted'),
    '#options' => $analyse_options,
    '#default_value' => $instance['settings']['field_to_analyse']
  );

  return $form;
}

/**
 * Implements hook_field_views_data().
 */
function dpicontenttypes_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  $current_table = _field_sql_storage_tablename($field);
  $column = _field_sql_storage_columnname($field['field_name'], 'sid');

  // Relationship: add a relationship for related atom.
  $data[$current_table][$column]['relationship'] = array(
    'base' => 'scald_atoms',
    'field' => $column,
    'handler' => 'views_handler_relationship',
    'label' => $data[$current_table][$field['field_name']]['title'],
    'field_name' => $field['field_name'],
  );

  return $data;
}

/**
 * Implements hook_field_validate().
 */
function dpicontenttypes_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  // Ensure that the types of the referenced atoms match the one of those
  // that were defined in the field configuration.
  $types = dpicontenttypes_field_referenceable_types($instance);
  foreach ($items as $delta => $item) {
    if (empty($item['sid'])) {
      continue;
    }
    $atom = scald_fetch($item['sid']);
    if (!isset($types[$atom->type]) || empty($types[$atom->type])) {
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'dpiatom_reference_bad_type',
        'message' => t("Atom %title is of type %type, which can't be referenced in field %field", array('%title' => $atom->title, '%type' => $atom->type, '%field' => $instance['label']))
      );
    }
  }
}



/**
 * Implements hook_field_info().
 */
function dpicontenttypes_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach($items as $delta=>$item){

    //First remove all items that come from the inline
    if(isset($item['inline']) and $item['inline']==1){
      unset($items[$delta]);
    }
    //Add extra meta
    if($atom=scald_atom_load($items[$delta]['sid'])){
      $items[$delta]['type']=$atom->type;
      $items[$delta]['provider']=$atom->provider;
      $items[$delta]['inline']=0;
    }


  }

  //@todo: should get the $field_to_analyse from the settings of the dpiatomreference filed
  $field_to_analyse='field_textbody';
  $field_to_analyse_items = field_get_items($entity_type, $entity,  $field_to_analyse, $entity->language);
  foreach($field_to_analyse_items as $field_to_analyse_item ){
    foreach($field_to_analyse_item['mee']['resource_manager'] as $sid=>$mee){

      if($atom=scald_atom_load($sid)){
        $values=array(
          'sid'=>$atom->sid,
          'type'=>$atom->type,
          'provider'=>$atom->provider,
          'inline'=>1,
        );
        $add_items[]=$values;
      }

    }
  }
  foreach ($add_items as $add_item){
    $items[]=$add_item;
  }
}
/**
 * Implements hook_field_is_empty().
 */
function dpicontenttypes_field_is_empty($item, $field) {
  return empty($item['sid']);
}

/**
 * Implements hook_field_formatter_info().
 */
function dpicontenttypes_field_formatter_info() {
  // Expose all the Scald Contexts as formatters for the Atom Reference field.
  $formatters = array();
  $contexts = scald_contexts();
  foreach ($contexts as $name => $context) {
    if (empty($context['hidden'])) {
      $formatters[$name] = array(
        'label' => $context['title'],
        'field types' => array('dpiatom_reference')
      );
    }
  }

  return $formatters;
}

/**
 * Implements hook_field_formatter_view.
 */
function dpicontenttypes_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $render_context = $display['type'];
  $contexts = scald_contexts();
  $element = array();

  if (!empty($contexts[$render_context])) {
    foreach ($items as $delta => $item) {
      $sid = $item['sid'];
      $element[$delta] = array('#markup' => scald_render($sid, $render_context));
    }
  }

  return $element;
}

/**
 * Implements hook_field_widget_info.
 */
function dpicontenttypes_field_widget_info() {
  return array(
    'dpiatom_reference_textfield' => array(
      'label' => t('Drop box'),
      'field types' => array('dpiatom_reference'),
    )
  );
}

/**
 * Implements hook_field_widget_form.
 */
function dpicontenttypes_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $all = scald_types();
  $options = array();
  $types = dpicontenttypes_field_referenceable_types($instance);

  foreach ($types as $name => $value) {
    if ($value && isset($all[$name])) {
      $options[$name] = $all[$name]->title;
    }
  }

  $help = format_plural(
      count($options),
      'Allowed resource format: %types',
      'Allowed resource formats: %types',
      array('%types' => implode(', ', $options))
  );

  $element['#description'] .= ' ' . $help;
  $element['#type'] = 'textfield';
  $element['#attributes'] = array('data-types' => implode(',', array_keys($options)));
  $element['#default_value'] = isset($items[$delta]) ? $items[$delta]['sid'] : '';
  $element['#process'][] = 'dpicontenttypes_field_widget_form_process';

  // Attach our JS and CSS to the element
  $path = drupal_get_path('module', 'dpiatom_reference');
  $element['#attached'] = array('library' => array(array('dpicontenttypes', 'library')));


  
  return array(
    'sid' => $element,
  );
  
}

/**
 * Atom types that are allowed to be referenced in that field instance.
 */
function dpicontenttypes_field_referenceable_types($instance) {
  $types = $instance['settings']['referencable_types'];
  $all = scald_types();

  // All types are allowed if no type is explicitely selected (default setting).
  if (!array_filter($types)) {
    $types = array_fill_keys(array_keys($all), '1');
  }

  return $types;
}

/**
 * Process the Atom Reference widget element.
 *
 * Add either the atom reference representation or the placeholder
 * on the fly, depending on the field being filled.
 */
function dpicontenttypes_field_widget_form_process(&$element) {
  // Get the default value, and format the placeholder accordingly.
  $default = $element['#value'];
  if ($default) {
    $prefix = '<div class="dpiatom_reference_drop_zone">' . scald_render($default, variable_get('dnd_context_default', 'sdl_editor_representation')) .'</div>';
  }
  else {
    $placeholder = t('Drop a resource here');
    $prefix = '<div class="dpiatom_reference_drop_zone"><em>' . $placeholder . '</em></div>';
  }
  $element['#field_prefix'] = $prefix;
  return $element;

}



/**
 * Implements hook_field_widget_error.
 */
function dpicontenttypes_field_widget_error($element, $error, $form, &$form_state) {
  $name = implode('][', $element['sid']['#array_parents']);
  form_set_error($name, $error['message']);
}
