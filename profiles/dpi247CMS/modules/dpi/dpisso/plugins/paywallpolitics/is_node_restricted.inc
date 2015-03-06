<?php
/**
 * Declare the ctools pluggin.
 */
$plugin = array (
  'label' => t ( 'Is Node Restricted' ),
  'handler' => array (
    'class' => 'IsNodeRestricted' 
  ) 
);

/**
 * Choosen decrements
 * 
 * @author lba
 *        
 */
class IsNodeRestricted extends PaywallPolitics {

  public function settingsform($default_value) {
    $form ["markup"] = array (
      '#markup' => t ( 'Every each hit decrements one page for the user' ) 
    );
    return $form;
  }

  /**
   * issecurepage method.
   *
   * Check if the page is secured (available or not for specifics roles.
   *
   * @param
   *          url The url of the page to check.
   * @param
   *          roles An array of drupal roles.
   *          
   * @return boolean Granted access (or not) to the page.
   */
  public function issecurepage($url, $roles = null) {
    require_once DRUPAL_ROOT . '/includes/menu.inc';
    require_once DRUPAL_ROOT . '/includes/path.inc';


    $url=ltrim('/',$url);
    $url=drupal_get_normal_path($url);

    $urls = explode ( '/', $url );



    if ($urls [1] == 'node' and ! isset ( $urls [3] )) {
      $is_restricted = db_query ( 'SELECT field_restrictedaccess_value FROM {field_data_field_restrictedaccess} as m WHERE  m.entity_id = :nid', array (
        ':nid' => $urls [2] 
      ) )->fetchField ();
      
      // Fetch next row as a stdClass object.
      if ($is_restricted) {
        return array (
          'code' => 200,
          'data' => " Access restricted: you should decrement" 
        );
      } else {
        return array (
          'code' => 403,
          'data' => " Access not restricted: you shouldnt decrement" 
        );
      }
    }
    return array (
      'code' => 403,
      'data' => "Access denied by default pawall politic plugin" 
    );
  }

  public function haveAccess() {
    global $user;
    $return = false;
    /*
     * if(isset($user->accessmanager["freemium_counter"]) && $user->accessmanager["freemium_counter"]){ $return = true; }
     */
    return $return;
  }
}