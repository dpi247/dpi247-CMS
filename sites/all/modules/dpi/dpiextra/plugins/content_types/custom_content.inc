<?php 

/**
 * Plugin definition.
 */
$plugin = array(
		'title' => t('custom content'),
		'no title override' => TRUE,
		'defaults' => array('admin_title' => '', 'title' => '', 'body' => '', 'format' => filter_default_format(), 'substitute' => TRUE),
		'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
		// Make sure the edit form is only used for some subtypes.
		'edit form' => array(
				'dpiextra_custom_content_content_type_edit_form'=>'custom content',
				'dpiextra_custom_contentEditable_content_type_edit_form'=>'edit',
				'dpiextra_view_revision_content_type_edit_form'=>'view revision',
				),
		//'add form' => 'dpiextra_custom_contentNew_content_type_edit_form',
		'edit text' => t('Edit'),
		'all contexts' => TRUE,
);
/**
 * Implementation of edit_form() to display revision
 *
 * @param $form
 * @param $form_state
 * @return $form
 */
function dpiextra_view_revision_content_type_edit_form($form, $form_state){
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
	$entity=dpiextra_get_custom_contententity_from_subtype($form_state);
	$entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
	
	$form['cid']['#value'] = $entity->cid;
	//unset buttons return and cancel
	unset($form['buttons']['return']);
	unset($form['buttons']['cancel']);
	unset($form['buttons']['next']);
	//change the step of the previous button
	$form['buttons']['previous']['#next'] = 'step1';
	
	return $form;
	
}
/**
 * Implementation of edit_form_submit() to display revision
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_view_revision_content_type_edit_form_submit($form, $form_state){
	
}
/**
 * Implementation of hook_content_type_content_types()
 *
 * @return $custom_content
 * all custom content
*/
function dpiextra_custom_content_content_type_content_types(){

	$results = dpiextra_custom_content_get_all();
	
	foreach($results as $record)
	{
		//existant content type
		$custom_content[$record->machine_name]= array(
				'title' => t('custom content version : '.$record->title),
				'cid' => $record->cid,
				'description' => t(''),
				'category' => t('Dpi extra'),
				'subtype'=> $record->machine_name,
		);

	}
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
	//new custom content type
	$custom_content['custom']= array(
			'name'           => 'custom_version',
			'title'          => t('New custom content version'),
			'top level'      => TRUE,
			'category'       => t('Custom'),
			'description'    => t('Create a completely custom piece of custom content.'),
			'add form' 		 => 'dpiextra_custom_contentNew_content_type_edit_form',
			'all contexts'   => TRUE,
	);

	return  $custom_content;
}

/**
 * Implementation of edit_form() for a new custom content block
 * 
 * @param $form
 * @param $form_state
 * @return $form 
 */
function dpiextra_custom_contentNew_content_type_edit_form($form,$form_state){

	//$form_state['pane']->pid = 'new';
	//$form_state['pane']->subtype = 'custom';
	dsm($form_state['display']->content);
	$form = dpiextra_get_entity_form('custom_content_form', $form_state);
	unset($form['revision']);
	dsm($form_state,Newform);
	dsm($form_state['display cache'],cache);
	dsm($form_state['pane'],NEwformpane);
	return $form;
}
/**
 * Implementation of edit_form_validate() for a new custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_contentNew_content_type_edit_form_validate($form, &$form_state){

	$form_state2=array();
	form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
	//build form
	$formFromCustomContent = drupal_build_form('custom_content_form',$form_state2,$entity);

	$form_state2['values'] = $form_state['values'];
	//unset the wrong token
	unset($formFromCustomContent['#token']);
	//validate the form
	drupal_validate_form('custom_content_form', $formFromCustomContent, $form_state2);
}
/**
 * Implementation of edit_form_submit() for a new custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_contentNew_content_type_edit_form_submit($form,$form_state){
	
	$form_state2=array();
	$form_state2['values']=$form_state['values'];
	//load form include
	form_load_include($form_state2, 'inc', 'dpiextra','includes/custom_content.admin');
	//submit
	drupal_form_submit('custom_content_form', $form_state2);
	//for the render function
	$form_state['conf']['custom_content'] = $form_state2['custom_content'];
	$form_state['pane']->subtype = $form_state2['custom_content']->machine_name;
	dsm($form_state['display']->content);
	dsm($form_state,Newformsubmit);
	dsm($form_state['pane'],NEwformSubmitpane);
}
/**
 * Implementation edit_form() for edit a custom content block
 *
 * @param $form
 * @param $form_state
 * @return $form
 */
