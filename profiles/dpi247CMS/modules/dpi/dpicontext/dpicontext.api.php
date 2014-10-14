<?php

function dpicontext_api_get_all_context() {
  try {
    $return = array ();
    $query = db_select ( 'dpi_context', 'dc' )->fields ( 'dc', array (
      'label',
      'type',
      'value',
      'id' 
    ) )->execute ();
    while( $row = $query->fetchAssoc () ) {
      $row ['value'] = unserialize ( $row ['value'] );
      
      $return [] = $row;
    }
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

function dpicontext_api_get_one_context($id) {
  try {
    $return = array ();
    $query = db_select ( 'dpi_context', 'dc' )->condition ( 'id', $id, '=' )->fields ( 'dc', array (
      'label',
      'type',
      'value',
      'id' 
    ) )->execute ();
    while( $row = $query->fetchAssoc () ) {
      $row ['value'] = unserialize ( $row ['value'] );
      
      $return = $row;
    }
    return $return;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

function dpicontext_api_delete($id) {
  try {
    $result = db_delete ( 'dpi_context' )->condition ( 'id', $id, '=' )->execute ();
    return $result;
  } catch ( Exception $e ) {
    return FALSE;
  }
}

function dpicontext_api_update($form_state) {
  $result = db_update ( 'dpi_context' )->fields ( array (
    'label' => $form_state ['values'] ['dpicontext_label'],
    'type' => $form_state ['values'] ['dpicontext_types'],
    'value' => serialize ( $form_state ['values'] ['input'] ) 
  ) )->condition ( 'id', $form_state ['values'] ['store'], '=' )->execute ();
  return $result;
}

function dpicontext_api_add($form_state) {
  $result = db_insert ( 'dpi_context' )->fields ( array (
    'label' => $form_state ['values'] ['dpicontext_label'],
    'type' => $form_state ['values'] ['dpicontext_types'],
    'value' => serialize ( $form_state ['values'] ['input'] ) 
  ) )->execute ();
  return $result;
}

