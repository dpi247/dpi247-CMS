<?php
/**
* @file
* dpiscald_videortl_player.inc
*/

function dpiscald_videortl_player() {
	return array(
		'name' => 'DPI videortl HTLM5 player',
		'description' => 'The HTML5 videortl player',
		'type' => array('videortl'),
		'settings' => array(),
    );
}

function dpiscald_videortl_player_settings_form($form, &$form_state, &$element) {

}

function dpiscald_videortl_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_videortl_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_videortl_player_html5', $theme_parameters);
}

function dpiscald_videortl_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
        array(
			'video_id'      => $atom->data['video_id'],
			'video_key'   => $atom->data['video_key'],
			'video_liveid'  => $atom->data['video_liveid'],
			'video_domain'     => $atom->data['video_domain'],
		),
		'atom' => $atom,
	);
}