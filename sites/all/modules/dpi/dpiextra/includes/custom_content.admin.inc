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
	$body = unserialize($custom_content->body);
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
			'#type' => 'text_format',
			'#description' => t(''),
			'#required' => TRUE,
			'#default_value' => $body['value'],
			'#format' => $body['format'],
	);
	
	$form['new_revision'] = array(
			'#type' => 'checkbox',
			'#title' => 'create a new revision ?',
			'#default_value' => FALSE,
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
function custom_content_form_validate($form, &$form_state){
	//no realy need the form_validate
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function custom_content_form_submit($form, &$form_state) {

	//get the good value from the new_revision checkbox
	$form_state['values']['new_revision'] = $form_state['input']['new_revision'];

	$customContent_submission = (object) $form_state['values'];
	$customContent_submission->body = serialize($customContent_submission->body);
	$customContent_submission->date = time();
	
	field_attach_submit('custom_content', $customContent_submission, $form, $form_state);
	
	$customContent_submission->is_new = FALSE;
	$customContent_submission->is_new_revision = $form_state['values']['new_revision'];
	//don't create a new revision
	//overwrite the current revision
	if(!$form_state['values']['new_revision']){
		$entity = custom_content_load($customContent_submission->cid);
		$customContent_submission->revision_id = $entity->revision_id;
	}
	
	//set the default revision
	entity_revision_set_default('custom_content', $customContent_submission);
	//save the entity
	custom_content_save($customContent_submission);
	//put entity in formstate to find it later
	$form_state['custom_content']=$customContent_submission;

	//and redirect to display the new entity
	$form_state['redirect'] = "custom_content/".$customContent_submission->cid;
}


