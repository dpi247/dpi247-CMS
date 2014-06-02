<?php

/**
 * Admin settings form
 */
function dpinewssitemap_admin_settings_page_form($form, $form_state) {
  $default_settings = array_merge(dpinewssitemap_default_settings(), dpi_variable_get('dpinewssitemap_sitemaps_settings', array()));

  $form['dpinewssitemap_path'] = array(
    '#type' => 'textfield',
    '#title' => t('News sitemap\'s XMLs folder path'),
    '#description' => t('News sitemap\'s XMLs will be saved there.<br>
        This must be a writable directory, relatively to the "public" stream.'),
    '#default_value' => $default_settings['dpinewssitemap_path'],
    '#required' => TRUE,
  );

  $form['dpinewssitemap_delta_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Nodes\' maximum age in hours'),
    '#default_value' => $default_settings['dpinewssitemap_delta_hours'],
    '#required' => TRUE,
  );

  $blacklist_defaults = implode(PHP_EOL, $default_settings['dpinewssitemap_blacklist']);
  $form['dpinewssitemap_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('URLs black list'),
    '#description' => t('These URLs will not be included in the news sitemap.<br>
        Insert one URL per line, without the site\'s domain.<br>
        Example : <i>contact</i> or <i>article/this-article-must-not-be-sitemapped</i>'),
    '#default_value' => $blacklist_defaults,
  );

  $add_medias_options = array(
    'dpinewssitemap_add_images' => t('Add images informations'),
    'dpinewssitemap_add_videos' => t('Add videos informations'),
  );
  $add_medias_defaults = array();
  if ($default_settings['dpinewssitemap_add_images']) $add_medias_defaults[] = 'dpinewssitemap_add_images';
  if ($default_settings['dpinewssitemap_add_videos']) $add_medias_defaults[] = 'dpinewssitemap_add_videos';
  $form['dpisnewsitemap_add_medias'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Medias handling'),
    '#options' => $add_medias_options,
    '#default_value' => $add_medias_defaults,
  );

  $form['dpinewssitemap_publication_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication name'),
    '#default_value' => $default_settings['dpinewssitemap_publication_name'],
  );

  $content_types = node_type_get_types();
  $content_types_options = array();
  foreach ($content_types as $content_type) {
    $content_types_options[$content_type->type] = $content_type->name;
  }
  $form['dpinewssitemap_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types to add to news sitemap'),
    '#options' => $content_types_options,
    '#default_value' => $default_settings['dpinewssitemap_content_types'],
    '#required' => TRUE,
  );

  $tags_fields_options = dpinewssitemap_content_field_get_taxonomy_fields();
  $form['dpinewssitemap_tags_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields used as tags'),
    '#description' => t('Some fields may not be used if they are not used in the selected content types.'),
    '#options' => $tags_fields_options,
    '#default_value' => $default_settings['dpinewssitemap_tags_fields'],
  );

  $freq_options = array(
    'always' => t('Always'),
    'hourly' => t('Hourly'),
    'daily' => t('Daily'),
    'weekly' => t('Weekly'),
    'monthly' => t('Monthly'),
    'yearly' => t('Yearly'),
    'never' => t('Never'),
  );
  $form['dpinewssitemap_nodes_freqs'] = array(
    '#type' => 'dpisitemap_taxonomy_tree',
    '#title' => t('Nodes refresh frequency per destination'),
    '#description' => t('Set refresh frequency parameter for nodes per destination'),
    '#freq_options' => $freq_options,
    '#default_value' => $default_settings['dpinewssitemap_nodes_freqs'],
    '#vid' => dpidestinations_api_get_sections_vocabulary_vid(),
    '#parent' => 0,
    '#expand_all' => TRUE,
    '#multiple' => TRUE,
    '#add_checkbox' => FALSE,
  );

  $styles = image_styles();
  $styles_options = array();
  foreach($styles as $style) {
    $styles_options[$style['name']] = $style['label'];
  }

  $form['dpinewssitemap_image_style'] = array(
    '#type' => 'select',
    '#title' => t('Preset used for images'),
    '#options' => $styles_options,
    '#default_value' => $default_settings['dpinewssitemap_image_style'],
  );

  $form['dpinewssitemap_image_license'] = array(
    '#type' => 'textfield',
    '#title' => t('Images license'),
    '#description' => t('This must be a valid URL'),
    '#default_value' => $default_settings['dpinewssitemap_image_license'],
  );

  $form['dpinewssitemap_video_style'] = array(
    '#type' => 'select',
    '#title' => t('Preset used for videos thumbnails'),
    '#options' => $styles_options,
    '#default_value' => $default_settings['dpinewssitemap_video_style'],
  );

  $form['dpinewssitemap_default_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image path'),
    '#description' => t('This must be a valid local path.<br>
        You may use a full path or a path using the files scheme URIs.'),
    '#default_value' => $default_settings['dpinewssitemap_default_img'],
  );

  $form['dpinewssitemap_allow_embed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow embeds'),
    '#description' => t('Is Google allowed to embed videos or not'),
    '#default_value' => $default_settings['dpinewssitemap_allow_embed'] == 'yes' ? 1 : 0,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#validate' => array(),
    '#submit' => array('dpinewssitemap_admin_settings_page_form_submit_reset'),
  );

  return $form;
}

