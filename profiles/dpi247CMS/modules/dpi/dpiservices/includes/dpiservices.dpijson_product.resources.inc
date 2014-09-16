<?php
module_load_include("inc", 'dpiservices', 'includes/dpiservices.pages.view_product');

function dpiservices_dpijson_product_retrieve($product){  
  $json_export = array();
  
  $product_element = dpiservices_get_one_product_by_label($product);
  
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
  $array_links = dpiservices_dpijson_product_get_element($tree);
  
  if(isset($tree) && empty($tree)){
    $json_export = array(
      'status' => 'fail'
    );
  }else{
    $json_export = array(
      'status' => 'success',
      'other_access' => true,
      'main' => array(
        $array_links
      ),
    );
  }
  
  return $json_export;  
}

function dpiservices_dpijson_product_get_element($tree){
  if(isset($tree) && empty($tree)) 
    return "no_child";
  
  foreach ($tree as $key => $v) {
    try {
      $wildcard = explode('/', $v['link']['link_path']);
      $node_links[] = array(
        'id' => $key,
        'label' => $v["link"]['link_title'],
        'original_url' => $v['link']['link_path'],
        'feed' => ((strcmp($v['link']['router_path'], "section/%")==0) ? dpi_variable_get("dpiservices_settings_link_url", "Error").'/dpijson_section/'.$wildcard[1].'.json' : ( (strcmp($v['link']['router_path'], "node/%")==0)? dpi_variable_get("dpiservices_settings_link_url", "Error").'/dpijson_packages/'.$wildcard[1].'.json' :'undifined resource')),
        'options' => array(
          "@todo"
        ),
        'childrens' => array(
          dpiservices_dpijson_product_get_element($v['below'])
        ),
      );
    } catch (Exception $e) {
      
    }
    
  }
  
  return $node_links;
}