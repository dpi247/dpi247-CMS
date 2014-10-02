<?php 

abstract class PaywallPolitics {
  
  function __construct() {
  }

  // return the settings form of the PayWallpolitics.
  public function settingsform($form, $form_state) {
    $form = array();
    return $form;
  }

  // 
  public function settingsform_getdefaultvalues($politic_settings) {

    foreach ($politic_settings as $key => $value) {
      if (is_array($value)) {
        if ($defaultvalue=dpi_variable_get($key,"void")) {
        	$politic_settings[$key]["#default_value"]=$defaultvalue;
        }
      }
    }
    
    
    return $politic_settings;
  }  
  
  // return the settings form of the PayWallpolitics.
  public function settingsform_submit($form, $form_state) {

    // Exclude unnecessary elements.
    form_state_values_clean($form_state);
    foreach ($form_state['values'] as $key => $value) {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }
      dpi_variable_set($key, $value);
    }
    drupal_set_message(t('The configuration options have been saved.'));
  }
  
}