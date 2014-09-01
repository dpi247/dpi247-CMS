<?php

/**
 * Manage search profiles and general settings
 */
function dpidam_page_admin_search() {
  $header = array(t('Machine name'), t('Name'), t('Default profile'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows = array();
  $noyes = array(0 => t('No'), 1 => t('Yes'));

  $profiles = dpidam_api_get_search_profiles();
  foreach ($profiles as $profile) {
    $operations = _dpidam_page_admin_search_get_operations($profile);
    $row = array(
      check_plain($profile['name']),
      check_plain($profile['label']),
      $noyes[isset($profile['default_profile']) && $profile['default_profile']],
      array('data' => $operations),
    );

    $rows[] = $row;
  }

  $table_vars = array('header' => $header, 'rows' => $rows);
  $output = !empty($rows) ? theme('table', $table_vars) : '';
  $output .= drupal_render(drupal_get_form('dpidam_page_admin_search_add_new_profile_form'));
  $output .= drupal_render(drupal_get_form('dpidam_page_admin_search_general_settings_form'));

  return $output;
}

/**
 * Add a new search profile form
 */
function dpidam_page_admin_search_add_new_profile_form($form, $form_state) {
  $form['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a new profile'),
    '#collapsible' => FALSE,
  );

  $form['add']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('New profile machine name'),
    '#description' => t('Must be a machine readable name (no spaces, no capital letters, no punctuation, "-" allowed).'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['add']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('New profile label'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['add']['add-submit'] = array(
    '#type' => 'submit',
    '#title' => t('Add'),
    '#value' => t('Add'),
  );

  $form['add']['default_profile'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );

  return $form;
}

/**
 * Validation of the dpidam_page_admin_search_add_new_profile_form
 */
function dpidam_page_admin_search_add_new_profile_form_validate($form, $form_state){
  $values = $form_state['values'];

  if (!preg_match('/^[a-z0-9\-]*$/', $values['name'])) {
    form_set_error('name', t('Please enter a machine readale name (no spaces, no capital letters, no punctuation, "-" allowed).'));
  }
}

/**
 * Submit of the dpidam_page_admin_search_add_new_profile_form
 */
function dpidam_page_admin_search_add_new_profile_form_submit($form, $form_state){
  $values = $form_state['values'];
  $values['settings'] = array();

  $existing_profiles = dpidam_api_get_search_profiles();
  if (empty($existing_profiles)) {
    $values['default_profile'] = 1;
  }

  drupal_write_record('dpidam_search_profiles', $values);
  drupal_set_message(t('Profile "@profile" saved! Do not forget to configure it before using it!', array('@profile' => $values['label'])));
}

/**
 * General settings for DAM search
 */
function dpidam_page_admin_search_general_settings_form($form, $form_state) {
  $form['gen'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['gen']['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query URL'),
    '#description' => t('URL that will actually be called to perform a search query'),
    '#default_value' => dpi_variable_get('dpidam_search_query_URL', ''),
    '#required' => TRUE,
  );

  $form['gen']['thumbnails'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnails URL'),
    '#description' => t('URL to get thumbnails from packages'),
    '#default_value' => dpi_variable_get('dpidam_search_thumbnails_URL', ''),
    '#required' => TRUE,
  );

  $form['gen']['image_subdirectory'] = array(
    '#type' => 'textfield',
    '#title' => t('Fetched Thumbnails Subdirectory'),
    '#description' => t('Directory where thumbnails fetched from DAM module will be stored locally, relatively to the "public" stream.'),
    '#default_value' => dpi_variable_get('dpidam_image_subdirectory', ''),
    '#required' => TRUE,
  );

  $form['gen']['thesaurus'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the search parameters\' thesaurus'),
    '#description' => t('Return list of defined NewsRooms, Products, Zones, Editions, ...'),
    '#default_value' => dpi_variable_get('dpidam_search_thesaurus_URL', ''),
    '#required' => TRUE,
  );

  $form['gen']['gen-submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['gen']['clear-thesaurus'] = array(
    '#type' => 'submit',
    '#value' => t('Clear parameters\' thesaurus cache')
  );

  return $form;
}

function dpidam_page_admin_search_general_settings_form_validate($form, $form_state) {
  $values = $form_state['values'];
  $clicked_button = $form_state['clicked_button'];

  switch ($clicked_button['#value']) {
    case $form['gen']['gen-submit']['#value']:
      if (!valid_url($values['query'], TRUE)) {
        form_set_error('query', t('The given query URL is not a valid URL'));
      }

      if (!valid_url($values['thumbnails'], TRUE)) {
        form_set_error('thumbnails', t('The given thumbnails URL is not a valid URL'));
      }

      if (!valid_url($values['thesaurus'], TRUE)) {
        form_set_error('thesaurus', t('The given search parameters\' thesaurus URL is not a valid URL'));
      }

      $path = 'public://'.$form_state['values']['image_subdirectory'];
      if (!file_prepare_directory($path)) {
        form_set_error('image_subdirectory', t('Error directory path : the path provided is not an existing writable directory'));
      }

      break;

    case $form['gen']['clear-thesaurus']['#value']:

      break;
  }
}

function dpidam_page_admin_search_general_settings_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $clicked_button = $form_state['clicked_button'];

  // If the URLs are changed, we better clear the cache
  cache_clear_all('dpidam_thesaurus', 'cache');

  switch ($clicked_button['#value']) {
    case $form['gen']['gen-submit']['#value']:
      dpi_variable_set('dpidam_search_query_URL', $values['query']);
      dpi_variable_set('dpidam_search_thumbnails_URL', $values['thumbnails']);
      dpi_variable_set('dpidam_search_thesaurus_URL', $values['thesaurus']);
      dpi_variable_set('dpidam_image_subdirectory', $values['image_subdirectory']);
      drupal_set_message(t('Configuration saved'));
      break;

    case $form['gen']['clear-thesaurus']['#value']:
      drupal_set_message(t('Cache cleared'));
      break;
  }
}

/**
 * Edit a DAM search profile form
 */
function dpidam_page_admin_search_edit_profile_form($form, $form_state, $pid) {
  $redirect_url = 'admin/dpi247/dpidam/search';
  $error = FALSE;

  $thesaurus = _dpidam_page_admin_search_get_thesaurus($error);

  if (!$error) {
    $default_values = _dpidam_page_admin_search_get_default_values();
    if ($profile = dpidam_api_get_search_profile($pid)) {
      $default_values = array_merge($default_values, $profile['settings']);
      $default_values['label'] = $profile['label'];
    } else {
      $error = t('No matching search profile');
    }
  }

  if ($error) {
    drupal_set_message($error, 'warning');
    drupal_goto($redirect_url);
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $default_values['label'],
    '#required' => TRUE,
  );

  list($options, $js_options) = _dpidam_page_admin_search_populate_thesaurus_options($thesaurus);

  $filetype_options = (isset($options['filetype']) && is_array($options['filetype'])) ? $options['filetype'] : array();

  // @todo Handle subtypes
  $form['filetype'] = array(
    '#type' => 'select',
    '#title' => t('Type of object on which the research is performed'),
    '#description' => t('The research will only be performed on selected type'),
    '#options' => $filetype_options,
    '#default_value' => $default_values['filetype'],
    '#multiple' => TRUE,
  );

  $form['advanced'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use advanced search'),
    '#description' => t('The advanced search increases the search possibilities (search on several words, exclude words, ...)'),
    '#default_value' => $default_values['advanced'],
  );

  $form['withthumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include thumbnails'),
    '#description' => t('Include photos thumbnails in search results'),
    '#default_value' => $default_values['withthumbs'],
  );

  $form['thesaurus_options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );

  $newsroom_options = (isset($options['newsroom']) && is_array($options['newsroom'])) ? $options['newsroom'] : array();
  $product_options = (isset($options['product']) && is_array($options['product'])) ? $options['product'] : array();
  $zone_options = (isset($options['zone']) && is_array($options['zone'])) ? $options['zone'] : array();
  $edition_options = (isset($options['edition']) && is_array($options['edition'])) ? $options['edition'] : array();

  $js_product_options = (isset($js_options['product']) && is_array($js_options['product'])) ? $js_options['product'] : array();
  $js_zone_options = (isset($js_options['zone']) && is_array($js_options['zone'])) ? $js_options['zone'] : array();
  drupal_add_js(array('damSearchSelectOptions' => array('products' => $js_product_options, 'zones' => $js_zone_options)), 'setting');
  drupal_add_js(drupal_get_path('module', 'dpidam').'/scripts/dam_search_dependent.js');

  $form['thesaurus_options']['newsroom'] = array(
    '#type' => 'select',
    '#title' => t('NewsRoom'),
    '#options' => $newsroom_options,
    '#default_value' => $default_values['newsroom'],
    '#multiple' => TRUE,
  );

  $form['thesaurus_options']['product'] = array(
    '#type' => 'select',
    '#title' => t('Product'),
    '#options' => $product_options,
    '#default_value' => $default_values['product'],
    '#multiple' => TRUE,
  );

  $form['thesaurus_options']['zone'] = array(
    '#type' => 'select',
    '#title' => t('Zone'),
    '#options' => $zone_options,
    '#default_value' => $default_values['zone'],
    '#multiple' => TRUE,
  );

  $form['thesaurus_options']['edition'] = array(
    '#type' => 'select',
    '#title' => t('Edition'),
    '#options' => $edition_options,
    '#default_value' => $default_values['edition'],
    '#multiple' => TRUE,
  );

  $form['cn'] = array(
    '#type' => 'select',
    '#title' => t('Connection NewsRoom'),
    '#description' => t('The NewsRoom of the user performing the search'),
    '#options' => array('none' => t('None')) + $newsroom_options,
    '#default_value' => $default_values['cn'],
  );

  $level_options = array(
    1 => t('Free'),
    2 => t('Published'),
    3 => t('Not published'),
    4 => t('PhoDoc'),
  );
  $form['level'] = array(
    '#type' => 'select',
    '#title' => t('Search level'),
    '#description' => t('Visibility level for searched articles :<br>
        Free : only free articles<br>
        Published : articles published with access limitations<br>
        Not published : articles not published<br>
        PhoDoc : articles which can\'t be found and used without PhoDoc\'s specific authorisation'),
    '#options' => $level_options,
    '#default_value' => $default_values['level'],
  );

  $sort_options = array(
    'none' => t('None'),
    'date' => t('Date'),
    'weight' => t('Weight'),
  );
  $form['sort'] = array(
    '#type' => 'select',
    '#title' => t('Sort Options'),
    '#options' => $sort_options,
    '#default_value' => $default_values['sort'],
    '#multiple' => TRUE,
    '#description' => t('Sort Options :<br>
        Date: Date Desc / Asc<br>
        Pertinence : Desc (only)<br>'),
  );

  $datefilter_options = array(
    'none' => t('None'),
    'anytime' => t('Anytime'),
    'today' => t('Today'),
    'yesterday' => t('Yesterday'),
    'lastweek' => t('Last Week'),
    'lastmonth' => t('Last Month'),
    'last6month' => t('Last six Months'),
    'lastyear' => t('Last Year'),
    'last5year' => t('Last 5 Year'),
    'last10year' => t('Last 10 Year'),
    // 'customrange' => t('Custom Range (todo)'),
  );
  $form['datefilter'] = array(
    '#type' => 'select',
    '#title' => t('Date filters'),
    '#options' => $datefilter_options,
    '#default_value' => $default_values['datefilter'],
    '#multiple' => TRUE,
    '#description' => t('Date Filter options.<br>'),
  );
   
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );

  $form['name'] = array(
    '#type' => 'hidden',
    '#value' => $profile['name'],
  );

  $form['default_profile'] = array(
    '#type' => 'hidden',
    '#value' => $profile['default_profile'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), $redirect_url),
  );

  return $form;
}

