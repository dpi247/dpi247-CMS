<?php 
/**
 * Declare the ctools pluggin.
 */
$plugin = array(
    'label' => t('Based on the restricted access field'),
    'handler' => array(
        'class' => 'RestrictedAccessField',
    ),
);

/**
 * PaywallPerType - Any choosen content type will decrease the paywall counter.
 * @author exxodus
 *
 */
class RestrictedAccessField extends PaywallPolitics {

  /**
   * settingsform method.
   * 
   * @see PaywallPolitics::settingsform()
   */
  public function settingsform($politic_settings_default_values) {

    // Build a options array with all roles    
    $roles = array_map('check_plain', user_roles(TRUE));
    
    $form = array(); 
    $form['restrictedaccessfieldroles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Paywall politic bypass roles'),
        '#description'=>t(" Choose all the roles that will bypass the paywall politic.Those roles will never decrease the paywall counter."),
        '#options' => $roles,
        '#default_value' => $politic_settings_default_values['restrictedaccessfieldroles'],
    );
    return $form;
  }
  
  /**
   * The list of module to be loaded in accesscontroller controler.
   * @see PaywallPolitics::getmodulelist()
   */
  public function getmodulelist() {
  	return array("user");
  }

  /**
   * Check if page is secured or in free access for roles.
   * @see PaywallPolitics::issecurepage()
   */
  public function issecurepage($url, $roles) {
  	require_once DRUPAL_ROOT . '/includes/path.inc';
  	
  	$settings = dpi_variable_get("paywallpolitic_RestrictedAccessField",null);
  	
  	// Each role spécified in the config bypass security based on restricted access field. 
	foreach($roles as $roles_item) {
		$role = user_role_load_by_name($roles_item);
		$rid = $role->rid; 
		if ($settings["restrictedaccessfieldroles"][$rid]=$rid) { 
    		return array("code"=>403, "data"=>array("is_secure_page"=>FALSE));
		}
	}

	
	// Check Restrictedaccess Value
	if (valid_url($url)) {
		// Cleaning the URL.
		$url = explode("?", $url);
		$url = $url[0] ;
		$url = preg_replace('/\/\/+/', '/', ltrim(rtrim($url,'/'),"/"));
		if ($systempath = drupal_lookup_path("source", $url)) {
			$node_path = explode('/', $systempath);
			$entity_nid = $node_path[1];
			$entity_type = "node";
			$result = db_query('SELECT f.field_restrictedaccess_value FROM {field_revision_field_restrictedaccess} f WHERE f.entity_id = :entity_id AND f.entity_type = :entity_type LIMIT 1', 
							    array(':entity_id' => $entity_nid, ':entity_type' => $entity_type ))->fetchObject();
			if ($result) {
				if ($result->field_restrictedaccess_value==1) {
					// Should be "402" Paiement required ;) 
					return array("code"=>200, "data"=>array("is_secure_page"=>TRUE));
				} else {
					return array("code"=>403, "data"=>array("is_secure_page"=>FALSE));
				};
			} else {
				// Can't found field_restrictedaccess_value from node.
				return array("code"=>501, "data"=>array("is_secure_page"=>"NOT IMPLEMENTED"));
							}
		} else {
			// Can't found URL. 
			return array("code"=>404, "data"=>array("is_secure_page"=>"URL NOT FOUND"));;				
		}
	}
	

	
	return NULL;
  }

  
}
