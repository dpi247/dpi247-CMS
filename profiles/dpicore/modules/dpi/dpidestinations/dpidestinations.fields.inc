<?php



/**
 * Implements hook_field_info().
 *
 * Provides the description of the field.
 */
function dpidestinations_field_info() {
  return array(
    // We name our field as the associative name of the array.
    'field_destinations' => array(
      'label' => t('Destinations'),
      'description' => t('A destination is a quadruplet of four information that define Where and how a node will be displayed.'),
      'default_widget' => 'field_destinations_default_widget',
      'default_formatter' => 'field_destinations_default_formatter',
    ),
  );
}


/**
 * Implements hook_field_validate().
 *
 * This hook gives us a chance to validate content that's in our
 * field. If it doesn't validate, we add
 *  our own error notification to the $errors parameter.
 *
 * @see dpidestinations_field_widget_error()
 * @see dpidestinations_field_is_empty()
 */
function dpidestinations_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  //Ici, il n'y a pas lieu de faire le validate car le hook_field_is_emtpy
  //ne déclenche pas la validation si le field est vide. or dans notre cas,
  //le hook_field_is_empty check justement la valeur du tid...
  // mais on laisse ca juste pour illustrer
  /*
  foreach ($items as $delta => $item) {
  if (!empty($item['tid'])) {
  $errors[$field['field_name']][$langcode][$delta][] = array(
    'error' => 'field_destinations_tid',
    'message' => t('You must select at least a destination term.'),
  );
  }
  }
  */


}



/**
 * Implements hook_field_is_empty().
 *
 * hook_field_is_emtpy() is where Drupal asks us if this field is empty.
 * Return TRUE if it does not contain data, FALSE if it does. This lets
 * the form API flag an error when required fields are empty.
 */
function dpidestinations_field_is_empty($item, $field) {
  if(empty($item['tid']) or $item['tid']==''){
    return TRUE;
  }
  return FALSE;
}

function dpidestinations_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items){
  _dpidestinations_virtualrank_update_items($items);

}

/**
 * Implements hook_field_widget_info().
 *
 * We only provide one default widget for the moment.
 * Later we would develop another Ajax based widget.
 *
 * These widget types will eventually show up in hook_field_widget_form,
 * where we will have to flesh them out.
 *
 * @see dpidestinations_field_widget_form()
 */
