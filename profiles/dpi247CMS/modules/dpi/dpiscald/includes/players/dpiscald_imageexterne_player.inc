<?php
/**
* @file
* dpiscald_imageexterne_player.inc
*/

function dpiscald_imageexterne_player() {
	return array(
		'name' => 'DPI Image externe HTLM5 player',
		'description' => 'The HTML5 Image externe player',
		'type' => array('imageexterne'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
			'caption' => '[atom:title], by [atom:author]',
		),
    );
}

function dpiscald_imageexterne_player_settings_form($form, &$form_state, &$element) {
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

function dpiscald_imageexterne_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_imageexterne_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_imageexterne_player_html5', $theme_parameters);
}

function dpiscald_imageexterne_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array('vars' =>
        array(
          'url'      => $atom->base_id,
          'title'    => $atom->title,
          'width' => $config->player['imageexterne']['settings']['width'], 
          'height' => $config->player['imageexterne']['settings']['height'], 
          'origin' => current_path()
        ),
        'atom' => $atom,
      );
}