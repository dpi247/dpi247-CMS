<?php

$plugin = array(
    'label' => t('Paywall Sample  2'),
    'handler' => array(
        'class' => 'paywalltestbis',
    ),
);


class paywalltestbis extends PaywallPolitics {

  public function settingsform() {
    $form = array();
    $form['PaywallTestbistitle'] = array(
      '#type' => 'textfield', 
      '#title' => t('Blabla 2'), 
      '#default_value' => "RIEN", 
      '#size' => 60, 
      '#maxlength' => 128, 
      '#required' => TRUE,
    );

    return $form;
  }


}