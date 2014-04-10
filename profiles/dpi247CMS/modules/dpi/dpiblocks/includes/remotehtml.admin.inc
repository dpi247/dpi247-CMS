<?php 

/**
 * Implementation of hook_form()
 *
 * @param $form
 * @param $form_state
 * @param $remotehtml
 *
 * @return array
 */
function remotehtml_form($form, &$form_state, $remotehtml,$op = 'edit') {
  ctools_include('dependent');
  $form_state['values']['rid'] = $remotehtml->rid;

  if($remotehtml->rid == '') {
    $form['define'] = array(
      '#type' => 'checkbox',
      '#title' => t('Define or edit the machine name ?'),
    );

    $form['machine_name_edit'] = array(
      '#type'          => 'textfield',
      '#title'         => t('machine name'),
      '#description'   => t('Define machine name'),
      '#default_value' => $remotehtml->machine_name,
      '#process'       => array('ctools_dependent_process'),
      '#dependency'    => array('edit-define' => array(1)),
    );
  }

  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $remotehtml->rid,
  );

  $form['machine_name'] = array(
    '#type' => 'value',
    '#value'=> $remotehtml->machine_name,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => $remotehtml->title,
  );

  $form['html_adresse'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Adress'),
    '#required' => TRUE,
    '#default_value' => $remotehtml->html_adresse,
  );

  $form['refresh_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh time (in minutes)'),
    '#required' => TRUE,
    '#default_value' => $remotehtml->refresh_time,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  field_attach_form('remotehtml', $remotehtml, $form, $form_state);

  return $form;
}

/**
 * Implementation of hook_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function remotehtml_form_validate($form, &$form_state) {
  $remotehtml_submission = (object) $form_state['values'];
  $results = dpiblocks_remotehtml_get_all();
  // If the user want to define the machine name
  if($form_state['input']['define']){
    $machine_name = $remotehtml_submission->machine_name_edit;
    // Check for string identifier sanity
    if (!preg_match('!^[a-z0-9_]+$!', $machine_name)) {
      form_set_error('machine_name_edit', t('The machine name can only consist of lowercase letters, underscores, and numbers.'));
    }
    else{
      foreach($results as $record) {
        if($remotehtml_submission->rid != $record->rid) {
          //if machine name already exist
          if($machine_name == $record->machine_name){
            form_set_error('machine_name_edit', t('The machine name already exist'));
          }
        }
      }
    }
  }
  // The machine name is define on the title
  else if($results != NULL) {
    // Construct the new machine name on the title
    $machine_name = str_replace(' ', '_', $remotehtml_submission->title);
    $machine_name = strtolower($machine_name);
    foreach($results as $record) {
      if($remotehtml_submission->rid != $record->rid) {
        // If the machine name already exist
        if($machine_name == $record->machine_name) {
          form_set_error('title', t('The machine name based on the title already exist change your title or define a custom machine name'));
        }
      }
    }
  }

  // Validate url
  if(!remotehtml_url_verification($remotehtml_submission->html_adresse)) {
    form_set_error('html_adresse', t('You must enter a valid URL adress'));
  }

  // Validate the refresh time
  if(!is_numeric($remotehtml_submission->refresh_time) || $remotehtml_submission->refresh_time < 0) {
    form_set_error('refresh_time', t('You must enter a positive numerical value for the refresh time'));
  }
}

/**
 * Implementation of hook_form_submit()
 *
 * @param $form
 * @param $form_state
 */
function remotehtml_form_submit($form, &$form_state) {
  $remotehtml_submission = (object) $form_state['values'];
  $remotehtml_submission->define = $form_state['input']['define'];
  field_attach_submit('remotehtml', $remotehtml_submission, $form, $form_state);

  // The user want define the machine name
  if($remotehtml_submission->define && $remotehtml_submission->machine_name == NULL) {
    $remotehtml_submission->is_new = TRUE;
    $remotehtml_submission->machine_name = $remotehtml_submission->machine_name_edit;
  }
  // The machine name is define on the title
  else if(!$remotehtml_submission->define && $remotehtml_submission->machine_name == NULL) {
    $remotehtml_submission->is_new = TRUE;
    $remotehtml_submission->machine_name = str_replace(' ', '_', $remotehtml_submission->title);
    $remotehtml_submission->machine_name = strtolower($remotehtml_submission->machine_name);
  }
  // The remote HTML entity exist and the machine name can't be redefine
  else {
    $remotehtml_submission->is_new = False;
  }

  // Save the entity
  remotehtml_save($remotehtml_submission);
  // Put entity in formstate to find it later
  $form_state['remotehtml'] = $remotehtml_submission;
  // And redirect to display the new entity
  $form_state['redirect'] = "remotehtml/".$remotehtml_submission->rid;
}
