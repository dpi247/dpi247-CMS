<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('DAM Facets'),
  'icon' => 'icon_search.png',
  'description' => t('The Facets results of a DAM search.'),
  'category' => t('Widgets'),
  'defaults' => array(
    'profile' => 'default',
  ),
);

/**
 * Returns an edit form for custom type settings.
*/
function dpidamwidgets_damfacets_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $profiles = dpidam_api_get_search_profiles();
  $profile_options = array();
  $profile_options['default'] = t('Default');
  foreach ($profiles as $pid => $profile) {
    $profile_options[$pid] = $profile['label'];
  }
  $form['profile'] = array(
    '#type' => 'select',
    '#title' => t('Search profile'),
    '#description' => t('The selected profile will set some parameters for the query'),
    '#options' => $profile_options,
    '#default_value' => $conf['profile'],
  );
}

/**
 * Submit handler for search form.
 */
function dpidamwidgets_damfacets_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Render the custom content type.
 */
function dpidamwidgets_damfacets_content_type_render($subtype, $conf, $panel_args, $context) {
  // Build the content type block.

  $block = new stdClass();
  $block->module = 'dpidamwidgets';
  $block->delta = 'damfetch';
  $block->title = t('DAM Facets Search');

  if ($conf['profile'] == 'default') {
    $profile = dpidam_api_get_loaded_default_search_profile();
  } else {
    $profile = dpidam_api_get_search_profile($conf['profile']);
  }

  if (!$profile || !isset($profile['settings']) || empty($profile['settings'])) {
    drupal_set_message(t('Search profile not found or not configured'), 'error');
    return $form;
  }

  $block->content = _dpidamwidgets_perform_facets_and_display($profile);

  return $block;
}

/**
 * Search form
 *
 * @todo: to be cleaned (legacy)
 */
function _LEGACY_wallydamwidgets_damsearch_content_type_facets_form($form_state, $conf) {
  $form = array();

  if ($conf['profile'] == 'default') {
    $profile = wallydam_get_loaded_default_search_profile();
  } else {
    $profile = wallydam_get_search_profile($conf['profile']);
  }

  if (!$profile || !isset($profile['settings']) || empty($profile['settings'])) {
    drupal_set_message(t('Search profile not found or not configured'), 'error');
    return $form;
  }

  wallydamwidgets_generate_facets_form($form, $profile);

  $form['conf'] = array(
    '#type' => 'hidden',
    '#value' => serialize($conf),
  );

  return $form;
}

function dpidamwidgets_damsearch_content_type_facets_form_submit(&$form, $form_state) {
  $values = $form_state['values'];
  $conf = unserialize($values['conf']);
  list($redirect_path, $redirect_args) = _dpidamwidgets_damfacet_get_redirect_url($values, $conf);
  drupal_goto($redirect_path, $redirect_args);
}

/**
 * Returns the administrative title for a type.
 */
function dpidamwidgets_damfacets_content_type_admin_title($subtype, $conf, $context) {
  return t('DAM facets search');
}

/**
 * Generate the redirect URL
 *
 * @todo: to be cleaned (legacy)
 */
function _LEGACY_wallydamwidgets_damfacet_get_redirect_url($values, $conf) {
  $base_path = $_GET['q'];
  $args = array();

  if (isset($values['profile']) && !empty($values['profile'])) {
    $profile = unserialize($values['profile']);
  } else {
    drupal_set_message(t('Search profile not set'), 'error');
    return $base_path;
  }

  $args['profile'] = $profile['pid'];
  if ($profile['settings']['advanced']) {
    $args['words'] = $values['words'];
    $args['excludes'] = $values['excludes'];
  } else {
    $args['word'] = $values['word'];
  }

  return array($base_path, $args);
}
