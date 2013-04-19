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
			'title' => t($record->name),
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
	//dsm($remotehtml);
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
	
  dsm($form_state,outer_form_state);
	$entity=dpiextra_get_remotehtmlentity_from_subtype($form_state);
  dsm($entity,ENTITY);
	
	$form_state['build_info']['args'][] = $entity;
	
	form_load_include($form_state, 'inc', 'dpiextra','includes/remotehtml.admin');
	
  // see: http://drupal.org/node/1651046 use drupal_retrieve_form and not drupal_get_form
	$form = drupal_retrieve_form('remotehtml_form',$form_state,$entity);
	

	return $form;
	

}




function dpiextra_remotehtml_content_type_edit_form_validate($form, $form_state){
dsm($form_state,FS_VALIDATE);

  $form_state2=array();
  form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
  
  // see: http://drupal.org/node/1651046 use drupal_retrieve_form and not drupal_get_form
  $formFromRemotehtml = drupal_retrieve_form('remotehtml_form',$form_state2,$entity);
  
  $form_state2['values']=$form_state['values'];
  $form_state['build_info']['args'] = array();
  drupal_validate_form('remotehtml_form', $formFromRemotehtml, $form_state2);
  

}

/**
 * Implementation of content_type_form()
 * 
 * @param $form
 * @param $form_state
 */
function dpiextra_remotehtml_content_type_edit_form_submit($form, $form_state){
  dsm($form_state,FS_SUBM);
  
  $form_state2=array();
  $form_state2['values']=$form_state['values'];

  $entity=dpiextra_get_remotehtmlentity_from_subtype($form_state);
  form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
  drupal_form_submit('remotehtml_form', $form_state2,$entity);
  
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

function dpiextra_get_remotehtmlentity_from_subtype($form_state){
  if(isset($form_state['subtype']['rid'])){
    $entity = remotehtml_load($form_state['subtype']['rid']);
  }
  else{
    watchdog('RemoteHTML','remote_html_tentity error when fetching the entity');
    return false;
  }
  return $entity;
  
}
