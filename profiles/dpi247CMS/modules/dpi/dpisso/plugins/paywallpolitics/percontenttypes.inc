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
  
  //public function settingsform_submit($form, $form_state) {
  //  dsm($form_state); 
  //}
  
  
  public function settingsform($politic_settings_default_values) {
        
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
    
    $element = array(); 
    $element['#tree'] = TRUE;
    $element['#title'] = t('Please choose Content Types');
    $element['#theme'] = array('checkboxgrid');
    $element['#options_row'] = $options_types;
    $element['#options_col'] = $roles;
    $element['#theme_wrappers'] = array('form_element');
    
    foreach ($options_types as $rkey => $rchoice) {
      $element += array($rkey => array());
      $element[$rkey]['#tree'] = TRUE;
      $element[$rkey]['#title'] = $rchoice;
      
      $weight = 0.0;
      foreach ($roles as $ckey => $cchoice) {
        $weight += 0.001;
        $element[$rkey][$ckey] = array(
          '#type' => 'checkbox',
          '#title' => "",
          '#return_value' => array($rkey,$ckey),
          '#weight' => $weight,
          '#default_value' => (isset($politic_settings_default_values['typesparroles'][$rkey][$ckey]))? $politic_settings_default_values['typesparroles'][$rkey][$ckey] : false,
        );
      }
    }
    
    $form['typesparroles'] = $element;

    return $form;
    
  }
  
  /**
   * The list of module to be loaded in accesscontroller controler.
   * @see PaywallPolitics::getmodulelist()
   */
  public function getmodulelist() {
  	return array("node","dla");
  }
  
  
}


