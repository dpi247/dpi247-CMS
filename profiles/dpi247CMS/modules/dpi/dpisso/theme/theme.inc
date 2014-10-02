<?php 
/**
 * 
 */
function dpissopaywallpoliticscheckboxesgrid($element) {

  print "jellloo";
  
  $elements = element_children(expand_checkboxes($element));
  dsm($element,"ele"); 
  dsm($elements,"eles");
  return theme("element",$element);
   
}