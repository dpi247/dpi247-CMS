<?php 

/**
 * A child class of Entity
 *
 * @author jpl
 */
class Custom_contentEntity extends Entity {
  /**
   * The id of custom content
   *
   * @var $cid
   */
  public $cid;

  /**
   * The machine_name of custom content
   *
   * @var $machine_name
   */
  public $machine_name;

  /**
   * The title of custom content
   *
   * @var $title
   */
  public $title;

  /**
   * The body of custom content
   *
   * @var $body
   */
  public $body;

  /**
   * The format of custom content
   *
   * @var $format
   */
  public $format;

  /**
   * The date of custom content
   *
   * @var $date
   */
  public $date;

  /**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    Entity::__construct($values, 'custom_content');
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
    return array('path' => 'custom_content/' . $this->machine_name);
  }
}