function dpidestinations_field_widget_info() {
  return array(
    'field_destinations_default_widget' => array(
      'label' => t('Destination default widget'),
      'field types' => array('field_destinations'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 *
 * hook_widget_form() is where Drupal tells us to create form elements for
 * our field's widget.
 *
 * We provide one form for the moment: field_destinations_default_widget.
 *
 *
 * 'field_destinations_default_widget' define four form element that are simple selectbox
 *
 */
function dpidestinations_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  
  // STEP 0: 
  //Generate a specific key used to identify this element to restore a default
  // value upon AJAX submission regardless of where this element is in the
  // $form array.
  $element_key = implode('|', array($element['#entity_type'], $element['#bundle'], $element['#field_name'], $element['#language'], $element['#delta']));
  // Store the key in the element array as a value so it can be easily retrieved
  // in context in the $form_state['values'] array in the element validator.
  $widget['element_key'] = array(
    '#type' => 'value',
    '#value' => $element_key,
  );
  
  // STEP 1:
  // Get the default destinations used to build the widget form elements, looking
  // first in the form state, then in the stored value for the field, and then
  // in the default values of the instance.
  $destinations = array();
  if (!empty($form_state['destinationsfield'][$element_key])) {
    // Use the value from the form_state if available.
    $destinations = $form_state['destinationsfield'][$element_key];
    $widget['temp_db_value'] = array(
      '#type' => 'value',
      '#value' => array('tid'=>$destinations['temp_db_value']['tid'],'target'=>$destinations['temp_db_value']['target']),
    );
  }
  elseif (!empty($items[$delta]['tid'])) {

    // Else use the saved value for the field.
    $destinations = $items[$delta];
    //Use this to know if the file already exist on DB => display Do not change in the select option field for rank
    $destinations['item_already_exist']=$items[$delta]['tid'];
    $widget['temp_db_value'] = array(
      '#type' => 'value',
      '#value' => array('tid'=>$items[$delta]['tid'],'target'=>$items[$delta]['target']),
    );
    $destinations['temp_db_value']=$widget['temp_db_value']["#value"];
    
  }
  else {
    // Otherwise use the instance default.
    $destinations = (array) $instance['default_value'][0];
    $destinations['item_already_exist']=FALSE;
  }
  
  $is_update=FALSE;
  if(isset($destinations['temp_db_value']) and $destinations['temp_db_value']['tid']==$destinations['tid'] and $destinations['temp_db_value']['target']==$destinations['target']){
    $is_update=TRUE;
  }
  
  // STEP 2:
  //Determine available options and change destination if need it
  $tids_options=dpidestinations_api_get_all_destinations_term_available_as_select_options($is_update);
  if(!$is_update){
    $destinations['tid']='';
  }
  $target_options=__dpidestinations_get_selectoption_get_targets_for_destinationterm($destinations['tid']);
  if(!isset($target_options[$destinations['target']])){
    $destinations['target']=current($target_options);
  }
  $layout_options= __dpidestinations_api_get_all_layouts_available_for_destination_tid_and_target($destinations['tid'],$destinations['target']);
  if(!isset($layout_options[$destinations['layout']])){
    $destinations['layout']=current($layout_options);
  }
  if($is_update){
    $destinations['rank']='DNC';
  }
  $rank_options= __dpidestinations_api_get_all_ranks_available_for_destination_tid_and_target($destinations['tid'],$destinations['target'],$is_update);
  if(!isset($rankt_options[$destinations['rank']])){
    $destinations['rank']=current($rank_options);
  }
 
  
  
  
  // STEP 3:
  // buid the form
  $widget['#delta'] = $delta;
  $widget['#prefix']= '<div id="dropdown-target-replace_'.$delta.'">';
  $widget['#suffix']= '</div>';
  $widget['#dpidestinationsfield'] = TRUE;
  
  switch ($instance['widget']['type']) {

    case 'field_destinations_default_widget':
      // Make this a fieldset with the four fields.
      $widget += array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('field-destinations-widget')),

      );

      $widget['tid'] = array(
        '#type' => 'select',
        '#options' => $tids_options,
        '#default_value'=>$destinations['tid'],
        '#title' => t("Destination's term"),
        '#attributes' => array('class' => array('field-destinations-widget-tid')),
        '#description' => t('The destination page where the node will appear'),
        '#limit_validation_errors'=>array(),
        '#ajax' => array(
          // When 'event' occurs, Drupal will perform an ajax request in the
          // background. Usually the default value is sufficient (eg. change for
          // select elements), but valid values include any jQuery event,
              // most notably 'mousedown', 'blur', and 'submit'.
              // 'event' => 'change',
              'callback' => 'dpidestinations_ajax_callback_dependent_dropdown',
              'wrapper' => 'dropdown-target-replace_'.$delta,
              'method'=>'replace',
          ),
        );
        $widget['target'] = array(
          '#type' => 'select',
          '#options' => $target_options,
          '#default_value'=> $destinations['target'],
          '#title' => t("Destination's target"),
          '#attributes' => array('class' => array('field-destinations-widget-tid')),
          '#description' => t('Define the target where the node will appear in the page'),
          '#limit_validation_errors'=>array(),
          '#element_validate' => array('dpidestinations_rebuildonchange_validate'),
          '#ajax' => array(
            'callback' => 'dpidestinations_ajax_callback_dependent_dropdown',
            'wrapper' => 'dropdown-target-replace_'.$delta,
            'method'=>'replace',
          ),
        );
        $widget['layout'] = array(
          '#type' => 'select',
          '#options' =>$layout_options,
        //  '#default_value'=> $destinations['layout'],
          '#title' => t("Destination's layout"),
          '#attributes' => array('class' => array('field-destinations-widget-tid')),
          '#description' => t('The way your article will be render in the target'),
          '#element_validate' => array('dpidestinations_rebuildonchange_validate'),
         '#limit_validation_errors'=>array(),
          
         
        );
        $widget['rank'] = array(
          '#type' => 'select',
          '#options' =>$rank_options,
         // '#default_value'=> $destinations['rank'],
          '#title' => t("Destination's rank"),
          '#attributes' => array('class' => array('field-destinations-widget-tid')),
          '#description' => t('The position of the article in the target'),
          '#element_validate' => array('dpidestinations_rebuildonchange_validate'),
          '#limit_validation_errors'=>array(),
          
        );
        if($is_update){
          $widget['rank']['#default_value']="DNC";
        }
        $widget['virtual_rank'] = array(
          '#type' => 'value',
          "#value"=> $destinations['virtual_rank'],
        );

        break;
  }
  $element = $widget;
  

  return $element;
}


/**
 * Element validate callback: rebuilds the form on country change and stores the current address value in the $form_state for retrieval on rebuild.
 */
function dpidestinations_rebuildonchange_validate($element, &$form_state) {
  if($form_state["triggering_element"]["#type"]=='select'){
  // If the country was changed, rebuild the form.
  if ($element['#default_value'] != $element['#value']) {
    $form_state['rebuild'] = TRUE;
  }

  $parents = $element['#parents'];
  array_pop($parents);

  //Search through the form values to find the current address.
  $destination = drupal_array_get_nested_value($form_state['values'], $parents);
  // Store the present address values in the form state for retrieval by the
  // widget form regardless of where the widget sits in the $form array.
  $form_state['destinationsfield'][$destination['element_key']] = array_diff_key($destination, array('element_key' => ''));

  }
}

function dpidestinations_field_extra_fields() {
  $return = array();
  $return['target']['target'] = array(
    'form' => array(
      'targetid' => array(
        'label' => t('Target Id'),
        'description' => t('Unique ID of the target from DB\'s primary key'),
        'weight' => 0,
      ),
      'machine_name' => array(
        'label' => t('Machine name'),
        'description' => t('Machine name\'s target'),
        'weight' => 0,
      ),
      'label' => array(
        'label' => t('Label'),
        'description' => t('Human\s readable label'),
        'weight' => 0,
      ),
      'description' => array(
        'label' => t('Description'),
        'description' => t('Description\'s target'),
        'weight' => 0,
      ),
      'view_name' => array(
        'label' => t('View name'),
        'description' => t('The view associated to the target'),
        'weight' => 0,
      ),
      'view_display' => array(
        'label' => t('View Display'),
        'description' => t('The view display associated to the target'),
        'weight' => 0,
      ),
    ),
    'display'=>array(
      'targetid' => array(
        'label' => t('Target Id'),
        'description' => t('Unique ID of the target from DB\'s primary key'),
        'weight' => 0,
      ),
      'machine_name' => array(
        'label' => t('Machine name'),
        'description' => t('Machine name\'s target'),
        'weight' => 0,
      ),
      'label' => array(
        'label' => t('Label'),
        'description' => t('Human\s readable label'),
        'weight' => 0,
      ),
      'description' => array(
        'label' => t('Description'),
        'description' => t('Description\'s target'),
        'weight' => 0,
      ),
      'view_name' => array(
        'label' => t('View name'),
        'description' => t('The view associated to the target'),
        'weight' => 0,
      ),
      'view_display' => array(
        'label' => t('View Display'),
        'description' => t('The view display associated to the target'),
        'weight' => 0,
      ),
    ),


  );

  return $return;
}

/**
 * Implements hook_field_formatter_info().
 *
 * We need to tell Drupal that we have one or more formatters
 * for this field. The default one will display all the information of the quadruplet
 * that define a destination
 * each other will display only one information of the quadruplet
 *
 * @see fdpidestinations_field_formatter_view()
 */
function dpidestinations_field_formatter_info() {
  return array(
    // This formatter just displays the hex value in the color indicated.
    'field_destinations_default_formatter' => array(
      'label' => t('Simple text-based formatter of the destination'),
      'field types' => array('field_destinations'),
    ),
    'field_destinations_tid_formatter' => array(
      'label' => t('Simple text-based formatter of the term of the destination'),
      'field types' => array('field_destinations'),
    ),
    'field_destinations_tid_name_formatter' => array(
      'label' => t('Simple text-based formatter of the term\'s name linked to the destination'),
      'field types' => array('field_destinations'),
    ),
    'field_destinations_tid_name_linked_formatter' => array(
      'label' => t('Simple text-based formatter of the term\'s name  of the destination'),
      'field types' => array('field_destinations'),
    ),
    'field_destinations_main_tid_name_formatter' => array(
      'label' => t('Simple text-based formatter of the term\'s name linked to the destination'),
      'field types' => array('field_destinations'),
    ),
    'field_destinations_main_tid_name_linked_formatter' => array(
      'label' => t('Simple text-based formatter of the term\'s name linked to the destination'),
      'field types' => array('field_destinations'),
    ),
    //@todo: http://drupal.stackexchange.com/questions/34823/what-does-the-property-type-key-do-in-hook-field-info/34826#34826
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 * Two formatters are implemented.
 * - field_example_simple_text just outputs markup indicating the color that
 *   was entered and uses an inline style to set the text color to that value.
 * - field_example_color_background does the same but also changes the
 *   background color of div.region-content.
 *
 * @see field_example_field_formatter_info()
 */
function dpidestinations_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();


  switch ($display['type']) {
    // This formatter simply outputs the field as text and with a color.

    case 'field_destinations_default_formatter':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'span',
          '#value' => $item['tid'].'-'.$item['target'].'-'.$item['layout'].'-'.$item['rank'],
        );
      }
      break;
    case 'field_destinations_tid_formatter':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'span',
          '#value' => $item['tid'],
        );
      }
      break;
    case 'field_destinations_tid_name_formatter':
      foreach ($items as $delta => $item) {
        $term=taxonomy_term_load($item['tid']);
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'span',
          '#value' => taxonomy_term_title($term),
        );
      }
      break;
    case 'field_destinations_tid_name_linked_formatter':
      foreach ($items as $delta => $item) {
        $term=taxonomy_term_load($item['tid']);
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'span',
          '#value' => l(taxonomy_term_title($term),taxonomy_term_uri($term)),
        );
      }

      break;
    case 'field_destinations_main_tid_name_formatter':
      $term=taxonomy_term_load($item['tid']);
      $element[0] = array(
        '#type' => 'html_tag',
        '#tag' => 'span',
        '#value' => taxonomy_term_title($term),
      );
      break;
    case 'field_destinations_main_tid_name__linked_formatter':
      $term=taxonomy_term_load($item['tid']);
      $element[0] = array(
        '#type' => 'html_tag',
        '#tag' => 'span',
        '#value' => l(taxonomy_term_title($term),taxonomy_term_uri($term)),
      );
      break;

  }

  return $element;
}


function dpidestinations_ajax_callback_dependent_dropdown($form, $form_state){

  $array_parents = $form_state['triggering_element']['#array_parents'];
  array_pop($array_parents);
  // Iterate over the form parents to find the element.
  $element = $form;
  foreach ($array_parents as $name) {
    $element = &$element[$name];
    if (!empty($element['#dpidestinationsfield'])) {
      break;
    }
  }

  // Return the address block, but remove the '_weight' element inserted
  // by the field API.
  unset($element['_weight']);
  return $element;


}