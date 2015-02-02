<?php 

/**
 * A child class of EntityApiController
 *
 * @author LBA
 */
class AccrocheController extends EntityAPIControllerExportable {

  public function load_by_title($machine_name) {
    $results = $this->load(FALSE, array('title' => $machine_name));
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
      'aid' => '',
      'machine_name' => '',
      'override_title' => '',
      'title' => '',
      'link' => '',
      'layout' => '',
      'data' => '',
    );
    return parent::create($values);
  }

  /**
   * Overrides buildContent function from EntityApiController
   *
   * @see EntityAPIController::buildContent()
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('accroche', $entity);

//     // Make Description and Status themed like default fields.
//     $content['description'] = array(
//       '#theme' => 'field',
//       '#weight' => 0,
//       '#title' =>t('Description'),
//       '#access' => TRUE,
//       '#label_display' => 'above',
//       '#view_mode' => 'full',
//       '#language' => LANGUAGE_NONE,
//       '#field_name' => 'field_fake_description',
//       '#field_type' => 'text',
//       '#entity_type' => 'custom_content',
//       '#bundle' => 'custom_content',
//       '#items' => array(array('value' => "")),
//       '#formatter' => 'text_default',
//       0 => array('#markup' => check_plain(""))
//     );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
