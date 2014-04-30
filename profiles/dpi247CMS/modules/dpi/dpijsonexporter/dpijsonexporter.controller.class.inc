<?php

class JsonproductController extends EntityAPIControllerExportable{
  public function create(array $values = array()) {
    $values += array(
      'targetid' => '',
      'machine_name' => '',
      'label' => '',
      'description' => '',
      'menu_name' => '',
      'presets' => array(),
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('jsonproduct', $entity);

    // Make Description and Status themed like default fields.
    $content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Description'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'jsonproduct',
      '#bundle' => 'jsonproduct',
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }

  public function load_jsonproduct_by_name($jsonproduct_name) {
    $conditions = array('machine_name' => $jsonproduct_name);
    $result = jsonproduct_load_multiple(FALSE, $conditions);
    if (!$result) {
      $result = NULL;
    }
    return current($result);
  }

}
