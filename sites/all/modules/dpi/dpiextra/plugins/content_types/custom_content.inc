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
		'edit form' => 'dpiextra_custom_content_content_type_edit_form',
		'add form' => '',
		'edit text' => t('Edit'),
		'all contexts' => TRUE,
);


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
		$custom_content[$record->name]= array(
				'title' => t('custom content version : '.$record->title),
				'cid' => $record->cid,
				'description' => t(''),
				'category' => t('Dpi extra'),
		);

	}
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
	$custom_content['custom']= array(
			'name'           => 'custom_version',
			'title'          => t('New custom content version'),
			'top level'      => TRUE,
			'category'       => t('Custom'),
			'description'    => t('Create a completely custom piece of custom content.'),
			'edit form'      => 'dpiextra_custom_content_content_type_edit_form',
			'all contexts'   => TRUE,
	);

	return  $custom_content;
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
	$entity=dpiextra_get_custom_contententity_from_subtype($form_state);
	
	$form = dpiextra_get_entity_form('custom_content_form', $form_state,$entity);

	return $form;
}

function dpiextra_custom_content_content_type_edit_form_validate($form, $form_state){

	$form_state2=array();
  	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
  
  	// see: http://drupal.org/node/1651046 use drupal_retrieve_form and not drupal_get_form
  	$formFromRemotehtml = drupal_build_form('custom_content_form',$form_state,$entity);
  
  	$form_state2['values']=$form_state['values'];

 	drupal_validate_form('custom_content_form', $formFromRemotehtml, $form_state2);
	$form_state = $form_state2;
}
/**
 * Implementation of content_type_edit_form_submit()
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_edit_form_submit($form,&$form_state){

  	$form_state2=array();
  	$form_state2['values']=$form_state['values'];
  	//get entity
  	$entity = dpiextra_get_custom_contententity_from_subtype($form_state);
  	$form_state2['values']['cid'] = $entity->cid;
  	//load form include
  	form_load_include($form_state2, 'inc', 'dpiextra','includes/custom_content.admin');
  	//submit
  	drupal_form_submit('custom_content_form', $form_state2,$entity);
  
  	$entity = $form_state2['custom_content'];
  	//for a new custom content when there is no cid in subtype
  	//for the render function
  	$form_state['conf']['custom_content'] = $entity;
dsm($form_state);
}
/**
 * Implementation of content_type_render()
 * @param $subtype
 * @param $conf
 * @param $panel_args
 * @param $context
 */
function dpiextra_custom_content_content_type_render($subtype, $conf, $panel_args, $context){
	
	$block = new stdClass();
	//get the custom content entity
	$entity = $conf['custom_content'];
	$entity->body = unserialize($entity->body);
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
 * this function get the entity with this id from form_state['subtype'] or
 * $form_state['conf']['custom_content']
 *
 * @param $form_state
 * @return $entity
 * the entity
 */
function dpiextra_get_custom_contententity_from_subtype($form_state){
	if(isset($form_state['subtype']['cid'])){
		$entity = custom_content_load($form_state['subtype']['cid']);
	}
	else if(isset($form_state['conf']['custom_content'])){
		$entity = custom_content_load($form_state['conf']['custom_content']->cid);
	}
	else{
		watchdog('CustomContent','custom_content entity error when fetching the entity');
		return false;
	}
	return $entity;

}
