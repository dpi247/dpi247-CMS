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

    return $info;
  }
}

