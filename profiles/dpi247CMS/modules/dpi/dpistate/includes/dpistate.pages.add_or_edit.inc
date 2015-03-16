<?php

/**
 * @author GVD - LBA
 */

/**
 * This function is a callback from menu to add state
 *
 * @param integer $id          
 * @return render(form)
 */
function dpistate_add_or_edit($id = NULL) {
  return drupal_get_form ( 'dpistate_add_form', $id );
}

/**
 * This function generate form to add state
 *
 * @param array $form          
 * @param array $form_state          
 * @param integer $id          
 * @return array
 */
function dpistate_add_form($form, &$form_state, $id) {
  $form = array ();
  
  if (isset ( $id )) {
    $form ['store'] = array (
      '#type' => 'value',
      '#value' => $id 
    );
    $state = dpistate_api_get_one_state ( $id );
    $text = "";
    if ($state['type']==1) {
      foreach ( $state ['value'] as $str ) {
        $text .= $str."\r\n";
      }
      $state ['value'] = $text;
    }
  }
  
  $form ['dpistate_label'] = array (
    
    '#type' => 'textfield',
    '#title' => t ( 'Enter your state name here' ),
    '#default_value' => (isset ( $state ['label'] )) ? $state ['label'] : "",
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE 
  );
  
  $form ['dpistate_types'] = array (
    '#type' => 'radios',
    '#title' => t ( 'State\'s type' ),
    '#options' => array (
      0 => t ( 'Numeric' ),
      1 => t ( 'Text' ),
      2 => t ( 'Date' ) 
    ),
    '#default_value' => (isset ( $state ['type'] )) ? $state ['type'] : 0,
    '#multiple' => FALSE,
    '#description' => t ( 'Select a state type' ),
    '#ajax' => array (
      'callback' => 'dpistate_ajax_typed_form',
      'wrapper' => 'value_field-div',
      'method' => 'replace',
      'effect' => 'fade' 
    ) 
  );
  
  $form ['value_fieldset'] = array (
    '#title' => t ( "Input field" ),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#prefix' => '<div id="value_field-div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset' 
  );
  
  // get db or default value for type
  if (isset ( $form_state ['values'] ['dpistate_types'] )) {
    $type = $form_state ['values'] ['dpistate_types'];
  } else {
    $type = (isset ( $state ['type'] )) ? $state ['type'] : 0;
  }
  
  switch ($type) {
    case 0 :
      $form ['value_fieldset'] ['input'] = array (
        '#title' => t ( 'Numeric values only' ),
        '#type' => 'textfield',
        '#default_value' => (isset ( $state ['value'] )) ? $state ['value'] : "",
        '#description' => t ( 'I.e if input = 20 you would have 0 to 20 values' ) 
      );
      
      break;
    case 1 :
      $form ['value_fieldset'] ['input'] = array (
        '#title' => t ( 'Text only' ),
        '#type' => 'textarea',
        '#default_value' => (isset ( $state ['value'] )) ? $state ['value'] : "",
        '#description' => t ( 'Each string should be separated with a line break' ) 
      );
      
      break;
    case 2 :
      $form ["value_fieldset"] ['input'] = array(
        "#markup" => t('The date is define into the settings'),
      );
      break;
  }
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  
  return $form;
}

/**
 * This is use to return fieldset after ajax call
 *
 * @param array $form          
 * @param array $form_state          
 * @return array:form_element
 */
function dpistate_ajax_typed_form($form, $form_state) {
  // delete previous value when input type is changed to avoid typo
  // i.e : if previous type is date and user select type text the previous date is show in the textarea
  $form ['value_fieldset'] ['input'] ['#value'] = "";
  return $form ['value_fieldset'];
}

/**
 * This is use to validate form and format informations
 *
 * @param array $form          
 * @param array $form_state          
 */
function dpistate_add_form_validate($form, &$form_state) {
  $type = $form_state ['values'] ['dpistate_types'];
  
  if ($type == 2) {
    $value [] = array();    
  } else {
    $value = $form_state ['values'] ['input'];
  }
  
  if (! empty ( $value )) {
    switch ($type) {
      case 0 :
          if (! is_numeric ( $value )) {
            form_set_error ( 'value_fieldset', t ( 'Please enter number.' ) );
          }
        break;
      case 1 :
          if (! is_string ( $value )) {
            form_set_error ( 'value_fieldset', t ( 'Please enter a valid texts.' ) );
          } else {
            $array_formated = array ();
            $text = trim ( $value );
            $textAr = explode ( "\r\n", $text );
            $textAr = array_filter ( $textAr, 'trim' );
            $form_state ['values'] ['input'] = $textAr;
          }         
        break;
      case 2 :
          $form_state ['values'] ['input'] = array();
        break;
    }
  } else {
    form_set_error ( 'value_fieldset', t ( 'Please enter a value' ) );
  }
}

/**
 * Process for updating or saving informations
 *
 * @param array $form          
 * @param array $form_state          
 */
function dpistate_add_form_submit($form, $form_state) {
  if (isset ( $form_state ['values'] ['store'] )) {
    if (dpistate_api_update_state_by_id ( $form_state ['values'] ['dpistate_label'], $form_state ['values'] ['dpistate_types'], $form_state ['values'] ['input'], $form_state ['values'] ['store'] )) {
      drupal_set_message ( 'Data updated successfully' );
      drupal_goto('admin/dpi247/dpistate');
    } else {
      drupal_set_message ( 'Error while updating data !', 'error' );
    }
  } else {
    if (dpistate_api_add_state ( $form_state ['values'] ['dpistate_label'], $form_state ['values'] ['dpistate_types'], $form_state ['values'] ['input'] )) {
      drupal_set_message ( 'Data saved successfully' );
      drupal_goto('admin/dpi247/dpistate');
    } else {
      drupal_set_message ( 'Error while saving data !', 'error' );
    }
  }
}
