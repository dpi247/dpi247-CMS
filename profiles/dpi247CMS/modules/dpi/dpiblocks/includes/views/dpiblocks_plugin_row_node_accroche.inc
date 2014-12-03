<?php

/**
 * @file
 * Contains the node view row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 *
 * @ingroup views_row_plugins
 */
class dpiblocks_plugin_row_node_accroche extends views_plugin_row_node_view {

  function render($row) {
    $build;
    $view_mode = "targetblock_small";
    if(isset($row->nid)){
      $node = node_load($row->nid);
      if(isset($this->view->accroche_view_mode)){
        $view_mode = $this->view->accroche_view_mode;
      }
      if(isset($node->view)){
        unset($node->view);
        unset($node->targetview_destination);
      }
      $build = node_view ( $node, $view_mode );
    }
    return drupal_render($build);
  }
  
  function option_definition() {
    $options = parent::option_definition ();
  
    unset ( $options ['view_mode'] );
    $options ['links'] = array (
      'default' => TRUE,
      'bool' => TRUE
    );
    $options ['comments'] = array (
      'default' => FALSE,
      'bool' => TRUE  
    );
  
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    parent::options_form ( $form, $form_state );
  
    $options = $this->options_form_summary_options ();
    // We don't need the view mode since the target layout will be used
    unset ( $form ['view_mode'] );
  
    $form ['links'] = array (
      '#type' => 'checkbox',
      '#title' => t ( 'Display links' ),
      '#default_value' => $this->options ['links']
    );
  
    $form ['comments'] = array (
      '#type' => 'checkbox',
      '#title' => t ( 'Display comments' ),
      '#default_value' => $this->options ['comments']
    );
  }
  
  // simpler summary title
  function summary_title() {
    return t ( 'Settings' );
  }
  
}