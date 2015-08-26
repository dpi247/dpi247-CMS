<?php
/**
 * @author lba
 */
module_load_include('inc', 'dpiservices', 'includes/dpiservices.pages.settings');

/**
 * This function is call to delete a product
 * 
 * @param String $arg
 */
function dpiservices_pages_form_delete($arg){
  if(isset($arg)){
    dpiservices_del_on_product_by_label($arg);
  }else{
    drupal_set_message("Argument is not valid", 'error');
  }
  drupal_goto('admin/dpi247/dpiservices');
}

/**
 * This function delete information in db with label condition
 * 
 * @param String $label
 */
function dpiservices_del_on_product_by_label($label){
  try {
    db_delete('dpiservices_product')
      ->condition('label',$label ,'=')->execute();
    drupal_set_message("$label is deleted");
  } catch (Exception $e) {
    drupal_set_message("An sql error is occuring", 'error');
  }
}