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
        l(t('Edit'), 'admin/dpi247/dpidestinations/destinationmultiple/edit/' . urlencode ( $row ["label"] )) . " - "
        . l ( t ( 'Delete' ), 'admin/dpi247/dpidestinations/destinationmultiple/del/' . urlencode ( $row ["label"] ) ) 
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
  return drupal_get_form ( "dpidestinations_form_multiple_destinations_form", $label);
}

/**
 * This function create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return array()
 */
function dpidestinations_form_multiple_destinations_form($form, &$form_state, $label) {
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
  try {
    $query = db_insert ( 'dpidestinations_multi_destinations' )->fields ( array (
      'label' => $form_state ["values"] ["label"],
      'destinations ' => serialize ( dpidestinations_get_destinations_from_form_value ( $form_state ) ) 
    ) )->execute ();
    drupal_set_message ( 'Successful registration' );
    drupal_goto ( 'admin/dpi247/dpidestinations/destinationmultiple' );
  } catch ( Exception $e ) {
    drupal_set_message ( 'Registration failed' );
  }
}

function dpidestinations_get_destinations_from_form_value($form) {
  $array_value = current ( $form ["values"] ["field_destinations"] ); // bypass language, It's always the first array
  if (array_pop ( $array_value ) != null) { // Pop button value to give only the destinations
    return $array_value;
  }
  return false;
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
        if(dpidestinations_api_del_multi_destination_by_label($status_form["values"]["label"])){
          drupal_set_message('Deleting success');
        }else{
          drupal_set_message('Deleting failed', 'error');
        }
      break;
  }
  drupal_goto ( 'admin/dpi247/dpidestinations/destinationmultiple' );
}

