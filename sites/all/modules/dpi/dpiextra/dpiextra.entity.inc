<?php

/**
 * Implementation of hook_entity_info()
 *
 * @return $entities
 * an array of definition
 */
function dpiextra_entity_info(){
	//create entities info
	$entities = array();
	//remotehtml
	$entities['remotehtml'] = array(
			'label' => t('Remote Html'),
			'entity class' => 'RemoteHtmlEntity',
			'controller class' => 'RemotehtmlController',
			'base table' => 'dpiextra_remotehtml',
			'fieldable' => True,
			'label callback' => 'entity_class_label',
			'uri callback' => 'entity_class_uri',
			'entity keys' => array(
					'id'=>'rid',
					'name' => 'name',
						
			),
			
			'bundles' => array(
					'remotehtml'=>array(
					),
			),
			
			'view modes' => array(
					'minimal' => array(
							'label' => t('Minimal'),
							'custom settings' => FALSE,
					),
					'full' => array(
							'label' => t('Full'),
							'custom settings' => TRUE,
					),
			),
				
			//Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
			'module'=> 'dpiextra',
			'access callback' => 'remotehtml_access_admin_callback',
			// Enable the entity API's admin UI.
			'admin ui' => array(
			
					'path' => 'admin/structure/remotehtml',
					// OPTIONAL- See "Customize the User Interface" below if you set.
					'file' => 'includes/remotehtml.admin.inc',
			),
	);
	
	//custom content
	$entities['custom_content'] = array(
			'label' => t('Custom content'),
			'entity class' => 'Custom_contentEntity',
			'controller class' => 'Custom_contentController',
			'base table' => 'dpiextra_custom_content',
			'revision table' => 'dpiextra_custom_content_revision',
			'fieldable' => True,
			'label callback' => 'entity_class_label',
			'uri callback' => 'entity_class_uri',
			'entity keys' => array(
					'id'=>'cid',
					'name'=>'name',
					'revision' => 'revision_id',
			),
				
			'bundles' => array(
					'custom_content'=>array(
					),
			),
				
			'view modes' => array(
					'minimal' => array(
							'label' => t('Minimal'),
							'custom settings' => FALSE,
					),
					'full' => array(
							'label' => t('Full'),
							'custom settings' => TRUE,
					),
			),
	
			//Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
			'module'=> 'dpiextra',
			'access callback' => 'custom_content_access_admin_callback',
			// Enable the entity API's admin UI.
			'admin ui' => array(
					'path' => 'admin/structure/custom_content',
					// OPTIONAL- See "Customize the User Interface" below if you set.
					'file' => 'includes/custom_content.admin.inc',
			),
	);
	return $entities;
}

/**
 * Implement the access_admin_callback() for the remotehtml
 * @return boolean
 */
function remotehtml_access_admin_callback(){

	return TRUE;
}
/**
 * Implement the access_admin_callback() for the custom content
 * @return boolean
 */
function custom_content_access_admin_callback(){
	return TRUE;
}

/**
 * load a remotehtml entity with this id
 *
 * @param $tid
 * @param $reset
 */
function remotehtml_load($tid = NULL, $reset = FALSE){

	$tids = (isset ($tid) ? array($tid) : array());

	$remotehtml = remotehtml_load_multiple($tids, $reset);

	return $remotehtml ? reset ($remotehtml) : FALSE;
}

/**
 * load a custom content entity with this id
 *
 * @param $tid
 * @param $reset
 */
function custom_content_load($tid = NULL, $reset = FALSE){

	$tids = (isset ($tid) ? array($tid) : array());

	$custom_content = custom_content_load_multiple($tids, $reset);

	return $custom_content ? reset ($custom_content) : FALSE;
}

/**
 * retrieve multiple remotehtml entities
 *
 * @param $tids
 * their ids
 * @param $conditions
 *
 * @param $reset
 *
 * @return object
 * return the entities
 */
function remotehtml_load_multiple($tids = array(), $conditions = array(), $reset = FALSE){

	return entity_load('remotehtml', $tids, $conditions, $reset);
}

