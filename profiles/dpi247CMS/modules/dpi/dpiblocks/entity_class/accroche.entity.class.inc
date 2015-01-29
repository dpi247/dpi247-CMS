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
   * The date of accroche content
   *
   * @var $date
   */
  public $data;

  /**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    Entity::__construct($values, 'accroche');
  }

  /**
   * Overrides defaultUri function from entity
   *
   * @see Entity::defaultUri()
   */
  protected function defaultUri() {
    return array('path' => 'accroche/' . $this->layout);
  }
}
