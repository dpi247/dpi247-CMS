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
   * The view_display.
   *
   * @var string
   */
  public $view_display;
  
  protected function defaultLabel() {
    return $this->label;
  }
  protected function defaultUri() {
    return array('path' => 'target/' . $this->targetid);
  }
}
