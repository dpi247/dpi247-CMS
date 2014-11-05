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
  	return array("node","user");
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
		if ($settings["restrictedaccessfieldroles"][$rid]=$rid) { return false; }
	}
	
	// Node load 
	if (valid_url($url)) {
		$node_path = explode('/', drupal_get_normal_path($url));
		$nid = $node_path[1];
		node_load($nid);
		dsm($node); 
		
	} else {
		echo "nein!";
	}

	return true;
  
  }

  
}
