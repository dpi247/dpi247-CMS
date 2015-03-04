<?php
class DPIPropertiesController extends EntityAPIController{
  
  public function create(array $values = array()) {
    $values += array(
      'entity' => '',
      'id' => '',
      'external_reference' => '',
      'external_reference_2' => '',
      'source' => '',
      'product_id' => '',
    );
    return parent::create($values);
  }
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('example_task', $entity);

    // Make Description and Status themed like default fields.
    $content['external_reference'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Exeternal Reference'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_external_reference',
      '#field_type' => 'text',
      '#entity_type' => 'dpiproperties',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->external_reference)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->external_reference))
    );
    
    $content['external_reference_2'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Exeternal Reference 2'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_external_reference_2',
      '#field_type' => 'text',
      '#entity_type' => 'dpiproperties',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->external_reference_2)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->external_reference_2))
    );
    
    $content['source'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Source'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_source',
      '#field_type' => 'text',
      '#entity_type' => 'dpiproperties',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->source)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->source))
    );
    $content['product_id'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Product ID'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_product_id',
      '#field_type' => 'text',
      '#entity_type' => 'dpiproperties',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->product_id)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->product_id))
    );
    $content['edit_link'] = array(
      '#markup' => l('Edit dpiproperties','dpiproperties/'.$entity->dpid.'/edit',array('query' => array('destination' => 'node/'.$entity->id.'/dpiproperties'))),
    );
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
  /*
  public function load_dpiproperties_by_id($id){
  
    $conditions = array('nid' => $nid);
    $result = dpiproperties_load_multiple(FALSE, $conditions);
    if (!$result){
      $result = NULL;
    }
    return current($result);
     
  }*/

}
/**
* UI controller for Task Type.
*/
class DPIPropertiesTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Task types.';
    return $items;
  }
}
class DPIPropertiesTypeController extends EntityAPIControllerExportable {
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save Task Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see _http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }
}
