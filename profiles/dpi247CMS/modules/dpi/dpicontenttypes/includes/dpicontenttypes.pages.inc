<?php 

/**
 * Display the dpiproperties of a content
 *
 * @param String $type
 * @param Object $entity
 */
function dpicontenttypes_entity_properties($type, $entity) {
  $content = '';
  $dpiproperties = dpiproperties_load_by_type_and_id($type, entity_id($type, $entity), TRUE);
  if (module_exists('devel')) {
    
  }
  if ($dpiproperties != NULL) {
    $dpiproperties_view = dpiproperties_view($dpiproperties);
    $content = render($dpiproperties_view);
  }
  return $content;
}

/**
 * Page callback: Generates a derivative, given a style, an entity and image path.
 *
 * Fork of the image_style_deliver callback to add the entity as parameter
 *
 * After generating an image, transfer it to the requesting agent.
 *
 * @param $style
 *   The image style
 * @param $entity
 *   A Drupal entity
 */
function dpicontenttypes_image_style_deliver($style, $entity, $atom, $scheme) {
  $args = func_get_args();
  array_shift($args);
  array_shift($args);
  array_shift($args);
  array_shift($args);
  $target = implode('/', $args);
 
  // Check that the style is defined, the scheme is valid, and the image
  // derivative token is valid. (Sites which require image derivatives to be
  // generated without a token can set the 'image_allow_insecure_derivatives'
  // variable to TRUE to bypass the latter check, but this will increase the
  // site's vulnerability to denial-of-service attacks. To prevent this
  // variable from leaving the site vulnerable to the most serious attacks, a
  // token is always required when a derivative of a derivative is requested.)
  $valid = !empty($style) && file_stream_wrapper_valid_scheme($scheme);
  if (!variable_get('image_allow_insecure_derivatives', FALSE) || strpos(ltrim($target, '\/'), 'dpistyles/') === 0) {
    $valid = $valid && isset($_GET[IMAGE_DERIVATIVE_TOKEN]) && $_GET[IMAGE_DERIVATIVE_TOKEN] === image_style_path_token($style['name'], $scheme . '://' . $target);
  }
  $valid = TRUE;
  if (!$valid) {
    return MENU_ACCESS_DENIED;
  }

  /* use source thumbnail to generate element */
  $image_uri = $atom->thumbnail_source;
  
  $derivative_uri = _dpicontenttypes_image_style_path($style['name'], $entity->entity_id, $atom->sid, $image_uri);

  // If using the private scheme, let other modules provide headers and
  // control access to the file.
  if ($scheme == 'private') {
    if (file_exists($derivative_uri)) {
      file_download($scheme, file_uri_target($derivative_uri));
    }
    else {
      $headers = module_invoke_all('file_download', $image_uri);
      if (in_array(-1, $headers) || empty($headers)) {
        return drupal_access_denied();
      }
      if (count($headers)) {
        foreach ($headers as $name => $value) {
          drupal_add_http_header($name, $value);
        }
      }
    }
  }

  // Don't start generating the image if the derivative already exists or if
  // generation is in progress in another thread.
  $lock_name = 'image_style_deliver:' . $style['name'] . ':' . drupal_hash_base64($image_uri);
  if (!file_exists($derivative_uri)) {
    $lock_acquired = lock_acquire($lock_name);
    if (!$lock_acquired) {
      // Tell client to retry again in 3 seconds. Currently no browsers are known
      // to support Retry-After.
      drupal_add_http_header('Status', '503 Service Unavailable');
      drupal_add_http_header('Retry-After', 3);
      print t('Image generation in progress. Try again shortly.');
      drupal_exit();
    }
  }

  // Add the current entity in the effect datas
  foreach ($style['effects'] as $effect_id => $effect) {
    if ($effect['name'] == 'dpicontenttypes_dpicrop' || $effect['name'] == 'dpicontenttypes_dpicropcopy') {
      $style['effects'][$effect_id]['data']['entity'] = $entity;
      $style['effects'][$effect_id]['data']['atom'] = $atom;
      $style_copy = $style;
      unset($style_copy['effects']);
      $style['effects'][$effect_id]['data']['style'] = $style_copy;
    }
  }

  // Try to generate the image, unless another thread just did it while we were
  // acquiring the lock.
  $success = file_exists($derivative_uri) || image_style_create_derivative($style, $image_uri, $derivative_uri);

  if (!empty($lock_acquired)) {
    lock_release($lock_name);
  }

  if ($success) {
    $image = image_load($derivative_uri);
    file_transfer($image->source, array('Content-Type' => $image->info['mime_type'], 'Content-Length' => $image->info['file_size']));
  }
  else {
    watchdog('image', 'Unable to generate the derived image located at %path.', array('%path' => $derivative_uri));
    drupal_add_http_header('Status', '500 Internal Server Error');
    print t('Error generating image.');
    drupal_exit();
  }
}

