<?php
/**
* @file
* dpiscald_twitter_player.inc
*/

function dpiscald_twitter_player() {
	return array(
		'name' => 'DPI twitter HTLM5 player',
		'description' => 'The HTML5 twitter player',
		'type' => array('twitter'),
		'settings' => array(),
    );
}

function dpiscald_twitter_player_settings_form($form, &$form_state, &$element) {

}

function dpiscald_twitter_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_twitter_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_twitter_player_html5', $theme_parameters);
}

function dpiscald_twitter_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'vars' => array(
			'url' => $atom->data['url'],
		),
		'atom' => $atom,
	);
}