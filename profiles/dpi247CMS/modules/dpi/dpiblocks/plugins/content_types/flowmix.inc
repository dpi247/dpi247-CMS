<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title'              => 'FlowMix',
  'no title override'  => TRUE,
  'defaults'           => dpiblocks_flowmix_content_type_form_summary_defaults(),
  // Make sure the edit form is only used for some subtypes.
  'add form'           => array(
    'dpiblocks_flowmix_content_type_form_summary_form'        => t('Summary'),
    'dpiblocks_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
    'dpiblocks_flowmix_content_type_form_plugins_form'        => t('Create Flow 2/2: Plugin settings'),
  ),
  'edit form'          => array(
    'dpiblocks_flowmix_content_type_form_summary_form'        => t('Summary'),
    'dpiblocks_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
    'dpiblocks_flowmix_content_type_form_plugins_form'        => t('Create Flow 2/2: Plugin settings'),
  ),
  'edit text'          => t('Edit'),
  'all contexts'       => TRUE,
  'category'           => t('DPI Blocks'),
  'group'              => t('DPI Core'),
);

/**
 * Implementation of hook_content_type_content_types()
 *
 * @return array $flowmixes
 *   All FlowMix blocks, plus an empty new one
*/
function dpiblocks_flowmix_content_type_content_types() {
  $results = dpiblocks_flowmix_get_all();

  // Display existing FlowMixes in the "DPI Extras" category
  foreach ($results as $record) {
    $flowmixes[$record->machine_name] = array(
      'title'        => t('FlowMix : '.$record->title),
      'machine_name' => $record->machine_name,
      'description'  => t(''),
      'add form'     => array(
        'dpiblocks_flowmix_content_type_form_summary_form'        => t('Summary'),
        'dpiblocks_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
        'dpiblocks_flowmix_content_type_form_plugins_form'        => t('Create Flow 2/2: Plugin settings'),
      ),
      'edit form'    => array(
        'dpiblocks_flowmix_content_type_form_summary_form'        => t('Summary'),
        'dpiblocks_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
        'dpiblocks_flowmix_content_type_form_plugins_form'        => t('Create Flow 2/2: Plugin settings'),
      ),
      'category'     => t('DPI Blocks'),
      'group'        => t('DPI Core'),
    );
  }

  // Display a "New FlowMix" link on the "DPI Core" section
  $flowmixes['new_flowmix'] = array(
    'name'           => 'flowmix',
    'title'          => t('New FlowMix'),
    'top level'      => TRUE,
    'category'       => t('Custom'),
    'icon'              => drupal_get_path("module", "dpiblocks").'/icons/add_content.ico',
    'group'          => t('DPI Core'),
    'description'    => t('Mix external flows from multiple sources.'),
    'add form' => array(
      'dpiblocks_flowmix_content_type_form_summary_form'        => t('Summary'),
      'dpiblocks_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
      'dpiblocks_flowmix_content_type_form_plugins_form'        => t('Create Flow 2/2: Plugin settings'),
    ),
    'all contexts'   => TRUE,
  );

  return $flowmixes;
}

/**
 * Implementation of content_type_render()
 *
 * @param $subtype
 * @param $conf
 * @param $panel_args
 * @param $context
 *
 * @return stdClass
 *   The block
 */
function dpiblocks_flowmix_content_type_render($subtype, $conf, $panel_args, $context) {
  // Get settings from DB
  $settings = dpiblocks_flowmix_content_type_generate_settings_for_subtype($subtype);

  // Create new block
  $block = new stdClass();
  $block->module = 'dpiblocks';
  $block->title = $settings->title;

  // Cache management
  $feed = dpicache_get_cache_entry('dpiblocks_flowmix_feed', $subtype);

  // Theming
  $theme_vars = array(
    'subtype' => $subtype,
    'context' => $context,
    'feed' => $feed,
    'settings' => dpiblocks_flowmix_content_type_generate_settings_for_subtype($subtype),
  );
  if ($settings->override_theme && $settings->override_theme_text != '') {
    $block->content = theme($settings->override_theme_text, $theme_vars);
  }
  else {
    $block->content = theme('dpiblocks_flowmix', $theme_vars);
  }

  return $block;
}

