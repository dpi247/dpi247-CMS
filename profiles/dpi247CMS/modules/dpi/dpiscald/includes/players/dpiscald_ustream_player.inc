<?php
/**
* @file
* dpiscald_ustream_player.inc
*/

function dpiscald_ustream_player() {
	return array(
		'name' => 'DPI ustream HTLM5 player',
		'description' => 'The HTML5 ustream player',
		'type' => array('ustream'),
		'settings' => array(),
    );
}

function dpiscald_ustream_player_settings_form($form, &$form_state, &$element) {

}

function dpiscald_ustream_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_ustream_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_ustream_player_html5', $theme_parameters);
}

function dpiscald_ustream_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
		array(
			'id'            => $atom->data['id'],
			'mode'          => $atom->data['mode'],
			'title'         => $atom->title,
			'thumbnail'     => $atom->rendered->thumbnail_source_url,
		),
		'atom' => $atom,
	);
}