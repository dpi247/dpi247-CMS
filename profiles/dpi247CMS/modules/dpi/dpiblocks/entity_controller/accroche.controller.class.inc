<?php 

/**
 * A child class of EntityApiController
 *
 * @author LBA
 */
class AccrocheController extends EntityAPIControllerExportable {

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
      'aid' => '',
      'machine_name' => '',
      'override_title' => '',
      'title' => '',
      'link' => '',
      'layout' => '',
      'type' => '',
      'mixing' => '',
      'nb_max_value' => '',
      'data' => '',
      'hash_cache_data' => '',
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
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
