<?php

class JsonproductEntity extends Entity {
  /**
   * The target id.
   *
   * @var integer
   */
  public $id;

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
   * The description.
   *
   * @var string
   */
  public $description;

  /**
   * The menu name.
   *
   * @var string
   */
  public $menu_name;
  /**
   * The available presets.
   *
   * @var string
   */
  public $presets;

  protected function defaultLabel() {
    return $this->label;
  }

  protected function defaultUri() {
    return array('path' => 'dpijsonexporter/jsonproduct/' . $this->id);
  }
}

