<?php
/**
 * Declare the ctools pluggin.
 */
$plugin = array(
  'label' => t('User has freemium'),
  'handler' => array(
    'class' => 'UserHasFreemium',
  ),
);

/**
 * Choosen decrements
 * @author lba
 *
 */
class UserHasFreemium extends PaywallPolitics{
  
  public function settingsform($default_value) {
    $form["markup"] = array(
      '#markup' => t('Every each hit decrements one page for the user')
    );
    return $form;
  }
  
}