/**
 * Cropings editon modal form
 */
function dpicontenttypes_ajax_editcrop($js, $entity_type, $eid, $aid) {
  if ($js) {
    ctools_include('ajax');
    ctools_include('modal');
    ctools_modal_add_js();

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('Cropings edition'),
    );

    _dpicontenntypes_ajax_editcrop_prepare_form_state_and_cropings_values($form_state, $entity_type, $eid, $aid);

    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    $output = ctools_modal_form_wrapper('dpicontenttypes_form_edit_crop_form', $form_state);

    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }

    // Return the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  } else {
    return drupal_get_form('dpicontenttypes_form_edit_crop_form');
  }
}

/**
 * Edition of the cropings of a photo.
 *
 * @param string $nid
 *  ID of the node containing the photo being edited.
 *
 * @see http://deepliquid.com/blog/archives/category/jcrop
 *  Jcrop homepage.
 *
 * @return
 *  HTML code of the image and the form containing the new values.
 */
function dpicontenttypes_form_edit_crop_form($form, $form_state) {
  $cropings = $form_state['cropings'];
  $atom = $form_state['atom'];

  $form = array(
    '#attributes' => array('class' => 'crops'),
    '#prefix' => '<div class="crop_wrap">',
    '#suffix' => '</div>',
  );

  $first = TRUE;
  foreach ($cropings as $style_name => $croping) {
    $croping_default_value = array(
      $croping[0],
      $croping[1],
      $croping[2] - $croping[0],
      $croping[3] - $croping[1],
    );
    $form[$style_name.'_serialCoord'] = array(
      '#type' => 'hidden',
      '#id' => $style_name.'_serialCoord',
      '#attributes' => array('id' => $style_name.'_serialCoord'),
      '#title' => $style_name.'_serialCoord',
      '#default_value' => implode($croping_default_value, ','),
    );

    $form[$style_name] = array(
      '#type' => 'submit',
      '#id' => $style_name,
      '#value' => $style_name,
    );

    if ($first) {
      $form[$style_name.'_serialCoord']['#prefix'] = '
          <label for="edit-crop">'.t('Edit cropings').' : <span id="crop-current-name">'.t('Choose a preset to edit.').'</span>
              </label>
              <div id="cropimage"><img src="'.file_create_url($atom->file_source).'" id="cropbox" /></div>
                  <div class="crops">';
      $first = FALSE;
    }
    if (sizeof(element_children($form)) == 2*(sizeof($cropings))) {
      $form[$style_name]['#suffix'] = '</div>';
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#prefix' => '<div class="crop_submit buttons">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit function for the cropings edition form
 */
function dpicontenttypes_form_edit_crop_form_submit(&$form, &$form_state) {
  $clicked_button = $form_state['clicked_button'];
  if ($clicked_button['#value'] == $form['submit']['#value']) {
    $values = $form_state['values'];
    $atom = $form_state['atom'];
    $aid = $atom->sid;
    $entity_type = $form_state['entity_type'];
    $eid = $form_state['eid'];

    $modif = FALSE;
    $orig_cropings = $form_state['cropings'];
    $new_cropings = array();
    foreach ($orig_cropings as $style_name => $orig_croping) {
      if (isset($values[$style_name.'_serialCoord']) && !empty($values[$style_name.'_serialCoord'])) {
        $new_croping = explode(',', $values[$style_name.'_serialCoord']);
        if (isset($new_croping[4]) && $new_croping[4] == '1') {
          // This crop has been modified
          $modif = TRUE;
        }
        unset($new_croping[4]);
        $new_cropings[$style_name] = $new_croping;
      }
    }

    if ($modif) {
      dpicontenttypes_api_image_path_flush($atom->file_source);

      if (is_string($eid) && !is_numeric($eid)) {
        // This is a form_token and not a real entity ID
        $form_token = $eid;
        $eid = 0;
      }

      _dpicontenttypes_cropings_update_cropings($entity_type, $eid, $aid, $new_cropings, TRUE, $form_token);
    }

    // Tell the browser to close the modal.
    $form_state['ajax_commands'][] = ctools_modal_command_dismiss();
  }
}

/**
 * Prepare structured croping values and add required JS and CSS
 */
function _dpicontenntypes_ajax_editcrop_prepare_form_state_and_cropings_values(&$form_state, $entity_type, $eid, $aid) {
  $form_state['entity_type'] = $entity_type;
  $form_state['eid'] = $eid;
  $form_state['atom'] = $atom = scald_atom_load($aid);
  $image = image_load($atom->file_source);
  $cropings = dpicontenttypes_api_get_cropings_for_entity_and_atom($entity_type, $eid, $aid);
  $transformed_cropings = array();
  foreach ($cropings as $style_name => $croping) {
    if ($transformed_croping = _dpicontenttypes_cropings_transform_literal_values($croping, $image)) {
      $transformed_cropings[$style_name] = $transformed_croping;
    }
  }
  $form_state['cropings'] = $transformed_cropings;

  $module_path = drupal_get_path('module', 'dpicontenttypes');
  drupal_add_js($module_path.'/js/editcrop/jquery.Jcrop.min.js');
  drupal_add_js($module_path.'/js/editcrop/jquery.once.min.js');
  drupal_add_js($module_path.'/js/editcrop/edit_crop.js');
  drupal_add_css($module_path.'/css/editcrop/jquery.Jcrop.min.css');
  drupal_add_css($module_path.'/css/editcrop/editcrop.css');
  drupal_add_js(array('dpicontenttypesCropings' => $transformed_cropings), 'setting');
}

/**
 * Transform literal values (top, right, center, ..) into concrete pixel values
 */
function _dpicontenttypes_cropings_transform_literal_values($croping, $image) {
  $transformed_croping = array();

  if (!is_numeric($croping[2]) || !is_numeric($croping[3])) {
    // width and height values have to be numerical values
    // @todo : add watchdog
    return FALSE;
  }

  $image_width = $image->info['width'];
  $image_height = $image->info['height'];
  if ($croping[0] == 'center_max' || $croping[1] == 'center_max') {
    $ratio = $croping[2]/$croping[3];

    // Determine the optimal width and height for the croping
    $new_width = min($image_width, floor($image_height*$ratio));
    $new_height = min($image_height, floor($image_width/$ratio));

    // Center X and Y according to the new width and height
    $transformed_croping[0] = image_filter_keyword('center', $image_width, $new_width);
    $transformed_croping[1] = image_filter_keyword('center', $image_height, $new_height);
    $transformed_croping[2] = $new_width;
    $transformed_croping[3] = $new_height;
  } else {
    // Transform the keywords (bottom, right, center, ..) into numerical values
    if (is_numeric($croping[0])) {
      $transformed_croping[0] = $croping[0];
    } else {
      $transformed_croping[0] = image_filter_keyword($croping[0], $image_width, $croping[2]);
    }
    if (is_numeric($croping[1])) {
      $transformed_croping[1] = $croping[1];
    } else {
      $transformed_croping[1] = image_filter_keyword($croping[1], $image_height, $croping[1]);
    }

    $transformed_croping[2] = $croping[2];
    $transformed_croping[3] = $croping[3];
  }

  if (($transformed_croping[0] + $transformed_croping[2]) > $image_width) {
    // x offset + width is greater than image width
    // @todo : add watchdog
    return FALSE;
  }
  if (($transformed_croping[1] + $transformed_croping[3]) > $image_height) {
    // y offset + height is greater than image height
    // @todo : add watchdog
    return FALSE;
  }

  $transformed_croping[4] = $transformed_croping[2]/$transformed_croping[3];
  $transformed_croping[2] += $transformed_croping[0];
  $transformed_croping[3] += $transformed_croping[1];
  return $transformed_croping;
}
