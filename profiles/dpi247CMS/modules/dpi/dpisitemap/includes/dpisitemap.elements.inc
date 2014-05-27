<?php

// Default value for the tree pager. Value can be overridden with the variable
// taxonomy_manager_pager_tree_page_size.
define('DPISITEMAP_TAXONOMY_MANAGER_PAGER_TREE_PAGE_SIZE_DEFAULT', 100);

function _dpisitemap_checkboxes_process_elements($element) {
  if (count($element['#check_options']) > 0) {
    $element['#tree'] = TRUE;
    $element['#element_validate'] = array('_dpisitemap_checkboxes_validate');

    $element['#elements'] = array();
    if (!isset($element['#default_value']) || $element['#default_value'] == 0) {
      $element['#default_value'] = array();
    }
    $default_values = $element['#default_value'];
    foreach ($element['#check_options'] as $key => $choice) {
      $elem_def_values = isset($default_values[$key]) ? $default_values[$key] : array();
      if (!isset($element[$key])) {
        $element['#elements'][$key] = array();

        $element['#elements'][$key]['check'] = array(
          '#type' => 'checkbox',
          '#title' => $choice,
          '#return_value' => $key,
          '#value' => $elem_def_values['check'],
          '#attributes' => $element['#attributes'],
          '#theme' => 'dpisitemap_taxonomy_tree_checkbox',
        );

        $element['#elements'][$key]['weight_select'] = array(
          '#type' => 'select',
          '#options' => $element['#select_options'],
          '#value' => $elem_def_values['weight'],
          '#theme' => 'dpisitemap_elements_select',
        );

        $element['#elements'][$key]['freq'] = array(
          '#type' => 'select',
          '#options' => $element['#freq_options'],
          '#value' => $elem_def_values['freq'],
          '#theme' => 'dpisitemap_elements_select',
        );

        $parents = $element['#parents'];
        $parents[] = $key;
        _dpisitemap_taxonomy_manager_tree_element_set_params($parents, $element['#elements'][$key]);
      }
    }
  }

  return $element;
}

/**
 * Selected values get added to associative array term IDs - weights for easy use in submit
 *
 * @param $form
 */
function _dpisitemap_checkboxes_validate($form, &$form_state) {
  $values = array();
  if (is_array($form['#value'])) {
    foreach ($form['#value'] as $menu_name => $form_value) {
      $tmp_values = array();
      $tmp_values['check'] = isset($form_value['check']) && $form_value['check'];
      $tmp_values['weight'] = $form_value['weight_select'];
      $tmp_values['freq'] = $form_value['freq'];
      $values[$menu_name] = $tmp_values;
    }
  }
  form_set_value($form, $values, $form_state);
}

/**
 * Processes the tree form element
 *
 * This function is mainly inspirated by the taxonomy_manager_tree_process_elements function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $element
 * @return the tree element
 */
