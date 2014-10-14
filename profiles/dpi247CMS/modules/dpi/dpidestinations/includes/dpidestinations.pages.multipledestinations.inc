<?php

/**
 * @author lba
 */

/**
 * This function create for to add and preview the table of destination
 *
 * @return theme
 */
function dpidestinations_page_multiple_destinations() {
  $render = l ( t ( 'Add multiple link' ), 'admin/dpi247/dpidestinations/destinationmultiple/add' );
  
  /* generate table */
  $header = array (
    'Label',
    'Operations' 
  );
  $rows_tmp = dpidestinations_api_get_all_multi_destination ();
  $rows = array ();
  if (is_array ( $rows_tmp )) {
    foreach ( $rows_tmp as $row ) {
      $rows [] = array (
        $row ["label"],
        l ( t ( 'Edit' ), 'admin/dpi247/dpidestinations/destinationmultiple/edit/' . urlencode ( $row ["label"] ) ) . " - " . l ( t ( 'Delete' ), 'admin/dpi247/dpidestinations/destinationmultiple/del/' . urlencode ( $row ["label"] ) ) 
      );
    }
  }
  
  $render .= theme_table ( array (
    'header' => $header,
    'rows' => $rows,
    'attributes' => array (),
    'colgroups' => null,
    "caption" => null,
    'empty' => null,
    'sticky' => FALSE 
  ) );
  
  return $render;
}

/**
 * This function manage add element
 *
 * @return render
 */
function dpidestinations_page_multiple_destinations_add($label = NULL) {
  return drupal_get_form ( "dpidestinations_form_multiple_destinations_form", $label );
}

/**
 * This function create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return array()
 */
function dpidestinations_form_multiple_destinations_form($form, &$form_state, $label = NULL) {
  $form = array ();
  $form_temp = $form;
  
  $form ['label'] = array (
    '#type' => 'textfield',
    '#title' => t ( 'Label' ),
    '#default_value' => $label,
    '#size' => 60,
    '#required' => TRUE 
  );
  
  $destination = new stdClass ();
  $destination->type = "package";
  field_attach_form ( 'node', $destination, $form_temp, $form_state );
  
  if ($label != NULL) {
    $form ['label_hidden'] = array (
      '#type' => 'hidden',
      '#value' => $label 
    );
    // step 1 : get informations
    $multi_destinations = dpidestinations_api_get_one_multi_destination_by_label ( $label );
    $destinations = unserialize ( $multi_destinations ["destinations"] );
    $cp_field_destination = $form_temp ['field_destinations'] [$form_temp ['field_destinations'] ["#language"]];
    $all_destinations = array ();
    $cpt = 0;
    
    // step 2 create table of information
    foreach ( $destinations as $destination ) {
      $target_options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $destination ['tid'] );
      $layout_options = __dpidestinations_api_get_all_layouts_available_for_sectiontid_and_target ( $destination ['tid'], $destination ['target'] );
      $rank_options = __dpidestinations_api_get_all_ranks_available_for_sectiontid_and_target ( $destination ['tid'], $destination ['target'], TRUE );
      
      $tmp = $cp_field_destination [0];
      
      $tmp ["element_key"] ["#value"] = $destination ["element_key"];
      
      $tmp ["#delta"] = $cpt;
      $tmp ["#prefix"] = '<div id="dropdown-target-replace_' . $cpt . '">';
      
      $tmp ["tid"] ["#default_value"] = $destination ["tid"];
      
      $tmp ["target"] ["#options"] = $target_options;
      $tmp ["target"] ["#default_value"] = $destination ["target"];
      $tmp ["target"] ["#ajax"] ["wrapper"] = 'dropdown-target-replace_' . $cpt;
      
      $tmp ["rank"] ["#options"] = $rank_options;
      $tmp ["rank"] ["#default_value"] = $destination ["rank"];
      
      $tmp ["layout"] ["#options"] = $layout_options;
      $tmp ["layout"] ["#default_value"] = $destination ["layout"];
      
      $tmp ["_weight"] ["#delta"] = $cpt;
      $tmp ["_weight"] ["#default_value"] = $cpt;
      
      $all_destinations [] = $tmp;
      $cpt ++;
    }
    
    foreach ( $all_destinations as $k => $v ) {
      $form_temp ['field_destinations'] [$form_temp ['field_destinations'] ["#language"]] [$k] = $v;
    }
    $form_temp ['field_destinations'] [$form_temp ['field_destinations'] ["#language"]] ["#max_delta"] = 2;
  }
  
  $form ["field_destinations"] = $form_temp ['field_destinations'];
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#weight' => 50,
    '#value' => t ( 'Create new Url' ) 
  );
  
  return $form;
}

