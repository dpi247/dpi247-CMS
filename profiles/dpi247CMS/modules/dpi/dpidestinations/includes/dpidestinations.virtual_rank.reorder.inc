<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function dpidestinations_reform_virtual_rank($form, &$form_state) {
  $form = array(
    "#prefix" => '<div id="form_target">',
    "#suffix" => '</div>',
  );

  $active = array(0 => t('Redéfinir le virtual rank de toutes les destinations'), 1 => t('Redéfinir le virtual rank d\'une destination'));

  $form['select'] = array(
    '#type' => 'radios',
    '#title' => t('Select destinations'),
    '#default_value' => 0,
    '#options' => $active,
  );

  $form ["destination"] = array(
    '#prefix' => '<div id="destination-add-form" class="container-inline" >',
    '#suffix' => '</div>',
    "#tree" => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 1),
      ),
    ),
  );

  $form ["destination"] ['destination_section'] = array(
    '#type' => 'select',
    '#title' => t('Choose a section'),
    '#required' => FALSE,
    '#options' => dpidestinations_api_get_all_sections_term_available_as_select_options(FALSE),
    '#ajax' => array(
      'callback' => 'dpiblocks_ajax_callback_target_from_entity',
      'wrapper' => 'form_target',
      'method' => 'replace'
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 1),
      ),
    ),
    '#default_value' => (isset($form_state ['values']['destination'] ['destination_section'])) ? $form_state ['values']['destination'] ['destination_section'] : ''
  );

  if (isset($form_state ['values']['destination'] ['destination_section'])) {
    $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm($form_state ['values']['destination'] ['destination_section']);
  }
  else {
    $options = array();
  }

  $form ['destination'] ['destination_target'] = array(
    '#type' => 'select',
    '#title' => t('Choose a target'),
    '#required' => FALSE,
    "#options" => $options,
    '#default_value' => (isset($form_state ['values']['destination'] ['destination_target'])) ? $form_state ['values'] ['destination'] ['destination_target'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 1),
      ),
    ),
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Execute'));

  return $form;
}

function dpiblocks_ajax_callback_target_from_entity($form, &$form_state) {
  return $form;
}

function dpidestinations_reform_virtual_rank_submit($form, &$form_state){  
  if($form_state['values']['select']){
    /* one targetblock */
    dpidestinations_update_virtual_rank_for_saturation($form_state['values']['destination']['destination_section'],$form_state['values']['destination']['destination_target']);
  }else{
    /* all targetblock */
    $tids = dpidestinations_api_get_all_sections_term_available_as_select_options(FALSE);
    if(is_array($tids)){      
      $keys = array_keys($tids);
      foreach($keys as $tid){
        if(isset($tid) && $tid != ""){
          $targets = __dpidestinations_get_selectoption_get_targets_for_sectionterm(intval($tid));
          if(is_array($targets)){
            foreach($targets as $k_target => $v_target){
              dpidestinations_update_virtual_rank_for_saturation($tid, $k_target);
            }
          }
        }
      }
    }
  }
}

function dpidestinations_update_virtual_rank_for_saturation($tid, $target){
  $results = dpidestinations_get_virtual_rank_saturation($tid, $target);   
  if($results){
    module_load_include('inc', 'dpidestinations', 'dpidestinations.api');
    $max_rank_value = (isset($results[0]->field_destinations_virtual_rank)) ? $results[0]->field_destinations_virtual_rank*2 : 0;
    $min_rank_value = (isset($results[count($results)-1]->field_destinations_virtual_rank)) ? $results[count($results)-1]->field_destinations_virtual_rank : 0;         
    $nb_result = count($results)-1;    
    if( $nb_result > 0 ){
      $range = $max_rank_value - $min_rank_value;
      $jump = $range/$nb_result;
      for($i=0; $i< count($results);$i++){
        $virtual_rank = (int) ($max_rank_value - ($i*$jump));
        _dpidestinations_virtualrank_update_virtualrank($results[$i]->entity_id, $tid, $target, $virtual_rank);        
      } 
    }
  }
}

function dpidestinations_get_virtual_rank_saturation($tid, $target){
  $return = FALSE;  
  try {
    $sql = "SELECT entity_id, field_destinations_virtual_rank
            FROM field_data_field_destinations 
            WHERE field_destinations_virtual_rank >= (SELECT MIN(p.field_destinations_virtual_rank)
                                                     FROM field_data_field_destinations p, field_data_field_destinations a
                                                     WHERE p.field_destinations_virtual_rank = a.field_destinations_virtual_rank
                                                        AND p.entity_id <> a.entity_id
                                                        AND p.field_destinations_tid = $tid
                                                        AND p.field_destinations_target = '$target'
                                                        AND a.field_destinations_tid = $tid
                                                        AND a.field_destinations_target = '$target')
                  AND field_destinations_tid = $tid
                  AND field_destinations_target = '$target'
            ORDER BY field_destinations_virtual_rank DESC";
    return db_query($sql)->fetchAll();
  } catch (Exception $exc) {      
  }  
  return $return;
}