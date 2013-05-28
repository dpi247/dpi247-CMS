<?php
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
  public $node_count_per_target;
  

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
  public $split_number;
  /**
   * The total number of splitted targetblockinstance for this targetblock in this display
   *
   * @var integer
   */
  public $split_total;
  
  
  protected function defaultLabel() {
    return $this->label;
  }
  protected function defaultUri() {
    return array('path' => 'targetblock/' . $this->targetid);
  }
}
