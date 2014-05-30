<?php

function dpisitemap_admin_settings_page_form($form, $form_state) {
  $default_settings = array_merge(dpisitemap_default_settings(), dpi_variable_get('dpisitemap_sitemaps_settings', array()));

  if (isset($default_settings['dpisitemap_path']) && !empty($default_settings['dpisitemap_path'])) {
    $form['dpisitemap_xml_links'] = array(
      '#type' => 'fieldset',
      '#title' => t('Links to XML files'),
    );

    $folder_path = $default_settings['dpisitemap_path'];

    $scheme = 'public';
    if (file_stream_wrapper_valid_scheme($scheme)) {
      if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://'.$folder_path)) {
        $folder_path = $wrapper->getDirectoryPath().'/'.$folder_path;
      }
    }

    $index_link_form = $main_link_form = FALSE;
    if (file_exists($folder_path.'/sitemapindex.xml')) {
      $index_link_form = array(
        '#markup' => l(t('Sitemaps index XML'), $folder_path.'/sitemapindex.xml').'<br>',
      );
    }
    if (file_exists($folder_path.'/sitemapmain.xml')) {
      $main_link_form = array(
        '#markup' => l(t('Main sitemap XML'), $folder_path.'/sitemapmain.xml').'<br>',
      );
    }
    if ($index_link_form || $main_link_form) {
      if ($index_link_form) {
        $form['dpisitemap_xml_links']['index_xml'] = $index_link_form;
      }
      if ($main_link_form) {
        $form['dpisitemap_xml_links']['main_xml'] = $main_link_form;
      }
    } else {
      $form['dpisitemap_xml_links']['not_found'] = array(
        '#markup' => t('No XMLs found'),
      );
    }
  }

  $form['dpisitemap_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Sitemap\'s XMLs folder path'),
    '#description' => t('Sitemap\'s and sitemap index\'s XMLs will be saved there.<br>
        This must be a writable directory, relatively to the "public" stream.'),
    '#default_value' => $default_settings['dpisitemap_path'],
    '#required' => TRUE,
  );

  $form['dpisitemap_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#description' => t('URL used as base to build the URLs listed in sitemaps'),
    '#default_value' => $default_settings['dpisitemap_base_url'],
    '#required' => TRUE,
  );

  $blacklist_defaults = implode(PHP_EOL, $default_settings['dpisitemap_blacklist']);
  $form['dpisitemap_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('URLs black list'),
    '#description' => t('These URLs will not be included in the main sitemap.<br>
        Insert one URL per line, without the site\'s domain.<br>
        Example : <i>contact</i> or <i>article/this-article-must-not-be-sitemapped</i>'),
    '#default_value' => $blacklist_defaults,
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
  $weights_options = array(
    '0' => '0',
    '0.1' => '0.1',
    '0.2' => '0.2',
    '0.3' => '0.3',
    '0.4' => '0.4',
    '0.5' => '0.5',
    '0.6' => '0.6',
    '0.7' => '0.7',
    '0.8' => '0.8',
    '0.9' => '0.9',
    '1' => '1',
  );

  $menus = menu_get_names();
  $menus_default_values = $default_settings['dpisitemap_handled_menus'];
  $menu_default_values = array(
    'check' => 0,
    'weight' => 0,
    'freq' => 'daily',
  );
  $menu_options = array();
  foreach ($menus as $menu_name) {
    if (!isset($menus_default_values[$menu_name])) {
      $menus_default_values[$menu_name] = array();
    }
    $menus_default_values[$menu_name] = array_merge($menu_default_values, $menus_default_values[$menu_name]);
    $menu_options[$menu_name] = $menu_name;
  }
  $form['dpisitemap_handled_menus'] = array(
    '#type' => 'dpisitemap_checkboxes',
    '#title' => t('Menus to use for sitemap construction'),
    '#description' => t('Set URLs parameters per menu :<br>
        The checkbox specifies wheter the menu\'s elements must be sitemapped or not,<br>
        The bigger the weight the more important the menu is,<br>
        The refresh frequency.'),
    '#check_options' => $menu_options,
    '#select_options' => $weights_options,
    '#freq_options' => $freq_options,
    '#default_value' => $menus_default_values,
    '#theme' => 'dpisitemap_checkboxes',
  );

  $form['dpisitemap_dests_params'] = array(
    '#type' => 'dpisitemap_taxonomy_tree',
    '#title' => t('Destinations parameters (active, weight and refresh frequency)'),
    '#description' => t('Set URLs parameters per destination :<br>
        The checkbox specifies wheter the destination must be sitemapped or not,<br>
        The bigger the weight the more important the destination is,<br>
        The refresh frequency.'),
    '#select_options' => $weights_options,
    '#freq_options' => $freq_options,
    '#default_value' => $default_settings['dpisitemap_dests_params'],
    '#vid' => dpidestinations_api_get_sections_vocabulary_vid(),
    '#parent' => 0,
    '#expand_all' => TRUE,
    '#multiple' => TRUE,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 9,
  );

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#validate' => array(),
    '#submit' => array('dpisitemap_admin_settings_page_form_submit_reset'),
    '#weight' => 10,
  );

  return $form;
}

function dpisitemap_admin_settings_page_form_validate($form, $form_state) {
  $values = $form_state['values'];

  $path = 'public://'.$values['dpisitemap_path'];
  if (!file_prepare_directory($path)) {
    form_set_error('dpisitemap_path', t('Error directory path : the path provided is not an existing writable directory'));
  }

  if (!valid_url($values['dpisitemap_base_url'], TRUE)) {
    form_set_error('dpisitemap_base_url', t('The given URL is not valid'));
  }
}

function dpisitemap_admin_settings_page_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $settings = array();
  $settings['dpisitemap_path'] = $values['dpisitemap_path'];
  $settings['dpisitemap_base_url'] = $values['dpisitemap_base_url'];
  $tmp_blacklist = explode(PHP_EOL, $values['dpisitemap_blacklist']);
  $settings['dpisitemap_blacklist'] = array();
  foreach ($tmp_blacklist as $tmp_blackelem) {
    $blackelem = trim(trim($tmp_blackelem), '/');
    if (!empty($blackelem)) $settings['dpisitemap_blacklist'][] = $blackelem;
  }
  $settings['dpisitemap_handled_menus'] = $values['dpisitemap_handled_menus'];
  $settings['dpisitemap_dests_params'] = $values['dpisitemap_dests_params'];

  dpi_variable_set('dpisitemap_sitemaps_settings', $settings);

  drupal_set_message(t('Settings saved'));
}

function dpisitemap_admin_settings_page_form_submit_reset($form, $form_state) {
  dpi_variable_del('dpisitemap_sitemaps_settings');
  drupal_set_message(t('Settings restored'));
}

/**
 * Build default settings.
 */
function dpisitemap_default_settings() {
  global $base_url;
  $settings = array(
    'dpisitemap_handled_menus' => array(
      'primary-links' => array('check' => 1, 'weight' => 0, 'freq' => 'daily'),
      'secondary-links' => array('check' => 1, 'weight' => 0, 'freq' => 'daily'),
    ),
    'dpisitemap_path' => 'sitemaps',
    'dpisitemap_base_url' => $base_url,
    'dpisitemap_blacklist' => array(),
    'dpisitemap_dests_params' => array(),
  );
  return $settings;
}
