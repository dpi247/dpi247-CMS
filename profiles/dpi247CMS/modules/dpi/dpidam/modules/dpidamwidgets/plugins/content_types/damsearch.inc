<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('DAM Search'),
  'icon' => 'icon_search.png',
  'description' => t('The results of a DAM search.'),
  'category' => t('Widgets'),
  'defaults' => array(
    'profile' => 'default',
    'display_results' => TRUE,
  ),
);

/**
 * Returns an edit form for custom type settings.
*/
function dpidamwidgets_damsearch_content_type_edit_form(&$form, &$form_state) {
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

  $form["display_results"] = array(
    '#type' => 'checkbox',
    '#default_value' => $conf["display_results"],
  );
}

/**
 * Submit handler for search form.
 */
function dpidamwidgets_damsearch_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Render the custom content type.
 */
function dpidamwidgets_damsearch_content_type_render($subtype, $conf, $panel_args, $context) {
  // Build the content type block.
  $block = new stdClass();
  $block->module = 'wallydamwidgets';
  $block->delta = 'damsearch';
  $block->title = t('DAM Search');
  $block->content = drupal_get_form('dpidamwidgets_damsearch_content_type_search_form', $conf);

  return $block;
}

/**
 * Search form
 */
function dpidamwidgets_damsearch_content_type_search_form($form_state, $conf) {
  $form = array();

  if ($conf['profile'] == 'default') {
    $profile = dpidam_api_get_loaded_default_search_profile();
  } else {
    $profile = dpidam_api_get_search_profile($conf['profile']);
  }

  if (!$profile || !isset($profile['settings']) || empty($profile['settings'])) {
    drupal_set_message(t('Search profile not found or not configured'), 'error');
    return $form;
  }

  _dpidamwidgets_generate_search_form($form, $profile, $conf);

  return $form;
}

function dpidamwidgets_damsearch_content_type_search_form_submit(&$form, $form_state) {
  $values = $form_state['values'];
  list($redirect_path, $redirect_args) = _dpidamwidgets_damsearch_get_redirect_url($values);
  drupal_goto($redirect_path, $redirect_args);
}

/**
 * Returns the administrative title for a type.
 */
function dpidamwidgets_damsearch_content_type_admin_title($subtype, $conf, $context) {
  return t('DAM search');
}

/**
 * Generate the redirect URL
 */
function _dpidamwidgets_damsearch_get_redirect_url($values) {
  $base_path = $_GET['q'];
  $args = array();

  $args['word'] = $values['word'];
  if (isset($values['excludes'])) {
    $args['excludes'] = $values['excludes'];
  }
  if (isset($values['sort'])) {
    $args['sort'] = $values['sort'];
  }
  if (isset($values['datefilter'])) {
    $args['datefilter'] = $values['datefilter'];
  }

  return array($base_path, $args);
}
