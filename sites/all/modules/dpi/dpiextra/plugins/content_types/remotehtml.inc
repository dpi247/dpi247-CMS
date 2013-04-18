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
		//'edit form' => 'dpiextra_remotehtml_content_type_edit_form',
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
			'title' => t('remotehtml : '.$record->name),
			'rid' => $record->rid,
			'description' => t(''),
			//'required context' => new ctools_context_required(t('Destination'), 'destination'),
			'category' => t('Dpi extra'),
			//'all contexts' => TRUE,
		);
		
	}
	form_load_include($form_state, 'inc', 'dpiextra','includes/remotehtml.admin');
	$remotehtml['custom']= array(
	    'name'           => 'remotehtml',
	    'title'          => t('New remote html'),
	    'top level'      => TRUE,
	    'category'       => t('Custom'),
	    'description'    => t('Create a completely custom piece of HTML content.'),
	    'edit form'      => 'remotehtml_form',
	    'all contexts'   => TRUE,
	    //'check editable' => 'wallyextra_custom_version_content_type_editable',
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

	if(empty($form_state['input']['name'])){
		$entity = remotehtml_load($form_state['subtype']['rid']);
		$form_state['build_info']['args'][] = $entity;
	}
	else
	{
		$form_state['build_info']['args'][] = $form_state['input'];
	}

	$tempForm = $form;

	form_load_include($form_state, 'inc', 'dpiextra','includes/remotehtml.admin');
	
	
	//$formFromRemotehtml = drupal_retrieve_form('remotehtml_form', $form_state);
	$formFromRemotehtml = drupal_build_form('remotehtml_form',$form_state);
	//$formFromRemotehtml3 = entity_ui_get_form('remotehtml',$entity,'edit',$form_state);
	
	$form = $formFromRemotehtml;

	$form['buttons'] = $tempForm['buttons'];

	return $form;
	

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
 * Implementation of content_type_form()
 * 
 * @param $form
 * @param $form_state
 */
function dpiextra_remotehtml_content_type_edit_form_submit($form, $form_state){
	$remotehtml_submission = (object) $form_state['values'];
	remotehtml_save($remotehtml_submission);
	//$form_state['conf']['url'] = $form_state['values']['html_adresse']; 
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
	//create new block
	$block = new stdClass();
	$block->module = 'dpiextra';
	$block->title = 'HTML Block';

	//$htmlblock = dpiextra_remotehtml_get_cache_or_create('cache_remotehtml',$conf);
	//get html of the url
	$block->content = file_get_contents($conf['url']);;

	return $block;
}
function dpiextra_remotehtml_content_type_edit_form_validate($form, $form_state){

	//return TRUE;

	$remotehtml_submission = (object) $form_state['values'];

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
