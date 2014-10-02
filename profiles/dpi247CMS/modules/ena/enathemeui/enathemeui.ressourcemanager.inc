<?php
/*
 *

$p= enathemeui_api_get_enarenderapi_plugins();

dsm($p);


$enarenderapi_plugin_name="enarenderapielement";
$class=enathemeui_api_get_enarenderapi_class($enarenderapi_plugin_name);
dsm($class);

print $class->operation(1,2);


*/

function enathemeui_ressourcemanager_get_active_ressource_for_formid(){
  return array(

    'node_form'=>array(
      'enarenderapielement'=>array(
        'uniqueid callback' =>'bla bla',
        'values callback'=>'toto',
      ),
    ),
    'menu_edit_menu'=>array(
      'enarenderapiwrapper'=>array(
        'uniqueid callback' =>'menu_edit_menu_get_uniqueid',
        'values callback'=>'menu_edit_menu_get_values',
      ),
    )

  );

}




function enathemeui_ressource_manager_get_active_plugin_for_form_id($form_id,$form,$form_state){
  $plugins= enathemeui_api_get_enarenderapi_plugins();


  $plugin_mapping=enathemeui_ressourcemanager_get_active_ressource_for_formid();

  $active_plugins_names=$plugin_mapping[$form_id];
  foreach($active_plugins_names as $key=>$plugin_name){
    $plugin=enathemeui_api_get_enarenderapi_class($key);
    $plugin->unique_id=call_user_func($plugin_name['uniqueid callback'],$form,$form_state);
    $plugin->default_values=call_user_func($plugin_name['values callback'],$form,$form_state);
    
    $active_plugins[$key]=$plugin;
  }
   
  return $active_plugins;

}



function enathemeui_ressource_manager_form_alter($active_plugins){
  $form=array();
  foreach($active_plugins as $plugin_name=>$plugin){
    $unique_id=$active_plugins['unique_id'];
    
    $default_values=$active_plugins['default_values'];
    
    $ressource_form=$plugin->settings_form($default_values);
    $ressource_form['unique_id']=array(
      "#type"=>'value',
      "#value"=>$unique_id
      );
     $form[$plugin_name]=$ressource_form;
  }
  dsm($form,form);
  
  return $form;

}











//*********************************


function menu_edit_menu_get_uniqueid($form,$form_state){
  
return str_replace('_','-',$form['menu_name']['#default_value']);

}



function menu_edit_menu_get_values($form,$form_state){


}


