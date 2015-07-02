<?php

define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
global $user;

load_all_module_using_ctools_plugin();



print "Hello ";
print $user->name;
$role_string = urlencode(implode("--", $user->roles));

$username = $user->uid ? $user->name : "anonymous";
drupal_add_http_header('X-Dpicache-userid', $user->uid);
drupal_add_http_header('X-Dpicache-username', $username);
drupal_add_http_header('X-Dpicache-roles', $role_string);



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
