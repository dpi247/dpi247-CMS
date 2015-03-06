<?php
/**
* @file
* dpiscald_dailymotion_player.inc
*/

function dpiscald_dailymotion_player() {
	return array(
		'name' => 'DPI Dailymotion HTLM5 player',
		'description' => 'The HTML5 Dailymotion player',
		'type' => array('dailymotion'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
			'caption' => '[atom:title], by [atom:author]',
		),
    );
}

function dpiscald_dailymotion_player_settings_form($form, &$form_state, &$element) {
	$element['width'] = array(
		'#type' => 'textfield',
		'#title' => t('Width'),
		'#size' => 4,
		'#default_value' => $form['#scald']['player_settings']['width'],
	);
	$element['height'] = array(
		'#type' => 'textfield',
		'#title' => t('Height'),
		'#size' => 4,
		'#default_value' => $form['#scald']['player_settings']['height'],
	);
}

function dpiscald_dailymotion_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_dailymotion_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_dailymotion_player_html5', $theme_parameters);
}

function dpiscald_dailymotion_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'video_id' => $atom->base_id, 
		'html_id' => 'player_'.$atom->base_id.drupal_html_id(), 
		'width' => $config->player['dailymotion']['settings']['width'], 
		'height' => $config->player['dailymotion']['settings']['height'], 
		'origin' => current_path()
	);
}