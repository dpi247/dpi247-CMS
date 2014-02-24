<?php

/**
 * Implementation of hook_entity_info_alter
 *
 * @param Array $entity_info
 */
function unfoldcompanion_entity_info_alter(&$entity_info) {
  $entity_info['node']['view modes'] += array(
    'targetblock_small' => array(
      'label' => t('TargetBlock Small'),
      'custom settings' => TRUE,
    ),
    'targetblock_medium' => array(
      'label' => t('TargetBlock Medium'),
      'custom settings' => TRUE,
    ),
    'targetblock_big' => array(
      'label' => t('TargetBlock Big'),
      'custom settings' => TRUE,
    ),
    'targetblock_red' => array(
      'label' => t('TargetBlock Red'),
      'custom settings' => TRUE,
    ),
  );
}
