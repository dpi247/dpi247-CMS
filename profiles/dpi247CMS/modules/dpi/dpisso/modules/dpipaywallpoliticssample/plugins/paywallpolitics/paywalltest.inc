<?php

$plugin = array(
    'label' => t('Paywall Sample'),
    'handler' => array(
        'class' => 'PaywallTest',
    ),
);



class PaywallTest extends PaywallPolitics {

  public function settingsform() {
    $form = array();

    $form['PaywallTesttitle'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => "",
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
    
    );

    return $form;
  } 


}