function dpiblocks_flowmix_content_type_generate_feed($subtype) {
  // Get settings from DB
  $settings = dpiblocks_flowmix_content_type_generate_settings_for_subtype($subtype);

  // Get the old feed, it will be served in case of error
  // and will be used to detect a modification in the feeds
  $hash = _dpicache_get_hash('dpiblocks_flowmix_feed', func_get_args());
  $old_feed = array();
  if ($old_cache = dpicache_fetch_cache('dpiblocks_flowmix_feed', $hash)) {
    $old_feed = $old_cache;
  }

  // Build the mix schema, according to configured pattern
  if ($settings->mix_schema == 'mix' && isset($settings->own_schema) && !empty($settings->own_schema)) {
    $mix_schema = explode(',', $settings->own_schema);
  } else {
    if ($settings->mix_schema == 'alt') {
      $mix_schema = array();
      for ($i = 0; $i < sizeof($settings->flows); $i++) $mix_schema[] = $i + 1;
    } else {
      $mix_schema = array('any');
    }
  }

  // Build an array of items for each feed
  $feeds_array = array();
  $empty_feed = FALSE;
  foreach ($settings->flows as $flow) {
    $flow = (array)$flow;
    $flow['caid'] = $settings->caid;
    $temp_feed_array = dpiblocks_flowmix_content_type_create_array_from_uri($flow);
    if ($temp_feed_array) {
      $feeds_array[] = $temp_feed_array;
    } else {
      $empty_feed = TRUE;
    }
  }

  // If one of the feeds is empty return an empty result
  if ($empty_feed) {
    return !empty($old_feed) ? $old_feed : array();
  }

  // Shuffle the results for the "random" schema
  if ($settings->mix_schema == 'ran' && $mix_schema == array('any')) {
    // In case of random : merge all feeds and shuffle result
    $feeds_array = array(call_user_func_array('array_merge', $feeds_array));
    shuffle($feeds_array[0]);
  }

  // Order the items according to the schema
  $feed = array();
  $maxitem_global = $settings->maxitem_global;
  for ($i = 0; $i < $maxitem_global; $i++) {
    $schema_elem = $mix_schema[$i % sizeof($mix_schema)];
    $available_items = dpiblocks_flowmix_content_type_get_available_items_from_schema($schema_elem, $feeds_array);
    $most_recent_index = dpiblocks_flowmix_content_type_flowmix_get_most_recent_from_availables($available_items);
    $item = array_shift($feeds_array[$most_recent_index]);
    if (isset($item)) {
      $item->__flow_id = $most_recent_index;
      array_push($feed, $item);
    }
  }

  // Apply filter to body
  foreach ($feed as $id => $item) {
    $entity_wrapper = entity_metadata_wrapper('node', $item);
    if (isset($entity_wrapper->field_textbody) && $body_value = $entity_wrapper->field_textbody->value()) {
      $format = $settings->flows[$item->__flow_id]->body_html;
      $cleaned_body = str_replace(array('<p>', '</p>'), '', check_markup($body_value['value'], $format));
      $entity_wrapper->field_textbody->set(array('value' => $body_value['value'], 'format' => $format, 'safe_value' => $cleaned_body));
    }
  }

  // Check if an item has changed, this is calculated to avoid a cache flush if not necessary
  $modif = FALSE;
  for ($i = 0; $i < sizeof($feed); $i++) {
    $elem_package = $feed[$i];
    $feed[$i]->feed_md5 = _dpiblocks_get_md5_from_package($elem_package);
    if (!isset($old_feed[$i]->feed_md5) || ($feed[$i]->feed_md5 != $old_feed[$i]->feed_md5)) {
      $modif = TRUE;
      break;
    }
  }

  // If we have a modification of one of the feeds we flush the current page cache
  if (false&&$modif) {
    //@todo : adapter à la gestion des caches
    $internal_url = drupal_get_normal_path(trim($_SERVER[REQUEST_URI], '/'));
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    wallytoolbox_flush_all_pages($internal_url, TRUE);
  }

  return $feed;
}

