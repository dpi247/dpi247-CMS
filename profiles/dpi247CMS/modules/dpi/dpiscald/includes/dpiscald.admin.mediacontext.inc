<?php 
function dpiscald_media_context_admin_form($form, & $form_state){
  $form = array();

    //prepare elements
    $entity_info = entity_get_info('node');
    $view_modes = array_keys($entity_info['view modes']);
    $scald_types = array_keys(scald_types());
    $contexts = dpiscald_get_scald_context_array();
    $contexts = array( "Not define" ) + $contexts;
    $default_values = variable_get("dpiscald_media_context_table_value", NULL);
    // Build a options array with all roles
    $roles = array_map('check_plain', user_roles(TRUE));
    
    $element = array(); 
    $element['#tree'] = TRUE;
    $element['#title'] = t('Please choose Content Types');
    $element['#theme'] = array('checkboxgrid');
    $element['#options_row'] = $scald_types;
    $element['#options_col'] = $view_modes;
    $element['#theme_wrappers'] = array('form_element');
    
    foreach ($scald_types as $rkey => $rchoice) {
      $element += array($rkey => array());
      $element[$rkey]['#tree'] = TRUE;
      $element[$rkey]['#title'] = $rchoice;
      
      $weight = 0.0;
      foreach ($view_modes as $ckey => $cchoice) {
        $weight += 0.001;
        $element[$rkey][$ckey] = array(
          '#type' => 'select',
          '#return_value' => array($rkey,$ckey),
          '#weight' => $weight,
          '#options' => $contexts,
          '#default_value' => (isset($default_values[$rchoice][$cchoice]))? $default_values[$rchoice][$cchoice] : FALSE,
        );
      }
    }
    
  $form['table_media_context'] = $element;
  
  // Add a submit button for fun.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

function dpiscald_media_context_admin_form_submit($form, & $form_state){
  try {
    
    $entity_info = entity_get_info('node');
    $view_modes = array_keys($entity_info['view modes']);
    $scald_types = array_keys(scald_types());
    $table_sav = array();
    
    foreach($scald_types as $rkey => $rchoice){
      $table_sav[$rchoice] = array();
      foreach($view_modes as $ckey => $cchoice){
        $table_sav[$rchoice][$cchoice] = $form_state["values"]["table_media_context"][$rkey][$ckey];
      }
    }
    variable_set("dpiscald_media_context_table_value", $table_sav);
    drupal_set_message("Table updated !");
  } catch (Exception $e) {
    drupal_set_message("Problem occured !", 'error');
  }
}

function dpiscald_get_scald_context_array(){
  $contexts = array_keys(scald_contexts());
  $return = array();
  foreach($contexts as $context){
    $return[$context] = $context;
  }
  return $return;
}