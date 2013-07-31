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
class TargetBlockEntity extends Entity{

  /**
   * The target id.
   *
   * @var integer
   */
  public $targetblockid;

  /**
   * The machine name.
   *
   * @var string
   */
  public $machine_name;

  /**
   * The target reference.
   *
   * @var integer
   */
  public $target;

  /**
   * The panel display reference.
   *
   * @var integer
   */
  public $page_manager_handlers_did;


  /**
   * Total numbers of nodes displayed in this target.
   *
   * @var integer
   */
  public $node_count_for_targetblockinstance;


  /**
   * Override pager settings.
   *
   * @var boolean
   */
  public $use_pager;


  /**
   * Offset for this targetblock instance.
   *
   * @var integer
   */
  public $offset;


  /**
   * should this pager be hide in this targetblockinstance.
   *
   * @var boolean
   */
  public $hide_pager;

  /**
   * Is this targetblock instance part of a splitted targetblock
   *
   * @var boolean
   */
  public $use_split;
  /**
   * The order of the splitted targetblockinstance
   *
   * @var integer
   */
  public $split_delta;



  protected function defaultLabel() {
    return $this->machine_name;
  }
  protected function defaultUri() {
    return array('path' => 'targetblock/' . $this->targetid);
  }
}
