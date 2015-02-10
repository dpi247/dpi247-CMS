<?php 

/**
 * Implementation of hook_form()
 *
 * @param $form
 * @param $form_state
 * @param $custom_content
 *
 * @return array
 */
function accroche_form($form, &$form_state, $custom_content = null, $op = 'edit') {
  module_load_include('inc', 'dpiblocks', 'dpiblocks.api');

  $form ['accroche'] = array(
    '#prefix' => '<div id="accroche-form">',
    '#suffix' => '</div>',
  );
  
  $form ['accroche'] ['aid'] = array(
    '#type' => 'hidden',
    '#value' => (isset($custom_content->aid))? $custom_content->aid : null,
    '#default_value' => (isset($custom_content->aid))? $custom_content->aid : null,
  );   

  $form ['accroche'] ['override_title'] = array(
    '#type' => 'value',
    '#value' => (isset($custom_content->override_title))? $custom_content->override_title : null,
  );

  $form ['accroche'] ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('title'),
    '#required' => TRUE,
    '#default_value' => (isset($custom_content->title)) ? $custom_content->title : null,
  );
  
  $form ['accroche'] ['machine_name'] = array(
    '#title' => t ( 'machine name' ),
    '#type' => 'machine_name',
    '#default_value' => (isset($custom_content->machine_name))? $custom_content->machine_name : null,
    '#size' => 30,
    '#maxlength' => 50,
    '#machine_name' => array(
      'exists' => '_dpiblocks_validate_machine_name',
      'source' => array('accroche','title'),
    ),
    '#description' => t ( 'A unique name to construct the machine name of view. It must only contain lowercase letters, numbers and hyphens.' ),
    '#disabled' => isset($custom_content->machine_name) && $custom_content->machine_name != "",
  );  

  $form ['accroche'] ['link'] = array(
    '#title' => t('link'),
    '#type' => 'textfield',
    '#description' => t('url for'),
    '#required' => FALSE,
    '#default_value' => (isset($custom_content->link)) ? $custom_content->link : null,
  );
  
  $layouts = variable_get("dpiblocks_accroche_settings", array());
  $infos = array();
  foreach ($layouts as $k => $el){
    $infos[$k] = $el[0];    
  }
  $form ['accroche'] ['layout'] = array (
    '#type' => 'select',
    '#title' => t ( 'Choose layout value' ),
    '#default_value' => (isset($custom_content->layout)) ? $custom_content->layout : null,
    '#options' => $infos,
    '#description' => t ( 'Choose layout render' ),    
  );

  $form ['accroche'] ['type'] = array (
    '#type' => 'select',
    '#title' => t ( 'Select type of value' ),
    '#default_value' => (isset($custom_content->type)) ? $custom_content->type : null,
    '#options' => array (
      '' => t ( 'Choose a node selection method' ),
      'node' => t ( 'Node' ),
      'destination' => t ( 'Destination' )
    ),
    '#description' => t ( 'Choose your type of informations' ),
    '#ajax' => array (
      'callback' => 'dpiblocks_ajax_callback_define_type_entity_form',
      'wrapper' => 'accroche-form',
      'method' => 'replace'
    )
  );
  
  form_load_include($form_state, 'inc', 'dpiblocks', 'includes/custom_accroche.admin');
  if ((isset($form_state ['values']['type']) && ! strcmp ( $form_state ['values']['type'], 'node' )) || (isset($custom_content->type) && $custom_content->type == 'node' && !isset($form_state ['values']['type']))) {
    $nb = 1;
    if(!isset($form_state['values']['node'])){
      $form_state['values']['node'] = unserialize($custom_content->data);
    }

    /* si je rebuild && si il s'agit du bouton add more */
    if (isset ( $form_state ['values']['node'] ) && isset ( $form_state ['triggering_element'] ) && $form_state ['triggering_element'] ["#type"] == "button" && strstr($form_state ['triggering_element'] ["#id"], 'delete') === FALSE) {
      $nb = count ( $form_state ['values']['node'] ) + 1;
    } elseif (strstr($form_state ['triggering_element'] ["#id"], 'delete') !== FALSE){
      $nb = count ( $form_state ['values']['node'] ) - 1;
      $id = $form_state ['triggering_element'] ['#parents'] [1];
      $elements = $form_state['values']['node'];
      unset($elements[$id]);
      $form_state['values']['node'] = array();
      foreach($elements as $element){
        $form_state['values']['node'][] = $element;
      }
    }elseif (isset ( $form_state ['values']['node'] )) {
      $nb = count ( $form_state ['values']['node'] );
    } 
    /* boucle */
    $form ['accroche']['node'] = array(
      '#tree' => TRUE,      
    );
    
    for($i = 0; $i < $nb; $i ++) {
      $form ["accroche"] ['node'] [$i] = array(
        '#prefix' => '<div class="container-inline" >',
        '#suffix' => '</div>',
      );
      $form ['accroche'] ['node'] [$i]["node"] = array (
        '#title' => t ( 'Enter node title' ),
        '#type' => 'textfield',
        '#required' => FALSE,                
        '#autocomplete_path' => 'package/autocomplete/input[name="node"]',
        '#default_value' => (isset($form_state ['values']['node'][$i]["node"]))? $form_state ['values']['node'][$i]["node"] : "",
      );
      
      if(isset($form_state ['values']['node'][$i]["node"]) && $form_state ['values']['node'][$i]["node"]!=""){
        $form ['accroche'] ['node'] [$i]["node"] = $form ['accroche'] ['node'] [$i]["node"] + array(
          '#value' => $form_state ['values']['node'][$i]["node"],
        );
      }
      
      if($i>0 || $nb >1){
        $form ['accroche'] ['node'] [$i]["delete"] = array (
          '#type' => 'button',
          '#value' => t("Delete"),
          '#name' => 'delete_'.$i,
          '#id' => 'delete_'.$i,
          '#ajax' => array (
            'callback' => 'dpiblocks_ajax_callback_define_del_element',
            'wrapper' => 'accroche-form',
            'method' => 'replace'
          )
        );
      }
    }
    
    $form ['accroche'] ['add_more'] = array (
      '#type' => 'button',
      '#value' => 'Add more',      
      '#ajax' => array (
        'callback' => 'dpiblocks_ajax_callback_define_get_destination',
        'wrapper' => 'accroche-form',
        'method' => 'replace'
      )
    );
    
    
  } elseif (isset($form_state ['values']['type']) && ! strcmp ( $form_state ['values']['type'], 'destination' ) || ($custom_content->type == 'destination' && !isset($form_state ['values']['type']))) {
    $nb = 1;
    if(!isset($form_state['values']['destination'])){
      $form_state['values']['destination'] = unserialize($custom_content->data);
    }
        
    /* si je rebuild && si il s'agit du bouton add more */
    if (isset ( $form_state ['values']['destination'] ) && isset ( $form_state ['triggering_element'] ) && $form_state ['triggering_element'] ["#type"] == "button" && strstr($form_state ['triggering_element'] ["#id"], 'delete') === FALSE) {
      $nb = count ( $form_state ['values']['destination'] ) + 1;
    } elseif (strstr($form_state ['triggering_element'] ["#id"], 'delete') !== FALSE){
      $nb = count ( $form_state ['values']['destination'] ) - 1;
      $id = $form_state ['triggering_element'] ['#parents'] [1];
      $elements = $form_state['values']['destination'];
      unset($elements[$id]);
      $form_state['values']['destination'] = array();
      foreach($elements as $element){
        $form_state['values']['destination'][] = $element;
      }
    } elseif (isset ( $form_state ['values']['destination'] )) {
      $nb = count ( $form_state ['values']['destination'] );
    }
    
    $form ['accroche'] ['nb_max_elem'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Number of max elements' ),
      '#required' => FALSE,
      '#default_value' => $custom_content->nb_max_value,
    );
    
    /* boucle */
    $form ['accroche']['destination'] = array(
      '#tree' => TRUE, 
    );
    for($i = 0; $i < $nb; $i ++) {
      $form ['accroche'] ['destination'] [$i] = array (
        '#prefix' => '<div id="destination-add-form" class="container-inline" >',
        '#suffix' => '</div>',
      );
      
      $form ['accroche'] ['destination'] [$i] ['destination_section'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a section' ),
        '#required' => TRUE,
        '#options' => dpidestinations_api_get_all_sections_term_available_as_select_options ( FALSE ),
        '#ajax' => array (
          'callback' => 'dpiblocks_ajax_callback_define_target_from_entity',
          'wrapper' => 'accroche-form',
          'method' => 'replace'
        ),
        '#default_value' => (isset ( $form_state ['values']['destination'] [$i] ['destination_section'] )) ? $form_state ['values']['destination'] [$i] ['destination_section'] : ''
      );
      
      if(isset($form_state ['values']['destination'][$i]["destination_section"]) && $form_state ['values']['destination'][$i]["destination_section"]!=""){
        $form ['accroche'] ['destination'] [$i]["destination_section"] = $form ['accroche'] ['destination'] [$i]["destination_section"] + array(
          '#value' => $form_state ['values']['destination'][$i]["destination_section"],
        );
      }

      if (isset ( $form_state ['values']['destination'] [$i] ['destination_section'] )) {
        $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $form_state ['values']['destination'] [$i] ['destination_section'] );
      } else {
        $options = array ();
      }

      $form ['accroche'] ['destination'] [$i] ['destination_target'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a target' ),
        '#required' => FALSE,
        "#options" => $options,
        '#default_value' => (isset ( $form_state ['values']['destination'] [$i] ['destination_target'] )) ? $form_state ['values'] ['destination'] [$i] ['destination_target'] : ''
      );
      
      if($i>0 || $nb >1){
        $form ['accroche'] ['destination'] [$i]["delete"] = array (
          '#type' => 'button',
          '#value' => t("Delete"),
          '#name' => 'delete_'.$i,
          '#id' => 'delete_'.$i,
          '#ajax' => array (
            'callback' => 'dpiblocks_ajax_callback_define_del_element',
            'wrapper' => 'accroche-form',
            'method' => 'replace'
          )
        );
      }
    }

    $form ['accroche'] ['add_more'] = array (
      '#type' => 'button',
      '#value' => 'Add more',      
      '#ajax' => array (
        'callback' => 'dpiblocks_ajax_callback_define_get_destination',
        'wrapper' => 'accroche-form',
        'method' => 'replace'
      )
    );
    
    if($nb>1){
      $mixing = array(
        'followed' => t('Followed'),
        'alternate' => t('Alternate'),
        'random' => t('Random'),
        'custom' => t('Define custom schema'),
      );
      $defaultval = (isset ( $custom_content->mixing )) ? $custom_content->mixing : '';
      if(strstr($custom_content->mixing, 'custom')!==FALSE){
        $el = explode('_', $custom_content->mixing);
        $defaultval = isset($el[0]) ? $el[0] : '';
        $specialschema = isset($el[1]) ? $el[1] : '';
      }
      $form ['accroche'] ['mixing'] = array (
        '#type' => 'select',
        '#title' => t ( 'Schema used for mixing the feeds ' ),
        '#required' => FALSE,
        "#options" => $mixing,
        '#default_value' => $defaultval,
        '#ajax' => array (
          'callback' => 'dpiblocks_ajax_callback_define_get_destination',
          'wrapper' => 'accroche-form',
          'method' => 'replace'
        )
      );
      if($form_state ['values']['mixing'] == "custom" || (!isset($form_state ['values']['mixing']) && $defaultval == 'custom')){
        $form ['accroche'] ['schema'] = array (
          '#type' => 'textfield',
          '#title' => t ( 'Define schema that you want to use' ),
          "#description" => t('Use line that you want to use and separate every number by ",". Example : 1,2,1,3,2,1. If you use this way the number of element cannot be used. The first line is 1.'),          
          '#default_value' => (isset ( $specialschema )) ? $specialschema : '',
        );
      }
      
    }
  }

  if(!isset($custom_content->is_new)) {
    $form ['accroche'] ['revision'] = array(
      '#type' => 'radios',
      '#title' => t('About Revision'),
      '#options' => array(t('New revision'), t('Overwrite')),
      '#default_value' => 0,
    );
  }
  
  $form ['accroche'] ['submit'] = array(
    '#type' => 'submit',    
    '#value' => t('Save'),
  );
  
  field_attach_form('accroche', $custom_content, $form, $form_state);

  return $form;
}