/**
 * Build a array based on an feed.
 *
 * @param $flow_uri
 *   The feed URI.
 * @param $maxitem
 *   A limitation in the number of returned feeds.
 * @param $settings
 *   The settings of the widget.
 *
 * @return
 *   The array.
 */
function dpiblocks_flowmix_content_type_create_array_from_uri($flow) {
  ctools_include('plugins');
  if ($function = ctools_plugin_get_function(dpiblocks_api_get_plugin_by_name($flow['plugin_name']), 'build')) {
    // To make sure the conf and feeds_md5 have not been serialized multiple times
    while (isset($flow['conf']) && is_string($flow['conf'])) {
      $flow['conf'] = unserialize($flow['conf']);
    }
    while (isset($flow['feeds_md5']) && is_string($flow['feeds_md5'])) {
      $flow['feeds_md5'] = unserialize($flow['feeds_md5']);
    }

    // Call the build function of the flow plugin and check the success of the process
    $struct_array = $function($flow);
    if ($struct_array['Packages']['__log']['success'] && !empty($struct_array['Packages']['Package'])) {
      return $struct_array['Packages']['Package'];
    }
  }

  return FALSE;
}

/**
 * Select the most recent feed of every flows list that matches the
 * schema.
 *
 * @param $schema_elem
 *   The schema containing the IDs of the allowed flows list(s).
 * @param $feed_array
 *   Array of all flows lists.
 *
 * @return
 *   The array containing the selected feeds and their respective flows list ID.
 */
function dpiblocks_flowmix_content_type_get_available_items_from_schema($schema_elem, $feed_array) {
  $available_items = array();
  $schema_elems = explode('-', $schema_elem);
  for ($i = 0; $i < sizeof($feed_array); $i++) {
    // "$i + 1" is necessary because the flow IDs starts at 0 but a user generated schema starts at 1
    if (in_array($i + 1, $schema_elems) || $schema_elems[0] == 'any') {
      if (isset($feed_array[$i][0]) && !empty($feed_array[$i][0])) {
        $feed_array[$i][0]->arrayID = $i;
        $available_items[] = $feed_array[$i][0];
      }
    }
  }
  return $available_items;
}

/**
 * Select the most recent feed (based on the editorial publication date) in the
 * given array.
 *
 * @param $available_items
 *   The array containing the available feeds.
 *
 * @return
 *   The ID of the flow list containing the most recent feed.
 */
function dpiblocks_flowmix_content_type_flowmix_get_most_recent_from_availables($available_items) {
  $most_recent = array_shift($available_items);

  foreach ($available_items as $available_item) {
    // Get editorial publication date of each package
    $most_recent_wrapper = entity_metadata_wrapper('node', $most_recent);
    $most_recent_date = $most_recent_wrapper->field_editorialpublicationdate->value();
    $current_wrapper = entity_metadata_wrapper('node', $available_item);
    $current_date = $current_wrapper->field_editorialpublicationdate->value();

    if ($current_date > $most_recent_date) {
      $most_recent = $available_item;
    }
  }

  return $most_recent->arrayID;
}

/**
 * Main form of the flowmix edition
 */