function dpinewssitemap_admin_settings_page_form_validate($form, &$form_state) {
  $values = &$form_state['values'];

  $path = 'public://'.$values['dpinewssitemap_path'];
  if (!file_prepare_directory($path)) {
    form_set_error('dpinewssitemap_path', t('Error directory path : the path provided is not an existing writable directory'));
  }

  if (!is_numeric($values['dpinewssitemap_delta_hours']) || $values['dpinewssitemap_delta_hours'] <= 0) {
    form_set_error('dpinewssitemap_delta_hours', t('The nodes\' maximum age must a numeric value greater than 0.'));
  }

  $image_path = $values['dpinewssitemap_default_img'];
  if (!empty($image_path)) {
    $uri_scheme = file_uri_scheme($image_path);
    if (file_stream_wrapper_valid_scheme($uri_scheme)) {
      if ($wrapper = file_stream_wrapper_get_instance_by_uri($image_path)) {
        $image_path = $wrapper->getDirectoryPath().'/'.str_replace($uri_scheme.'://', '', $image_path);
        if (!is_file($image_path)) {
          form_set_error('dpinewssitemap_default_img', t('The given file doesn\'t exists.'));
        }
      }
    }
    else {
      if (strstr($image_path, '://')) {
        form_set_error('dpinewssitemap_default_img', t('Unrecognized file scheme.'));
      }
      elseif (!is_file($image_path)) {
        form_set_error('dpinewssitemap_default_img', t('The given file doesn\'t exists.'));
      }
    }
  }
}

