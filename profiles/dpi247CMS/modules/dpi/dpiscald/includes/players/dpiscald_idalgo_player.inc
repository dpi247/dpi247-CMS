<?php
/**
* @file
* dpiscald_idalgo_player.inc
*/

function dpiscald_idalgo_player() {
	return array(
		'name' => 'DPI Idalgo HTLM5 player',
		'description' => 'The HTML5 Idalgo player',
		'type' => array('idalgo'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
		),
    );
}

function dpiscald_idalgo_player_settings_form($form, &$form_state, &$element) {
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

function dpiscald_idalgo_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_idalgo_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_idalgo_player_html5', $theme_parameters);
}

function dpiscald_idalgo_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'vars' => array(
			'url' => $atom->data['url'],
			'width' => $config->player['idalgo']['settings']['width'], 
			'height' => $config->player['idalgo']['settings']['height'], 
		),
		'atom' => $atom,
	);
}