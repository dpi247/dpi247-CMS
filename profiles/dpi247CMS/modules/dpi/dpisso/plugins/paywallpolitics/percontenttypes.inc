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
  
  public function settingsform_submit($form, $form_state) {
    dsm($form_state); 
  }
  
  
  public function settingsform($form, $form_state) {

    // get all content types & build a options array. 
    $options_types = array(); 
    foreach (node_type_get_types() as $id=>$type) {
      $options_types[$id]=$type->name; 
    }

    $options_types["2"]="row type 2";
    $options_types["3"]="row type 3";
    $options_types["4"]="row type 4";
    $options_types["5"]="row type 5";
    
    
    // Build a options array with all roles
    $roles = array_map('check_plain', user_roles(TRUE));
    
    $form = array();
    $form['PaywallPerType_types'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Please choose Content Types'),
        '#options' => $options_types,
        '#description'=>t("Choose all the content types that will decrease the paywall counter."),
    );
    
    $form['typesroles'] = array(
        '#type' => 'checkboxgrid',
        '#title' => t('Please choose Content Types'),
        '#options_row' => $options_types,
        '#options_column' => $roles,
        '#description'=>t("tst"),
    );
    
    return $form;
    
  }

  
  
  
}
