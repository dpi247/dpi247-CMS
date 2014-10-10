<?php 



function enathemeui_api_get_enarenderoperation_plugins($plugin_name=NULL){
  ctools_include('plugins');
  $plugins=ctools_get_plugins('enathemeui', 'enarenderoperation');
  if($plugin_name){
    return $plugins[$plugin_name];
  }
  return $plugins;
}


function enathemeui_api_get_enarenderoperation_class($plugin_name,$unique_id=NULL){
  ctools_include('plugins');

  $plugin= ctools_get_plugins('enathemeui', 'enarenderoperation',$plugin_name);
  //ctools_plugin_get_class me donne le nom de la class
  $class_name= ctools_plugin_get_class($plugin, 'handler');
  //Ici j'instancie un  nouvel objet de type "$class_name" dynamiquement
  return new $class_name($plugin_name,$unique_id);
  
}