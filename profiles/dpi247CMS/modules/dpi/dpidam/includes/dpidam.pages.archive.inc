<?php

/**
 * Archive management settings form
 */
function dpidam_page_admin_archive_form($form, $form_state) {
  $form['dpidam_archive_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active archive retrieving'),
    '#default_value' => dpi_variable_get('dpidam_archive_on', FALSE),
    '#description' => t('<strong>Untill this checkbox is not checked, no archived packages will be available!</strong>'),
  );

  $importer = dpimport_api_get_all_importer_settings();
  $options = array('' => t('Please choose an importer'));
  foreach ($importer as $imp){
    $options[$imp->name] = $imp->name;
  }
  $form['dpidam_archive_importer'] = array(
    '#type' => 'select',
    '#title' => t('Importer to use to extract package'),
    '#description' => t('You must have an importer of type packages to use the archive'),
    '#default_value' => dpi_variable_get('dpidam_archive_importer', ''),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['dpidam_archive_geturl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the DAM to retrieve archive'),
    '#default_value' => dpi_variable_get('dpidam_archive_geturl', ''),
    '#required' => TRUE,
  );

  $form['dpidam_archive_ttl'] = array(
    '#type' => 'textfield',
    '#title' => t('Time to live of the cache of the page in Varnish (in hours)'),
    '#default_value' => dpi_variable_get('dpidam_archive_ttl', '24'),
    '#required' => TRUE,
  );

  $form['dpidam_security'] = array(
    '#type' => 'fieldset',
    '#title' => t('Security limitations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $black_or_white_options = array(
    'white' => t('White list'),
    'black' => t('Black list'),
  );
  $form['dpidam_security']['dpidam_archive_regex_black_or_white'] = array(
    '#type' => 'radios',
    '#title' => t('Uses regexes as a white or a black list'),
    '#options' => $black_or_white_options,
    '#default_value' => dpi_variable_get('dpidam_archive_regex_black_or_white', 'black'),
    '#prefix' => t('These settings are made to avoid, as much as possible, to send URLs requests to the DAM when we souldn\'t.
        There are two types of limitations :<br>
        - Use regexes to perform requests only on some pattern of URLs<br>
        - Use a failed requests limitation per user IP<br>
        Note that those limitations can be used together.'),
  );

  $form['dpidam_security']['dpidam_archive_regexes'] = array(
    '#type' => 'textarea',
    '#title' => t('Regexes list'),
    '#description' => t('Perform URL request only on URL matching (or not matching) the following patterns.<br>
        <strong>One pattern per line, do not forget to use delimiters.</strong>'),
    '#default_value' => dpi_variable_get('dpidam_archive_regexes', ''),
  );

  $form['dpidam_security']['dpidam_archive_failed_recovery'] = array(
    '#type' => 'textfield',
    '#title' => t('Failed archive recovery limit per day'),
    '#description' => t('Each time a page is not found (404) the DAM is called to find a matching package.<br>
        To avoid overloading the DAM with fake searches you can limit the amount of package recovery failures per IP and per day.<br>
        For example, if set to 100, a given user will be able to get as many real archived package as requested but will be limited to 100 failed requested.<br>
        If set to 0, no check will be performed.'),
    '#default_value' => dpi_variable_get('dpidam_archive_failed_recovery', 500),
  );

  // Add the destination field
  $items = dpi_variable_get('dpidam_archive_defaultdestination', array());
  $langcode = 'und';
  $field = array(
    'field_name' => 'field_destinations',
    'type' => 'destination',
    'cardinality' => 1,
    'columns' => array('tid' => '', 'target' => '', 'layout' => '', 'rank' => '', 'virtual_rank' => ''),
  );
  $instance = array(
    'label' => '',
    'description' => '',
    'widget' => array('type' => 'destination_default_widget', 'module' => 'dpidestinations'),
    'entity_type' => 'node',
    'bundle' => 'package',
    'required' => TRUE,
  );
  $form_copy = $form;
  $form_copy['#parents'] = array();
  $form_copy['#entity'] = new stdClass();
  $form_state_copy = $form_state;
  $form_state_copy['programmed'] = NULL;
  $form_state_copy['dpidam_archive_defaultdestination'] = array();
  foreach ($items as $delta => $item) {
    $form_state_copy['dpidam_archive_defaultdestination'][$item['element_key']] = $item;
  }
  $form['dpidam_archive_defaultdestination'] = field_multiple_value_form($field, $instance, $langcode, $items, $form_copy, $form_state_copy);
  $form['dpidam_archive_defaultdestination'][0]['#title'] = t('Fake destination set in retrieved archived packages');
  $form['dpidam_archive_defaultdestination']['#tree'] = TRUE;

  return dpi_system_settings_form($form);
}

/**
 * Validation of the page_admin_archive_form
 */
function dpidam_page_admin_archive_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $ttl = $values['dpidam_archive_ttl'];
  if (!is_numeric($ttl) || $ttl < 1) {
    form_set_error('dpidam_archive_ttl', t('The TTL must be a numerical value greater or equal than 1 hour.'));
  }

  if (!valid_url($values['dpidam_archive_geturl'], TRUE)) {
    form_set_error('dpidam_archive_geturl', t('The given URL is not a valid URL'));
  }

  $regex_type = $values['dpidam_archive_regex_black_or_white'];
  $regexes_list = $values['dpidam_archive_regexes'];
  if ($regex_type == 'white' && empty($regexes_list)) {
    form_set_error('dpidam_security][dpidam_archive_regexes', t('You have to set some patterns for your white list otherwise no URL requests are going to be sent to the DAM.'));
  }
  if ($regex_type == 'black' && empty($regexes_list)) {
    drupal_set_message(t('No patterns given if your black list, every URLs are going to be sent to the DAM.'), 'warning');
  }

  $failed_limit = $values['dpidam_archive_failed_recovery'];
  if (!is_numeric($failed_limit) || $failed_limit < 0) {
    form_set_error('dpidam_security][dpidam_archive_failed_recovery', t('The failed limit must be a numerical value greater or equal than 0.'));
  }

  $destination_tid = $values['dpidam_archive_defaultdestination'][0]['tid'];
  $destination_target = $values['dpidam_archive_defaultdestination'][0]['target'];
  if (empty($destination_tid) || $destination_tid == 0 || empty($destination_target) || $destination_target == 'none') {
    form_set_error('dpidam_archive_defaultdestination', t('The default destination for DAM package can\'t be left empty.'));
  }
}
