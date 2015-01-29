<?php 

/**
 * Implementation of hook_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function accroche_form_validate($form, &$form_state) {
//   $customContent_submission = (object) $form_state['values'];
//   $results = dpiblocks_reuse_accroche_get_all();
//   if($results != NULL) {
//     foreach($results as $record) {
//       // Validate title
//       if($customContent_submission->aid != $record->aid) {
//         if($customContent_submission->title == $record->title) {
//           form_set_error('title', t('the title already exist'));
//         }
//       }
//     }
//   }
}

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
  $form ['accroche'] = array (
    '#prefix' => '<div id="accroche-form">',
    '#suffix' => '</div>',
    "#tree" => true
  );
  
  $form ['accroche'] ['aid'] = array(
    '#type' => 'value',
    '#value' => $custom_content->aid,
  );

  $form ['accroche'] ['override_title'] = array(
    '#type' => 'value',
    '#value' => $custom_content->override_title,
  );

  $form ['accroche'] ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('title'),
    '#required' => TRUE,
    '#default_value' => $custom_content->title,
  );

  $form ['accroche'] ['link'] = array(
    '#title' => t('link'),
    '#type' => 'textfield',
    '#description' => t('url for'),
    '#required' => FALSE,
    '#default_value' => $custom_content->link,
  );

  $form ['accroche'] ['type'] = array (
    '#type' => 'select',
    '#title' => t ( 'Select type of value' ),
    '#default_value' => (isset($custom_content->type))?$custom_content->type : null,
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

  form_load_include ( $form_state, 'inc', 'dpiblocks', 'includes/custom_accroche.admin' );
  if (isset($form_state ['values'] ['accroche'] ['type']) && ! strcmp ( $form_state ['values'] ['accroche'] ['type'], 'node' )) {
    $form ['accroche'] ['node'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Enter node id' ),
      '#description' => t ( 'add ; between each node id' ),
      '#required' => FALSE,
      '#autocomplete_path' => 'package/autocomplete/input[name="node"]',
      '#default_value' => $custom_content->node
    );
  } elseif (isset($form_state ['values'] ['accroche'] ['type']) && ! strcmp ( $form_state ['values'] ['accroche'] ['type'], 'destination' )) {
    $nb = 1;

    // watchdog('debug dpi', '<pre>'.print_r($form_state,1).'</pre>');
    /* si je rebuild && si il s'agit du obuton add more */
    if (isset ( $form_state ['values'] ['accroche'] ['destination'] ) && isset ( $form_state ['triggering_element'] ) && $form_state ['triggering_element'] ["#type"] == "button") {
      $nb = count ( $form_state ['values'] ['accroche'] ['destination'] ) + 1;
    } elseif (isset ( $form_state ['values'] ['accroche'] ['destination'] )) {
      $nb = count ( $form_state ['values'] ['accroche'] ['destination'] );
    } elseif (isset($custom_content->destination)) {
      $nb = count($custom_content->destination);
    }

    /* boucle */
    for($i = 0; $i < $nb; $i ++) {
      $form ['accroche'] ['destination'] [$i] = array (
        '#prefix' => '<div id="destination-add-form" class="container-inline" >',
        '#suffix' => '</div>'
      );

      $form ['accroche'] ['destination'] [$i] ['destination_section'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a section' ),
        '#required' => TRUE,
        '#options' => dpidestinations_api_get_all_sections_term_available_as_select_options ( FALSE ),
        '#ajax' => array (
          'callback' => 'dpiblocks_ajax_callback_define_target_from_form',
          'wrapper' => 'accroche-form',
          'method' => 'replace'
        ),
        '#default_value' => (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] )) ? $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] : isset ( $conf ["destination"] [$i] ["section"] ) ? $conf ["destination"] [$i] ["section"] :  ''
      );

      if (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] )) {
        $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] );
      } elseif (isset ( $conf ["destination"] [$i] ["section"] ) && $conf ["destination"] [$i] ["section"] != "") {
        $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $conf ["destination"] [$i] ["section"] );
      } else {
        $options = array ();
      }

      $form ['accroche'] ['destination'] [$i] ['destination_target'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a section' ),
        '#required' => FALSE,
        "#options" => $options,
        '#default_value' => (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_target'] )) ? $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_target'] : isset ( $conf ["destination"] [$i] ["target"] ) ? $conf ["destination"] [$i] ["target"] : ''
      );

      $form ['accroche'] ['destination'] [$i] ['destination_nb_element'] = array (
        '#type' => 'textfield',
        '#title' => t ( 'Enter number of element that you want' ),
        '#required' => TRUE,
        '#default_value' => (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_nb_element'] )) ? $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_nb_element'] :isset ( $conf ["destination"] [$i] ["nb_element"] ) ? $conf ["destination"] [$i] ["nb_element"] :  ''
      );
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

  return $form;
}

function dpiblocks_ajax_callback_define_type_entity_form($form, $form_state) {
  return $form ['accroche'];
}

function dpiblocks_ajax_callback_define_target_from_form($form, $form_state) {
  // $i = $form_state['triggering_element']['#parents'][2];
  return $form ["accroche"];
}

function dpiblocks_ajax_callback_define_get_destination($form, $form_state) {
  return $form ['accroche'];
}

/**
 * Implementation of hook_submit()
 *
 * @param $form
 * @param $form_state
 */
function accroche_form_submit($form, &$form_state) {
  dsm('ok');
}
