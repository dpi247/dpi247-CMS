<?php 

/**
 * A child class of EntityApiController
 *
 * @author jpl
 */
class Custom_contentController extends EntityAPIControllerExportable {

  public function load_by_machine_name($machine_name) {
    $results = $this->load(FALSE, array('machine_name' => $machine_name));
    $result = current($results);
    return $result;
  }

  /**
   * Overrides create function from EntityApiController
   *
   * @see EntityAPIController::create()
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'cid' => '',
      'machine_name' => '',
      'title' => '',
      'body' => '',
      'format' => '',
      'date' => '',
    );
    return parent::create($values);
  }

  /**
   * Overrides buildContent function from EntityApiController
   *
   * @see EntityAPIController::buildContent()
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('custom_content', $entity);

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
      '#entity_type' => 'custom_content',
      '#bundle' => 'custom_content',
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