function _dpisitemap_taxonomy_tree_process_elements($element) {
  global $_taxonomy_manager_existing_ids; //TEMP: seems like this functions gets called twice in preview and cause problem because of adding the settings to js twice
  $_taxonomy_manager_existing_ids = is_array($_taxonomy_manager_existing_ids) ? $_taxonomy_manager_existing_ids : array();

  $element_defaults = array(
    '#siblings_page' => 0,
    '#pager' => FALSE,
    '#search_string' => NULL,
    '#language' => NULL,
    '#terms_to_highlight' => array(),
    '#default_value' => array(),
  );
  $element += $element_defaults;

  $module_path = drupal_get_path('module', 'dpisitemap') . '/';
  $id = drupal_clean_css_identifier(implode('-', $element['#parents']));
  $element['#id'] = $id;
  $vid = $element['#vid'];

  if (!$element['#siblings_page'] && !in_array($id, $_taxonomy_manager_existing_ids)) {
    $_taxonomy_manager_existing_ids[$id] = $id;
    drupal_add_css($module_path . 'css/taxonomy_manager.css', array('media' => 'screen'));
    drupal_add_js($module_path . 'js/tree.js');

    drupal_add_js(array('siblingsForm' => array('url' => url('admin/structure/taxonomy_manager/siblingsform'), 'modulePath' => $module_path)), 'setting');
    drupal_add_js(array('childForm' => array('url' => url('admin/structure/taxonomy_manager/childform'), 'modulePath' => $module_path)), 'setting');
    drupal_add_js(array('taxonomytree' => array(array('id' => $id, 'vid' => $vid, 'parents' => $element['#parents']))), 'setting');
  }

  if (empty($element['#operations'])) {
    $opertions_callback = 'taxonomy_manager_' . implode('_', $element['#parents']) . '_operations';
    if (function_exists($opertions_callback)) {
      $element['#operations_callback'] = $opertions_callback;
    }
  }
  if (!isset($element['#link'])) {
    $link_callback = 'taxonomy_manager_' . implode('_', $element['#parents']) . '_link';
    if (function_exists($link_callback)) {
      $element['#link_callback'] = $link_callback;
    }
  }

  $element['#elements'] = array();
  $tree = _dpisitemap_taxonomy_manager_tree_get_item($element['#vid'], $element['#parent'], $element['#pager'], $element['#siblings_page'], $element['#search_string'], $element['#language']);
  if (count($tree)) {
    if ($element['#pager'] && !($element['#parent'] || $element['#siblings_page'])) {
      $element['pager'] = array('#value' => theme('pager'));
    }

    $terms_to_expand = array();

    if (!empty($element['#language'])) {
      $element['#elements']['language'] = array('#type' => 'hidden', '#value' => $element['#language'], '#attributes' => array('class' => 'tree-lang'));
      _dpisitemap_taxonomy_manager_tree_element_set_params($element['#parents'], $element['#elements']);
    }

    if (!is_array($element['#terms_to_highlight'])) {
      $element['#terms_to_highlight'] = array($element['#terms_to_highlight']);
    }

    $index = 0;
    $select_options = $element['#select_options'] ? $element['#select_options'] : array();
    $freq_options = $element['#freq_options'] ? $element['#freq_options'] : array();
    $addcheckbox = isset($element['#add_checkbox']) ? $element['#add_checkbox'] : TRUE;
    _dpisitemap_taxonomy_manager_tree_build_form($index, $tree, $element['#elements'], $element, $element['#parents'], $element['#default_value'], $select_options, $freq_options, $addcheckbox);

    $element['#element_validate'] = array('_dpisitemap_taxonomy_tree_validate');
  }

  return $element;
}

/**
 * Selected values get added to associative array term IDs - weights for easy use in submit
 *
 * @param $form
 */
function _dpisitemap_taxonomy_tree_validate($form, &$form_state) {
  $values = array();
  if (isset($form['#value']) && is_array($form['#value'])) {
    _dpisitemap_taxonomy_tree_validate_recurse($values, $form['#value']);
  }
  form_set_value($form, $values, $form_state);
}

function _dpisitemap_taxonomy_tree_validate_recurse(&$return_values, $values) {
  foreach ($values as $id => $value) {
    $tmp_values = array();
    $tmp_values['check'] = isset($value['check']) && $value['check'];
    $tmp_values['weight'] = $value['weight_select'];
    $tmp_values['freq'] = $value['freq'];
    if (isset($value['children']) && is_array($value['children'])) {
      _dpisitemap_taxonomy_tree_validate_recurse($return_values, $value['children']);
    }
    $return_values[$id] = $tmp_values;
  }
}

/**
 * Recursive function for building nested form array
 * with checkboxes and weight forms for each term
 *
 * Nested form array are allways appended to parent-form['children']
 *
 * This function is mainly inspirated by the taxonomy_manager_tree_build_form function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $index current index in tree, start with 0
 * @param $tree of terms (generated by taxonomy_get_tree)
 * @return a form array
 */
