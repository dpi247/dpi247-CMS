<?php
class TargetEntity extends Entity{

  /**
   * The target id.
   *
   * @var integer
   */
  public $targetid;

  /**
   * The machine name.
   *
   * @var string
   */
  public $machine_name;

  /**
   * The label.
   *
   * @var string
   */
  public $label;

  /**
   * The view_name.
   *
   * @var string
   */
  public $view_name;

  /**
   * The view_id.
   *
   * @var string
   */
  public $view_id;
  
  protected function defaultLabel() {
    return $this->label;
  }
  protected function defaultUri() {
    dsm($this);
    return array('path' => 'target/' . $this->targetid);
  }
}
