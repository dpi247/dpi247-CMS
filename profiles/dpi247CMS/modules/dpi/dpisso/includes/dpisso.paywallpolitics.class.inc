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
   * @param $politic
   *   The politic settings name
   *   
   * @return Array $politic_settings
   *   The politic settings default values.
   */
  public function settingsform_getdefaultvalues($politic) {
    return dpi_variable_get("paywallpolitic_".$politic,array());
  }
  
  /**
   * settingsform_submit method.
   * 
   * If a subclass didn't implement the submit method, we provide a simple dpi settings
   * method to store settings in the dpi cache.
   * 
   * @see system_settings_form_submit
   * 
  */
  public function settingsform_submit($form, $form_state) {
    form_state_values_clean($form_state);     
    dpi_variable_set('dpisso_paywall_paywallpolitics',$form_state["values"]["dpisso_paywall_paywallpolitics"]);
    dpi_variable_set("paywallpolitic_".$form_state["values"]["dpisso_paywall_paywallpolitics"], $form_state["values"]);
    drupal_set_message(t('The configuration options have been saved.'));
  }
  
}
