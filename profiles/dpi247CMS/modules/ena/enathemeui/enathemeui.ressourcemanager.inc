<?php
/*
 *

$p= enathemeui_api_get_enarenderoperation_plugins();

dsm($p);


$enarenderoperation_plugin_name="enarenderoperationelement";
$class=enathemeui_api_get_enarenderoperation_class($enarenderoperation_plugin_name);
dsm($class);

print $class->operation(1,2);


*/






function enathemeui_ressource_manager_get_active_plugin_for_form_id($form_id,$form,$form_state){
  $plugins= enathemeui_api_get_enarenderoperation_plugins();

  $plugin_mapping=enathemeui_ressourcemanager_get_active_ressource_for_formid();

  $active_plugins_names=$plugin_mapping[$form_id];
  foreach($active_plugins_names as $plugin_name=>$plugin_info){
    $unique_id=call_user_func($plugin_info['form uniqueid callback'],$form,$form_state);
    
    $plugin=enathemeui_api_get_enarenderoperation_class($plugin_name,$unique_id);
    $plugin->default_values=$plugin->get_values();
    
    $active_plugins[$plugin_name]=$plugin;
  }
   
  return $active_plugins;

}



function enathemeui_ressource_manager_form_alter($active_plugins){
  $form=array();
  foreach($active_plugins as $plugin_name=>$plugin){
    $ressource_form=$plugin->settings_form();
    $ressource_form['unique_id']=array(
      "#type"=>'value',
      "#value"=>$plugin->unique_id,
      );
     $form[$plugin_name]=$ressource_form;
  }
  
  return $form;

}











//*********************************
//*********************************
//*********************************




function menu_edit_menu_get_form_uniqueid($form,$form_state){
  return str_replace('_','-',$form['menu_name']['#default_value']);

}


function menu_edit_menu_registry_alter(& $registry_element){
  dsm(OK);
  foreach ($registry_element['preprocess functions'] as $key => $value) {
    if ($value == 'template_preprocess_menu_tree') {
      unset($registry_element['preprocess functions'][$key]);
    }
  }
}
function menu_edit_menu_preprocess_menu_tree(& $variables){
  $unique_id=menu_edit_menu_get_element_uniqueid($variables);
  
  
  //@todo: change the hard coding of menu-edit-menu
  // all this source code should be encapsuled in a plugin with his own definition?
  $definitions=enathemeui_ressourcemanager_get_active_ressource_for_formid();
  $definition=$definitions['menu_edit_menu'];
  
  $variables["#unique_id"]=$unique_id;
  
  array_pop($variables['#theme_wrappers']);
  foreach($definition as $render_operation_plugin_name => $infos){
    dsm($render_operation_plugin_name);
    $variables['#plugin_name'][]=$render_operation_plugin_name;
    
    $variables['#theme_wrappers'][]='enathemeui_theme_operation';
  }
  $variables['#theme_wrappers'][]="block";
  dsm($variables);
}



function menu_edit_menu_get_element_uniqueid(& $variables){
  
}


