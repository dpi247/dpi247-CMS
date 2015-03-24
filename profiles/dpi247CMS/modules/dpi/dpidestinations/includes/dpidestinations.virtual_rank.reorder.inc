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

  $active = array(
    0 => t('None'),
    1 => t('Redéfinir le virtual rank de toutes les destinations'), 
    2 => t('Redéfinir le virtual rank d\'une destination'),
    3 => t('Redéfinir un virtual rank pour les valeurs null'),
  );

  $form['select'] = array(
    '#type' => 'radios',
    '#title' => t('Select destinations'),
    '#default_value' => 0,
    '#options' => $active,
  );

  $form ["all"] = array(
    '#type' => 'item',
    "#markup" => t('Modifie le virtual rank identique pour une même destination afin d\'éviter la saturation'),
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 1),
      ),
    ),
  );
  
  $form ["destination"] = array(
    '#prefix' => '<div id="destination-add-form" class="container-inline" >',
    '#suffix' => '</div>',
    "#tree" => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 2),
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
      'wrapper' => 'destination-add-form',
      'method' => 'replace'
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 2),
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
        ':input[name="select"]' => array('value' => 2),
      ),
    ),
  );

  $form ["Null"] = array(
    '#type' => 'item',
    "#markup" => t('Set la valeur 0 dans chacun des éléments aillant null comme virtual rank'),
    '#states' => array(
      'visible' => array(
        ':input[name="select"]' => array('value' => 3),
      ),
    ),
  );
  
  $form['submit'] = array('#type' => 'submit', '#value' => t('Execute'));

  return $form;
}

function dpiblocks_ajax_callback_target_from_entity($form, &$form_state) {
  return $form["destination"];
}

function dpidestinations_reform_virtual_rank_submit($form, &$form_state){  
  try {
    switch($form_state['values']['select']){
      case 0:
          //nothing to do
        break;
      case 1:
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
          drupal_set_message('Updating with success');
        break;
      case 2:
          /* One targetblock */
          dpidestinations_update_virtual_rank_for_saturation($form_state['values']['destination']['destination_section'],$form_state['values']['destination']['destination_target']);
          drupal_set_message('Updating with success');
        break;
      case 3:
          /* set 0 to null value */
          $return_msg = dpidestinations_change_null_virtual_rank_value(0);
          drupal_set_message($return_msg);
        break;
      default:
          drupal_set_message('La valeur selectionnée n\'est pas prise en charge');
        break;
    }    
  } catch (Exception $exc) {
    drupal_set_message('Updating with error', 'error');  
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

function dpidestinations_change_null_virtual_rank_value($value = null){
  $str_error = "";
  if(!isset($value)){
    $value = 0;
  }
  $NullValues = dpidestinations_get_null_virtual_rank();
  if($NullValues && is_array($NullValues) && !empty($NullValues)){
    module_load_include('inc', 'dpidestinations', 'dpidestinations.api');
    foreach($NullValues as $info){
      try {
        _dpidestinations_virtualrank_update_virtualrank($info->entity_id, $info->field_destinations_tid, $info->field_destinations_target, $value);
      }
      catch (Exception $exc) {
        $str_error .= "Error : ".$exc->getMessage()."\n";
      }      
    }
  }
  if($str_error == ""){
    $str_error = t('Modification ajoutée avec succès');
  }
  return $str_error;
}

function dpidestinations_get_null_virtual_rank(){
  $return = FALSE;  
  try {
    $sql = "SELECT *
            FROM field_data_field_destinations 
            WHERE field_destinations_virtual_rank IS NULL";
    return db_query($sql)->fetchAll();
  } catch (Exception $exc) {      
  }  
  return $return;
}