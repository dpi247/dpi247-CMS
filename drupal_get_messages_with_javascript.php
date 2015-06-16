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


define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/theme.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);



$variables['display']=NULL;
print drupal_get_status_messages_with_javascript($variables);