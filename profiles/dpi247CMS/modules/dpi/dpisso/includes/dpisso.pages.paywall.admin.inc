<?php

/**
 * Form constructor for General Paywall Admin form.
 */
function dpisso_paywall_admin_settings_page_form($form, $form_state){  

  $form=array();
  $form = array_merge($form, dpisso_paywall_admin_settings_paywallpolitic($form, &$form_state));

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Call the submit method for paywall politic.
 */
function dpisso_paywall_admin_settings_page_form_submit($form, $form_state){
  
  $politic = $form_state["values"]["dpisso_paywall_paywallpolitics"]; 
  if ($politic_instance = _dpisso_get_paywall_instance($politic)) {
    $politic_instance->settingsform_submit($form, &$form_state);
  }
  
}


/**
 * Form constructor for Paywall politics Admin Form.
 */
function dpisso_paywall_admin_settings_paywallpolitic($form, $form_state) {

  ctools_include('plugins');
  $politics = ctools_get_plugins('dpisso', 'paywallpolitic');
  $politic_options = array();
  
  $default_politic = FALSE;  
  foreach ($politics as $id => $politic) {
    if (!$default_politic) $default_politic = $id;
    $politic_options[$id] = $politic['label'];
  }
  
  if (empty($politic_options)) {
    $form['message'] = array(
        '#markup' => t('Sorry, there is no paywall politic plugins available.'),
    );
    return $form;
  }

  // Did we trigger the ajax form ?
  if (isset($form_state["triggering_element"])) {
    // We did, so builf the form from triggrered politic.
    $politic = $form_state['triggering_element']['#value'];
  } else {
    $politic = dpi_variable_get('dpisso_paywall_paywallpolitics',$default_politic);
  }  
  
  // Build a select options list from available pluggins.
  $form['dpisso_paywall_paywallpolitics'] = array(
      '#type' => 'select',
      '#title' => t('Please choose a paywall politic'),
      '#description'=>t(" Paywall politics determine whenever a ressource (url) will count as a paid ressource or not."),
      '#options' => $politic_options,
      '#default_value'=>dpi_variable_get('dpisso_paywall_paywallpolitics',null),
      '#ajax' => array(
          'callback' => 'dpisso_paywall_admin_settings_paywallpolitic_callback',
          'wrapper' => 'dpisso_paywall_paywallpolitics_settings_div',
          'method' => 'replace',
          'effect' => 'fade',
      ),
  );

  // Firld set for politic ctools pluggin.
  $form['dpisso_paywall_paywallpolitics_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Paywall politic settings for: '.$politic_options[$politic]),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="dpisso_paywall_paywallpolitics_settings_div">',
      '#suffix' => '</div>',
  );
  
  if ($politic_instance = _dpisso_get_paywall_instance($politic)) {
    $politic_settings = $politic_instance->settingsform($form, &$form_state);
    $form['dpisso_paywall_paywallpolitics_settings'][]=$politic_instance->settingsform_getdefaultvalues($politic_settings);
  }
  
  return $form;
}


function dpisso_paywall_admin_settings_paywallpolitic_callback($form, $form_state) {
  return $form['dpisso_paywall_paywallpolitics_settings'];
}