function dpinewssitemap_admin_settings_page_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $orig_settings = array_merge(dpinewssitemap_default_settings(), dpi_variable_get('dpinewssitemap_sitemaps_settings', array()));

  $settings = $orig_settings;
  $settings['dpinewssitemap_path'] = $values['dpinewssitemap_path'];
  $settings['dpinewssitemap_delta_hours'] = $values['dpinewssitemap_delta_hours'];
  $tmp_blacklist = explode(PHP_EOL, $values['dpinewssitemap_blacklist']);
  $settings['dpinewssitemap_blacklist'] = array();
  foreach ($tmp_blacklist as $tmp_blackelem) {
    $blackelem = trim(trim($tmp_blackelem), '/');
    if (!empty($blackelem)) $settings['dpinewssitemap_blacklist'][] = $blackelem;
  }
  $settings['dpinewssitemap_add_images'] = $values['dpisnewsitemap_add_medias']['dpinewssitemap_add_images'] ? 1 : 0;
  $settings['dpinewssitemap_add_videos'] = $values['dpisnewsitemap_add_medias']['dpinewssitemap_add_videos'] ? 1 : 0;
  $settings['dpinewssitemap_publication_name'] = $values['dpinewssitemap_publication_name'];
  $settings['dpinewssitemap_content_types'] = array();
  foreach ($values['dpinewssitemap_content_types'] as $content_type) {
    if ($content_type) {
      $settings['dpinewssitemap_content_types'][] = $content_type;
    }
  }
  $settings['dpinewssitemap_tags_fields'] = array();
  foreach ($values['dpinewssitemap_tags_fields'] as $tag_field) {
    if ($tag_field) {
      $settings['dpinewssitemap_tags_fields'][] = $tag_field;
    }
  }
  $settings['dpinewssitemap_nodes_freqs'] = $values['dpinewssitemap_nodes_freqs'];
  $settings['dpinewssitemap_image_style'] = $values['dpinewssitemap_image_style'];
  $settings['dpinewssitemap_image_license'] = $values['dpinewssitemap_image_license'];
  $settings['dpinewssitemap_video_style'] = $values['dpinewssitemap_video_style'];
  $settings['dpinewssitemap_default_img'] = $values['dpinewssitemap_default_img'];
  $settings['dpinewssitemap_allow_embed'] = $values['dpinewssitemap_allow_embed'] ? 'yes' : 'no';

  dpi_variable_set('dpinewssitemap_sitemaps_settings', $settings);

  drupal_set_message(t('Settings saved.'));
}

function dpinewssitemap_admin_settings_page_form_submit_reset($form, $form_state) {
  $orig_settings = array_merge(dpinewssitemap_default_settings(), dpi_variable_get('dpinewssitemap_sitemaps_settings', array()));
  $settings = array();
  $settings['dpinewssitemap_consolidate_sitemap'] = $orig_settings['dpinewssitemap_consolidate_sitemap'];
  $settings['dpinewssitemap_consolidate_sitemap_type'] = $orig_settings['dpinewssitemap_consolidate_sitemap_type'];
  $settings['dpinewssitemap_consolidate_sitemap_amount'] = $orig_settings['dpinewssitemap_consolidate_sitemap_amount'];
  dpi_variable_set('dpinewssitemap_sitemaps_settings', $settings);
  drupal_set_message(t('Settings restored'));
}

/**
 * Will be called by dpinewssitemap to build default settings.
 */
function dpinewssitemap_default_settings() {
  return array(
    'dpinewssitemap_path' => 'sitemaps',
    'dpinewssitemap_delta_hours' => 48,
    'dpinewssitemap_consolidate_sitemap' => 0,
    'dpinewssitemap_consolidate_sitemap_type' => 'global',
    'dpinewssitemap_consolidate_sitemap_amount' => 100,
    'dpinewssitemap_add_images' => 0,
    'dpinewssitemap_add_videos' => 0,
    'dpinewssitemap_blacklist' => array(),
    'dpinewssitemap_publication_name' => variable_get('site_name', ''),
    'dpinewssitemap_content_types' => array('package', 'page'),
    'dpinewssitemap_tags_fields' => array(),
    'dpinewssitemap_image_style' => '',
    'dpinewssitemap_image_license' => '',
    'dpinewssitemap_video_style' => '',
    'dpinewssitemap_default_img' => drupal_get_path('module', 'dpinewssitemap').'/images/default_pic.jpg',
    'dpinewssitemap_allow_embed' => 'no',
    'dpinewssitemap_nodes_freqs' => array(),
  );
}

/**
 * Get all taxonomy fields.
 *
 * @return
 *   The field arrays.
 */
function dpinewssitemap_content_field_get_taxonomy_fields() {
  $query = db_select('field_config', 'fc')
    ->condition('fc.module', 'taxonomy');
  $query->join('field_config_instance', 'fci', 'fc.id = fci.field_id');
  $db_results = $query->fields('fci')
    ->execute();
  $fields = array();
  foreach ($db_results as $db_result) {
    $field_settings = unserialize($db_result->data);
    $fields[$db_result->field_name] = $field_settings['label'];
  }
  return $fields;
}