function dpiextra_custom_contentEditable_content_type_edit_form($form,$form_state){
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
	//get the entity
	$entity=dpiextra_get_custom_contententity_from_subtype($form_state);
	$entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
	
	$form = dpiextra_get_entity_form('custom_content_form', $form_state,$entity);

	//unset the buttons next and cancel
	unset($form['buttons']['next']);
	unset($form['buttons']['cancel']);
	//add a save button
	$form['buttons']['save'] = array(
			'#type' => 'submit',
			'#value' => 'Save',
			'#next' => 'step1',
			'#wizard type' => 'next',
			'#weight' => 1000,
			);
dsm($form_state,editableform);
dsm($form_state['pane'],editableformpane);
	return $form;
}
/**
 * Implementation edit_form_validate() for edit a custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_contentEditable_content_type_edit_form_validate($form, &$form_state){

	$form_state2=array();
	form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
	//build form
	$formFromCustomContent = drupal_build_form('custom_content_form',$form_state2,$entity);

	$form_state2['values'] = $form_state['values'];
	//unset the wrong token
	unset($formFromCustomContent['#token']);
	//validate the form
	drupal_validate_form('custom_content_form', $formFromCustomContent, $form_state2);
}
/**
 * Implementation edit_form_submit() for edit a custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_contentEditable_content_type_edit_form_submit($form,$form_state){

	$form_state2=array();
	$form_state2['values']=$form_state['values'];
	//get entity
	$entity = dpiextra_get_custom_contententity_from_subtype($form_state);
	$entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
	
	$form_state2['values']['cid'] = $entity->cid;
	//load form include
	form_load_include($form_state2, 'inc', 'dpiextra','includes/custom_content.admin');
	//submit
	drupal_form_submit('custom_content_form', $form_state2,$entity);

}
/**
 * Implementation of content_type_form()
 *
 * @param $form
 * @param $form_state
 * @return $form
 */
function dpiextra_custom_content_content_type_edit_form($form, $form_state){
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
	//get the entity
	dsm($form_state);
	$entity=dpiextra_get_custom_contententity_from_subtype($form_state);
	$entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
	//when the revision is changing the subtype is overwrite so the entity
	//is saved in $form_state['conf']['custom_content']
	$form_state['conf']['custom_content'] = $entity;
	//unserialize the body and the format
	$body = unserialize($entity->body);
	
	$form['machine_name'] = array(
			'#markup' => '<p>'.$entity->machine_name.'</p>'
		);
	$form['title'] = array(
			'#markup' => '<p>'.$entity->title.'</p>'
		);
	$form['body'] = array(
			'#markup' => '<p>'.$body['value'].'</p>'
		);
		
	//Buttons
	$form['modify'] = array(
			'#type' => 'submit',
			'#value' => t('Modify'),
			'#next' => 'step2',
			'#wizard type' => 'next',
		);
	

	$form['revision'] = array(
			'#type' => 'submit',
			'#value' => t('Revision'),
			'#next' => 'step3',
			'#wizard type' => 'next',
		);
	
	unset($form['buttons']);
	$form['buttons']['return'] = array(
			'#type' => 'submit',
			'#value' => 'Finish',
			'#wizard type' => 'finish',
		);
		

	$form['buttons']['cancel'] = array(
			'#type' => 'submit',
			'#value' => 'Cancel',
			'#wizard type' => 'cancel',
	);
	dsm($form_state,editform);
	dsm($form_state['pane'],editformpane);
	return $form;
}

/**
 * Implementation of content_type_edit_form_submit()
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_edit_form_submit($form,&$form_state){
	dsm(ccc);
	dsm($form_state);
	dsm($form_state['pane']);
	if($form_state['clicked_button']['#wizard type'] == 'finish'){
		//get entity*/
		$entity = dpiextra_get_custom_contententity_from_subtype($form_state);
		$entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
		
		//for the render function
		$form_state['conf']['custom_content'] = $entity;
		//$form_state['pane']->subtype = $entity->machine_name;
	}
}
/**
 * Implementation of content_type_render()
 * @param $subtype
 * @param $conf
 * @param $panel_args
 * @param $context
 */
function dpiextra_custom_content_content_type_render($subtype, $conf, $panel_args, $context){
	dsm($subtype);
	$block = new stdClass();
	//get the custom content entity
	$entity = $conf['custom_content'];
	$entity->body = unserialize($entity->body);
	//create the renderable block
	$block->module = 'dpiextra';
	$block->title = t($entity->title);
	$block->content = check_markup($entity->body['value'], $entity->body['format']);
	
	return $block;
}
/**
 * this function get all custom content from db
 * 
 * @return $results
 * the results from db
 */
function dpiextra_custom_content_get_all(){
	//get all remotehtml
	$results = db_query('SELECT * FROM {dpiextra_custom_content}');
	return $results;
}

/**
 * this function get the entity with this id from form_state['subtype']
 *
 * @param $form_state
 * @return $entity
 * the entity
 */
function dpiextra_get_custom_contententity_from_subtype($form_state){
	
	if(isset($form_state['subtype']['cid'])){
		$entity = custom_content_load($form_state['subtype']['cid']);
	}
	else if(isset($form_state['conf']['custom_content']))
	{
		$entity = custom_content_load($form_state['conf']['custom_content']->cid);
	}
	else{
		watchdog('CustomContent','custom_content entity error when fetching the entity');
		return false;
	}
	return $entity;

}

function _dpiextra_custom_content_get_entity_by_subtype($subtype){
	
	$a=new Custom_contentController('custom_content');
	return $a->load_by_machine_name($subtype);
	
}
