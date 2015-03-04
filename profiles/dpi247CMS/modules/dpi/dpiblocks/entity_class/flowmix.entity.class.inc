<?php 

/**
 * A child class of Entity
 */
class FlowMixEntity extends Entity {
  /**
   * The fid of the FlowMix
   *
   * @var $fid
   */
  public $fid;

  /**
   * The machine_name of the FlowMix
   *
   * @var $machine_name
   */
  public $machine_name;

  /**
   * The title of FlowMix
   *
   * @var $title
   */
  public $title;

  /**
   * Serialized datas of the Flowmix
   *
   * $var $datas
   */
  public $datas;

  /**
   * Overrides defaultLabel function from entity
   *
   * @see Entity::defaultLabel()
   */
  protected function defaultLabel() {
    return $this->title;
  }
   
  /**
   * Overrides defaultUri function from entity
   *
   * @see Entity::defaultUri()
   */
  protected function defaultUri() {
    return array('path' => 'flowmix/' . $this->fid);
  }
}
