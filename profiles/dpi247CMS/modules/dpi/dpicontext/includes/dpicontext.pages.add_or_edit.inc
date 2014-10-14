<?php

function dpicontext_add_or_edit($id = NULL) {
  return drupal_get_form ( 'dpicontext_add_form', $id );
}

function dpicontext_add_form($form, &$form_state, $id) {
  $form = array ();
  
  if (isset ( $id )) {
    
    $form ['store'] = array (
      '#type' => 'value',
      '#value' => $id 
    );
    
    $context = dpicontext_api_get_one_context ( $id );
    $text = "";
    if (is_array ( $context ['value'] )) {
      foreach ( $context ['value'] as $str ) {
        $text .= $str;
      }
      $context ['value'] = $text;
    }
  }
  
  $form ['dpicontext_label'] = array (
    
    '#type' => 'textfield',
    '#title' => t ( 'Enter your context name here' ),
    '#default_value' => (isset ( $context ['label'] )) ? $context ['label'] : "",
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE 
  );
  
  $form ['dpicontext_types'] = array (
    '#type' => 'radios',
    '#title' => t ( 'Context\'s type' ),
    '#options' => array (
      0 => t ( 'Numeric' ),
      1 => t ( 'Text' ),
      2 => t ( 'Date' ) 
    ),
    '#default_value' => (isset ( $context ['type'] )) ? $context ['type'] : 0,
    '#multiple' => FALSE,
    '#description' => t ( 'Select a context type' ),
    '#ajax' => array (
      'callback' => 'dpicontext_ajax_typed_form',
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
  
  if (isset ( $form_state ['values'] ['dpicontext_types'] )) {
    $type = $form_state ['values'] ['dpicontext_types'];
  } else {
    $type = (isset ( $context ['type'] )) ? $context ['type'] : 0;
  }
  
  switch ($type) {
    case 0 :
      $form ['value_fieldset'] ['input'] = array (
        '#title' => t ( 'Numeric values only' ),
        '#type' => 'textfield',
        '#default_value' => (isset ( $context ['value'] )) ? $context ['value'] : "",
        '#description' => t ( 'I.e if input = 20 you would have 0 to 20 values' ) 
      );
      
      break;
    case 1 :
      $form ['value_fieldset'] ['input'] = array (
        '#title' => t ( 'Text only' ),
        '#type' => 'textarea',
        '#default_value' => (isset ( $context ['value'] )) ? $context ['value'] : "",
        '#description' => t ( 'Each string should be separated with a line break' ) 
      );
      
      break;
    case 2 :
      
      $date = format_date ( REQUEST_TIME, 'custom', 'd-m-Y H:i' );
      $format = 'd-m-Y H:i';
      
      $form ['value_fieldset'] ['input'] ['date_start'] = array (
        '#description' => t ( 'Select a start/end date' ),
        '#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
        '#title' => t ( 'Select start date' ),
        '#default_value' => $date,
        '#date_format' => $format,
        '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
        '#date_increment' => 1, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
        '#date_year_range' => '-0:+10' 
      );
      
      $form ['value_fieldset'] ['input'] ['date_end'] = array (
        '#description' => t ( 'Select a start/end date' ),
        '#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
        '#title' => t ( 'Select end date' ),
        '#default_value' => $date,
        '#date_format' => $format,
        '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
        '#date_increment' => 1, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
        '#date_year_range' => '-0:+10' 
      );
      
      break;
  }
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  return $form;
}

function dpicontext_ajax_typed_form($form, $form_state) {
  
  // delete previous value when input type is changed to avoid typo
  // i.e : if previous type is date and user select type text the previous date is show in the textarea
  $form ['value_fieldset'] ['input'] ['#value'] = "";
  
  return $form ['value_fieldset'];
}

/**
 * Validate annotation settings submission.
 */
function dpicontext_add_form_validate($form, &$form_state) {
  // TODO verify label ? only string ?
  $type = $form_state ['values'] ['dpicontext_types'];
  
  if ($type == 2) {
    $value [] = $form_state ['values'] ['date_start'];
    $value [] = $form_state ['values'] ['date_end'];
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
          $array_formated = array();
          $text = trim ( $value );
          $textAr = explode ( "\n", $text );
          $textAr = array_filter ( $textAr, 'trim' );
          $form_state ['values'] ['input'] = $textAr;
        }
        break;
      case 2 :
        if (! (strtotime ( $value [0] ) < strtotime ( $value [1] ))) {
          form_set_error ( 'value_fieldset', t ( 'End date must be greater than start date' ) );
        }
        $form_state ['values'] ['input'] = $value;
      default :
        break;
    }
  } else {
    form_set_error ( 'value_fieldset', t ( 'Please enter a value' ) );
  }
}

/**
 * **
 * Process form submission.
 */
function dpicontext_add_form_submit($form, $form_state) {
  if (isset ( $form_state ['values'] ['store'] )) {
    
    if (dpicontext_api_update ( $form_state )) {
      drupal_set_message ( 'Data updated successfully' );
    } else {
      drupal_set_message ( 'Error while updating data !', 'error' );
    }
  } else {
    if (dpicontext_api_add ( $form_state )) {
      drupal_set_message ( 'Data saved successfully' );
    } else {
      drupal_set_message ( 'Error while saving data !', 'error' );
    }
    
    drupal_goto ( 'admin/config/dpicontext/settings' );
  }
}
