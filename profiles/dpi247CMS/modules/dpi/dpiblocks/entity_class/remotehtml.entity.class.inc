<?php 

/**
 * A child class of Entity
 *
 * @author jpl
 */
class RemoteHtmlEntity extends Entity {
  /**
   * The rid of remote HTML
   *
   * @var $rid
   */
  public $rid;

  /**
   * The machine_name of remote HTML
   *
   * @var $machine_name
   */
  public $machine_name;

  /**
   * The title of remote HTML
   *
   * @var $title
   */
  public $title;

  /**
   * The HTML adress of remote HTML
   *
   * @var $html_adresse
   */
  public $html_adresse;

  /**
   * The refresh time of remote HTML
   *
   * @var $refresh_time
   */
  public $refresh_time;

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
    return array('path' => 'remotehtml/' . $this->rid);
  }
}
