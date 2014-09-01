<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('DAM Results'),
  'icon' => 'icon_search.png',
  'description' => t('The results of a DAM search.'),
  'category' => t('DAM Archives'),
  'group' => t('DPI Core'),
  'edit form' => array(
    'dpidamwidgets_damresult_content_type_view_form' => 'DAM Results',
  ),
  'defaults' => array(
    'profile' => 'default',
  ),
);

/**
 * Implementation of hook_content_type_content_types()
 */
function dpidamwidgets_damresult_content_type_content_types() {
  $blocks['damresult'] = array(
    'single' => TRUE,
    'title' => t('DAM Results'),
    'icon' => 'icon_search.png',
    'description' => t('The results of a DAM search.'),
    'category' => t('DAM Archives'),
    'group' => t('DPI Core'),
    'edit form' => array(
      'dpidamwidgets_damresult_content_type_view_form' => 'DAM Results',
    ),
    'defaults' => array(
      'profile' => 'default',
    ),
  );

  return $blocks;
}

/**
 * Returns an edit form for custom type settings.
*/
function dpidamwidgets_damresult_content_type_view_form($form, $form_state) {
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

  return $form;
}

/**
 * Submit handler for search form.
 */
function dpidamwidgets_damresult_content_type_view_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Render the custom content type.
 */
function dpidamwidgets_damresult_content_type_render($subtype, $conf, $panel_args, $context) {
  // Build the content type block.

  $block = new stdClass();
  $block->module = 'dpidamwidgets';
  $block->delta = 'damresult';
  $block->title = t('DAM Search Results');

  if ($conf['profile'] == 'default') {
    $profile = dpidam_api_get_loaded_default_search_profile();
  } else {
    $profile = dpidam_api_get_search_profile($conf['profile']);
  }

  if (!$profile || !isset($profile['settings']) || empty($profile['settings'])) {
    drupal_set_message(t('Search profile not found or not configured'), 'error');
    return $form;
  }

  $block->content = _dpidamwidgets_perform_result_and_display($profile);

  return $block;
}

/**
 * Returns the administrative title for a type.
 */
function dpidamwidgets_damresult_content_type_admin_title($subtype, $conf, $context) {
  return t('DAM search results');
}
