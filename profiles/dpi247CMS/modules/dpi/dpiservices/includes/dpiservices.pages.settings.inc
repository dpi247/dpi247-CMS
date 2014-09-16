<?php
function dpiservices_pages_form_map_preset_form() {
  $products_table = dpiservices_create_table_product ();
  
  $render_array = dpiservices_create_table_product();
  $render_array .= theme('pager');
  
  return $render_array;
}
function dpiservices_create_table_product() {
  $products = dpiservices_get_all_product_info (10);
  
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
    $table_rows [] = array (
      $product ['label'],
      $product ['machine_name'],
      $product ['description'],
      $product ['product'],
      $product ['product_type'],
      '<a href="http://'.$_SERVER['SERVER_NAME'].'/'.dpi_variable_get("dpiservices_settings_link_url", "").'/dpijson_product/'.$product ['label'].'.json">See informations</a>',
      '<a href="/admin/dpi247/dpijsonexporter/edit/'.$product ['label'].'">Modify</a><br /><a href="/admin/dpi247/dpijsonexporter/del/'.$product ['label'].'">Delete</a>',      
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
  
  return $table;
}
function dpiservices_get_all_product_info($nb = 0) {
  $return = array ();
  if($nb <= 0){
    $query = db_select('dpiservices_product', 'dp')->fields('dp', array ())->execute();
  }else{
    $query = db_select('dpiservices_product', 'dp')->fields('dp', array ())->range(0, $nb)->execute();
  }
  
  
  while( $row = $query->fetchAssoc () ) {
    $return [] = $row;
  }
  return $return;
}