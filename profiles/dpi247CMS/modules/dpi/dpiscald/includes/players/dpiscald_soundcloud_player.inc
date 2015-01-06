<?php
/**
* @file
* dpiscald_soundcloud_player.inc
*/

function dpiscald_soundcloud_player() {
	return array(
		'name' => 'DPI Soundcloud HTLM5 player',
		'description' => 'The HTML5 Soundcloud player',
		'type' => array('soundcloud'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
			'caption' => '[atom:title], by [atom:author]',
		),
    );
}

function dpiscald_soundcloud_player_settings_form($form, &$form_state, &$element) {
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
	$element['caption'] = array(
		'#type' => 'textfield',
		'#title' => t('Text pattern used for caption'),
		'#size' => 40,
		'#default_value' => $form['#scald']['player_settings']['caption'],
	);
}

function dpiscald_soundcloud_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_soundcloud_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_soundcloud_player_html5', $theme_parameters);
}

function dpiscald_soundcloud_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'sound_id' => $atom->base_id, 
		'html_id' => 'player_'.$atom->base_id.drupal_html_id(), 
		'width' => $config->player['soundcloud']['settings']['width'], 
		'height' => $config->player['soundcloud']['settings']['height'], 
		'origin' => current_path()
	);
}