function dpidam_page_admin_search_edit_profile_form_validate($form, $form_state) {
  // @todo Validations
  // Simple search => 1! filetype
  // Simple search => only newsrooms
}

function dpidam_page_admin_search_edit_profile_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $settings = array();
  $settings['filetype'] = $values['filetype'];
  $settings['advanced'] = $values['advanced'];
  $settings['withthumbs'] = $values['withthumbs'];
  $thesaurus_options = $values['thesaurus_options'];
  $settings['newsroom'] = isset($thesaurus_options['newsroom']) ? $thesaurus_options['newsroom'] : 'all';
  $settings['product'] = isset($thesaurus_options['product']) ? $thesaurus_options['product'] : 'all';
  $settings['zone'] = isset($thesaurus_options['zone']) ? $thesaurus_options['zone'] : 'all';
  $settings['edition'] = isset($thesaurus_options['edition']) ? $thesaurus_options['edition'] : 'all';
  $settings['cn'] = $values['cn'];
  $settings['level'] = $values['level'];
  $settings['sort'] = $values['sort'];
  $settings['datefilter'] = $values['datefilter'];

  $profile = array(
    'pid' => $values['pid'],
    'name' => $values['name'],
    'label' => $values['label'],
    'default_profile' => $values['default_profile'],
    'settings' => $settings,
  );

  drupal_write_record('dpidam_search_profiles', $profile, array('pid'));
  $form_state['redirect'] = 'admin/dpi247/dpidam/search';
}

