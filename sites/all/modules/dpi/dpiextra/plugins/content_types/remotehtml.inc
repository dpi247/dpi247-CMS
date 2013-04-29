<?php 

/**
 * Plugin definition.
 */
$plugin = array(
		'title' => t('Remote html'),
		'no title override' => TRUE,
		'defaults' => array('admin_title' => '', 'title' => '', 'body' => '', 'format' => filter_default_format(), 'substitute' => TRUE),
		'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
		// Make sure the edit form is only used for some subtypes.
		'edit form' => 'dpiextra_remotehtml_content_type_edit_form',
		'add form' => '',
		'edit text' => t('Edit'),
		'all contexts' => TRUE,
);


/**
 * Implementation of hook_content_type_content_types()
 * 
 * @return $remotehtml
 * all remotehtml
 */
function dpiextra_remotehtml_content_type_content_types(){
	
	$results = dpiextra_remotehtml_get_all();
	
	foreach($results as $record)
	{
		$remotehtml[$record->name]= array(
			'title' => t('remote html : '.$record->name),
			'rid' => $record->rid,
			'description' => t(''),
			'category' => t('Dpi extra'),
		);
		
	}
	form_load_include($form_state, 'inc', 'dpiextra','includes/remotehtml.admin');
	$remotehtml['custom']= array(
	    'name'           => 'remotehtml',
	    'title'          => t('New remote html'),
	    'top level'      => TRUE,
	    'category'       => t('Custom'),
	    'description'    => t('Create a completely custom piece of HTML content.'),
	    'edit form'      => 'dpiextra_remotehtml_content_type_edit_form',
	    'all contexts'   => TRUE,
	  );
	
	return  $remotehtml;
}
/**
 * Implementation of content_type_form()
 * 
 * @param $form
 * @param $form_state
 * @return $form
 */
function dpiextra_remotehtml_content_type_edit_form($form, &$form_state){
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/remotehtml.admin');
	//get the entity id from subtype or conf
	$entity=dpiextra_get_remotehtmlentity_from_subtype($form_state);
	//get the entity form and unset the save button
	$form = dpiextra_get_entity_form('remotehtml_form', $form_state, $entity);
	
	return $form;
	

}

/**
 * Implementation of edit_form_validate()
 * 
 * @param $form
 * @param $form_state
 */
function dpiextra_remotehtml_content_type_edit_form_validate($form, &$form_state){

  	$form_state2=array();
  	form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
  
  	// see: http://drupal.org/node/1651046 use drupal_retrieve_form and not drupal_get_form
  	$formFromRemotehtml = drupal_build_form('remotehtml_form',$form_state2,$entity);
  
  	$form_state2['values'] = $form_state['values'];
  	//unset the wrong token
	unset($formFromRemotehtml['#token']);
  	//validate the form
  	drupal_validate_form('remotehtml_form', $formFromRemotehtml, $form_state2);
}

/**
 * Implementation of content_type_form()
 * 
 * @param $form
 * @param $form_state
 */
function dpiextra_remotehtml_content_type_edit_form_submit($form, $form_state){
  
	$form_state2=array();
	//create a new form_state with just the values
	$form_state2['values']=$form_state['values'];
	
	$entity = dpiextra_get_remotehtmlentity_from_subtype($form_state);
	$form_state2['values']['rid'] = $entity->rid;
	
	//load form include
	form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
	//submit
	drupal_form_submit('remotehtml_form', $form_state2,$entity);

	$entity = $form_state2['remotehtml'];
	//for a new remotehtml when there is no rid in subtype
	//for the render function
	$form_state['conf']['remotehtml'] = $entity;
}

/**
 * Implementation of content_type_render()
 * 
 * @param $subtype
 * @param $conf
 * @param $panel_args
 * @param $context
 * @return stdClass
 * return the block
 */
function dpiextra_remotehtml_content_type_render($subtype, $conf, $panel_args, $context){
	$entity = $conf['remotehtml'];
	//create new block
	$block = new stdClass();
	$block->module = 'dpiextra';
	$block->title = $entity->name;	
	//get html of the url
	$block->content = file_get_contents($entity->html_adresse);

	return $block;
}


/**
 * this function get all remote html name from db
 *
 * @return $allRemotehtml
 * return all remote html
 */
function dpiextra_remotehtml_get_all(){
  	//get all remotehtml
  	$results = db_query('SELECT * FROM {dpiextra_remotehtml}');
  	return $results;
}

/**
 * this function get the entity with this id from form_state['subtype'] or 
 * $form_state['conf']['remotehtml']
 * 
 * @param $form_state
 * @return $entity
 * the entity
 */
function dpiextra_get_remotehtmlentity_from_subtype($form_state){
  	
	//get entity id from subtype
	if(isset($form_state['subtype']['rid'])){
    	$entity = remotehtml_load($form_state['subtype']['rid']);
  	}
  	//get entity id from conf
  	else if(isset($form_state['conf']['remotehtml'])){
  		$entity = remotehtml_load($form_state['conf']['remotehtml']->rid);
  	}
  	else{
    	watchdog('RemoteHTML','remote_html_tentity error when fetching the entity');
    	return false;
  	}
  	return $entity;
  
}