function dpiblocks_flowmix_content_type_form_summary_form($form, &$form_state) {
  $default_settings = $form_state['plugin']['defaults'];

  $current_pane = FALSE;
  if ($form_state['step'] == 'step1' && (!isset($form_state['conf']) || empty($form_state['conf']))) {
    // We have to reinitialize the cached pane at the first load of the form
    // If we didn't do that there would be a confusion between the cached pane and the current pane
    // after the submission of a new flow (when the "OK" button is pressed on the "plugin" form)
    $current_pane = $form_state['pane'];

    if ($current_pane->subtype != 'new_flowmix') {
      $default_settings = dpiblocks_flowmix_content_type_generate_settings_for_subtype($current_pane->subtype);
    } else {
          $form_state['display cache']->new_pane = panels_new_pane($pane->type, $pane->subtype, TRUE);

    }
  }
  elseif ($form_state['step'] == 'step1' && !isset($form_state['clicked_button']) && $form_state['input'] < 5) {
    // We want to load the settings for this flowmix, only when we are editing it
    // The "$form_state['input'] < 5" is there to make sure we just click the "Edit" button in the page manager
    $default_settings = dpiblocks_flowmix_content_type_generate_settings_for_subtype($form_state['conf']['flow_obj']->machine_name);
    $form_state['conf'] = array();
  }

  // Get current conf or defaults
  $settings = dpiblocks_flowmix_content_type_get_conf($form_state, $default_settings);
  if (array_key_exists('temp_flow', $settings->flows)) {
    unset($settings->flows['temp_flow']);
  }
  // Save settings for next steps
  $form_state['settings'] = $settings;

  if (isset($settings->fid)) {
    $form['fid'] = array(
      '#type'          => 'hidden',
      '#value'         => $settings->fid,
    );
  }
  
  $form['caid'] = array (
    '#type'            => 'hidden',
    '#value'           => $settings->caid,
  );

  // We are using a reusable flowmix, there's no form for this case
  if ($current_pane && $current_pane->subtype != 'new_flowmix') {
    unset($form['buttons']['next']);

    $form['buttons']['save'] = array(
      '#type'          => 'submit',
      '#value'         => t('Save'),
      '#wizard type'   => 'finish',
      '#name'          => 'save',
    );

    return $form;
  }

  // Display current flows
  foreach ($settings->flows as $key => $flow) {
    $form['flows'][$key]['edit_action'] = array(
      '#type'          => 'submit',
      '#value'         => t('Edit'),
      '#wizard type'   => 'next',
      '#name'          => 'edit_'.$key,
      '#next'          => 'step2'
    );

    $form['flows'][$key]['delete_action'] = array(
      '#type'          => 'submit',
      '#value'         => t('Remove'),
      '#wizard type'   => 'next',
      '#name'          => 'delete_'.$key,
      '#next'          => 'step2'
    );

    $form['flows'][$key]['title'] = array(
      '#markup'        => $flow->name,
    );
  }

  $form['title'] = array(
    '#type'            => 'textfield',
    '#title'           => t('Title'),
    '#description'     => t('Human title of this flowmix.'),
    '#default_value'   => $settings->title,
    '#required'        => TRUE,
    '#weight'          => 0,
  );

  $form['machine_name'] = array(
    '#type'            => 'machine_name',
    '#title'           => t('Machine name'),
    '#machine_name'    => array(
      'exists'         => 'dpiblocks_flowmix_content_type_get_entity_by_subtype',
      'source'         => array('title'),
    ),
    '#description'     => t('The machine readable name of this flowmix. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
    '#default_value'   => $settings->machine_name,
  );

  $form['url'] = array(
    '#type'            => 'textfield',
    '#title'           => t('URL'),
    '#description'     => t('The title of the block will point to this URL.'),
    '#default_value'   => $settings->url,
  );

  $form['maxitem_global'] = array (
    '#type'            => 'textfield',
    '#title'           => t('Max item displayed in the final mixed flow'),
    '#default_value'   => $settings->maxitem_global,
    '#required'        => TRUE,
  );

  $mix_schemas = array(
    'alt'              => t('Alternate'),
    'any'              => t('Most Recent'),
    'ran'              => t('Random'),
    'mix'              => t('Define custom schema'),
  );
  $form['mix_schema'] = array(
    '#type'            => 'select',
    '#title'           => t('Schema used for mixing the feeds'),
    '#default_value'   => $settings->mix_schema,
    '#options'         => $mix_schemas,
  );

  $form['own_schema'] = array (
    '#type'            => 'textfield',
    '#title'           => t('Define your custom schema used for mixing the feeds'),
    '#description'     => t('Examples :
        1,2,3 / 1-2,3,2,1-3,2,2-3 / any,2,1-2,3,any,2 / ... see <a href="http://rossel.audaxis.com/xwiki/wiki/drupal/view/Main/User_RSS_mix" target="_new">RSS mix documentation</a>.'),
    '#default_value'   => $settings->own_schema,
    '#required'        => FALSE,
    '#states'          => array(
      'visible'        => array(
        ':input[name="mix_schema"]' => array('value' => 'mix'),
      ),
    ),
  );

  $form['override_theme'] = array (
    '#type'            => 'checkbox',
    '#default_value'   => $settings->override_theme,
    '#id'              => 'override-theme-checkbox',
    '#title'           => t('Override Theming Function'),
  );

  $form['override_theme_text'] = array (
    '#type'            => 'textfield',
    '#title'           => t('Theming Function Name'),
    '#default_value'   => $settings->override_theme_text,
    '#id'              => 'override-theme-textfield',
    '#states'          => array(
      'visible'        => array(
        ':input[name="override_theme"]' => array('checked' => TRUE),
      ),
    ),
  );

  // We want our owns buttons
  unset($form['buttons']['next']);
  unset($form['buttons']['previous']);

  // We can only save if there are flows defined
  if(count($settings->flows)) {
    $form['buttons']['save'] = array(
      '#type'          => 'submit',
      '#value'         => t('Save'),
      '#wizard type'   => 'finish',
      '#name'          => 'save',
    );
  }

  // Add a new flow and pass the next flow id via the "#name" attribute
  $current_max_key = (!isset($settings->flows) || empty($settings->flows)) ? 1 : max(array_keys($settings->flows)) + 1;
  $form['buttons']['add_flow'] = array(
    '#type'            => 'submit',
    '#value'           => t('Add a new flow'),
    '#wizard type'     => 'next',
    '#name'            => 'next_'.$current_max_key,
    '#next'            => 'step2',
  );

  return $form;
}

/**
 * Validate the main form of the flowmix edition
 */
function dpiblocks_flowmix_content_type_form_summary_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];

  // Check that the given URL responds
  if (!empty($values['url'])) {
    $check_url = drupal_http_request($file_uri, array('timeout' => _dpimport_variable_get('dpicommons_http_request_timeout', 3)));
    module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
    if (!dpicommons_is_http_request_successfull($check_url)) {
      form_set_error('url', t('The URL doesn\'t respond.'));
    }
  }
}

