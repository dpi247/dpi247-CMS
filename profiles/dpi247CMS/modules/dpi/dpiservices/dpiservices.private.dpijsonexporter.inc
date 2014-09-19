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
  $array_links = dpiservices_dpijsonexporter_product_get_element($tree);

  if(isset($tree) && empty($tree)){
    $json_export = array(
      'status' => 'fail'
    );
  }else{
    $json_export = array(
      'status' => 'success',
      'other_access' => 'N/A',
      'main' => array(
        $array_links
      ),
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
function dpiservices_dpijsonexporter_product_get_element($tree){
  if(isset($tree) && empty($tree))
    return "no_child";

  foreach ($tree as $key => $v) {
    try {
      $wildcard = explode('/', $v['link']['link_path']);
      $node_links[] = array(
        'id' => $key,
        'label' => $v["link"]['link_title'],
        'original_url' => $v['link']['link_path'],
        'feed' => ((strcmp($v['link']['router_path'], "section/%")==0) ? dpi_variable_get("dpiservices_settings_link_url", "Error").'/dpi-section/'.$wildcard[1].'.json' : ( (strcmp($v['link']['router_path'], "node/%")==0)? dpi_variable_get("dpiservices_settings_link_url", "Error").'/dpi-packages/'.$wildcard[1].'.json' :'undifined resource')),
        'old_feed' => ((strcmp($v['link']['router_path'], "section/%")==0) ? dpi_variable_get("dpiservices_settings_link_url", "Error").'/wally-section/'.$wildcard[1].'.json' : ( (strcmp($v['link']['router_path'], "node/%")==0)? dpi_variable_get("dpiservices_settings_link_url", "Error").'/wally-packages/'.$wildcard[1].'.json' :'undifined resource')),
        'options' => array(
          //add options with hook alter
        ),
        'childrens' => array(
          dpiservices_dpijsonexporter_product_get_element($v['below'])
        ),
      );
      drupal_alter('dpijson_product_add_options', $node_links);
    } catch (Exception $e) {
      return "fail";
    }
  }
  return $node_links;
}