<?php

/**
 * Callback used to define metadata of the destination field
 *
 * This callback was specified witin the dpidestinations_field_info() hook
 * for the Entity_api module see @link https://drupal.org/node/1156554 Property info for fields @endlink
 * from Entity api documentation
 *
 * @param array $info
 *  an array that define the info attached to the entity
 * @param string $entity_type
 * @param array $field
 * @param array $instance
 * @param string $field_type
 *
 * @see dpidestinations_field_info()
 *
 * @todo
 *  USe better definition for layout with proper option_list
 *
 */
function entity_metadata_destination_property_callback(&$info, $entity_type, $field, $instance, $field_type){
  if ( $field['type'] == 'destination') {
    // Define a data structure for dealing with text that is formatted or has
    // a summary.
    $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$field['field_name']];

    $property['getter callback'] = 'entity_metadata_field_verbatim_get';
    $property['setter callback'] = 'entity_metadata_field_verbatim_set';
    unset($property['query callback']);

    $property['type'] = ($field['cardinality'] != 1) ? 'list<destinations>' : 'destinations';
    $property['property info'] = array(
      'tid' => array(
        'type' => 'integer',
        'label' => t('Destination Term Id'),
        'sanitized' => TRUE,
        'getter callback' => 'entity_metadata_field_text_get',
        'setter callback' => 'entity_property_verbatim_set',
        'raw getter callback' => 'entity_property_verbatim_get',
      ),
      'target' => array(
        'type' => 'token',
        'label' => t('Target'),
        'sanitized' => TRUE,
        'getter callback' => 'entity_metadata_field_text_get',
        'setter callback' => 'entity_property_verbatim_set',
        'raw getter callback' => 'entity_property_verbatim_get',
      ),
      'layout' => array(
        'type' => 'text',
        'label' => t('Layout'),
        'getter callback' => 'entity_metadata_field_text_get',
        'setter callback' => 'entity_property_verbatim_set',
        'raw getter callback' => 'entity_property_verbatim_get',
      ),
      'rank' => array(
        'type' => 'integer',
        'label' => t('Rank'),
        'getter callback' => 'entity_metadata_field_text_get',
        'setter callback' => 'entity_property_verbatim_set',
        'raw getter callback' => 'entity_property_verbatim_get',
      ),
      'virtual_rank' => array(
        'type' => 'integer',
        'label' => t('Rank'),
        'getter callback' => 'entity_metadata_field_text_get',
        'setter callback' => 'entity_property_verbatim_set',
        'raw getter callback' => 'entity_property_verbatim_get',
      ),
    );

    // Enable auto-creation of the item
    $property['auto creation'] = 'entity_property_create_array';
  }
}

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
      'entity_type' => array('label' => ''),
      'group_type' => array('label' => ''),
      'field_name' => array('label' => ''),
      'language' => array('label' => ''),
    );
    $properties['available_targetlayouts']['getter callback'] = '_target_metadata_getter';
    $properties['available_targetlayouts']['setter callback'] = '_target_metadata_setter';

    $properties['available_templateoverrides']['getter callback'] = '_target_metadata_getter';
    $properties['available_templateoverrides']['setter callback'] = '_target_metadata_setter';

    $properties['targetview']['getter callback'] = '_target_metadata_getter';
    //$properties['targetview']['setter callback']='_target_metadata_setter';
    $properties['targetview']['label'] = '';

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
      'entity_type' => array('label' => ''),
      'group_type' => array('label' => ''),
      'field_name' => array('label' => ''),
      'language' => array('label' => ''),
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
    $properties['other_splitted_targetblocks']['getter callback'] = '_targetblock_metadata_getter';
    $properties['other_splitted_targetblocks']['label'] = '';

    return $info;
  }
}
