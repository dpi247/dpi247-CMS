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
class dpidestinations_plugin_row_node_targetview extends views_plugin_row_node_view {

  function render($row) {
    if (isset ( $this->nodes [$row->{$this->field_alias}] )) {
      $node = $this->nodes [$row->{$this->field_alias}];
      $destination = $this->get_destination($node);
      $view_mode = 'teaser';
      $view = $this->view;
      if (isset ( $view->dpidestinations ['conf']['targetblock_rank_settings'] )) {
        $view_mode = $this->get_view_mode_by_position ($destination);
      } else {
        $view_mode = $this->get_view_mode_by_destination_layout ( $destination );
      }
      $node->view = $view;
      // @todo: Documenter le fait qu'il y ai un $node->targetview_destination
      $node->targetview_destination = $destination;
      $build = node_view ( $node, $view_mode );
      
      return drupal_render ( $build );
    }
  }

  function get_destination($node) {
    if ($destination = dpidestinations_api_get_destination_from_node_for_tid ( $node, $this->view->targetview_tid )) {
    } else {
      $index = 0;
      foreach ( $this->view->argument as $argument_name => $argument_object ) {
        if ($argument_name == "field_destinations_tid") {
          $destination = dpidestinations_api_get_destination_from_node_for_tid ( $node, $this->view->args [$index] );
        }
        $index ++;
      }
    }
    return $destination;
  }

  function get_view_mode_by_destination_layout($destination) {
    $view_mode = 'teaser';
    if (isset ( $destination ['layout'] )) {
      $view_mode = $destination ['layout'];
    }
    return $view_mode;
  }

  function get_view_mode_by_position($destination) {
    $conf = $this->view->dpidestinations ['conf'];
    $index = $this->view->row_index;
    $view_mode = $this->dpidestinations_define_targetblock_viewmode_by_rank ( $conf ['targetblock_rank_settings'], $index );
    if(!isset($view_mode)){
      $view_mode = $this->get_view_mode_by_destination_layout($destination);
    }
    return $view_mode;
  }

  private function dpidestinations_define_targetblock_viewmode_by_rank($info, $i) {
    $cpt = 0;
    foreach ( $info as $rank ) {
      if ($rank ['to'] == "" || (intval ( $rank ['from'] ) <= $i && $i <= intval ( $rank ['to'] ))) {
        return $rank ['view_modes'];
      } else {
        $cpt ++;
      }
    }
    return NULL;
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