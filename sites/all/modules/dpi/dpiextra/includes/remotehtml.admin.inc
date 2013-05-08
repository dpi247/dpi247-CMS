<?php 

/**
 * Implementation of hook_form()
 *
 * @param $form
 * @param $form_state
 * @param $remotehtml
 * @return array
 */
function remotehtml_form($form, &$form_state, $remotehtml,$op = 'edit') {

	$form_state['values']['rid']=$remotehtml->rid;
	$form['rid'] = array(
			'#type' => 'value',
			'#value'=> $remotehtml->rid,
	);
	$form['machine_name'] = array(
			'#type' => 'value',
			'#value'=> $remotehtml->machine_name,
	);
	$form['title'] = array(
			'#type' => 'textfield',
			'#title' => t('Title'),
			'#required' => TRUE,
			'#default_value' => $remotehtml->title,
	);
	$form['html_adresse'] = array(
			'#type' => 'textfield',
			'#title' => t('url adresse'),
			'#required' => TRUE,
			'#default_value' => $remotehtml->html_adresse,
	);

	$form['refresh_time'] = array(
			'#type' => 'textfield',
			'#title' => t('refresh time in minutes'),
			'#required' => TRUE,
			'#default_value'=> $remotehtml->refresh_time,
	);

	$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save'),
	);

	
	field_attach_form('remotehtml', $remotehtml, $form, $form_state);

	return $form;
}

/**
 * Implementation of hook_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function remotehtml_form_validate($form, &$form_state){
	$remotehtml_submission = (object) $form_state['values'];
	$results = dpiextra_remotehtml_get_all();
	if($results!=Null){
		foreach($results as $record){
			//validate label
			if($remotehtml_submission->rid != $record->rid){
				if($remotehtml_submission->title == $record->title){
					form_set_error('title',t('the label already exist'));
				}
			}
		}
	}
	
	//validate url
	if(!remotehtml_url_verification($remotehtml_submission->html_adresse))
	{
		form_set_error('html_adresse',t('you must enter a valid url adresse'));
	}
	//validate the refresh time
	if(!is_numeric($remotehtml_submission->refresh_time))
	{
		form_set_error('refresh_time',t('you must enter a number for the refresh time'));
	}
	
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function remotehtml_form_submit($form, &$form_state) {

	
	$remotehtml_submission = (object) $form_state['values'];
	$remotehtml_submission->machine_name = str_replace(' ', '_', $remotehtml_submission->title);
	field_attach_submit('remotehtml', $remotehtml_submission, $form, $form_state);
	/*if($remotehtml_submission->machine_name == NULL){
		$remotehtml_submission->is_new = TRUE;
		$remotehtml_submission->machine_name = str_replace(' ', '_', $remotehtml_submission->label);
	}
	else{
		$remotehtml_submission->is_new = False;
	}*/
	//save the entity
	remotehtml_save($remotehtml_submission);
	//put entity in formstate to find it later
	$form_state['remotehtml'] = $remotehtml_submission;
	
	//and redirect to display the new entity
	$form_state['redirect'] = "remotehtml/".$remotehtml_submission->rid;
}


