<?php 
/**
 * 
 * @param unknown $vars
 * @return Ambigous <An, string>
 */

function theme_checkboxgrid($variables) {
  
  $element = $variables['element'];
  // Prepare the header of table. 
  $header = array();
  $header[] = ""; // Skip the first position, because of row labels.
  foreach ($element["#options_col"] as $ck=>$col) {
    $header[]=$col;
  }

  // Prepare the rows - get the element redered. 
  $rows = array();
  foreach (element_children($element) as $key) {
    $child = $element[$key];
    $cols = array();
    // Cols 1st position is the label.
    $cols[]= array('data' => $element["#options_row"][$key]);
    foreach (element_children($child) as $keyitem) {
      $item = $child[$keyitem];
      $cols[] = array('data' => render($item));
    }
    $rows[]=$cols;
  }
  // Theme as a table.
  return theme('table', array('header' => $header, 'rows' => $rows));
}

