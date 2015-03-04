<?php


/**
 * Implementation of hook_rules_event_info().
 * 
 * We talk about a package if the content type has a destination field whatever his content type
 * 
 * @ingroup rules
 */
function dpicontenttypes_rules_event_info() {
  return array(
    'dpicontenttypes_rules_event_package_create' => array(
      'label' => t('After a package is created'),
      'module' => 'dpicontenttypes',
      'group'=>'DPI247 Packages',
      'variables' => rules_events_node_variables(t('created content')),
    
      'arguments' => array(
        'acting_user' => array('type' => 'user', 'label' => t('The user who create the package.')),
        'package' => array('type' => 'package', 'label' => t('The purchased package.')),
      ),
    ),
    'dpicontenttypes_rules_event_package_edit' => array(
      'label' => t('After a package is edited'),
      'module' => 'dpicontenttypes',
      'group'=>'DPI247 Packages',
      'variables' => rules_events_node_variables(t('updated content'), TRUE),
    ),
    'dpicontenttypes_rules_event_package_create_or_edit' => array(
      'label' => t('Before a package is created or edited'),
      'module' => 'dpicontenttypes',
      'group'=>'DPI247 Packages',
      'variables' => rules_events_node_variables(t('updated content'), TRUE),
    
    ),
    'dpicontenttypes_rules_event_package_deleted' => array(
      'label' => t('After a package is deleted'),
      'module' => 'dpicontenttypes',
      'group'=>'DPI247 Packages',
      'variables' => rules_events_node_variables(t('delete content'), TRUE),
    
    ),
    'dpicontenttypes_rules_event_package_is_published' => array(
      'label' => t('A package is published (embargo date)'),
      'module' => 'dpicontenttypes',
      'group'=>'DPI247 Content types',
      'variables' => rules_events_node_variables(t('updated content'), TRUE),
    
    ),
    'dpicontenttypes_rules_event_package_is_unpublished' => array(
      'label' => t('A package is published (unpublish date)'),
      'module' => 'dpicontenttypes',
      'group'=>'DPI247 Content types',
      'variables' => rules_events_node_variables(t('updated content'), TRUE),
    
    ),
  );
}

