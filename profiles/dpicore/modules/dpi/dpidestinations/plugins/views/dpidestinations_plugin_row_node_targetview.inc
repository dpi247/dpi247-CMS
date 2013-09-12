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
    if (isset($this->nodes[$row->{$this->field_alias}])) {
      $node = $this->nodes[$row->{$this->field_alias}];
      if($destination=dpidestinations_api_get_destination_from_node_for_tid($node,$this->view->targetview_tid)){
        $view_mode=$destination['layout'];
      }
      else{
        $view_mode='teaser';  
      }
      $node->view = $this->view;
      //@todo: Documenter le fait qu'il y ai un $node->targetview_destination 
      $node->targetview_destination = $destination;
      $build = node_view($node, $view_mode);

      return drupal_render($build);
    }
  }
}