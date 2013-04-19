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
				//'required context' => new ctools_context_required(t('Destination'), 'destination'),
				'category' => t('Dpi extra'),
				//'all contexts' => TRUE,
		);

	}
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
	$custom_content['custom']= array(
			'name'           => 'custom_version',
			'title'          => t('New custom content version'),
			'top level'      => TRUE,
			'category'       => t('Custom'),
			'description'    => t('Create a completely custom piece of custom content.'),
			'edit form'      => 'custom_content_form',
			'all contexts'   => TRUE,
			//'check editable' => 'wallyextra_custom_version_content_type_editable',
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
function dpiextra_custom_content_content_type_edit_form($form, &$form_state){
	
	if(isset($form_state['input']['name'])){
		$entity = custom_content_load($form_state['subtype']['cid']);
		foreach($form_state['input'] as $key => $val)
		{
			if($key == 'op')
			{
				
			}
			else
			{
				$entity->$key = $val;
			}
		}
	}
	else
	{
		$entity = custom_content_load($form_state['subtype']['cid']);
	}

	
	$tempForm = $form;

	form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');

	$form_state['build_info']['args'][] = $entity;

	//$formFromRemotehtml = drupal_build_form('custom_content_form',$form_state);
	$formFromRemotehtml = entity_ui_get_form('custom_content',$entity);
	$form = $formFromRemotehtml;
	
	$form['buttons'] = $tempForm['buttons'];
	return $form;
	//return theme('dpiextra_custom_content',$form);
}
/**
 * Implementation of content_type_edit_form_submit()
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_edit_form_submit($form,$form_state){
	
	$form_state['values'] = $form_state['input'];
	$form_state['values']['cid'] = $form_state['subtype']['cid'];
	drupal_form_submit('custom_content_form_submit', $form_state);

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
	$custom_content = $conf['custom_content'];
	
	$block->title = t($custom_content->title);
	$block->content = $custom_content->body;
	
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