function _dpisitemap_taxonomy_manager_tree_build_form(&$index, $tree, &$form, $root_form, $parents = array(), $default_values = array(), $select_options = array(), $freq_options = array(), $add_checkbox = TRUE) {
  $current_depth = $tree[$index]->depth;
  while ($index < count($tree) && $tree[$index]->depth >= $current_depth) {
    $term = $tree[$index];

    $attributes = array();

    $this_parents = $parents;
    $this_parents[] = $term->tid;

    $values = array_key_exists($term->tid, $default_values) ?
    $default_values[$term->tid] :
    array(
      'check' => 1,
      'weight' => 0,
      'freq' => 'daily',
    );

    if ($add_checkbox) {
      $form[$term->tid]['check'] = array(
        '#type' => 'checkbox',
        '#title' => $term->name,
        '#value' => $values['check'],
        '#theme' => 'dpisitemap_taxonomy_tree_checkbox',
      );
    }
    if (!empty($select_options)) {
      $form[$term->tid]['weight_select'] = array(
        '#type' => 'select',
        //'#title' => $term->name,
        '#options' => $select_options,
        '#value' => $values['weight'],
        '#theme' => 'dpisitemap_elements_select',
      );
    }
    if ($freq_options) {
      $form[$term->tid]['freq'] = array(
        '#type' => 'select',
        //'#title' => $term->name,
        '#options' => $freq_options,
        '#value' => $values['freq'],
        '#theme' => 'dpisitemap_elements_select',
      );
    }

    _dpisitemap_taxonomy_manager_tree_element_set_params($this_parents, $form[$term->tid]);

    $array_class = array();
    $class = _dpisitemap_taxonomy_manager_tree_term_set_class($array_class, $index, $tree, ($root_form['#expand_all'] || in_array($term->tid, $terms_to_expand)));
    if (!isset($form[$term->tid]['#attributes'])) {
      $form[$term->tid]['#attributes'] = array('class' => '');
    }
    if (!empty($class)) {
      $form[$term->tid]['#attributes']['class'] .= $class;
    }

    $index++;

    if (isset($tree[$index]->depth) && $tree[$index]->depth > $current_depth) {
      _dpisitemap_taxonomy_manager_tree_build_form($index, $tree, $form[$term->tid]['children'], $root_form, array_merge($this_parents, array('children')), $default_values, $select_options, $freq_options, $add_checkbox);
    }
  }
}

/**
 * Loads tree with terms (depending on various settings)
 *
 * This function is a copy of the _taxonomy_manager_tree_get_item function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $vid
 * @param $parent
 * @param $pager
 * @param $siblings_page
 * @return array with term elements
 */
function _dpisitemap_taxonomy_manager_tree_get_item($vid, $parent = 0, $pager = FALSE, $siblings_page = 0, $search_string = NULL, $language_code = NULL) {
  $tree = array();
  if (module_exists('i18n_taxonomy') && $language_code != "") {
    return _dpisitemap_taxonomy_manager_tree_get_translated_item($vid, $parent, $pager, $siblings_page, $search_string, $language_code);
  }
  if ($pager) {
    if ($parent || $siblings_page) {
      $start = ($siblings_page-1) * DPISITEMAP_TAXONOMY_MANAGER_PAGER_TREE_PAGE_SIZE_DEFAULT;
      $result = db_query_range("SELECT t.* FROM {taxonomy_term_data} t INNER JOIN {taxonomy_term_hierarchy} h ON t.tid = h.tid WHERE vid = :vid AND h.parent = :parent ORDER BY t.weight, t.name", $start, variable_get('taxonomy_manager_pager_tree_page_size', 100), array(':vid' => $vid, ':parent' => $parent));
    }
    else {
      $query = db_select('taxonomy_term_data', 't')->extend('PagerDefault');
      $query->fields('t');
      $table_alias = $query->join('taxonomy_term_hierarchy', 'h', 't.tid = h.tid AND h.parent = 0');
      $query->condition('t.vid', $vid)
        ->orderBy('t.weight', 'ASC')
        ->orderBy('t.name', 'ASC')
        ->limit(DPISITEMAP_TAXONOMY_MANAGER_PAGER_TREE_PAGE_SIZE_DEFAULT);

      if ($search_string) {
        $query->condition('name', '%' . db_like($search_string) . '%', 'LIKE');
      }
      $result = $query->execute();
    }
    foreach ($result as $term) {
      $term->depth = 0;
      $tree[] = $term;
    }
  }
  else {
    $tree = taxonomy_get_tree($vid, $parent);
  }
  return $tree;
}

/**
 * Loads translated tree with terms (depending on various settings)
 *
 * This function is a copy of the _taxonomy_manager_tree_get_translated_item function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $vid
 * @param $parent
 * @param $pager
 * @param $siblings_page
 * @return array with term elements
 */
