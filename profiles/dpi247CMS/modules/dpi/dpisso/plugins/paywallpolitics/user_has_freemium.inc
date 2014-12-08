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
  
  /**
   * issecurepage method.
   *
   * Check if the page is secured (available or not for specifics roles.
   *
   * @param url The url of the page to check.
   * @param roles An array of drupal roles.
   *
   * @return boolean Granted access (or not) to the page.
   */
  public function issecurepage($url, $roles) {
    
    
    global $user;
    print_r($user);
    return array('code'=>403,'data'=>"Access denied by default pawall politic plugin");
  }
  
  public function haveAccess(){
    global $user;
    $return = false;
    /*if(isset($user->accessmanager["freemium_counter"]) && $user->accessmanager["freemium_counter"]){
      $return = true;
    }*/
    return $return;
  }
  
}