/**
 * Delete a DAM search profile confirmation form
 */
function dpidam_page_admin_search_delete_profile_form($form, $form_state, $pid) {
  if ($profile = dpidam_api_get_search_profile($pid)) {
    $question = t('Are you sure you want to delete this search profile?');

    $add_form = array();
    $add_form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $pid,
    );

    $form = confirm_form($add_form, $question, 'admin/dpi247/dpidam/search');

    return $form;
  } else {
    drupal_set_message(t('No matching search profile'), 'warning');
    drupal_goto('admin/dpi247/dpidam/search');
  }
}

function dpidam_page_admin_search_delete_profile_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $pid = $values['pid'];
  $profile = dpidam_api_get_search_profile($pid);

  db_delete('dpidam_search_profiles')
    ->condition('pid', $pid)
    ->execute();
  if ($profile['default_profile'] == 1) {
    $db_min_pid = db_select('dpidam_search_profiles', 'dsp');
    $db_min_pid->addExpression('MIN(dsp.pid)', 'min_pid');
    $res_min_pids = $db_min_pid->execute();
    $min_pid = FALSE;
    foreach ($res_min_pids as $res_min_pid) {
      $min_pid = $res_min_pid->min_pid;
      break;
    }
    if ($min_pid) {
      db_update('dpidam_search_profiles')
        ->fields(array('default_profile' => 1))
        ->condition('pid', $min_pid)
        ->execute();
    }
  }
  $form_state['redirect'] = 'admin/dpi247/dpidam/search';
}

