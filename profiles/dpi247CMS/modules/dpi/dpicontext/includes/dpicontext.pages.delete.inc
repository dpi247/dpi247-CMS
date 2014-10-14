
<?php 

function dpicontext_delete($id = NULL) {
	return drupal_get_form ( 'dpicontext_delete_form', $id );
}

function dpicontext_delete_form($form, &$form_state, $id){
	
	if (isset ( $id )) {
	
		$form ['store'] = array (
				'#type' => 'value',
				'#value' => $id
		);
	}

	$form ['add'] = array (
			'#title' => t ( 'Delete context ?' ),
			'#type' => 'fieldset'
	);
	$form ['add']['submit'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Submit' )
	);
	return $form;
	
}


/**
 * **
 * Process form submission.
 */
function dpicontext_delete_form_submit($form, $form_state) {
	
	if(dpicontext_api_delete($form_state ['values'] ['store'] )){
		drupal_set_message ( 'Data deleted successfully' );
	}else{
		drupal_set_message ( 'Error while deleting data','error' );
	}

		drupal_goto ( 'admin/config/dpicontext/settings' );
}