function dpiblocks_ajax_callback_define_type_entity_form($form, $form_state) {
  return $form['accroche'];
}

function dpiblocks_ajax_callback_define_target_from_entity($form, $form_state) {
  return $form['accroche'];
}

function dpiblocks_ajax_callback_define_get_destination($form, $form_state) {
  return $form['accroche'];
}

function dpiblocks_ajax_callback_define_del_element($form,& $form_state) {  
  return $form['accroche'];
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function accroche_form_submit($form, &$form_state) {
  if(isset($form_state['values']['revision'])) {
    $is_revision = $form_state['values']['revision'];
  }
  else {
    $is_revision = FALSE;
  }
  unset($form_state['values']['revision']);
  
  if(!isset($form_state['values']['aid']) && isset($form_state['input']['aid']) && $form_state['input']['aid']!=""){
    $form_state['values']['aid'] = $form_state['input']['aid'];
  }
  $array_val = array(
    'aid' => $form_state['values']['aid'],
    'machine_name' => $form_state['values']['machine_name'],
    'override_title' => $form_state['values']['override_title'],
    'title' => $form_state['values']['title'],
    'link' => $form_state['values']['link'],
    'mixing' => (isset($form_state['values']['schema'])) ? $form_state['values']['mixing'].'_'.$form_state['values']['schema'] : $form_state['values']['mixing'],
    'nb_max_value' => (isset($form_state['values']['nb_max_elem']))? $form_state['values']['nb_max_elem'] : count(explode(';', $form_state['values']['node']))-1,
    'layout' => $form_state['values']['layout'],
    'type' => $form_state['values']['type'],    
    'data' => serialize($form_state['values'][$form_state['values']['type']]),
  );
  //hash_cache_data
  if($form_state['values']['type'] == "node"){    
    $str = ";";
    if(is_array($form_state['values']['node'])){    
      foreach($form_state['values']['node'] as $el){
        preg_match("/.*?\((.*?)\)/U", $el["node"], $matches);
        if(!empty($matches) && isset($matches[1])){
          $str .= $matches[1].';';
        }
      }
    }
    $array_val = $array_val + array(
      'hash_cache_data' => $str,
    );
  }elseif ($form_state['values']['type'] == "destination"){      
    if(is_array($form_state['values']['destination'])){
      $string_tmp = "";
      foreach($form_state['values']['destination'] as $dest){
        $string_tmp .= '|'.$dest['destination_section']."_".$dest['destination_target'].'|';
      }
      $array_val = $array_val + array(
        'hash_cache_data' => $string_tmp,
      );
    }    
  }  
  
  $accroche_entity = new AccrocheEntity($array_val);
  $accroche_entity->is_new = FALSE;
  $accroche_entity->is_new_revision = !$is_revision;
  
  // Overwrite the current revision
  if($is_revision) {
    $entity = accroche_load($accroche_entity->aid);
    $accroche_entity->revision_id = $entity->revision_id;
  }
    
  // Set the default revision
  entity_revision_set_default('accroche', $accroche_entity);
  
  accroche_save($accroche_entity);
  $form_state['accroche']=$accroche_entity;
  $form_state['redirect'] = "admin/dpi247/AccrocheCustom";
}

function _dpiblocks_validate_machine_name($element, $elements, $form_state){
  module_load_include('inc', 'dpiblocks', 'plugins/content_types/accroche');
  $entity = _dpiblocks_accroche_get_entity_by_subtype($element);
  if(isset($entity) && !empty((array) $entity)){
    return TRUE;
  }
  return FALSE;
}