<?php 



function enathemeui_api_get_enarenderapi_plugins(){
  ctools_include('plugins');

  return ctools_get_plugins('enathemeui', 'enarenderapi');
}


function enathemeui_api_get_enarenderapi_class($enarenderapi_plugin_name){
  ctools_include('plugins');

  $plugin= ctools_get_plugins('enathemeui', 'enarenderapi',$enarenderapi_plugin_name);
  //ctools_plugin_get_class me donne le nom de la class
  $class_name= ctools_plugin_get_class($plugin, 'handler');
  //Ici j'instancie un  nouvel objet de type "$class_name" dynamiquement
  return new $class_name;
  
}