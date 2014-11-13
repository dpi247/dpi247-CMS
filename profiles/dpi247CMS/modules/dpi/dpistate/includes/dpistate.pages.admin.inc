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
  $rows = array ();
  
  foreach ( $data_list as $k => $v ) {
    if (is_array ( $v ['value'] ) && $v ['type'] == 2) {
      dpistate_api_change_date_format ( $data_list, $v, $k );
    }
    $val_recorded = variable_get(VARIABLE_NAME, array());
	
   // $rows ['row_' . $v ['id']] = dpicontext_get_form_element_by_type ( $data_list, $v ['id'], $k, $val_recorded );
	$rows ['row_' . $v['id']] = dpistate_get_settings_row_data($v,isset($val_recorded[$v['id']])?$val_recorded[$v['id']]:array());
  }
  
  $form ['table'] = array (
    '#theme' => 'dpistate_table_theme',
    '#header' => array (
      t ( 'Name' ),
      t ( 'Options' ) ,
	  t ( 'Active' )
    ),
    'rows' => $rows 
  );
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  
  return $form;
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
    if (isset ( $val_info [1] ) && isset ( $val_info [0] ) && $val_info [0] == 'activate') {
      
      switch ($val_info [2]) {
        case 0 :
          $return [$val_info [1]] = array (
            'actif' => $v,
            'value' => $infos ['options_' . $val_info [1] . '_0'], 
          );
          break;
        case 1 :
          $return [$val_info [1]] = array (
            'actif' => $v,
            'value' => $form['table']['rows']['row_'.$val_info [1]]['options_'.$val_info [1].'_1']['#options'][$infos ['options_' . $val_info [1] . '_1']],
            'index' =>   $infos ['options_' . $val_info [1] . '_1'],
          );
          break;
        case 2 :
          $return [$val_info [1]] = array (
            'actif' => $v,
            'value' => 'Valeur date',
          );
          break;
      }
    }
  }
  return $return;
}

/**
 * This function return right form element necessary for the form
 *
 * @param array $data_list          
 * @param integer $v          
 * @param integer $k          
 * @return form element
 */
function dpistate_get_form_element_by_type($data_list, $v, $k, $records) {
  $return = NULL;
  switch ($data_list [$k] ['type']) {
    case 0 :
      $return = array (
        'label_' . $v . '_0' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_0' => array (
          '#type' => 'select',
          '#options' => range ( 0, $data_list [$k] ['value'] ),
          '#default_value' => (isset($records[$v]['value']))? $records[$v]['value'] : FALSE,
        ) ,
		
        'activate_' . $v . '_0' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset($records[$v]['actif']))? $records[$v]['actif'] : TRUE, 
        ),
      );
      break;
    case 1 :
      $return = array (
        'label_' . $v . '_1' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_1' => array (
          '#type' => 'select',
          '#options' => $data_list [$k] ['value'],
          '#default_value' => (isset($records[$v]['index']))? $records[$v]['index'] : FALSE,
        ),
		'activate_' . $v . '_1' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset($records[$v]['actif']))? $records[$v]['actif'] : TRUE,
        ), 
      );
      break;
    case 2 :
      $return = array (
        'label_' . $v . '_2' => array (
          '#markup' => $data_list [$k] ['label'] 
        ),
        'options_' . $v . '_2' => array (
          '#markup' => t ( 'Activate state : @state for date => @date', array (
            '@state' => $data_list [$k] ['label'],
            '@date' => $data_list [$k] ['value'],
          ) ) 
        ) ,
        'activate_' . $v . '_2' => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset($records[$v]['actif']))? $records[$v]['actif'] : TRUE,
        ),
      );
      break;
  }
  return $return;
}

function dpistate_get_settings_row_data($row, $values) {
	$options = array();
	
	switch ($row['type']) {
		case 0:
			$options = array (
			  '#type' => 'select',
			  '#options' => range ( 0, $row['value'] ),
			  '#default_value' => (isset($values['value']))? $values['value'] : FALSE,
			);
			break;
		case 1:
			$options = array (
			  '#type' => 'select',
			  '#options' => $row['value'],
			  '#default_value' => (isset($values['index']))? $values['index'] : FALSE,
			);
			break;
		case 2:
			//echo "ici<pre>".print_r($row['value'],true);die();
			$date_start = date('d/m/Y H:i',strtotime($row['value'][0]));
			$date_end = date('d/m/Y H:i',strtotime($row['value'][1]));
			$options = array (
				  '#markup' => t ( 'Activate state : @state for date => @datestart to @dateend', array (
					'@state' => $row['label'],
					'@datestart' => $date_start,
					'@dateend' => $date_end,
				  )));
			break;	
	}
	
	$return = array (
        'label_' . $row['id'] . '_' . $row['type'] => array (
          '#markup' => $row['label'] 
        ),
        'options_' . $row['id'] . '_' . $row['type'] => $options,
        'activate_' . $row['id'] . '_' . $row['type'] => array (
          '#type' => 'checkbox',
          '#title' => t ( 'Active' ),
          '#default_value' => (isset($values['actif']))? $values['actif'] : TRUE,
        ),
      );
	  
	 return $return;
}