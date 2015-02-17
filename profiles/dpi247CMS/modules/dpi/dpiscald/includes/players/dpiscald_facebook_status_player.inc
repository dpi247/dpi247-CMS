<?php
/**
* @file
* dpiscald_facebook_status_player.inc
*/

function dpiscald_facebook_status_player() {
	return array(
		'name' => 'DPI Facebook Status HTLM5 player',
		'description' => 'The HTML5 Facebbok status player',
		'type' => array('facebook_status'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
		),
    );
}

function dpiscald_facebook_status_player_settings_form($form, &$form_state, &$element) {
	$element['width'] = array(
		'#type' => 'textfield',
		'#title' => t('Width'),
		'#description' => t("Please enter a number between 350 and 750 (facebook limitation)"),
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

function dpiscald_facebook_status_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_facebook_status_get_theme_parameters($atom, $context, $options, $mode, $config);
	dsm($theme_parameters);
	$atom->rendered->player = theme('dpiscald_facebook_status_player_html5', $theme_parameters);
}

function dpiscald_facebook_status_get_theme_parameters($atom, $context, $options, $mode, $config) {
	dsm($atom);
	return array('vars' =>
        array(
          'title'       => $atom->title,
          'base_id'     => $atom->base_id,
          'user_id'     => $atom->data['user_id'],
          'data_href'   => $atom->data['data_href'],
          'country'     => $atom->data['country'],
          'thumbnail'   => $atom->thumbnail_source,
          'width'		=> $config->player['facebook_status']['settings']['width'], 
          'height'		=> $config->player['facebook_status']['settings']['height'], 
        ),
        'atom' => $atom,
    );
}