/**
 * Submit the main form of the flowmix edition
 */
function dpiblocks_flowmix_content_type_form_summary_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $submitted = $form_state['values'];

  if ((isset($form_state['input']['save']) && !isset($form_state['input']['title']))) {
    // This means no values were posted so we must be in the case where we use a reusable
    $flowmix_item = $form_state['settings'];
  } else {
    // Get the list of the values to be saved
    $defaults = (array)dpiblocks_flowmix_content_type_form_summary_defaults();
    unset($defaults['flows']);
    $defaults['fid'] = 0;
    foreach(array_keys($defaults) as $value) {
      // Set the values in the cache object to pass them to the next step
      if (isset($submitted[$value])) {
        $flowmix_item->$value = $submitted[$value];
      }
    }
  }
  // The current flows couldn't have been edited at this step, get the old ones back
  $flowmix_item->flows = $form_state['settings']->flows;

  // Check in which operation we are
  if (strpos($form_state['clicked_button']['#name'], 'delete') !== FALSE) {
    $form_state['flow_to_delete'] = str_replace('delete_', '', $form_state['clicked_button']['#name']);
  }
  elseif (strpos($form_state['clicked_button']['#name'], 'edit') !== FALSE) {
    if ($form_state['step'] == 'step1') {
      $flowmix_item->index = (int)str_replace('edit_', '', $form_state['clicked_button']['#name']);
      $form_state['clicked_button']['#next'] = 'step2';
    }
  }
  elseif (strpos($form_state['clicked_button']['#name'], 'next') !== FALSE) {
    if ($form_state['step'] == 'step1') {
      $flowmix_item->index = (int)str_replace('next_', '', $form_state['clicked_button']['#name']);
    }
  }
  elseif (strpos($form_state['clicked_button']['#name'], 'save') !== FALSE) {
    if ($form_state['step'] == 'step1') {
      dpiblocks_flowmix_content_type_save_reusable($form_state);
    }
  }
}

