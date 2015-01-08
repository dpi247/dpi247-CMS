<?php
/**
* @file
* dpiscald_scribblelive_player.inc
*/

function dpiscald_scribblelive_player() {
	return array(
		'name' => 'DPI Scribble Live HTLM5 player',
		'description' => 'The HTML5 Scribble Live player',
		'type' => array('scribblelive'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
			'caption' => '[atom:title], by [atom:author]',
		),
    );
}

function dpiscald_scribblelive_player_settings_form($form, &$form_state, &$element) {
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

function dpiscald_scribblelive_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_scribblelive_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_scribblelive_player_html5', $theme_parameters);
}

function dpiscald_scribblelive_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'id_scribble' => $atom->data['id'], 
		'html_id' => 'player_'.$atom->base_id.drupal_html_id(), 
		'width' => $config->player['scribblelive']['settings']['width'], 
		'height' => $config->player['scribblelive']['settings']['height'], 
		'origin' => current_path()
	);
}