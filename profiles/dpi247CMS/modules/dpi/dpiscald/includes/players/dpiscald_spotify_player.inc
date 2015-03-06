<?php
/**
* @file
* dpiscald_spotify_player.inc
*/

function dpiscald_spotify_player() {
	return array(
		'name' => 'DPI Spotify HTLM5 player',
		'description' => 'The HTML5 Spotify player',
		'type' => array('spotify'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
			'caption' => '[atom:title], by [atom:author]',
		),
    );
}

function dpiscald_spotify_player_settings_form($form, &$form_state, &$element) {
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

function dpiscald_spotify_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_spotify_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_spotify_player_html5', $theme_parameters);
}

function dpiscald_spotify_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'base_url' => $atom->data['url'],
		'html_id' => 'player_'.$atom->base_id.drupal_html_id(), 
		'width' => $config->player['spotify']['settings']['width'], 
		'height' => $config->player['spotify']['settings']['height'], 
		'origin' => current_path()
	);
}