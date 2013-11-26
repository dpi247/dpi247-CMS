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

function     dpidestinations_page_administration_targets(){
  return 't';
}

function     dpidestinations_page_administration_targetblocks(){
  return 't';
}
