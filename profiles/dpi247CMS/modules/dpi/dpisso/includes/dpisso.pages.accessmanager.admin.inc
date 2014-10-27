<?php
function dpisso_accessmanager_admin_settings_page_form() {
	
	// Get ssoClient config from file.
	$ssoconfig = dpisso_get_config ();
	
	if (isset ( $ssoconfig ['unitId'] )) {
		$response = AccessManager::getRemoteWhiteList ();
	}
	
	if (is_array ( $response->whiteList )) {
		$dpisso_accessmanager_whiteList = implode ( "\n", $response->whiteList );
	}
	
	if (is_array ( $response->blackList )) {
		$dpisso_accessmanager_blackList = implode ( "\n", $response->blackList );
	}
	
	// If no accessmanager nor invalid uri diseable the send button.
	if (! valid_url ( $ssoconfig ["domain_AM"] )) {
		$dpisso_accessmanager_send = array (
				"#disabled" => TRUE 
		);
	} else {
		$dpisso_accessmanager_send = array (
				"#disabled" => FALSE 
		);
	}
	
	$form = array ();
	
	$form ['dpisso_accessmanager_uri'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Access Manager URI' ),
			'#description' => t ( " Do not indicate the scheme such as http:// neither the trailling slash. DPI will inform the Access Manager of any changes in the form bellow." ),
			'#default_value' => $ssoconfig ["domain_AM"],
			'#disabled' => TRUE 
	);
	
	$form ['lists'] = array (
			'#type' => 'fieldset',
			'#title' => t ( 'White & Black lists settings' ),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE 
	);
	
	$form ['lists'] ['dpisso_accessmanager_whiteList'] = array (
			'#type' => 'textarea',
			'#title' => t ( 'Access Manager Whitelist' ),
			'#description' => t ( " One url (or patern) per line. All theses urls will <strong>always</strong> be on free access." ),
			'#default_value' => trim ( $dpisso_accessmanager_whiteList ) 
	);
	
	$form ['lists'] ['dpisso_accessmanager_blackList'] = array (
			'#type' => 'textarea',
			'#title' => t ( 'Access Manager Blacklist' ),
			'#description' => t ( " One url (or patern) per line. All theses urls will <strong>never</strong> be on free access." ),
			'#default_value' => trim ( $dpisso_accessmanager_blackList ) 
	);
	
	$form ['lists'] ['dpisso_accessmanager_send'] = array (
			'#type' => 'submit',
			'#submit' => array (
					'dpisso_accessmanager_send_form_submit' 
			),
			'#value' => t ( 'Send tp Access Manager' ) 
	);
	$form ['lists'] ['dpisso_accessmanager_send'] = array_merge ( $form ['lists'] ['dpisso_accessmanager_send'], $dpisso_accessmanager_send );
	
	return $form;
}

/*
 *
 */
function dpisso_accessmanager_send_form_submit($form, &$form_state) {
	$config = dpisso_get_config ();
	$postDatas = array ();
	$postDatas['unitId'] = $config ['unitId'];
	$postDatas['whiteList'] = array(); 
	$postDatas['blackList'] = array(); 
	
	foreach( explode( "\n", $form_state['values']['dpisso_accessmanager_whiteList'] ) as $v) {
		if (strlen(trim($v))) { 
			if (!in_array(trim($v),$postDatas['whiteList'])) { $postDatas['whiteList'][]=trim($v); } 
		}
    }
    foreach( explode( "\n", $form_state['values']['dpisso_accessmanager_blackList'] ) as $v) {
    	if (strlen(trim($v))) { 
    		if (!in_array(trim($v),$postDatas['blackList'])) { $postDatas['blackList'][]=trim($v); } 
    	}
    }

	if (isset ( $postDatas ['unitId'] )) {
		$response = AccessManager::sendWhiteList ( $postDatas );
	}
	return ($response);
}
