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

	$form['machine_name'] = array(
			'#type' => 'value',
			'#value'=> $custom_content->machine_name,
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

	if(!isset($custom_content->is_new)){
		$form['revision'] = array(
				'#type' => 'radios',
				'#title' => t('About Revision'),
				'#options' => array(t('New revision'), t('Overwrite')),
				'#default_value' => 0,
		);
	}
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
    // Check for string identifier sanity
  /*  if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['name'])) {
      form_error($form['name'], t('The custom content has not added/modify because the name can only 
      		consist of lowercase letters, underscores, and numbers.'));
    }
    // Check for string identifier sanity
    if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['title'])) {
    	form_error($form['title'], t('The custom content has not added/modify because the title can only 
    			consist of lowercase letters, underscores, and numbers.'));
    }*/
	
	$customContent_submission = (object) $form_state['values'];
	$results = dpiextra_custom_content_get_all();
	if($results!=Null){
		foreach($results as $record){
			//validate title
			if($customContent_submission->cid != $record->cid){
				if($customContent_submission->title == $record->title){
					form_set_error('title',t('the title already exist'));
				}
			}
		}
	}
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function custom_content_form_submit($form, &$form_state) {
	
	if(isset($form_state['values']['revision'])){
		$is_revision = $form_state['values']['revision'];
	}
	else
	{
		$is_revision = FALSE;
	}
	unset($form_state['values']['revision']);
	$customContent_submission = (object) $form_state['values'];
	$customContent_submission->body = serialize($customContent_submission->body);
	$customContent_submission->machine_name = str_replace(' ', '_', $customContent_submission->title);
	$customContent_submission->date = time();
		
	field_attach_submit('custom_content', $customContent_submission, $form, $form_state);
		
	$customContent_submission->is_new = FALSE;
	$customContent_submission->is_new_revision = !$is_revision;
    dsm($customContent_submission,save);
	//overwrite the current revision
	if($is_revision){
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


