<?php
/**
* @file
* dpiscald_infogram_player.inc
*/

function dpiscald_infogram_player() {
	return array(
		'name' => 'DPI Infogram HTLM5 player',
		'description' => 'The HTML5 Infogram player',
		'type' => array('infogram'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
		),
    );
}

function dpiscald_infogram_player_settings_form($form, &$form_state, &$element) {
	$element['width'] = array(
		'#type' => 'textfield',
		'#title' => t('Width'),
		'#size' => 4,
		'#default_value' => $form['#scald']['player_settings']['width'],
	);
}

function dpiscald_infogram_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_infogram_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_infogram_player_html5', $theme_parameters);
}

function dpiscald_infogram_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
        array(
          'base_url'      => $atom->data['url'],
          'id'            => $atom->data['id'],
          'title'         => $atom->title,
          'thumbnail'     => $atom->rendered->thumbnail_source_url,
          'width' 		  => $config->player['infogram']['settings']['width'], 
        ),
        'atom' => $atom,
      );
}