<?php 
/**
 * Declare the ctools pluggin.
 */
$plugin = array(
    'label' => t('Based on content types'),
    'handler' => array(
        'class' => 'PaywallPerType',
    ),
);

/**
 * PaywallPerType - Any choosen content type will decrease the paywall counter.
 * @author exxodus
 *
 */
class PaywallPerType extends PaywallPolitics {

  /**
   * settingsform method.
   * 
   * @see PaywallPolitics::settingsform()
   * 
   * @return array A checkboxes form element with all the content types. 
   */
  public function settingsform($form, $form_state) {

    // get all content types & build a options array. 
    $options_types = array(); 
    foreach (node_type_get_types() as $id=>$type) {
      $options_types[$id]=$type->name; 
    }

    // The checkboxes form element.
    $form = array();
    $form['PaywallPerType_types'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Please choose Content Types'),
        '#options' => $options_types,
        '#description'=>t(" Choose all the content types that will decrease the paywall counter."),
    );
    
    return $form;
  }

}
