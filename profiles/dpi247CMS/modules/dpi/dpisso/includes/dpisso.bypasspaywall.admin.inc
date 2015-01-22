<?php

function dpisso_bypasspaywall_admin_settings_page_form($form, $form_state) {
  $roles = dpisso_switch_key_value(user_roles ());

  $form ['roles'] = array (
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc ( $roles ),
    '#title' => t ( 'Sélectionner les rôles sur lesquelles vous désirez un bypass :' ),
    '#default_value' => dpi_variable_get('dpisso_bypass_user_roles', array()),
  );
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Enregistrer' ) 
  );
  
  return $form;
}

function dpisso_bypasspaywall_admin_settings_page_form_submit($form, & $form_state) {
  if(isset($form_state['values']['roles'])){
    $sav = array();
    foreach($form_state['values']['roles'] as $el){
      if($el){
        $sav[$el] = $el;
      }
    }
    if(isset($sav) || !empty($form_state['values']['roles'])){
      dpi_variable_set('dpisso_bypass_user_roles', $sav);
      drupal_set_message(t("Enregistrement bien effectué"));
    }
  }else{
    drupal_set_message(t("Sauvegarde des rôles non effectué ! Un problème est survenu durant l'opération"), 'error');
  }
}

function dpisso_switch_key_value($array){
  $return = array();
  if(is_array($array)){    
    foreach($array as $el){
      $return[$el] = $el;
    }
  }
  return $return;
}
