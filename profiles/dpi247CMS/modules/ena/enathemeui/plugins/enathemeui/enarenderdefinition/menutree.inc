<?php 


class menutree{

  function get_definition(){
    return array(

      'menu_edit_menu'=>array(
        'enarenderoperationwrapper'=>array(
//          'form uniqueid callback' =>'menu_edit_menu_get_form_uniqueid',
//          'element uniqueid callback' =>'menu_edit_menu_get_cocoid',
//          'alter theme registry' => 'menu_edit_menu_registry_alter',
          'render operation'=>array(
            'menu_tree' => array(
              'preprocess functions'=>'mafonctiondepreprocess_menu_edit_menu'
            ),
          ),
        ),
      ),
    );
  }
  
  function get_form_uniqueid($form,$form_state){
    return str_replace('_','-',$form['menu_name']['#default_value']);
  }


  function get_element_uniqueid(& $variables){

  }

  function registry_alter(& $registry_element){
    foreach ($registry_element['preprocess functions'] as $key => $value) {
      if ($value == 'template_preprocess_menu_tree') {
        unset($registry_element['preprocess functions'][$key]);
      }
    }
  }
  function preprocess_menu_tree(& $variables){
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
  }




}