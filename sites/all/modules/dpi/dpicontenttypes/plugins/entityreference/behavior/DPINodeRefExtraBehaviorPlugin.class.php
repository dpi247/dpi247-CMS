<?php
//I know convention name is strange but CTOOLS prefer .class.php instead of .inc extensions. ...

/**
 * The "DPINodeRefExtraBehaviorPlugin" Entity Reference plugin extend a relation with to extra informations.
 * The referenced node type and a "media type" like picture, video, music...
 */
class DPINodeRefExtraBehaviorPlugin extends EntityReference_BehaviorHandler_Abstract {

  public function insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
    $this->dpi_get_extra_infos($items);
  }

  public function update($entity_type, $entity, $field, $instance, $langcode, &$items) {
    $this->dpi_get_extra_infos($items);
  }

  public function schema_alter(&$schema, $field) {
    $schema['columns']['node_type']=array('type' => 'varchar', 'length' => 254, 'not null' => FALSE);
    $schema['columns']['media_type']=array('type' => 'varchar', 'length' => 254, 'not null' => FALSE);
  }


  private function dpi_get_extra_infos(& $items){
    foreach($items as &$item){
      $node=node_load($item['target_id']);
      $item['node_type']=$node->type;
      $item['media_type']=NULL;
      $item['media_type']='picture';

    }
  }

}