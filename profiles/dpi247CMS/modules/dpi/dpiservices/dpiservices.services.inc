<?php
/**
 * @author lba
 */

/**
 * This fonction implements hook_services_resources
 * to create new type of resources in services
 * 
 * @return array
 */
function dpiservices_services_resources(){
  return array(
    'dpi-packages' => array(
        'retrieve' => array(
          'help' => t('Return a packages for .'),
          'file' => array(
            'type' => 'inc',
            'module' => 'dpiservices',
            'name' => 'dpiservices.private.dpi-packages',
          ),
          'callback' => 'dpiservices_dpi_packages_retrieve',          
          'access callback' => 'dpiservices_get_permission_validity',
          'access arguments' => array('retrieve'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'nid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The nid of the node to get',
            ),
          ),
        ),
      ),
    'dpi-section' => array(
      'retrieve' => array(
        'help' => t('Return a packages for .'),
        'file' => array(
          'type' => 'inc',
          'module' => 'dpiservices',
          'name' => 'dpiservices.private.dpi-section',
        ),
        'callback' => 'dpiservices_dpi_section_retrieve',
        'access callback' => 'dpiservices_get_permission_validity',
        'access arguments' => array('retrieve'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'tid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to get',
          ),
          array(
            'name' => 'section',
            'optional' => TRUE,
            'source' => array('path' => 1),
            'type' => 'int',
            'description' => 'The nid of the node to get',
          ),
        ),       
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'dpiservices', 'name' => 'dpiservices.private.dpi-section'),
        'callback' => 'dpiservices_dpi_section_index',
        'args' => array(),
        'access arguments' => array('index'),
      ),
    ),
    'dpijsonexporter' => array(
      'retrieve' => array(
        'help' => t('Return a packages for .'),
        'file' => array(
          'type' => 'inc',
          'module' => 'dpiservices',
          'name' => 'dpiservices.private.dpijsonexporter',
        ),
        'callback' => 'dpiservices_dpijsonexporter_product_retrieve',
        'access callback' => 'dpiservices_get_permission_validity',
        'access arguments' => array('retrieve'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'product',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'varchar',
            'description' => 'The product to get json',
          ),
        ),
      ),
    ),
    'wally-packages' => array(
      'retrieve' => array(
        'help' => t('Return a packages for .'),
        'file' => array(
          'type' => 'inc',
          'module' => 'dpiservices',
          'name' => 'dpiservices.private.wally-packages',
        ),
        'callback' => 'dpiservices_wally_package_retrieve',
        'access callback' => 'dpiservices_get_permission_validity',
        'access arguments' => array('retrieve'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to get',
          ),
        ),
      ),
    ),
    'wally-section' => array(
      'retrieve' => array(
        'help' => t('Return a packages for .'),
        'file' => array(
          'type' => 'inc',
          'module' => 'dpiservices',
          'name' => 'dpiservices.private.wally-section',
        ),
        'callback' => 'dpiservices_wally_section_retrieve',
        'access callback' => 'dpiservices_get_permission_validity',
        'access arguments' => array('retrieve'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'tid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to get',
          ),
          array(
            'name' => 'section',
            'optional' => TRUE,
            'source' => array('path' => 1),
            'type' => 'int',
            'description' => 'The nid of the node to get',
          ),
        ),
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'dpiservices', 'name' => 'dpiservices.private.dpi-section'),
        'callback' => 'dpiservices_dpi_section_index',
        'args' => array(),
        'access arguments' => array('index'),
      ),
    ),
  );
}

/**
 * This function manage access
 * @param unknown $op
 * @return boolean
 */
function dpiservices_get_permission_validity($op){
  return true;
}
