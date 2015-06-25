<?php

/**
 * @file
 * Scald link Module
 */

/**
 * Implements hook_scald_add_form().
 */
function dpiscald_link_scald_add_form(&$form, &$form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Titre'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
  
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Media URL'),
    '#element_validate' => array('dpiscald_link_validate_id'),
    '#default_value' => '',
    '#maxlength' => 1000
  );
}

function dpiscald_link_scald_fetch($atom, $type){
  $atom->omit_legend = TRUE;
}

/**
 * Implements hook_scald_add_form_fill().
 */
function dpiscald_link_scald_add_form_fill(&$atoms, $form, $form_state) {  
  $atom = is_array($atoms) ? reset($atoms) : $atoms;
  $atom->base_id = md5($url);
  $atom->title = $form_state['values']['title'];
  $atom->data['url'] = $form_state['values']['url'];
  $atom->scald_thumbnail[LANGUAGE_NONE][0] = (array) $file; 
}

/**
 * Implements hook_scald_prerender().
 */
function dpiscald_link_scald_prerender($atom, $context, $options, $mode) {
  if ($mode == 'atom') {
    if ($context != 'sdl_library_item') {
      $atom->rendered->player = theme('dpiscald_link_player',
      array('vars' =>
        array(
          'url'      => $atom->data['url'],
          'title'     => $atom->title,
        ),
        'atom' => $atom,
      )
    );
    }
  }
}

/**
 * Form element validation handler for media url.
 */
function dpiscald_link_validate_id($element, &$form_state) {
  $url = $form_state['values']['url'];
  $id = md5($url);
  if (!valid_url($url, TRUE)) {
    form_error($element, t('Invalid media url, you must specify an absolute url.'));
  }
}

/**
 * Implements hook_theme().
 */
function dpiscald_link_theme() {
  return array(
    'dpiscald_link_player' => array(
      'variables' => array('vars' => NULL, 'atom' => NULL),
      'template' => 'providers/link/dpiscald_link_player'
    ),
  );
}

/**
 * Implémentation dpiscald_provider_parse_id()
 */
function dpiscald_link_parse_id(){
  //obligatoire pour l'import
}

/**
 * Implémentation dpiscald_provider_get_import_info()
 */
function dpiscald_link_get_import_info($this, $entity, $wrapper, $url){
  try {
    $entity->data['url'] = $url;
    $entity->data['orig_identifier'] = $url;
  } catch (Exception $e) {
    dpi_log('dpiscald', "import", "Impossible d'importer le media Embedly : " . $url);
  }
}