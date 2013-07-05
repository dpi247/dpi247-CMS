<?php



/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the default target metadata properties.
 */
class DpiDestinationsTargetMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    // Initialize values to prevent WSOD, no certain cases of clear cache.
    $properties = !empty($properties) ? $properties : array();
    $properties += array(
      'entity_type' => array(),
      'group_type' => array(),
      'field_name' => array(),
      'language' => array(),
    );
    $properties['available_targetlayouts']['getter callback']='_target_metadata_getter';
    $properties['available_targetlayouts']['setter callback']='_target_metadata_setter';
    
    $properties['available_templateoverrides']['getter callback']='_target_metadata_getter';
    $properties['available_templateoverrides']['setter callback']='_target_metadata_setter';
    
    $properties['targetview']['getter callback']='_target_metadata_getter';
    //$properties['targetview']['setter callback']='_target_metadata_setter';
    
    return $info;
  }
}


/**
 * Extend the default targetblock metadata properties.
 */
class DpiDestinationsTargetblockMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    // Initialize values to prevent WSOD, no certain cases of clear cache.
    $properties = !empty($properties) ? $properties : array();
    $properties += array(
      'entity_type' => array(),
      'group_type' => array(),
      'field_name' => array(),
      'language' => array(),
    );
    //Handling entity references

    unset($properties['target']);
    
    $properties['target']=array(
      'label' => t("Target"),
      'type' => 'target',
      'description' => t("The target of the target node."),
      'required' => TRUE,
      'schema field' => 'target',
      );
    
    //differ when targetblockinstaance is splitted: sum of all splitted instance
    $properties['other_splitted_targetblocks']['getter callback']='_targetblock_metadata_getter';
    
    
    return $info;
  }
}

