<?php
/**
 * @author lba
 */

/**
 * This function generate array for json retrieve
 * 
 * @param String $product
 * @return array
 */
function dpiservices_dpijsonexporter_product_retrieve($product){
  module_load_include("inc", 'dpiservices', 'dpiservices.api');
  
  $json_export = array();
  $product_element = dpiservices_product_get_one_product_by_label($product);
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu' => $product_element['product_type']), array('fetch' => PDO::FETCH_ASSOC));
  $links = array();
    
  foreach ($result as $item) {
    $links[] = $item;
  }
  
  $tree = menu_tree_data($links);
  $array_links = array();
  $array_links = dpiservices_dpijsonexporter_product_get_element($tree, $product_element);

  if(isset($tree) && empty($tree)){
    $json_export = array(
      'status' => 'fail'
    );
  }else{
    $json_export = array(
      'status' => 'success',
      'other_access' => 'N/A',
      'main' => $array_links,
    );
    drupal_alter('dpijson_product_add_other_access', $json_export);
  }

  return $json_export;
}

/**
 * This function is use to generate child by recursivity
 * 
 * @param array $tree
 * @return array:multiple
 */
function dpiservices_dpijsonexporter_product_get_element($tree, $product_element){
  if(isset($tree) && empty($tree))
    return null;

  foreach ($tree as $key => $v) {
    try {
      $wildcard = explode('/', $v['link']['link_path']);
      $node_links[] = array(
        'id' => $wildcard[1],
        'label' => $v["link"]['link_title'],
        'original_url' => $v['link']['link_path'],
        'feed' => dpiservices_dpijsonexporter_product_get_feed($v['link']['router_path'], $product_element, $wildcard),
        'options' => array(
          //add options with hook alter
        ),        
      ); 
      if(($var = dpiservices_dpijsonexporter_product_get_element($v['below'], $product_element))!=null){
        $node_links["childrens"] = $var;
      }
    } catch (Exception $e) {
      return "fail";
    }
  }
  drupal_alter('dpijson_product_add_options', $node_links);
  return $node_links;
}

/**
 * This function test the version for feed url
 * 
 * @param String $path
 * @param product $product_element
 * @return string
 */
function dpiservices_dpijsonexporter_product_get_feed($path, $product_element, $wildcard){
  $feed;
  if(is_numeric(strpos($product_element["version"], "dpi"))){
    $feed = $product_element["version"].'/dpi-section/'.$wildcard[1].'/'.$product_element["label"].'.json';
  }else{
    $feed = $product_element["version"].'/wally-section/'.$wildcard[1].'.json';
  }
  return $feed;
}