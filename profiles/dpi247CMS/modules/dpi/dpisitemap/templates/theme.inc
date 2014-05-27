<?php

/**
 * Theme function for root element
 *
 * @param $element
 * @return html output
 */
function theme_dpisitemap_checkboxes($variables) {
  $element = $variables['element'];

  $tree = theme('dpisitemap_checkboxes_elements', array('element' => $element['#elements']));

  return $tree;
}

function theme_dpisitemap_checkboxes_elements($variables) {
  $element = $variables['element'];

  $output = '<ul class="treeview">';
  if (is_array($element)) {
    foreach (element_children($element) as $menu_name) {
      $output .= '<li>';
      $output .= '<div class="term-line">';
      $output .= drupal_render($element[$menu_name]['check']);
      $output .= drupal_render($element[$menu_name]['weight_select']);
      $output .= drupal_render($element[$menu_name]['freq']);
      $output .= '</div>';
      $output .= '</li>';
    }
  }
  $output .= '</ul>';

  return $output;
}

/**
 * Theme function for root element
 *
 * @param $element
 * @return html output
 */
function theme_dpisitemap_taxonomy_tree($variables) {
  $element = $variables['element'];

  $tree = theme('dpisitemap_taxonomy_tree_elements', array('element' => $element['#elements']));

  $output = '<div id="'.$element['#id'].'">';
  $output .= $tree;
  $output .= '</div>';

  return $output;
}

/**
 * recursive theme function for term elements
 *
 * @param $element
 * @return html lists
 */
function theme_dpisitemap_taxonomy_tree_elements($variables) {
  $element = $variables['element'];

  $output = '<ul class="treeview">';

  if (is_array($element)) {
    foreach (element_children($element) as $tid) {
      if (is_numeric($tid)) {
        
        $output .= '<li '.drupal_attributes($element[$tid]['#attributes']) .'>';
        if (strpos($element[$tid]['#attributes']['class'], 'has-children') !== FALSE || (isset($element[$tid]['children']) && is_array($element[$tid]['children']))) {
          $output .= '<div class="hitArea"></div>';
        }
        $output .= '<div class="term-line">';
        $output .= drupal_render($element[$tid]['check']);
        $output .= drupal_render($element[$tid]['weight_select']);
        $output .= drupal_render($element[$tid]['freq']);

        if (isset($element[$tid]['weight']) && is_array($element[$tid]['weight']) && isset($element[$tid]['tid']) && is_array($element[$tid]['tid'])) {
          $output .= drupal_render($element[$tid]['weight']);
          $output .= drupal_render($element[$tid]['tid']);
        }

        $output .= '</div>';

        if (isset($element[$tid]['children']) && is_array($element[$tid]['children'])) {
          $output .= theme('dpisitemap_taxonomy_tree_elements', array('element' => $element[$tid]['children']));
        }

        $output .= '</li>';
      }
    }
  }

  $output .= '</ul>';

  if (isset($element['language'])) {
    $output .= drupal_render($element['language']);
  }

  return $output;
}

/**
 * Themes a select list
 */
function theme_dpisitemap_elements_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-select'));

  $select = '<select ';
  $select .= 'name="'. $element['#name'] .'" ';
  $select .= 'id="'. $element['#id'] .'" ' ;
  $select .= drupal_attributes($element['#attributes']) .' />';
  $select .= form_select_options($element);
  $select .= '</select>';

  return $select;
}

/**
 * Themes a checkbox
 */
function theme_dpisitemap_taxonomy_tree_checkbox($variables) {
  $element = $variables['element'];

  _form_set_class($element, array('form-checkbox'));
  $checkbox = '<input ';
  $checkbox .= 'type="checkbox" ';
  $checkbox .= 'name="'. $element['#name'] .'" ';
  $checkbox .= 'id="'. $element['#id'] .'" ' ;
  $checkbox .= 'value="'. $element['#return_value'] .'" ';
  $checkbox .= $element['#value'] ? ' checked="checked" ' : ' ';
  $checkbox .= drupal_attributes($element['#attributes']) .' />';

  return $checkbox;
}
