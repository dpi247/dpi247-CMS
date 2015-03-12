<?php

/**
 * Implement the hook_form
 */
function dpisso_paywall_admin_settings_page_form($form, $form_state) {
  $form = array ();
  drupal_add_css ( drupal_get_path ( 'module', 'dpisso' ) . "/css/dpisso.css" );
  $parameters_access = dpisso_api_get_paywall_info ();
  $access_matrice = dpisso_get_matrice_table_form ();
  $y = dpisso_get_row_title ();
  $x = dpisso_get_header_table ();
  
  $rows = array ();
  foreach ( $access_matrice as $row_k => $row ) {
    foreach ( $row as $col_k => $col ) {
      switch ($col) {
        case - 1 :
          $rows ['r' . $row_k] ['c' . $col_k] = dpisso_get_markup ( $y [$row_k] );
          break;
        case 0 :
          $rows ['r' . $row_k] ['c' . $col_k] = dpisso_get_markup ( "N/A" );
          break;
        case 1 :
          if ($col_k == 1 && variable_get ( "dpisso_paywall_get_type_of_paywall", 0 ) == $row_k) {
            $elem_form = dpisso_get_select_element ( dpisso_create_table_of_variable ( 20 ), FALSE, ((isset($parameters_access->freemium->validityTypeCookie))? $parameters_access->freemium->validityTypeCookie : 0) );
            $active_part_form = dpisso_get_select_element ( dpisso_get_selected_type_element (), TRUE, ((isset($parameters_access->freemium->validityTypeCookie))? $parameters_access->freemium->validityTypeCookie : "months") );
            $rows ['r' . $row_k] ['c' . $col_k] = array (
              '#nb_element' => 2,
              $elem_form,
              $active_part_form 
            );
          } elseif ($col_k == 2 && variable_get ( "dpisso_paywall_get_type_of_paywall", 0 ) == $row_k) {
            $elem_form = dpisso_get_select_element ( dpisso_create_table_of_variable ( 20 ), FALSE, ((isset($parameters_access->freemium->nbViewLM))? $parameters_access->freemium->nbViewLM : 0) );
            $active_part_form = dpisso_get_select_element ( dpisso_get_selected_type_element (), TRUE, ((isset($parameters_access->freemium->validityTypeLM))? $parameters_access->freemium->validityTypeLM : "months") );
            $rows ['r' . $row_k] ['c' . $col_k] = array (
              '#nb_element' => 2,
              $elem_form,
              $active_part_form 
            );
          } else {
            $elem_form = dpisso_get_select_element ( dpisso_create_table_of_variable ( 20 ), FALSE, 0 );
            $active_part_form = dpisso_get_select_element ( dpisso_get_selected_type_element (), TRUE, 'months' );
            $rows ['r' . $row_k] ['c' . $col_k] = array (
              '#nb_element' => 2,
              $elem_form,
              $active_part_form 
            );
          }
          break;
      }
    }
    if (variable_get ( "dpisso_paywall_get_type_of_paywall", 0 ) == $row_k) {
      $rows ['r' . $row_k] ["#attributes"] = array (
        'class' => array (
          'dpi-color-inline' 
        ) 
      );
    }
  }
  
  $form ['form_wrapper'] = array (
    "#prefix" => '<div id="selectelement">',
    "#suffix" => '</div>' 
  );
  
  $form ['form_wrapper'] ['fieldset_paywall'] = array (
    '#type' => 'fieldset',
    '#title' => t ( 'Paywall' ) 
  );
  
  $form ['form_wrapper'] ['fieldset_paywall'] ['choose'] = array (
    '#type' => 'select',
    '#title' => t ( 'Select your paywall style' ),
    '#options' => $y,
    '#default_value' => variable_get ( "dpisso_paywall_get_type_of_paywall", 0 ),
    '#description' => t ( 'You need to complete the paywall politics if you choose simple or double style.' ) 
  );
  
  $form ['form_wrapper'] ['fieldset_paywall'] ['fieldset_table'] = array (
    '#type' => 'fieldset',
    '#title' => t ( 'Mode de paywall' ),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE 
  );
  
  $form ['form_wrapper'] ['fieldset_paywall'] ['fieldset_table'] ["table"] = array (
    // Theme this part of the form as a table.
    "#theme" => "formtable_form_table",
    // Pass header information to the theme function.
    "#header" => $x,
    // Rows in the form table.
    "rows" => array (
      // Make it a tree for easier traversing of the entered values on submission.
      "#tree" => TRUE,
      
      $rows ['r0'],
      $rows ['r1'],
      $rows ['r2'],
      $rows ['r3'] 
    ) 
  );
  
  /* gestion plugins */
  $form = array_merge ( $form, dpisso_paywall_admin_settings_paywallpolitic ( $form, $form_state ) );
  $form ['actions'] ['#type'] = 'actions';
  
  $form ['form_wrapper'] ['actions'] ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Save configuration' ) 
  );
  
  return $form;
}