/**
 * Set a DAM search profile as default profile
 */
function dpidam_page_admin_search_set_default_profile($pid) {
  db_update('dpidam_search_profiles')
    ->fields(array('default_profile' => 0))
    ->execute();
  db_update('dpidam_search_profiles')
    ->fields(array('default_profile' => 1))
    ->condition('pid', $pid)
    ->execute();
  drupal_goto('admin/dpi247/dpidam/search');
}

/**
 * Get links to available operations
 */
function _dpidam_page_admin_search_get_operations($profile) {
  $return = l(t('Edit'), 'admin/dpi247/dpidam/search/edit/'.$profile['pid']).' - ';
  $return .= l(t('Delete'), 'admin/dpi247/dpidam/search/delete/'.$profile['pid']).' - ';
  $return .= l(t('Set as default'), 'admin/dpi247/dpidam/search/setdefault/'.$profile['pid']);
  return $return;
}

/**
 * Get default values for the profile edit form
 */
function _dpidam_page_admin_search_get_default_values() {
  return array(
    'label' => '',
    'filetype' => 'Text',
    'advanced' => 0,
    'newsroom' => 'all',
    'product' => 'all',
    'zone' => 'all',
    'edition' => 'all',
    'cn' => '',
    'level' => 1,
    'sort' => 'none',
    'datefilter' => 'none',
    'withthumbs' => 0,
  );
}

