<?php 
/**
 * Class de Paywall Politic.
 * Any custom paywall politic class extend this one.
 */
abstract class PaywallPolitics {
  
  function __construct() {
  }

  /**
   * Settingsform method.
   * 
   * @param Array $form
   *   The complete settings form
   * @param Array $form_state
   *   The complete settings forms state
   *   
   * @return Array
   *   A form array with paywall politics settings.
   */
  public function settingsform($form, $form_state) {
    $politic_settings = array();
    return $politic_settings;
  }

  /**
   * settingsform_getdefaultvalues method.
   * 
   * If a subclass didn't implement the submit method, the PaywallPolitics class
   * provide a method to store the settings as variables in the dpi cache. This method
   * will return the defaults values 
   * 
   * @param Array $politic_settings
   *   The politic settings form
   *   
   * @return Array $politic_settings
   *   The politic settings form with default values.
   */
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
  
  /**
   * settingsform_submit method.
   * 
   * If a subclass didn't implement the submit method, we provide a simple dpi settings
   * method to store settings in the dpi cache.
   * 
   * @param Array $form
   *   The complete settings form
   * @param Array $form_state
   *   The complete settings forms state
   *   
   * @see system_settings_form_submit
   * 
  */
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