function dpisso_create_table_of_variable($number_item) {
  $nb = array ();
  for($i = 0; $i < $number_item; $i ++) {
    $nb [$i] = $i;
  }
  return $nb;
}

function dpisso_get_matrice_table_form() {
  return array (
    array (
      - 1,
      1,
      0 
    ),
    array (
      - 1,
      1,
      1 
    ),
    array (
      - 1,
      0,
      0 
    ),
    array (
      - 1,
      0,
      0 
    ) 
  );
}

function dpisso_get_row_title() {
  return array (
    t ( 'Simple métré' ),
    t ( 'Double métré' ),
    t ( 'Soft' ),
    t ( 'Hard' ) 
  );
}

function dpisso_get_header_table() {
  return array (
    '',
    t ( 'Accès anonymes' ),
    t ( 'Accès connectés' ) 
  );
}

function dpisso_get_selected_type_element() {
  return array (
    'days' => t ( 'Jour(s)' ),
    'weeks' => t ( 'Semaine(s)' ),
    'months' => t ( 'Mois' ) 
  );
}

function dpisso_get_markup($text) {
  return array (
    '#markup' => $text 
  );
}

function dpisso_get_select_element($options, $disabled, $default_value) {
  return array (
    '#type' => 'select',
    '#options' => $options,
    "#disabled" => $disabled,
    '#default_value' => $default_value 
  );
}

/**
 * Theme callback for the form table.
 */
function theme_formtable_form_table(&$variables) {
  $form = $variables ["form"];
  $rows = $form ["rows"];
  $header = $form ["#header"];
  
  $content = array (
    "#theme" => "table",
    "#header" => $header,
    "#rows" => array () 
  );
  $number_row = variable_get ( "dpisso_paywall_get_type_of_paywall", 0 );
  foreach ( element_children ( $rows ) as $row_index ) {
    $row = array ();
    foreach ( element_children ( $rows [$row_index] ) as $col_index ) {
      if ($col_index [0] != "#" && ($var = render ( $rows [$row_index] [$col_index] )) != FALSE) {
        if($number_row==$row_index){
          $row [] = "<div class=\"dpi-inline dpi-color-inline\"> * " . $var . "</div>";
        }else{
          $row [] = "<div class=\"dpi-inline\">" . $var . "</div>";
        }
      } else {
        if ($col_index [0] != "#" && isset ( $rows [$row_index] [$col_index] ["#nb_element"] )) {
          $elemental = "";
          for($i = 0; $i < $rows [$row_index] [$col_index] ["#nb_element"]; $i ++) {
            $elemental .= render ( $rows [$row_index] [$col_index] [$i] );
          }
          if($number_row==$row_index){
            $row [] = "<div class=\"dpi-inline dpi-color-inline\"> * " . $elemental . "</div>";
          }else{
            $row [] = "<div class=\"dpi-inline\">" . $elemental . "</div>";
          }
        }
      }
    }
    $content ["#rows"] [] = $row;
  }
  return drupal_render ( $content );
}

/**
 * implementation du hook_form_subit
 */
function dpisso_paywall_admin_settings_page_form_submit($form, & $form_state) {
  /* set variable of choice */
  module_load_include('inc', 'dpisso', 'dpisso.private');
  variable_set ( "dpisso_paywall_get_type_of_paywall", $form_state ["values"] ["choose"] );
  
  /* send value */
  $define_info = dpisso_define_info_from_form ( $form_state ["values"] );
  $return  = dpisso_api_set_paywall_info($define_info);
  if($return){
    drupal_set_message("Mise à jour des informations réussie.");
  }else{
    drupal_set_message("Mise à jour des informations échouée.", 'error');
  }
  
  /* start submitting of politic plugins */
  $politic = $form_state ["values"] ["dpisso_paywall_paywallpolitics"];
  if ($politic_instance = _dpisso_get_paywall_instance ( $politic )) {
    $politic_instance->settingsform_submit ( $form, $form_state );
  }
}

function dpisso_define_info_from_form($values_form) {
  $return = array();
  switch ($values_form['choose']) {
    case 0 :
        $return = dpisso_set_array_info(1, $values_form['rows'][0]["c1"][1], intval($values_form['rows'][0]["c1"][0]), 1, "months", 0);
      break;
    case 1 :
        $return = dpisso_set_array_info(1, $values_form['rows'][1]["c1"][1], intval($values_form['rows'][1]["c1"][0]), 1, $values_form['rows'][1]["c2"][1], intval($values_form['rows'][1]["c2"][0]));
      break;
    case 2 :
        $return = dpisso_set_array_info(1, "months", 0, 1, "months", 0);
      break;
    case 3 :
        $return = dpisso_set_array_info(1, "months", 0, 1, "months", 0);
      break;
  }
  return $return;
}

