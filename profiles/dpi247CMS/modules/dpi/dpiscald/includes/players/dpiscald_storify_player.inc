<?php
/**
* @file
* dpiscald_storify_player.inc
*/

function dpiscald_storify_player() {
	return array(
		'name' => 'DPI storify HTLM5 player',
		'description' => 'The HTML5 storify player',
		'type' => array('storify'),
		'settings' => array(),
    );
}

function dpiscald_storify_player_settings_form($form, &$form_state, &$element) {

}

function dpiscald_storify_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_storify_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_storify_player_html5', $theme_parameters);
}

function dpiscald_storify_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
        array(
          'base_url'      => $atom->data['url'],
          'title'         => $atom->title,
          'thumbnail'     => $atom->rendered->thumbnail_source_url,
        ),
        'atom' => $atom,
    );
}