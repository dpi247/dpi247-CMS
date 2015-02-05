<?php 

/**
 * A child class of Entity
 *
 * @author LBA
 */
class AccrocheEntity extends Entity {
  /**
   * The accroche id of accroche content
   *
   * @var $aid
   */
  public $aid;
  
  /**
   * The machine_name of custom content
   *
   * @var $machine_name
   */
  public $machine_name;

  /**
   * The override title of accroche content
   *
   * @var $override_title
   */
  public $override_title;

  /**
   * The title of accroche content
   *
   * @var $title
   */
  public $title;

  /**
   * The link of accroche content
   *
   * @var $link
   */
  public $link;

  /**
   * The layout of accroche content
   *
   * @var $layout
   */
  public $layout;
  
  /**
   * The type of accroche content
   *
   * @var $type
   */
  public $type;

  /**
   * The type of accroche content
   *
   * @var $type
   */
  public $mixing;
  
  /**
   * The data of accroche content
   *
   * @var $date
   */
  public $data;

  /**
   * The hash cache data of accroche content
   *
   * @var $date
   */
  public $hash_cache_data;
  
  /**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    Entity::__construct($values, 'accroche');
  }

  /**
   * Overrides defaultLabel function from entity
   *
   * @see Entity::defaultLabel()
   */
  protected function defaultLabel() {
    return $this->machine_name;
  }
  
  /**
   * Overrides defaultUri function from entity
   *
   * @see Entity::defaultUri()
   */
  protected function defaultUri() {
    return array('path' => 'accroche/' . $this->machine_name);
  }
}
