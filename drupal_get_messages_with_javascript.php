<?php

/*
 * This file is used to load messages from Drupal's drupal_get_message()
 *
 * and additional argument allow to retrive the format in JSON for future purpose
 * Like (Headless RESTFul Server or mobile application)
 * use the url drupal_get_messages_with_javascript.php?format=json
 *
 *
 *
 *
 */





define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/theme.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

load_all_module_using_ctools_plugin();


$variables['display']=NULL;
print drupal_get_status_messages_with_javascript($variables);






function load_all_module_using_ctools_plugin()
{

	/*
	  You should maintain this list manually .
	  All the keys of this call should belong to the array:

	  module_load_include('inc','ctools','includes/plugins');
	  dsm(ctools_get_plugins_info());
	*/

	$modules_that_uses_ctools_plugin_system = array(
		'ctools',
		'dpiblocks',
		'dpicache',
		'dpimport',
		'dpisocial',
		'dpisso',
		'enabootstrap_companion',
		'entityreference',
		'panels',
		'views_bulk_operations',
		'page_manager',
		'addressfield',
		//'feeds',
		//'spaces',
		//'context',
		//'quicktabs',

	);

	foreach ($modules_that_uses_ctools_plugin_system as $module_name) {
		module_load_include('module', $module_name);
	}
}


function drupal_get_status_messages_with_javascript($variables) {
	$display = $variables ['display'];
	$output = '';

	$status_heading = array(
		'status' => t('Status message'),
		'error' => t('Error message'),
		'warning' => t('Warning message'),
	);
	foreach (drupal_get_messages($display) as $type => $messages) {
		$output .= "<div class=\"messages $type\">\n";
		if (!empty($status_heading [$type])) {
			$output .= '<h2 class="element-invisible">' . $status_heading [$type] . "</h2>\n";
		}
		if (count($messages) > 1) {
			$output .= " <ul>\n";
			foreach ($messages as $message) {
				$output .= '  <li>' . $message . "</li>\n";
			}
			$output .= " </ul>\n";
		}
		else {
			$output .= reset($messages);
		}
		$output .= "</div>\n";
	}
	return $output;
}