<?php

/**
 * @author LBA 
 */

/**
 * This function create form
 * The render of the form table is create by the theme
 * into the function dpistate_table_theme
 *
 * @param array $form          
 * @param array $form_state          
 * @return multitype:array
 */
function dpistate_settings_form($form, &$form_state) {
  $form = array ();
  $data_list = dpistate_api_get_all_states();
  $val_recorded = variable_get(VARIABLE_NAME, array());
  $rows =  array();  
  
  foreach ( $data_list as $k => $v ) {
    if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
      dpistate_api_change_date_format ( $data_list, $v, $k );
    }        
    // $rows ['row_' . $v ['id']] = dpicontext_get_form_element_by_type ( $data_list, $v ['id'], $k, $val_recorded );
    $rows ['row_' . $v['id']] = dpistate_get_settings_row_data($v,isset($val_recorded[$v['id']])?$val_recorded[$v['id']]:array());
  }

  $form ['table'] = array (
    '#theme' => 'dpistate_table_theme',
    '#header' => array (
      t ( 'Name' ),
      t ( 'Options' )
    ),
    'rows' => $rows,
  );
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  
  return $form;
}

/**
 * Validate for the date's type, that the start date is before end date
 * 
 * @param array $form
 * @param array $form_state
 */
function dpistate_settings_form_validate($form, & $form_state){
  foreach($form_state['values'] as $value){
    if(is_array($value) && isset($value['start']) && isset($value['end'])){
      if (! (strtotime ( $value ['start'] ) < strtotime ( $value ['end'] ))) {
        form_set_error ( 'value_fieldset', t ( 'End date must be greater than start date' ) );
      }
    }
  }
}

/**
 * This function render the part of the form name table
 *
 * @param array:form $variables          
 * @return render
 */
function theme_dpistate_table_theme($variables) {
  $form = $variables ['element'];
  $rows = $form ['rows'];
  $header = $form ['#header'];
  
  $content = array (
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array () 
  );
  
  foreach ( element_children ( $rows ) as $row_index ) {
    $row = array ();
    foreach ( element_children ( $rows [$row_index] ) as $col_index ) {
      $row [] = drupal_render ( $rows [$row_index] [$col_index] );
    }
    $content ['#rows'] [] = $row;
  }
  
  // Render the table and return it.
  return drupal_render ( $content );
}

/**
 * Save informations to dpi_variable after submitting
 *
 * @param array $form          
 * @param array $form_state          
 */
function dpistate_settings_form_submit($form, & $form_state) {
  try {
    $temp_form_state = $form_state ['values'];
    unset ( $temp_form_state ['submit'] );
    unset ( $temp_form_state ['form_build_id'] );
    unset ( $temp_form_state ['form_token'] );
    unset ( $temp_form_state ['form_id'] );
    unset ( $temp_form_state ['op'] );
    
    $val = dpistate_reorder_informations_to_save ( $temp_form_state, $form );
    
    variable_set(VARIABLE_NAME, $val);
    drupal_set_message ( 'Saved successfuly !' );
  } catch (Exception $e) {
    drupal_set_message ( 'An error is happened to create values' );
  }
}

/**
 * This function réorder informations to create a table
 * that we can use to compare with view state
 * 
 * @param array $infos
 * @param array $form
 * @return multiple:array
 */
function dpistate_reorder_informations_to_save($infos, $form) {
  $return = array ();
  foreach ( $infos as $k => $v ) {
    $val_info = explode ( '_', $k );
    if (isset ( $val_info [1] )) {
      
      switch ($val_info [2]) {
        case '0' :
          $return [$val_info [1]] = array (
            'value' => $infos ['options_' . $val_info [1] . '_0'], 
          );
          break;
        case '1' :
          $return [$val_info [1]] = array (
            'value' => $form['table']['rows']['row_'.$val_info [1]]['options_'.$val_info [1].'_1']['#options'][$infos ['options_' . $val_info [1] . '_1']],
            'index' =>   $infos ['options_' . $val_info [1] . '_1'],
          );
          break;
        case '2' :
          $return [$val_info [1]] = array (
            'value' => $v,
          );
          break;
        default:
            $return [$val_info [1]] = current(module_invoke_all('dpistate_define_info_to_save', $val_info[2], $v));
          break;
      }
    }
  }
  return $return;
}

/**
 * This function define form element that we need to use for settings
 * 
 * @param array $row
 * @param array $values
 * @return array
 * 
*/
function dpistate_get_settings_row_data($row, $values) {
	$options = array();
	switch ($row['type']) {
		case '0':
			$options = array (
			  '#type' => 'select',
			  '#options' => range ( 0, $row['value'] ),
			  '#default_value' => (isset($values['value']))? $values['value'] : FALSE,
			);
			break;
		case '1':
			$options = array (
			  '#type' => 'select',
			  '#options' => $row['value'],
			  '#default_value' => (isset($values['index']))? $values['index'] : FALSE,
			);
			break;
		case '2':
			$format = 'd-m-Y H:i';
			$options = array(
        'start' => array(
          '#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
          '#title' => t('Select start date'),
          '#default_value' => (isset($values['value']["start"]))? $values['value']["start"] : date($format),
          '#date_format' => $format,
          '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
          '#date_increment' => 1, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
          '#date_year_range' => '-0:+10'
        ),
        'end' => array(         
          '#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
          '#title' => t ( 'Select end date' ),
          '#default_value' => (isset($values['value']["end"]))? $values['value']["end"] : date($format),
          '#date_format' => $format,
          '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
          '#date_increment' => 1, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
          '#date_year_range' => '-0:+10' 
        ),
        "#tree" => TRUE,
      );
			break;	
    default:
        $options = current(module_invoke_all('dpistate_define_options_settings', $row, $values)) + array( "#tree" => TRUE );
      break;
	}
	
	$return = array (
    'label_' . $row['id'] . '_' . $row['type'] => array (
      '#markup' => $row['label'] 
    ),
    'options_' . $row['id'] . '_' . $row['type'] => $options,        
  );
	  
	 return $return;
}