/**
 * retrieve multiple custom content entities
 *
 * @param $tids
 * their ids
 * @param $conditions
 *
 * @param $reset
 *
 * @return object
 * return the entities
 */
function custom_content_load_multiple($tids = array(), $conditions = array(), $reset = FALSE){

	return entity_load('custom_content', $tids, $conditions, $reset);
}

/**
 * this function verify the validity of an url
 *
 * @param $url
 * the url
 * @return $isGood
 * return a boolean
 */
function remotehtml_url_verification($url){

	if (isset($url))
	{
		//http request on the url
		$uri_response = drupal_http_request($url, array(), 'GET', NULL);
		
		if ($uri_response->status_message != 'OK')
		{
			$isGood = false;
		}
		else
		{
			$isGood = true;
		}
	}

	return $isGood;
}

/**
 * return the name of the given remotehtml entities
 *
 * @param $remotehtml
 * the given entities (remote html)
 */
function remotehtml_page_title($remotehtml){

	return $remotehtml->name;
}


/**
 * return the name of the given custom content entities
 *
 * @param $custom_content
 * the given entities (custom_content)
 */
function custom_content_page_title($custom_content){

	return $custom_content->title;
}

/**
 * Delete single remotehtml entity.
 *
 * @param $remotehtml
 * the entity
 */
function remotehtml_delete($remotehtml) {

	entity_delete('remotehtml', entity_id('remotehtml' ,$remotehtml));
}

/**
 * Delete single custom content entity.
 *
 * @param $custom_content
 * the entity
 */
function custom_content_delete($custom_content) {

	entity_delete('custom_content', entity_id('custom_content' ,$custom_content));
}

/**
 * Delete multiple remotehtml entities.
 *
 * @param $remotehtml_ids
 * their ids
 */
function remotehtml_delete_multiple($remotehtml_ids) {

	entity_delete_multiple('remotehtml', $remotehtml_ids);
}


/**
 * Delete multiple custom content entities.
 *
 * @param $custom_content_ids
 * their ids
 */
function custom_content_delete_multiple($custom_content_ids) {

	entity_delete_multiple('custom_content', $custom_content_ids);
}
/**
 * Save an remotehtml entity
 *
 * @param $remotehtml
 */
function remotehtml_save(&$remotehtml){

	return entity_save('remotehtml', $remotehtml);
}

/**
 * Save an custom content entity
 *
 * @param $custom_content
 */
function custom_content_save(&$custom_content){

	return entity_save('custom_content', $custom_content);
}

/**
 * Generate an array for rendering the given remotehtml entities
 *
 * @param $remotehtml
 * the given entities
 * @param $view_mode
 */
function remotehtml_view($remotehtml, $view_mode = 'full'){

	return $remotehtml->view();
}


/**
 * Generate an array for rendering the given custom content entities
 *
 * @param $custom_content
 * the given entities
 * @param $view_mode
 */
function custom_content_view($custom_content, $view_mode = 'full'){

	return $custom_content->view();
}
/**
 *
 * @param $remotehtml
 * @param $view_mode
 */
function remotehtml_page_view($remotehtml, $view_mode = 'full'){

	$remotehtml->content = array();

	// Build fields content.
	field_attach_prepare_view('remotehtml', array($remotehtml->tid => $remotehtml), $view_mode);
	entity_prepare_view('remotehtml', array($remotehtml->tid => $remotehtml));
	$remotehtml->content += field_attach_view('remotehtml', $remotehtml, $view_mode);

	return $remotehtml->content;
}

/**
 *
 * @param $custom_content
 * @param $view_mode
 */
function custom_content_page_view($custom_content, $view_mode = 'full'){

	$custom_content->content = array();

	// Build fields content.
	field_attach_prepare_view('custom_content', array($custom_content->tid => $custom_content), $view_mode);
	entity_prepare_view('custom_content', array($custom_content->tid => $custom_content));
	$custom_content->content += field_attach_view('custom_content', $custom_content, $view_mode);

	return $custom_content->content;
}