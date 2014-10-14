<?php

/**
 * @file
 * Plugin to provide access control/visibility based on length of
 * simplecontext argument (in URL).
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array (
		'title' => t ( "dpicontext status" ),
		'description' => t ( 'Control access by dpicontext status argument.' ),
		'callback' => 'dpicontext_status_ctools_access_check',
		'settings form' => 'dpicontext_status_ctools_access_settings',
		'summary' => 'dpicontext_status_ctools_access_summary' 
);
// 'required context' => new ctools_context_required(t('Simplecontext'), 'simplecontext'),

/**
 *
 * @param int $dpicontext_status        	
 * @return int $dpicontext_value
 */
function get_dpicontext_status() {
	$dpicontext_status = dpi_variable_get ( 'dpicontext_level', 5 );
	
	$dpicontext_value = $min = 1;
	$max = 5;
	if (($min <= $dpicontext_status) && ($dpicontext_status <= $max)) {
		$dpicontext_value = $dpicontext_status;
	}
	
	return $dpicontext_value;
}

/**
 * Settings form for the 'by role' access plugin.
 */
function dpicontext_status_ctools_access_settings(&$form, &$form_state, $conf) {
	
	// build header
	$header [] = array (
			'data' => "Label" 
	);
	$header [] = array (
			'data' => "Operation" 
	);
	$header [] = array (
			'data' => "Value" 
	);
	
	$data_list = array ();
	
	$data_list = dpicontext_api_get_all_context ();
	
	$output_table = array ();
	foreach ( $data_list as $k => $v ) {
		$text = "";
		
		if (is_array ( $v ['value'] )) {
			
			foreach ( $v ['value'] as $str ) {
				// if date then modify date format from y-m-d to d-m-y
				if ($v ['type'] == 2) {
					$str = date ( "d-m-Y H:i", strtotime ( $str ) );
				}
				$text .= $str . ' - ';
			}
			$text = substr ( $text, 0, - 3 );
			$data_list [$k] ['value'] = $text;
			$text = "";
		}
		
		// We only want to render 3 column from data_list array : Label / Type ( wich will result in a form of type select) / value (also render as a form select)
		
		$data = array (
				'label' => $data_list [$k] ['label'],
				'type' => $data_list [$k] ['type'],
				'value' => $data_list [$k] ['value'] 
		);
		
		// TODO create function to handle $data creation
		// insert the right selector for the given type
		switch ($data ['type']) {
			case 0 :
				$data ['type'] = array (
						'data' => array (
								array (
										'#type' => 'select',
										'#options' => array (
												">",
												"=",
												"<" 
										) 
								) 
						) 
				);
				$data ['value'] = array (
						'data' => array (
								array (
										'#type' => 'select',
										'#options' => range ( 0, $data ['value'] ) 
								) 
						) 
				);
				break;
			case 1 :
				$data ['type'] = array (
						'data' => array (
								array (
										'#type' => 'select',
										'#options' => array (
												"=" 
										) 
								) 
						) 
				);
				break;
			case 2 :
				
				// TODO date picker to choose interval between start date & end date ?
				$data ['type'] = array (
						'data' => array (
								array (
										'#type' => 'select',
										'#options' => array (
												"between",
												"around" 
										) 
								) 
						) 
				);
				break;
		}
		
		dsm ( $output_table );
		$output_table [] = $data;
		unset ( $data );
	}
	
	// $form ['contexts'] = array (
	
	// '#type' => 'fieldset',
	// '#title' => t ( 'Administrate Context\'s settings' ),
	
	// );
	
	// $form ['contexts']['fields'] = array (
	
	// '#type' => 'fieldset',
	// '#title' => t ( 'Administrate Context\'s settings' ),
	
	// );
	
	// $form ['submit'] = array (
	// '#type' => 'submit',
	// '#value' => t ( 'Submit' )
	// );
	
	$variables = array (
			
			'header' => $header,
			'rows' => $output_table,
			'attributes' => array (),
			'caption' => NULL,
			'colgroups' => array (),
			'sticky' => FALSE,
			'empty' => "no value" 
	);
	
	return theme_table ( $variables );
	// $form ['settings'] ['greater_than'] = array (
	// '#type' => 'radios',
	// '#title' => t ( 'Grant access if dpicontext value is' ),
	// '#options' => array (
	// 0 => t ( 'Equal to' ),
	// 1 => t ( 'Greater than' ),
	// 2 => t ( 'Less than' )
	// ),
	// '#default_value' => $conf ['greater_than']
	// );
	// $form ['settings'] ['dpicontext_value'] = array (
	// '#type' => 'radios',
	// '#title' => t ( 'dpicontext value' ),
	// '#description' => t ( 'Access/visibility will be granted based on dpicontext value.' ),
	// '#options' => array (
	// 5 => t ( '5' ),
	// 4 => t ( '4' ),
	// 3 => t ( '3' ),
	// 2 => t ( '2' ),
	// 1 => t ( '1' ),
	// 0 => t('Always show')
	// ),
	// '#default_value' => $conf ['dpicontext_value']
	// );
}

/**
 * Check for access.
 */
function dpicontext_status_ctools_access_check($conf, $context = NULL) {
	
	// As far as I know there should always be a context at this point, but this
	// is safe.
	if (! isset ( $context ) && (empty ( $context ) || empty ( $context->data ))) {
		return FALSE;
	}
	
	// if always is check then show whatever dpicontext has been set
	if (! $conf ['dpicontext_value'])
		return TRUE;
	
	$dpicontext_status = get_dpicontext_status ();
	
	switch ($conf ['greater_than']) {
		
		// equal to
		case 0 :
			if ($conf ['dpicontext_value'] == $dpicontext_status)
				return TRUE;
			break;
		// greater than
		case 1 :
			if ($conf ['dpicontext_value'] < $dpicontext_status)
				return TRUE;
			break;
		// lesser than
		case 2 :
			if ($conf ['dpicontext_value'] > $dpicontext_status)
				return TRUE;
			break;
		
		default :
			return FALSE;
	}
	
	return FALSE;
}

/**
 * Provide a summary description based upon the checked roles.
 */
function dpicontext_status_ctools_access_summary($conf, $context) {
	if ($conf ['greater_than'] == 2) {
		
		$comp = 'less than';
	} else
		$comp = ! $conf ['greater_than'] ? 'equal to' : 'greater than';
	
	return t ( 'dpicontext value must be !comp @length ', array (
			'!comp' => $comp,
			'@length' => $conf ['dpicontext_value'] 
	) );
}


