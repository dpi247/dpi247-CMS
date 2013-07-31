<?php
class TargetController extends EntityAPIControllerExportable{
  public function create(array $values = array()) {
    $values += array(
      'targetid' => '',
      'machine_name' => '',
      'label' => '',
      'description' => '',
      'view_name' => '',
      'view_display' => '',
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('target', $entity);

    // Make Description and Status themed like default fields.
    $content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Description'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'target',
      '#bundle' => 'target',
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
  public function load_target_by_name($target_name){
  
    $conditions=array('machine_name'=>$target_name);
    $result=target_load_multiple(FALSE, $conditions);
    if(!$result){
      $result=NULL;
    }
    return current($result);
     
  }

}
class TargetBlockController extends EntityAPIControllerExportable{
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'use_pager' => FALSE,
    );
    return parent::create($values);
  }
  //return an array keyed with tid of targetblocks that belongs to a display id
  public function load_targetblocks_by_panels_display($did){

    $conditions=array('did'=>$did);
    return  targetblock_load_multiple(FALSE, $conditions);

     
  }
  public function load_targetblock_by_panels_panelpid($pid){

    $conditions=array('pid'=>$pid);
    $result=current(targetblock_load_multiple(FALSE, $conditions));
    if(!$result){
      $result=new TargetBlockEntity(array(),'TargetBlock');
    }
    return $result;
     
  }
  public function load_targetblock_by_did_and_target($did, $target_name){
    $results=$this->load_targetblocks_by_did_and_target($did,$target_name);
    return current($results);
  }
  public function load_targetblock_by_did_target_and_splitdelta($did, $target_name,$split_delta){
    $conditions=array('did'=>$did,' target'=>$target_name,'split_delta'=> $split_delta);
    $results=targetblock_load_multiple(FALSE, $conditions);
    $result=current($results);
    if(!$result){
      $result=NULL;
    }
    return $result;
  }

  public function load_targetblock_by_termid_and_target($term_id,$target_name){
     $did=dpidestination_api_get_panelsdisplayid_for_destinationterm($term_id);
     return $this->load_targetblock_by_did_and_target($did,$target_name);
  }
  
  public function load_targetblocks_by_did_and_target($did,$target_name){
    $conditions=array('did'=>$did,'target'=>$target_name);
    $results=targetblock_load_multiple(FALSE, $conditions);
    if(!$results){
      $results=array();
    }
    return $results;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL){
    _dpidestinatons_targetblock_calculate_node_count_for_targetblock($entity);
    $result=parent::save($entity);
    _dpidestinatons_targetblock_sync_node_count_for_targetblock($entity);
    return $result;

  }
}