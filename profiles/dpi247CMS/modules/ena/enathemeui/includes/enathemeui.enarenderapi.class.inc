<?php 


class EnaRenderApi{

  function get_values_for_uniqueid(){

  }

  function get_uniqueid(){

  }

  function settings_form($form,$form_state){
    $form=array();
    return $form;
  }

  function settings_form_submit($form,$form_state){
     
  }


  function get_render_array(){

  }

  function get_summary(){

  }

  function save_values($plugin_name,$unique_id,$values_item){

    db_merge('enathemeui_values')
    ->key(array('plugin_name'=>$plugin_name,'unique_id'=>$unique_id))
    ->fields(array(
    'plugin_name'=>$plugin_name,
    'unique_id'=>$unique_id,
    'form_values'=>serialize($values_item)
    ))
    ->execute();
     
  }

  function get_values($plugin_name,$unique_id){
    $query = db_select('enathemeui_values', 'e');
    // Add extra detail to this query object: a condition, fields and a range
    $query->condition('e.plugin_name', $plugin_name, '=');
    $query->condition('e.unique_id', $unique_id, '=');
    $query->fields('e', array('form_values'));
    $results = $query->execute();
    foreach($results as $result){
      
    }
    $form_values=unserialize($result->form_values);
    return $form_values;
    

  }


}
