<?php

/**
 * Implementation of hook_dpidashboard_status_item().
 *
 * @return array
 *   An array containing associative arrays with statuses
 *
 *   Status values:
 *		0: Everything is ok
 *		1: Information displayed
 *		2: Something needs extra attention
 *		3: Error
 *
 *
 */
function dpidashboard_dpidashboard_status_item() {
	$status = array(
		array(
			'title' 	=> 'DPI Dashboard',
			'status'	=> 0,
			'message'	=> t('Tout va bien'),
			'link'		=> '/admin/dashboard',
		),
		array(
			'title' 	=> 'Autre status',
			'status'	=> 2,
			'message'	=> t('Problème dans le module'),
			'link'		=> '/admin/dashboard',
		)
	);
	
	return $status;	
}