<?php
/**
* @file
* dpiscald_slideshare_player.inc
*/

function dpiscald_slideshare_player() {
	return array(
		'name' => 'DPI slideshare HTLM5 player',
		'description' => 'The HTML5 slideshare player',
		'type' => array('slideshare'),
		'settings' => array(),
    );
}

function dpiscald_slideshare_player_settings_form($form, &$form_state, &$element) {

}

function dpiscald_slideshare_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_slideshare_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_slideshare_player_html5', $theme_parameters);
}

function dpiscald_slideshare_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
        array(
			'slideshow_id'	=> $atom->data['slideshow_id'],
			'html'			=> $atom->data['html'],
			'title'			=> $atom->title,
			'thumbnail'		=> $atom->rendered->thumbnail_source_url,
        ),
        'atom' => $atom,
      );
}