function _dpisitemap_taxonomy_manager_tree_get_translated_item($vid, $parent = 0, $pager = FALSE, $siblings_page = 0, $search_string = NULL, $language_code = NULL) {
  //TODO merge with function above

  $tree = array();
  if ($language_code == "no language") {
    $language_code = ""; //get terms where no language is specified
  }
  if ($pager) {
    if ($parent || $siblings_page) {
      $start = ($siblings_page-1) * DPISITEMAP_TAXONOMY_MANAGER_PAGER_TREE_PAGE_SIZE_DEFAULT;
      $result = db_query_range("SELECT t.* FROM {taxonomy_term_data} t INNER JOIN {taxonomy_term_hierarchy} h ON t.tid = h.tid WHERE vid = :vid AND h.parent = :parent AND t.language = :language ORDER BY t.weight, t.name", $start, variable_get('taxonomy_manager_pager_tree_page_size', 50), array(':vid' => $vid, ':parent' => $parent, ':language' => $language_code));
    }
    else {
      $query = db_select('taxonomy_term_data', 't')->extend('PagerDefault');
      $query->fields('t');
      $table_alias = $query->join('taxonomy_term_hierarchy', 'h', 't.tid = h.tid AND h.parent = 0');
      $query->condition('t.vid', $vid)
        ->condition('t.language', $language_code)
        ->orderBy('t.weight', 'ASC')
        ->orderBy('t.name', 'ASC')
        ->limit(DPISITEMAP_TAXONOMY_MANAGER_PAGER_TREE_PAGE_SIZE_DEFAULT);

      if ($search_string) {
        $query->condition('name', '%' . db_like($search_string) . '%', 'LIKE');
      }
      $result = $query->execute();

    }
    foreach ($result as $term) {
      $term->depth = 0;
      $tree[] = $term;
    }
  }
  else {
    $tree = i18n_taxonomy_get_tree($vid, $language_code, $parent);
  }
  return $tree;
}

/**
 * Adds #id and #name to all form elements
 *
 * This function is a copy of the _taxonomy_manager_tree_element_set_params function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $parents
 * @param $form
 */
function _dpisitemap_taxonomy_manager_tree_element_set_params($parents, &$form) {
  foreach (element_children($form) as $field_name) {
    $field_parents = array_merge($parents, array($field_name));
    $form[$field_name]['#tree'] = TRUE;
    $form[$field_name]['#post'] = array();
    $form[$field_name]['#parents'] = $field_parents;
    $form[$field_name]['#id'] = 'edit-'. implode('-', $field_parents);
    $form[$field_name]['#name'] = array_shift($field_parents) .'['. implode('][', $field_parents) .']';
  }
}

/**
 * Calculates class type (expandable, lastExpandable) for current element
 *
 * This function is a copy of the _taxonomy_manager_tree_term_set_class function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $current_index in tree array
 * @param $tree array with terms
 */
function _dpisitemap_taxonomy_manager_tree_term_set_class(&$class, $current_index, $tree, $expand) {
  $term = $tree[$current_index];

  $next_index = ++$current_index;
  $next = isset($tree[$next_index]) ? $tree[$next_index] : NULL;

  $children = FALSE;
  if (!empty($next) && $next->depth > $term->depth) {
    $children = TRUE;
  }

  if ($children) {
    if (!empty($next->depth) && $next->depth == $term->depth) {
      $class[] = ($expand) ? 'collapsable' : 'expandable';
    }
    else {
      $class[] = ($expand) ? 'lastCollapsable' : 'lastExpandable';
    }
  }
  elseif (_dpisitemap_taxonomy_manager_tree_term_children_count($term->tid) > 0) {
    $class[] = 'has-children';
    if ($current_index == count($tree)) {
      $class[] = 'lastExpandable';
    }
    else {
      $class[] = 'expandable';
    }
  }
  elseif ((count($tree) == $current_index) || (!empty($next) && $term->depth > $next->depth)) {
    $class[] = 'last';
  }

  return implode(' ', $class);
}

/**
 * This function is a copy of the _taxonomy_manager_tree_term_children_count function from the taxonomy_manager module
 * @see : http://www.drupal.org/project/taxonomy_manager
 *
 * @param $tid
 * @return children count
 */
function _dpisitemap_taxonomy_manager_tree_term_children_count($tid) {
  static $tids = array();

  if (!isset($tids[$tid])) {
    $query = db_select('taxonomy_term_hierarchy', 'h');
    $query->condition('h.parent', $tid);
    $tids[$tid] = $query->countQuery()->execute()->fetchField();
  }

  return $tids[$tid];
}
