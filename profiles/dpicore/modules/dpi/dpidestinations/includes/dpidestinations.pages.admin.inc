<?php



/**
 * @file
 * Group membersihp type editing UI.
 */

/**
 * UI controller.
 */
class DpidestinationsTargetUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();


    //Since we use the same controller
    $items[$this->path]['description'] = 'Manage group memberships type, including fields.';
    if(isset($items['admin/dpi247/dpidestinations/target'])){
      $items['admin/dpi247/dpidestinations/target']['type']=MENU_LOCAL_TASK;
      $items['admin/dpi247/dpidestinations/target']['weight']=0;
    }
    if(isset($items['admin/dpi247/dpidestinations/targetblock'])){
      $items['admin/dpi247/dpidestinations/targetblock']['type']=MENU_LOCAL_TASK;
      $items['admin/dpi247/dpidestinations/targetblock']['weight']=9;

    }
    return $items;
  }

  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    $form = parent::overviewForm($form, &$form_state);
    return $form;
  }

}


/**
 * @file
 * Group membersihp type editing UI.
 */

/**
 * UI controller.
 */
class DpidestinationsTargetblockUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
  
  
    //Since we use the same controller
    $items[$this->path]['description'] = 'Manage group memberships type, including fields.';
    if(isset($items['admin/dpi247/dpidestinations/targetblock'])){
      $items['admin/dpi247/dpidestinations/targetblock']['type']=MENU_LOCAL_TASK;
      $items['admin/dpi247/dpidestinations/targetblock']['weight']=9;
  
    }
    return $items;
  }
}

function     dpidestinations_page_administration_targets(){
  return t('You can add/manage field for the target entity here. Do that only if you exactly know what you are doing :)');
}

function     dpidestinations_page_administration_targetblocks(){
  return t('You can add/manage field for the targetblock entity here. Do that only if you exactly know what you are doing :)');
  }


  function dpidestinations_page_administration_destinations(){
  
    module_load_include("inc", "taxonomy","taxonomy.admin");
    $destination_vid=dpidestinations_api_get_destinations_vocabulary_vid();
    $destination=entity_load_unchanged('taxonomy_vocabulary',$destination_vid);
    return drupal_get_form('taxonomy_overview_terms',$destination);
  
  }
  