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
dsm($form_state,build1);
dsm($remotehtml);
	$form['rid'] = array(
			'#type' => 'value',
			'#value'=> $remotehtml->rid,
	);

	$form['name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name'),
			'#required' => TRUE,
			'#default_value' => $remotehtml->name,
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
function remotehtml_form_validate($form, $form_state){
//dsm($form_state);
//return TRUE;
dsm('validate');
	$remotehtml_submission = (object) $form_state['values'];
	
	dsm($remotehtml_submission->html_adresse);
	//validate url
	if(!remotehtml_url_verification($remotehtml_submission->html_adresse))
	{
		form_set_error('html adresse',t('you must enter a valid url adresse'));
	}
	//validate the refresh time
	if(!is_numeric($remotehtml_submission->refresh_time))
	{
		form_set_error('refresh time',t('you must enter a number for the refresh time'));
	}
	
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function remotehtml_form_submit($form, &$form_state) {

	dsm('submit');
	
	$remotehtml_submission = (object) $form_state['values'];
	field_attach_submit('remotehtml', $remotehtml_submission, $form, $form_state);
	//save the entity
	$form_state['conf']['url'] = $remotehtml_submission->html_adresse;
	dsm($remotehtml_submission,TOSAVE);
	remotehtml_save($remotehtml_submission);
	//and redirect to display the new entity
	$form_state['redirect'] = "remotehtml/".$remotehtml_submission->rid;
}


