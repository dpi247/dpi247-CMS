<?php
/**
* @file
* ena_scald_player_videos_html5.inc
*/

function dpiscald_youtube_player() {
	return array(
		'name' => 'DPI Youtube HTLM5 player',
		'description' => 'The HTML5 youtube player',
		'type' => array('youtube'),
		'settings' => array(
			'width' => 640,
			'height' => 400,
			'caption' => '[atom:title], by [atom:author]',
		),
    );
}

function dpiscald_youtube_player_settings_form($form, &$form_state, &$element) {
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
	$element['caption'] = array(
		'#type' => 'textfield',
		'#title' => t('Text pattern used for caption'),
		'#size' => 40,
		'#default_value' => $form['#scald']['player_settings']['caption'],
	);
}

function dpiscald_youtube_build_render(&$atom, $context, $options, $mode, $config) {
	$theme_parameters = dpiscald_youtube_get_theme_parameters($atom, $context, $options, $mode, $config);
	$atom->rendered->player = theme('dpiscald_youtube_player_html5', $theme_parameters);
}

function dpiscald_youtube_get_theme_parameters($atom, $context, $options, $mode, $config) {
	return array( 
		'video_id' => $atom->base_id, 
		'html_id' => 'player_'.$atom->base_id.drupal_html_id(), 
		'width' => $config->player['video']['settings']['width'], 
		'height' => $config->player['video']['settings']['height'], 
		'origin' => current_path()
	);
}