<?php

function dpiservices_services_resources(){
  return array(
    'dpijson_packages' => array(
        'retrieve' => array(
          'help' => t('Return a packages for .'),
          'file' => array(
            'type' => 'inc',
            'module' => 'dpiservices',
            'name' => 'includes/dpiservices.dpijson_packages.resources',
          ),
          'callback' => 'dpiservices_dpijson_packages_retrieve',          
          'access callback' => 'my_module_get_user_count_access_callback',
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
    'dpijson_section' => array(
      'retrieve' => array(
        'help' => t('Return a packages for .'),
        'file' => array(
          'type' => 'inc',
          'module' => 'dpiservices',
          'name' => 'includes/dpiservices.dpijson_section.resources',
        ),
        'callback' => 'dpiservices_dpijson_section_retrieve',
        'access callback' => 'my_module_get_user_count_access_callback',
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
        'file' => array('type' => 'inc', 'module' => 'dpiservices', 'name' => 'includes/dpiservices.dpijson_section.resources'),
        'callback' => 'dpiservices_dpijson_section_index',
        'args' => array(),
        'access arguments' => array('index'),
      ),
    ),
    'dpijson_product' => array(
      'retrieve' => array(
        'help' => t('Return a packages for .'),
        'file' => array(
          'type' => 'inc',
          'module' => 'dpiservices',
          'name' => 'includes/dpiservices.dpijson_product.resources',
        ),
        'callback' => 'dpiservices_dpijson_product_retrieve',
        'access callback' => 'my_module_get_user_count_access_callback',
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
  );
}

/**
 *
 * @param unknown $op
 * @return boolean
 */
function my_module_get_user_count_access_callback($op){
  return true;
}
