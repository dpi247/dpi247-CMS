<?php
/**
* @file
* dpiscald_kaltura_player.inc
*/

function dpiscald_kaltura_player() {
	return array(
		'name' => 'DPI kaltura Thumbnail player',
		'description' => 'The Thumbnail kaltura player',
		'type' => array('kaltura'),
		'settings' => array(),
    );
}

function dpiscald_kaltura_player_settings_form($form, &$form_state, &$element) {

}

function dpiscald_kaltura_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_kaltura_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_kaltura_player_html5', $theme_parameters);
}

function dpiscald_kaltura_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
        array(
			'title'			=> $atom->title,
			'thumbnail'		=> $atom->rendered->thumbnail_source_url,
        ),
        'atom' => $atom,
      );
}