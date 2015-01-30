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
function accroche_form($form, &$form_state, $custom_content, $op = 'edit') {
  module_load_include('inc', 'dpiblocks', 'dpiblocks.api');
  
  $form ['aid'] = array(
    '#type' => 'value',
    '#value' => $custom_content->aid,
    '#prefix' => '<div id="accroche-form">',
  );

  $form ['override_title'] = array(
    '#type' => 'value',
    '#value' => $custom_content->override_title,
  );

  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('title'),
    '#required' => TRUE,
    '#default_value' => $custom_content->title,
  );

  $form  ['link'] = array(
    '#title' => t('link'),
    '#type' => 'textfield',
    '#description' => t('url for'),
    '#required' => FALSE,
    '#default_value' => $custom_content->link,
  );

  $form ['type'] = array (
    '#type' => 'select',
    '#title' => t ( 'Select type of value' ),
    '#default_value' => $custom_content->layout,
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

  if ((isset($form_state ['values']['type']) && ! strcmp ( $form_state ['values']['type'], 'node' )) || ($custom_content->layout == 'node' && !isset($form_state ['values']['type']))) {
    if(isset($custom_content->layout) && $custom_content->layout == "destination"){
      $valnode = '';
    }else{
      $valnode = $custom_content->data;
    }
    $form ['node'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Enter node id' ),
      '#description' => t ( 'add ; between each node id' ),
      '#required' => FALSE,
      '#autocomplete_path' => 'package/autocomplete/input[name="node"]',
      '#default_value' => $valnode
    );
  } elseif (isset($form_state ['values']['type']) && ! strcmp ( $form_state ['values']['type'], 'destination' ) || ($custom_content->layout == 'destination' && !isset($form_state ['values']['type']))) {
    $nb = 1;
    if(!isset($form_state['values']['destination'])){
      $form_state['values']['destination'] = unserialize($custom_content->data);
    }
    // watchdog('debug dpi', '<pre>'.print_r($form_state,1).'</pre>');
    /* si je rebuild && si il s'agit du obuton add more */
    if (isset ( $form_state ['values']['destination'] ) && isset ( $form_state ['triggering_element'] ) && $form_state ['triggering_element'] ["#type"] == "button") {
      $nb = count ( $form_state ['values']['destination'] ) + 1;
    } elseif (isset ( $form_state ['values']['destination'] )) {
      $nb = count ( $form_state ['values']['destination'] );
    }
    
    /* boucle */
    $form ['form_dest']['destination'] = array(
      '#tree' => TRUE, 
    );
    
    for($i = 0; $i < $nb; $i ++) {
      $form ['form_dest']['destination'] [$i] = array (
        '#prefix' => '<div id="destination-add-form" class="container-inline" >',
        '#suffix' => '</div>',
      );
      
      $form ['form_dest']['destination'] [$i] ['destination_section'] = array (
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

      if (isset ( $form_state ['values']['destination'] [$i] ['destination_section'] )) {
        $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $form_state ['values']['destination'] [$i] ['destination_section'] );
      } else {
        $options = array ();
      }

      $form ['form_dest']['destination'] [$i] ['destination_target'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a section' ),
        '#required' => FALSE,
        "#options" => $options,
        '#default_value' => (isset ( $form_state ['values']['destination'] [$i] ['destination_target'] )) ? $form_state ['values'] ['destination'] [$i] ['destination_target'] : ''
      );

      $form ['form_dest']['destination'] [$i] ['destination_nb_element'] = array (
        '#type' => 'textfield',
        '#title' => t ( 'Enter number of element that you want' ),
        '#required' => TRUE,
        '#default_value' => (isset ( $form_state ['values']['destination'] [$i] ['destination_nb_element'] )) ? $form_state ['values'] ['destination'] [$i] ['destination_nb_element'] : ''
      );
    }

    $form ['add_more'] = array (
      '#type' => 'button',
      '#value' => 'Add more',      
      '#ajax' => array (
        'callback' => 'dpiblocks_ajax_callback_define_get_destination',
        'wrapper' => 'accroche-form',
        'method' => 'replace'
      )
    );
  }

  if(!isset($custom_content->is_new)) {
    $form ['revision'] = array(
      '#type' => 'radios',
      '#title' => t('About Revision'),
      '#options' => array(t('New revision'), t('Overwrite')),
      '#default_value' => 0,
    );
  }

  $form ['submit'] = array(
    '#type' => 'submit',    
    '#value' => t('Save'),
  );
  
  $form ['complet'] = array(
    '#markup' => t('complet all form correctly'),
    '#suffix' => '</div>',
  );
  
  return $form;
}

function dpiblocks_ajax_callback_define_type_entity_form($form, $form_state) {
  return $form;
}

function dpiblocks_ajax_callback_define_target_from_entity($form, $form_state) {
  return $form;
}

function dpiblocks_ajax_callback_define_get_destination($form, $form_state) {
  return $form;
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function accroche_form_submit($form, &$form_state) {  
  $array_val = array(
    'aid' => $form_state['values']['aid'],
    'override_title' => $form_state['values']['override_title'],
    'title' => $form_state['values']['title'],
    'link' => $form_state['values']['link'],
    'layout' => $form_state['values']['type'],
  );
  
  if($form_state['values']['type'] == "node"){
    $array_val = $array_val + array(
      'data' => $form_state['values']['node'],
    );
  }elseif ($form_state['values']['type'] == "destination"){    
    $array_val = $array_val + array(
      'data' => serialize($form_state['values']['destination']),
    );         
  }
  
  $accroche_entity = new AccrocheEntity($array_val);
  if(accroche_save($accroche_entity)){
    drupal_set_message(t('Enregistrement réussis'));
  }else{
    drupal_set_message(t('Echec de l\'enregistrement. Si le problème survient encore, veuillez contacter l\'administrateur.'), 'error');
  }
  $form_state['custom_content']=$accroche_entity;
  $form_state['redirect'] = "admin/dpi247/AccrocheCustom";
}