/**
 * Second step form, this form can have two purposes :
 *   - Choose the type of flow to add/edit and some basic settings
 *   - Confirmation of the delete of a flow
 */
function dpiblocks_flowmix_content_type_form_basicsetttings_form($form, $form_state) {
  // Delete confirmation form
  if ((isset($form_state['flow_to_delete']) && is_numeric($form_state['flow_to_delete'])) || in_array(t('Confirm'), $form_state['input'])) {
    $form['confirmation'] = array(
      '#type' => 'markup',
      '#value' => t('Are you sure you want to delete this flow?'),
    );

    if (in_array(t('Confirm'), $form_state['input'])) {
      foreach ($form_state['input'] as $key => $value) {
        if ($value == t('Confirm')) {
          $confirm_name = $key;
          break;
        }
      }
    } else {
      $confirm_name = 'confirm_'.$form_state['flow_to_delete'];
    }

    $form['buttons']['confirm'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#next' => 'step1',
      '#wizard type' => 'next',
      '#weight' => -1000,
      '#name' => $confirm_name,
    );

    unset($form['buttons']['next']);
  }
  // Flow add/edit form
  else {
    $flowmix_item = &$form_state['conf']['flow_obj'];

    // We come from the main form, create a temporary flow that will be used during the process
    // so that the real flow are not impacted by a modification that could be reverted
    if ($form_state['clicked_button']['#value'] != t('Back')) {
      if (!isset($flowmix_item->flows['temp_flow'])) {
        $flowmix_item->flows['temp_flow'] = new stdClass();
        if (isset($flowmix_item->flows[(int)$flowmix_item->index])) {
          $flowmix_item->flows['temp_flow'] = clone $flowmix_item->flows[(int)$flowmix_item->index];
        }
      }
    }
    $temp_flow = &$flowmix_item->flows['temp_flow'];
    $flow_conf = dpiblocks_flowmix_content_type_form_basicsetttings_defaults($temp_flow);

    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Flow feed name'),
      '#description' => t('The name you want to give to this flow.'),
      '#default_value' => $flow_conf->name,
      '#size' => 80,
      '#maxlength' => 512,
      '#required' => TRUE,
      '#weight' => 0,
    );

    $form['plugin_name'] = array(
      '#type' => 'select',
      '#title' => t('Type of flow'),
      '#default_value' => $flow_conf->plugin_name,
      '#required' => TRUE,
      '#options' => dpiblocks_api_get_available_flow_types(),
      '#weight' => 0.1,
    );

    $body_html_options = array();
    $formats = filter_formats();
    foreach ($formats as $format) {
      $body_html_options[$format->format] = $format->name;
    }
    $form['body_html'] = array(
      '#type' => 'select',
      '#title' => t('Use HTML in the text body'),
      '#description' => t('Use HTML in the text body.'),
      '#options' => $body_html_options,
      '#default_value' => $flow_conf->body_html,
      '#weight' => 0.2,
    );
  }

  unset($form['buttons']['cancel']);
  $form['buttons']['previous']['#submit'][] = 'dpiblocks_flowmix_content_type_form_basicsetttings_form_back_submit';

  return $form;
}

/**
 * Submit the second step form for the "Back" operation
 */
function dpiblocks_flowmix_content_type_form_basicsetttings_form_back_submit($form, &$form_state) {
  // We go back to the main form so we have to delete the temporary flow
  $flowmix_item = &$form_state['conf']['flow_obj'];
  unset($flowmix_item->flows['temp_flow']);
}