/**
 * Get thesaurus from URL or cache
 */
function _dpidam_page_admin_search_get_thesaurus(&$error) {
  $thesaurus = array();

  $thesaurus_URL = dpi_variable_get('dpidam_search_thesaurus_URL', '');
  if (empty($thesaurus_URL) || !valid_url($thesaurus_URL)) {
    $error = t('The thesaurus URL has not been set or is invalid');
  }

  $cached_thesaurus = cache_get('dpidam_thesaurus', 'cache');
  if (!is_object($cached_thesaurus) || empty($cached_thesaurus) || !isset($cached_thesaurus->data) || empty($cached_thesaurus->data)) {
    $http_thesaurus = drupal_http_request($thesaurus_URL);
    module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
    if (dpicommons_is_http_request_successfull($http_thesaurus)) {
      $thesaurus = json_decode($http_thesaurus->data);
      if (!$thesaurus) {
        $error = t('Unable to read search parameters\' thesaurus datas');
      } else {
        cache_set('dpidam_thesaurus', $thesaurus);
      }
    } else {
      $error = t('Couldn\'t reach search parameters\' thesaurus URL');
    }
  } else {
    $thesaurus = $cached_thesaurus->data;
  }

  return $thesaurus;
}

/**
 * Populate selectboxes options according to the thesaurus
 */
function _dpidam_page_admin_search_populate_thesaurus_options($thesaurus) {
  $all_options = array(
    'filetype' => array(),
    'newsroom' => array('all' => t('All')),
    'product' => array('all' => t('All')),
    'zone' => array('all' => t('All')),
    'edition' => array('all' => t('All')),
  );

  $js_options = array(
    'product' => array(),
    'zone' => array(),
  );

  if (isset($thesaurus->thInfo->Type->values) && is_array($thesaurus->thInfo->Type->values)) {
    foreach ($thesaurus->thInfo->Type->values as $filetype_value) {
      $all_options['filetype'][$filetype_value->code] = $filetype_value->displayName;
    }
  }

  if (isset($thesaurus->thInfo->Newsroom->values) && is_array($thesaurus->thInfo->Newsroom->values)) {
    foreach ($thesaurus->thInfo->Newsroom->values as $newsroom_value) {
      $all_options['newsroom'][$newsroom_value->code] = strtolower($newsroom_value->displayName);
      $js_options['product'][$newsroom_value->code] = array();
    }
  }

  if (isset($thesaurus->thInfo->Product->values) && is_array($thesaurus->thInfo->Product->values)) {
    foreach ($thesaurus->thInfo->Product->values as $product_value) {
      if (isset($product_value->links->Newsroom) && is_array($product_value->links->Newsroom)) {
        foreach ($product_value->links->Newsroom as $linked_newsroom) {
          $js_options['product'][$linked_newsroom->code][] = array(
            'code' => $product_value->code,
            'dname' => $product_value->displayName,
          );
        }
      }
      $all_options['product'][$product_value->code] = $product_value->displayName;
      $js_options['zone'][$product_value->code] = array();
    }
  }

  if (isset($thesaurus->thInfo->Zone->values) && is_array($thesaurus->thInfo->Zone->values)) {
    foreach ($thesaurus->thInfo->Zone->values as $zone_value) {
      if (isset($zone_value->links->Product) && is_array($zone_value->links->Product)) {
        foreach ($zone_value->links->Product as $linked_product) {
          $js_options['zone'][$linked_product->code][] = array(
            'code' => $zone_value->code,
            'dname' => $zone_value->displayName,
          );
        }
      }
      $all_options['zone'][$zone_value->code] = $zone_value->displayName;
    }
  }

  if (isset($thesaurus->thInfo->Edition->values) && is_array($thesaurus->thInfo->Edition->values)) {
    foreach ($thesaurus->thInfo->Edition->values as $edition_value) {
      $all_options['edition'][$edition_value->code] = $edition_value->displayName;
    }
  }

  return array($all_options, $js_options);
}