function dpisso_set_array_info($validityCookie, $validityTypeCookie, $nbViewCookie, $validityLM, $validityTypeLM, $nbViewLM){
  return array(
    "validityCookie" => $validityCookie,
    "validityTypeCookie" => $validityTypeCookie,
    "nbViewCookie" => $nbViewCookie,
    "validityLM" => $validityLM,
    "validityTypeLM" => $validityTypeLM,
    "nbViewLM" => $nbViewLM,
  );
}

/**
 * Generate the specific paywall settings form.
 */
function dpisso_paywall_admin_settings_paywallpolitic($form, $form_state) {
  module_load_include('inc', 'dpisso', 'dpisso.private');
  // get the list of all declared ctools 'paywallpolitic' pluggin
  ctools_include ( 'plugins' );
  $politics = ctools_get_plugins ( 'dpisso', 'paywallpolitic' );
  $politic_options = array ();
  
  // Build a options array.
  $default_politic = FALSE;
  foreach ( $politics as $id => $politic ) {
    if (! $default_politic)
      $default_politic = $id;
    $politic_options [$id] = $politic ['label'];
  }
  
  // If no politics declared (no 'paywallpolitic' pluggin active, we quit.
  if (empty ( $politic_options )) {
    $form ['message'] = array (
      '#markup' => t ( 'Sorry, there is no paywall politic plugins available.' ) 
    );
    return $form;
  }
  
  // Getting the right current politic (pluggin)
  if (isset ( $form_state ["triggering_element"] )) {
    // We triggered the ajax.
    $politic = $form_state ['triggering_element'] ['#value'];
  } else {
    // We just build the form, get the current politic or the default one
    $politic = dpi_variable_get ( 'dpisso_paywall_paywallpolitics', $default_politic );
  }
  
  $form ['form_wrapper'] ['plugins'] = array (
    '#type' => 'fieldset',
    '#title' => t ( 'Plugins' ),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#states' => array (
      'visible' => array (
        ':input[name="choose"]' => array (
          array (
            'value' => 0 
          ),
          array (
            'value' => 1 
          ) 
        ) 
      ) 
    ) 
  );
  
  // Build a select options list from available 'paywallpolitic' pluggins.
  $form ['form_wrapper'] ['plugins'] ['dpisso_paywall_paywallpolitics'] = array (
    '#type' => 'select',
    '#title' => t ( 'Please choose a paywall politic' ),
    '#description' => t ( " Paywall politics determine whenever a ressource (url) will count as a paid ressource or not." ),
    '#options' => $politic_options,
    '#default_value' => dpi_variable_get ( 'dpisso_paywall_paywallpolitics', null ),
    '#ajax' => array (
      'callback' => 'dpisso_paywall_admin_settings_paywallpolitic_callback',
      'wrapper' => 'dpisso_paywall_paywallpolitics_settings_div',
      'method' => 'replace',
      'effect' => 'fade' 
    ) 
  );
  
  // Fieldset for politic ctools pluggin settings.
  $form ['form_wrapper'] ['plugins'] ['dpisso_paywall_paywallpolitics_settings'] = array (
    '#type' => 'fieldset',
    // '#title' => t('Paywall politic settings for: '.$politic_options[$politic]),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="dpisso_paywall_paywallpolitics_settings_div">',
    '#suffix' => '</div>' 
  );
  
  if ($politic_instance = _dpisso_get_paywall_instance ( $politic )) {
    
    // Call the getdefaultvalues method to get the pluggins default values.
    $default_values = $politic_instance->settingsform_getdefaultvalues ( $politic );
    
    // Call the settingsform method to get the pluggins settings
    $politic_settings = $politic_instance->settingsform ( $default_values );
    // We try to put default values on settings, in case the settingsform did not dit it.
    foreach ( $politic_settings as $k => $setting ) {
      $politic_settings [$k] ["#default_value"] = (isset ( $default_values [$k] )) ? $default_values [$k] : false;
    }
    // Prepare the form.
    $form ['form_wrapper'] ['plugins'] ['dpisso_paywall_paywallpolitics_settings'] [] = $politic_settings;
  }
  return $form;
}

/**
 * The call back function for the Ajax select form element: dpisso_paywall_paywallpolitics
 *
 * @param Array $form
 *          The complete settings form
 * @param Array $form_state
 *          The complete settings form state
 *          
 * @return Array $form
 *         The mofified element.
 */
function dpisso_paywall_admin_settings_paywallpolitic_callback($form, $form_state) {
  return $form ['form_wrapper'] ['plugins'] ['dpisso_paywall_paywallpolitics_settings'];
}