/**
 * Submit the second step form for the "Next" and "Confirm" operations
 */
function dpiblocks_flowmix_content_type_form_basicsetttings_form_submit($form, &$form_state) {
  // Check the operation
  if (strpos($form_state['clicked_button']['#name'], 'confirm') !== FALSE) {
    $flow_to_delete = str_replace('confirm_', '', $form_state['clicked_button']['#name']);
    unset($form_state['conf']['flow_obj']->flows[$flow_to_delete]);
  }
  else {
    $edited_flow = &$form_state['conf']['flow_obj']->flows['temp_flow'];
    $submitted = $form_state['values'];

    $save_values = array_keys((array)dpiblocks_flowmix_content_type_form_basicsetttings_defaults());
    foreach ($save_values as $value) {
      // Set the values in the cache object to pass them to the next step
      $edited_flow->$value = $submitted[$value];
    }
  }
}

/**
 * Flow plugin specific form
 */
function dpiblocks_flowmix_content_type_form_plugins_form($form, $form_state) {
  // Get the temporary flow
  $edited_flow = &$form_state['conf']['flow_obj']->flows['temp_flow'];

  $plugin = dpiblocks_api_get_plugin_by_name($edited_flow->plugin_name);
  $form['conf'] = array('#tree' => TRUE);
  // Get the plugin specific form, if defined
  dpiblocks_api_get_settings_form_of_plugin($plugin, $form['conf'], $edited_flow->conf);

  // Add plugin specific validate and submit functions
  if (!isset($form['conf']['#validate'])) {
    $form['conf']['#validate'] = array();
  }
  if (isset($plugin['settings form validate']) && !in_array($plugin['settings form validate'], $form['conf']['#validate'])) {
    $form['conf']['#validate'][] = $plugin['settings form validate'];
  }
  if (!isset($form['conf']['#submit'])) {
    $form['conf']['#submit'] = array();
  }
  if (isset($plugin['settings form submit']) && !in_array($plugin['settings form submit'], $form['conf']['#submit'])) {
    $form['conf']['#submit'][] = $plugin['settings form submit'];
  }

  $form['buttons']['Ok'] = array(
    '#type'            => 'submit',
    '#value'           => t('Ok'),
    '#wizard type'     => 'next',
    '#name'            => 'next_'.max(0, count($flowmix_item->flows) - 1),
    '#next'            => 'step1',
    '#weight'          => -1000,
  );

  unset($form['buttons']['cancel']);

  return $form;
}

/**
 * Validate the flow plugin specific form
 */
function dpiblocks_flowmix_content_type_form_plugins_form_validate(&$form, &$form_state) {
  // Call the plugin's proper(s) validation(s) function(s)
  if (!empty($form['conf']['#validate'])) {
    foreach ($form['conf']['#validate'] as $validate_func) {
      $conf_values = array('values' => $form_state['values']['conf']);
      $validate_func($form['conf'], $conf_values);
    }
  }

  // Set the form errors on the proper elements
  foreach (form_get_errors() as $field => $error) {
    form_set_error('conf]['.$field, $error);
  }
}

/**
 * Submit the flow plugin specific form
 */
function dpiblocks_flowmix_content_type_form_plugins_form_submit(&$form, &$form_state) {
  // Call the plugin's proper(s) submit(s) function(s)
  if (isset($form['conf']['#submit']) && is_array($form['conf']['#submit']) && !empty($form['conf']['#submit'])) {
    foreach ($form['conf']['#submit'] as $submit_func) {
      $conf_values = array('values' => $form_state['values']['conf']);
      $submit_func($form['conf'], $conf_values);
    }
  }

  // Get the temporary flow and set it as a real flow, then remove the temporary flow
  $edited_flow = &$form_state['conf']['flow_obj']->flows['temp_flow'];
  $edited_flow->conf = $form_state['values']['conf'];
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $flowmix_item->flows[(int)$flowmix_item->index] = $edited_flow;
  unset($flowmix_item->flows['temp_flow']);

  // Only save the flowmix if the "Finish" button has been clicked
  if (strpos($form_state['clicked_button']['#wizard type'], 'finish') !== FALSE) {
    dpiblocks_flowmix_content_type_save_reusable($form_state);
  }
}

