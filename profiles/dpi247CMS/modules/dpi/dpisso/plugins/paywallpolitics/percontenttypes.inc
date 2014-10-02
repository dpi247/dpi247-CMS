<?php 

$plugin = array(
    'label' => t('Per content types'),
    'handler' => array(
        'class' => 'PaywallPerType',
    ),
);


class PaywallPerType extends PaywallPolitics {

  public function settingsform($form, &$form_state) {

    // get all content types. 
    $options_types = array(); 
    foreach (node_type_get_types() as $id=>$type) {
      $options_types[$id]=$type->name; 
    }
    
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
