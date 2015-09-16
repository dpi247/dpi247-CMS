<?php
/**
 * @author lba
 */

/**
 * This function create
 * 
 * @return render(table)
 */
function dpiservices_pages_form_table_product_form() {
  module_load_include('inc', 'dpiservices', 'dpiservices.api');
  $render_array = dpiservices_create_table_product();
  $render_array .= theme('pager');  
  return $render_array;
}

/**
 * This function return a theme table
 * 
 * @return array
 */
function dpiservices_create_table_product() {
  drupal_add_css(drupal_get_path('module', 'dpiservices')."/css/dpiservices.css");
  
  $products = dpiservices_product_get_all_product_info (10);
  
  $render = l(t('Add product'), 'admin/dpi247/dpiservices/add', array('attributes' => array('class' => 'btn-dpiservices')));
  
  $header_row = array (
    'Label',
    'Name',
    'Description',
    'Product',
    'Product type',
    'Product link',
    'Operations',
  );
  
  $table_rows = array ();
  foreach ( $products as $product ) {    
    $endpoint = services_endpoint_load($product ["version"]);
    $resource = $endpoint->resources;
    $resource_info = (isset($resource["dpijsonexporter"])) ? "dpijsonexporter" : 'wallyjsonexporter';
    $table_rows [] = array (
      $product ['label'],
      $product ['machine_name'],
      $product ['description'],
      $product ['product'],
      $product ['product_type'],
      '<a href="http://'.$_SERVER['SERVER_NAME'].'/'.$product ['version'].'/'.$resource_info.'/'.$product ['machine_name'].'.json">See the result</a>',
      '<a href="/admin/dpi247/dpiservices/edit/'.$product ['machine_name'].'">Modify</a> - <a href="/admin/dpi247/dpiservices/del/'.$product ['machine_name'].'">Delete</a>',      
    );
  }
  
  $table = theme_table ( array (
    'header' => $header_row,
    'rows' => $table_rows,
    'attributes' => array('width' => '100%'),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('No data is available'),
  ) );
  
  return $render . $table;
}