/**
 * Save the flowmix in DB
 */
function dpiblocks_flowmix_content_type_save_reusable(&$form_state) {
  // Reset the flows IDs so that they start at 0
  $flows = $form_state['conf']['flow_obj']->flows;
  $form_state['conf']['flow_obj']->flows = array();
  foreach($flows as $flow) {
    $form_state['conf']['flow_obj']->flows[] = $flow;
  }
  $values = $form_state['conf']['flow_obj'];

  // Apply updates to the content object.
  if (isset($values->fid)) {
    $content->fid = $values->fid;
    $content->is_new = FALSE;
  } else {
    $content->is_new = TRUE;
  }
  $content->machine_name = $values->machine_name;
  $content->title = $values->title;
  // Set all settings to be serialized in the "data" column
  $defaults = (array)dpiblocks_flowmix_content_type_form_summary_defaults();
  $content->datas = new stdClass();
  foreach (array_keys($defaults) as $key) {
    if (isset($values->{$key})) {
      $content->datas->{$key} = $values->{$key};
    }
  }
  flowmix_save($content);

  // Set the pane subtype with the machine name
  $form_state['pane']->subtype = $content->machine_name;
}

/**
 * Get default values for the summary form
 */
function dpiblocks_flowmix_content_type_form_summary_defaults() {
  $defaults = new stdClass();
  $defaults->title = '';
  $defaults->machine_name = '';
  $defaults->url = '';
  $defaults->maxitem_global = '';
  $defaults->mix_schema = 'alt';
  $defaults->own_schema = '';
  $defaults->override_theme = 0;
  $defaults->override_theme_text = '';
  $defaults->caid = 'flowmix_'.time();
  $defaults->flows = array();

  return $defaults;
}

/**
 * Get default values for the basic settings form
 */
function dpiblocks_flowmix_content_type_form_basicsetttings_defaults($flow_conf = NULL) {
  if(isset($flow_conf) && !empty($flow_conf)) {
    return $flow_conf;
  }

  $defaults = new stdClass();
  $defaults->name = '';
  $defaults->plugin_name = '';
  $defaults->body_html = 0;

  return $defaults;
}

/**
 * Returns the current conf or the default values
 */
function dpiblocks_flowmix_content_type_get_conf($form_state, $default_settings) {
  if (isset($form_state['conf']['flow_obj'])) {
    $settings = $form_state['conf']['flow_obj'];
  } else {
    $settings = $default_settings;
  }
  return $settings;
}

/**
 * Get the settings of a flowmix from its machine name
 */
function dpiblocks_flowmix_content_type_generate_settings_for_subtype($machine_name) {
  $entity = dpiblocks_flowmix_content_type_get_entity_by_subtype($machine_name);
  $defaults = new stdClass();
  $defaults->fid = $entity->fid;
  $defaults->machine_name = $entity->machine_name;
  $defaults->title = $entity->title;
  $default_keys = array_keys((array)dpiblocks_flowmix_content_type_form_summary_defaults());
  $datas = unserialize(serialize($entity->datas));
  foreach ($default_keys as $key) {
    if (isset($datas->{$key})) {
      $defaults->{$key} = $datas->{$key};
    }
  }
  return $defaults;
}

/**
 * This function allow to get the FlowMix entity by machine_name
 *
 * @param $machine_name
 *   The machine name
 *
 * @return $entity
 *   The entity
 */
function dpiblocks_flowmix_content_type_get_entity_by_subtype($machine_name) {
  // Create a new flowmix controller
  $flowmix_controller = new FlowMixController('flowmix');
  // Return the flowmix entity
  return $flowmix_controller->load_by_machine_name($machine_name);
}
