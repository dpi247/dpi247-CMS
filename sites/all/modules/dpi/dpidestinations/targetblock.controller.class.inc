<?php
class TargetBlockController extends EntityAPIControllerExportable{
  public function create(array $values = array()) {
    global $user;
    $values += array(
        'use_pager' => FALSE,
    );
    return parent::create($values);
  }
  //return an array keyed with tid of targetblocks that belongs to a display id  
public function load_targetblock_by_panels_display($did){
    
    $conditions=array('did'=>$did);
    return  targetblock_load_multiple(FALSE, $conditions);
  
     
  }
  public function load_targetblock_by_panels_panelid($pid){
  
    $conditions=array('pid'=>$pid);
    $result=current(targetblock_load_multiple(FALSE, $conditions));
    if(!$result){
      $result=new TargetBlockEntity(array(),'TargetBlock');
    }
    return $result;
     
  }
  
}