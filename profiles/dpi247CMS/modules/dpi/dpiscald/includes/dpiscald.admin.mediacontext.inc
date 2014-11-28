<?php 
function dpiscald_media_context_admin_form($form, & $form_state){
  $form = array();

  //prepare elements
  $entity_info = entity_get_info('node');
  $view_modes = array_keys($entity_info['view modes']);
  $scald_types = array_keys(scald_types());
  $contexts = array_keys(scald_contexts());

  $header = array();
  $rows = array();
  
  //create select
  $selected_element = array(
    '#type' => 'select',
    '#title' => t('Select scald context'),
    '#options' => $contexts,
    '#default_value' => 0,
  );
    
  //create rows elements
  $index = 0;
  foreach($scald_types as $type){
    if($index == 0){
      $header = array("");
      foreach($view_modes as $mode){
        $header[] = $mode;
      }
      $index++;
    }else{
      $col = 0;
      foreach($view_modes as $mode){
        if($col == 0){
          $rows[$type][$col] = $type;
        }else{
          $rows[$type][$col] = render($selected_element);
        } 
        $col++;
      }
      $rows[$type][$col] = render($selected_element);
    }    
  }
  $form['table'] = array(
    // Theme this part of the form as a table.
    '#theme' => 'formtable_form_table',
    // Pass header information to the theme function.
    '#header' => $header,
    // Rows in the form table.
    'rows' => $rows,
  );
  // Add a submit button for fun.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function dpiscald_media_context_admin_form_submit($form, & $form_state){
  dsm($form_state);
}