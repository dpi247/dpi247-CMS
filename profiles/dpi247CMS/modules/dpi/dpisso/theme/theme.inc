<?php 
/**
 * 
 * @param unknown $vars
 * @return Ambigous <An, string>
 */
function theme_checkboxtable($vars) {

  $element = $vars['element'];
  
  $header=array();
  
  $rows = array();

  foreach (element_children($element) as $key) {

//    dsm($element[$key],"elk"); 
    
    $header[]=$element[$key]['#extra_data']['field_name'];

    $rows[$k] = array();
    $rows[$k][] = array('data' => $element[$key]['#extra_data']['field_name']);
    
    foreach ($element[$key]["#options"] as $k=>$value) {
      
      //dsm($k);
       
      $newelem = $element[$key];
      $newelem["#options"] = array($k);
      $newelem[2] = null;
      $newelem[4] = null;
      
      dsm($newelem); 
      
      $rows[$k][] = array('data' => render($newelem));
    }

        
  }
  
  return theme('table', array('header' => $header, 'rows' => $rows));
  
  
}
