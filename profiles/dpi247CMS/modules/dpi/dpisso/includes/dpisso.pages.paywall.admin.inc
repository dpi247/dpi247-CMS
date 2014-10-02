<?php
/**
 * Implement the hook_form
 */
function dpisso_paywall_admin_settings_page_form($form, $form_state){  

  $form=array();
  
  $form = array_merge($form, dpisso_paywall_admin_settings_paywallpolitic($form, $form_state));

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * implementation du hook_form_subit
 */
function dpisso_paywall_admin_settings_page_form_submit($form, $form_state){

  // get the current paywall politic & insert submit method of the current class.
  $politic = $form_state["values"]["dpisso_paywall_paywallpolitics"]; 
  if ($politic_instance = _dpisso_get_paywall_instance($politic)) {
    $politic_instance->settingsform_submit($form, &$form_state);
  }
}

/**
 * Generate the specific paywall settings form.
 */
function dpisso_paywall_admin_settings_paywallpolitic($form, $form_state) {

  // get the list of all declared ctools 'paywallpolitic' pluggin 
  ctools_include('plugins');
  $politics = ctools_get_plugins('dpisso', 'paywallpolitic');
  $politic_options = array();

  // Build a options array. 
  $default_politic = FALSE;  
  foreach ($politics as $id => $politic) {
    if (!$default_politic) $default_politic = $id;
    $politic_options[$id] = $politic['label'];
  }

  // If no politics declared (no 'paywallpolitic' pluggin active, we quit.
  if (empty($politic_options)) {
    $form['message'] = array(
        '#markup' => t('Sorry, there is no paywall politic plugins available.'),
    );
    return $form;
  }

  // Getting the right current politic (pluggin)
  if (isset($form_state["triggering_element"])) {
    // We triggered the ajax.
    $politic = $form_state['triggering_element']['#value'];
  } else {
    // We just build the form, get the current politic or the default one
    $politic = dpi_variable_get('dpisso_paywall_paywallpolitics',$default_politic);
  }  
  
  // Build a select options list from available 'paywallpolitic' pluggins.
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

  // Fieldset for politic ctools pluggin settings.
  $form['dpisso_paywall_paywallpolitics_settings'] = array(
      '#type' => 'fieldset',
     // '#title' => t('Paywall politic settings for: '.$politic_options[$politic]),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="dpisso_paywall_paywallpolitics_settings_div">',
      '#suffix' => '</div>',
  );
  
  // Call the settingsform method to get the pluggins settings.
  if ($politic_instance = _dpisso_get_paywall_instance($politic)) {
    $politic_settings = $politic_instance->settingsform($form, &$form_state);
    $form['dpisso_paywall_paywallpolitics_settings'][]=$politic_instance->settingsform_getdefaultvalues($politic_settings);
  }
  
  return $form;
}

/**
 * The call back function for the Ajax select form element: dpisso_paywall_paywallpolitics
 * 
 * @param Array $form
 *   The complete settings form 
 * @param Array $form_state
 *   The complete settings form state
 * 
 * @return Array $form
 *   The mofified element.
 */
function dpisso_paywall_admin_settings_paywallpolitic_callback($form, $form_state) {
  return $form['dpisso_paywall_paywallpolitics_settings'];
}