function dpidestinations_form_multiple_destinations_form_validate($form, &$form_state) {
  if (! is_array ( $val_tmp = dpidestinations_get_destinations_from_form_value ( $form_state ) ))
    form_set_error ( 'Value array', 'Form content is not available' );
  if (! isset ( $val_tmp [0] ) || $val_tmp [0] ["tid"] == null)
    form_set_error ( 'Value array', 'Please fill in all fields' );
}

function dpidestinations_form_multiple_destinations_form_submit($form, $form_state) {
  if (isset ( $form_state ["values"] ["label_hidden"] )) {
    try {
      $query = db_update ( 'dpidestinations_multi_destinations' )->fields ( array (
        'destinations ' => serialize ( dpidestinations_get_destinations_from_form_value ( $form_state ) ) 
      ) )->condition ( 'label', $form_state ["values"] ["label"], '=' )->execute ();
      drupal_set_message ( 'Successful updating' );
      drupal_goto ( 'admin/dpi247/dpidestinations/destinationmultiple' );
    } catch ( Exception $e ) {
      drupal_set_message ( 'Registration failed', 'error' );
    }
  } else {
    try {
      $query = db_insert ( 'dpidestinations_multi_destinations' )->fields ( array (
        'label' => $form_state ["values"] ["label"],
        'destinations ' => serialize ( dpidestinations_get_destinations_from_form_value ( $form_state ) ) 
      ) )->execute ();
      drupal_set_message ( 'Successful registration' );
      drupal_goto ( 'admin/dpi247/dpidestinations/destinationmultiple' );
    } catch ( Exception $e ) {
      drupal_set_message ( 'Registration failed', 'error' );
    }
  }
}

function dpidestinations_get_destinations_from_form_value($form_state) {
  $array_value = current ( $form_state ["values"] ["field_destinations"] ); // bypass language, It's always the first array
  $return_val = array ();
  foreach ( $array_value as $k => $v ) {
    if (strcmp ( $k, "add_more" ))
      $return_val [$k] = $v;
  }
  return $return_val;
}

function dpidestinations_page_multiple_destinations_del($label) {
  return drupal_get_form ( 'dpidestinations_form_multiple_destinations_del_form', $label );
}

function dpidestinations_form_multiple_destinations_del_form($form, & $form_state, $label) {
  $form = array ();
  
  $form ["info"] = array (
    '#markup' => t ( '<p>Are you sure that you want to delete information about <span style="color: #1400CD;">@label</span> ?</p>', array (
      "@label" => $label 
    ) ) 
  );
  
  $form ['label'] = array (
    '#type' => 'hidden',
    '#value' => $label 
  );
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Delete' ) 
  );
  
  $form ['Cancel'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Cancel' ) 
  );
  
  return $form;
}

function dpidestinations_form_multiple_destinations_del_form_submit($form, & $status_form) {
  $op = $status_form ["values"] ["op"];
  switch ($op) {
    case "Delete" :
      if (dpidestinations_api_del_multi_destination_by_label ( $status_form ["values"] ["label"] )) {
        drupal_set_message ( 'Deleting success' );
      } else {
        drupal_set_message ( 'Deleting failed', 'error' );
      }
      break;
  }
  drupal_goto ( 'admin/dpi247/dpidestinations/destinationmultiple' );
}

