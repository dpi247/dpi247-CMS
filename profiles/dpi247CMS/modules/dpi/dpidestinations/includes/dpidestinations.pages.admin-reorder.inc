<?php 

function dpidestinations_get_reorder_form($term = NULL,$target = NULL){
  if (isset($term) && isset($target)){
    $infos = dpidestinations_define_result_by_term($term, $target);
    $nodes = $infos->result;
    $targetblock = $infos->targetblock;
  }
  
  if(!isset($nodes)){
    return drupal_get_form('dpidestinations_destination_define_target_form');
  }elseif(empty($nodes)){
    $val = array(
      '#markup' => t("Aucun élément n'est associé à cette destination"),
    );
    return array(drupal_get_form('dpidestinations_destination_define_target_form', $term->tid, $target->machine_name),$val);
  }else{
    return array(drupal_get_form('dpidestinations_destination_define_target_form', $term->tid, $target->machine_name),drupal_get_form('dpidestinations_page_targetview_reorder_form',$nodes,$term->tid ,$targetblock->target));
  }
}

function dpidestinations_define_result_by_term($term, $target){
  module_load_include('inc', 'dpidestinations', 'dpidestinations.api');
  module_load_include('inc', 'dpidestinations', 'includes/dpidestinations.pages.reorder');
  $return = new stdClass();
  
  $targetblock=NULL;
  $targetblocks=dpidestinations_api_get_all_targetblocks_from_target_for_destination($target->machine_name,$term->tid);
  $targetblock=current($targetblocks);
  $targetblock->node_count_for_targetblockinstance=$targetblock->node_count_for_targetblock;
  $targetblock->offset=0;
  
  $targetview=_dpidestinations_targetviewreorder_initialize_view($targetblock,$term);
  $targetview->pre_execute();
  $targetview->execute();
  
  $return->result = $targetview->result;
  $return->targetblock = $targetblock;
  
  return $return;
}

function dpidestinations_destination_define_target_form($form, & $form_state, $tid = NULL, $machName = NULL){
  $form['accroche'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select the destination that you wish to reorder'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="accroche-form">',
    '#suffix' => '</div>',
  );
  
  $form ['accroche']['selectsection'] = array (
    '#type' => 'select',
    '#title' => t ( 'Choose a section' ),
    '#required' => TRUE,
    '#options' => dpidestinations_api_get_all_sections_term_available_as_select_options ( FALSE ),
    '#ajax' => array (
      'callback' => 'dpidestinations_ajax_callback_define_target',
      'wrapper' => 'accroche-form',
      'method' => 'replace'
    ),
    '#default_value' => (isset ( $form_state ['values']['accroche']['selectsection'] )) ? $form_state ['values']['accroche']['selectsection'] : (isset($tid)) ? $tid : ""
  );
  
  if (isset ( $form_state ['values']['accroche']['selectsection'] )) {
    $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $form_state ['values']['accroche']['selectsection'] );
  } elseif (isset($machName) && isset($tid)) {
    $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $tid );    
  }else{
    $options = array ();
  }
  
  $form ['accroche']['selecttarget'] = array (
    '#type' => 'select',
    '#title' => t ( 'Choose a target' ),
    '#required' => FALSE,
    "#options" => $options,
    '#default_value' => (isset ( $form_state ['values']['accroche']['selecttarget'] )) ? $form_state ['values']['accroche']['selecttarget'] : (isset($machName)) ? $machName : ""
  );
  
  $form['accroche']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reorder'),
    '#id' => 'getelement',
  );
  
  return $form;
}

function dpidestinations_destination_define_target_form_submit($form, & $form_state){  
  $op = $form_state['triggering_element']['#id'];
  switch ($op){
    case 'getelement':
        drupal_goto('admin/content/reorder_targetblock/'.$form_state['values']['accroche']['selectsection'].'/'.$form_state['values']['accroche']['selecttarget']);
      break;
    default:        
      break;
  }
}

function dpidestinations_ajax_callback_define_target($form, & $form_state){
  return $form['accroche'];
}