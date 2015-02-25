<?php

/**
 * Callback to get form element
 * @return array
 */
function dpipackagegenerator_generate_complementary_element() {
  return drupal_get_form ( 'dpipackagegenerator_get_process_form' );
}

/**
 * Form to get information about
 *
 * @param array $form          
 * @param array $form_state          
 * @return array
 */
function dpipackagegenerator_get_process_form($form, & $form_state) {
  $options = array (
    'embededdobjects' => array (
      'title' => 'Embededdobjects',
      'number' => array (
        'data' => array (
          '#type' => 'textfield',
          '#default_value' => '',
          '#title' => 'number',
          '#title_display' => 'invisible',
          '#name' => 'number[embededdobjects]' 
        ) 
      ) 
    ),
    'destination' => array (
      'title' => 'Destination',
      'number' => array (
        'data' => array (
          '#type' => 'textfield',
          '#default_value' => '',
          '#title' => 'number',
          '#title_display' => 'invisible',
          '#name' => 'number[destination]' 
        ) 
      ) 
    ),
    'links_list' => array (
      'title' => 'Link list',
      'number' => array (
        'data' => array (
          '#type' => 'textfield',
          '#default_value' => '',
          '#title' => 'number',
          '#title_display' => 'invisible',
          '#name' => 'number[links_list]' 
        ) 
      ) 
    ) 
  );
  
  $header = array (
    'title' => t ( 'Name' ),
    'number' => t ( 'Numbers of element' ) 
  );
  
  $form ['table'] = array (
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t ( 'No content available.' ) 
  );
  
  $form ['number'] = array (
    '#type' => 'value' 
  );
  
  $form ['element'] = array (
    '#type' => 'value' 
  );
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => 'Generate' 
  );
  
  return $form;
}

function dpipackagegenerator_get_process_form_validate($form, & $form_state) {
  if (isset ( $form_state ['values'] ) && isset ( $form_state ['values'] ['table'] ) && isset ( $form_state ['values'] ['number'] ) && isset ( $form_state ['values'] ['element'] )) {
    $tmp_key = dpipackagegenerator_get_table_selection ( $form_state ['values'] ['table'] );
    if (! empty ( $tmp_key )) {
      foreach ( $tmp_key as $k => $v ) {
        if (! isset ( $form_state ['values'] ['number'] [$v] ) || ! is_numeric ( $form_state ['values'] ['number'] [$v] ) || intval ( $form_state ['values'] ['number'] [$v] ) <= 0) {
          form_set_error ( 'table', t ( 'Veuillez compléter tous les champs du tableau et entrer des nombres supérieurs à 0' ) );
        }
      }
    } else {
      form_set_error ( 'table', 'Select an element' );
    }
  } else {
    form_set_error ( 'table', 'Table malfunction' );
  }
}

function dpipackagegenerator_get_process_form_submit($form, & $form_state) {
  $tmp_key = dpipackagegenerator_get_table_selection ( $form_state ['values'] ['table'] );
  foreach ( $tmp_key as $v ) {
    if (! isset ( $form_state ['values'] ['element'] [$v] )) {
      /* taxonomy element */
      dpipackagegenerator_generate_taxonomy_term ( $v, intval ( $form_state ['values'] ['number'] [$v] ) );
    } else {
      /* special element */
    }
  }
}

function dpipackagegenerator_get_table_selection($array_val) {
  $tmp_key = array ();
  foreach ( $array_val as $tab ) {
    if ($tab) {
      $tmp_key [$tab] = $tab;
    }
  }
  return $tmp_key;
}

function dpipackagegenerator_generate_taxonomy_term($term, $number) {
  $php_error = array ();
  switch ($term) {
    case 'destination' :
      for($i = 0; $i < $number; $i ++) {
        try {
          dpipackagegenerator_create_section_taxonomy ();
        } catch ( Exception $e ) {
          $php_error [] = $e->getMessage ();
        }
      }
      break;
    case 'embededdobjects' :
      $atoms = entity_load( 'scald_atom', array(), array('type' => 'image'));
      if (isset ( $atoms )) {
        $atom = current ( $atoms );
        for($i = 0; $i < $number; $i ++) {
          $atom = clone $atom;
          unset($atom->sid);
          try {
            scald_atom_save ( $atom );
          } catch ( Exception $e ) {
            $php_error [] = $e->getMessage ();
          }
        }
      } else {
        drupal_set_message ( t ( 'Veuillez générer un noeud de type image afin de le dupliquer !' ), "error" );
      }
      break;
    case 'links_list':
      $atoms = entity_load( 'scald_atom', array(), array('type' => 'links_list'));
      if (isset ( $atoms )) {
        $atom = current ( $atoms );
        for($i = 0; $i < $number; $i ++) {
          $atom = clone $atom;
          unset($atom->sid);
          try {
            scald_atom_save ( $atom );
          } catch ( Exception $e ) {
            $php_error [] = $e->getMessage ();
          }
        }
      } else {
        drupal_set_message ( t ( 'Veuillez générer un noeud de type image afin de le dupliquer !' ), "error" );
      }
      break;
  }
  
  if (! empty ( $php_error )) {
    foreach ( $php_error as $msg ) {
      drupal_set_message ( 'Error : ' . $msg, 'error' );
    }
  } else {
    drupal_set_message ( 'Ajout ' . $term . ' réussis' );
  }
}

function dpipackagegenerator_create_section_taxonomy() {
  $dest_voc = 'sections';
  $dest_vid = _dpidestinations_variable_get ( 'dpidestinations_section_vid', 0 );
  if ($dest_vid) {
    $base_term = new stdClass ();
    $base_term->vid = $dest_vid;
    $base_term->vocabulary_machine_name = $dest_voc;
    $copied_term = clone $base_term;
    $copied_term->name = generateRandomString ( 'section', 5 );
    taxonomy_term_save ( $copied_term );
  }
}

function dpipackagegenerator_create_embeddedobject_element($atom) {  
}

function generateRandomString($prefix = "", $length = 10) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $charactersLength = strlen ( $characters );
  $randomString = $prefix . ' ';
  for($q = 0; $q < $length; $q ++) {
    $randomString .= $characters [rand ( 0, $charactersLength - 1 )];
  }
  return $randomString;
}

