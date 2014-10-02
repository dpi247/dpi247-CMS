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

    $form = array(); 
    $form['message'] = array(
        '#markup' => t('There is no specific settings for this paywall politic.'),
    );
    return $form;
  }

}
