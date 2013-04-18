<?php 


/**
 * Implementation of hook_form()
 *
 * @param $form
 * @param $form_state
 * @param $custom_content
 * @return array
 */
function custom_content_form($form, &$form_state, $custom_content,$op = 'edit') {
	
	$form['cid'] = array(
			'#type' => 'value',
			'#value'=> $custom_content->cid,
	);

	$form['name'] = array(
			'#type' => 'textfield',
			'#title' => t('name'),
			'#required' => TRUE,
			'#default_value' => $custom_content->name,
	);
	
	$form['title'] = array(
			'#type' => 'textfield',
			'#title' => t('title'),
			'#required' => TRUE,
			'#default_value' => $custom_content->title,
	);
	
	$form['body'] = array(
			'#title' => t('body'),
			'#type' => 'textarea',
			'#description' => t(''),
			'#required' => TRUE,
			'#default_value' => $custom_content->body,
	);

	$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save'),
	);


	field_attach_form('custom_content', $custom_content, $form, $form_state);

	return $form;
}

/**
 * Implementation of hook_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function custom_content_form_validate($form, $form_state){
	
	return TRUE;

	$customContent_submission = (object) $form_state['values'];

}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function custom_content_form_submit($form, &$form_state) {

	$customContent_submission = (object) $form_state['values'];
	$customContent_submission->date = time();
	field_attach_submit('custom_content', $customContent_submission, $form, $form_state);
	$form_state['conf']['custom_content'] = $customContent_submission;
	
	
	//say to entity api we use revision
	$customContent_submission->is_new = False;
	$customContent_submission->is_new_revision = TRUE;
	
	//set the default revision
	entity_revision_set_default('custom_content', $customContent_submission);
	
	//save the entity
	custom_content_save($customContent_submission);
	//$form_state['build_info']['args'][0]  = custom_content_load($form_state['subtype']['cid']);
	//$form_state['test'] = custom_content_load($form_state['subtype']['cid']);
	//and redirect to display the new entity
	$form_state['redirect'] = "custom_content/".$customContent_submission->cid;
}


