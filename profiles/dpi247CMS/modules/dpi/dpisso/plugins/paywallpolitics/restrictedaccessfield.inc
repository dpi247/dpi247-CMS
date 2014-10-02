<?php 
/**
 * Declare the ctools pluggin.
 */
$plugin = array(
    'label' => t('Based on the restricted access field'),
    'handler' => array(
        'class' => 'RestrictedAccessField',
    ),
);

/**
 * PaywallPerType - Any choosen content type will decrease the paywall counter.
 * @author exxodus
 *
 */
class RestrictedAccessField extends PaywallPolitics {

  /**
   * settingsform method.
   * 
   * @see PaywallPolitics::settingsform()
   */
  public function settingsform($form, $form_state) {

    // Build a options array with all roles    
    $roles = array_map('check_plain', user_roles(TRUE));
    
    $form = array(); 
    $form['restrictedaccessfieldroles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Paywall politic bypass roles'),
        '#description'=>t(" Choose all the roles that will bypass the paywall politic.Those roles will never decrease the paywall counter."),
        '#options' => $roles,
        '#access' => $roles && user_access('administer permissions'),
    );
    return $form;
  }

}
