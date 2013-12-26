<?php

/**
 * Preprocess the primary entry level theme.
 */
function dpidestinations_preprocess_dpidestinations_add_content_modal(&$vars) {
  // Process the list of categories.
  $group = $vars['group'];
  foreach($vars['groups'] as $group_key => $group_info){
    $class = 'panels-modal-tab-group';
    if ($group_key == $group) {
      $class .= ' active';
    }
    $url = $vars['renderer']->get_url('select-content', $vars['region'], 'none', $group_key);
    $vars['groups_array'][] = ctools_ajax_text_button($vars['groups'][$group_key]['title'], $url, $group, $class);
  }

  // Process the list of categories.
  foreach ($vars['categories'] as $key => $category_info) {
    // 'root' category is actually displayed under the categories, so
    // skip it.
    if ($key == 'root') {
      continue;
    }

    $class = 'panels-modal-add-category';
    if ($key == $vars['category']) {
      $class .= ' active';
    }

    $url = $vars['renderer']->get_url('select-content', $vars['region'], $key, $group);
    $vars['categories_array'][] = ctools_ajax_text_button($category_info['title'], $url, '', $class);
  }

  // Now render the top level buttons (aka the root category) if any.
  $vars['root_content'] = '';
  if (!empty($vars['categories']['root'])) {
    foreach ($vars['categories']['root']['content'] as $content_type) {
      $vars['columns'][$which] .= theme('panels_add_content_link', array('renderer' => $vars['renderer'], 'region' => $vars['region'], 'content_type' => $content[$title]));
    }
  }
}

/**
 * Process the panels add content modal.
 *
 * This is run here so that preprocess can make changes before links are
 * actually rendered.
 */
function dpidestinations_process_dpidestinations_add_content_modal(&$vars) {
   $content = !empty($vars['categories'][$vars['category']]['content']) ? $vars['categories'][$vars['category']]['content'] : array();
  // If no category is selected or the category is empty or our special empty
  // category render a 'header' that will appear instead of the columns.
  if (empty($vars['category']) || empty($content) || $vars['category'] == 'root') {
    $vars['header'] = t('Content options are divided by category. Please select a category from the left to proceed.');
  }
  else {
    $titles = array_keys($content);
    natcasesort($titles);

    // This will default to 2 columns in the theme definition but could be
    // changed by a preprocess. Ensure there is at least one column.
    $columns = max(1, $vars['column_count']);
    $vars['columns'] = array_fill(1, $columns, '');

    $col_size = count($titles) / $columns;
    $count = 0;
    foreach ($titles as $title) {
      $which = floor($count++ / $col_size) + 1;
      $vars['columns'][$which] .= theme('panels_add_content_link', array('renderer' => $vars['renderer'], 'region' => $vars['region'], 'content_type' => $content[$title]));
          }
  }

  $vars['messages'] = theme('status_messages');
}



/**
 * Processes variables for block-admin-display-form.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see block-admin-display.tpl.php
 * @see theme_block_admin_display()
 */
function template_preprocess_dpidestinations_page_targetview_reorder_form(&$variables) {
  // Add each block in the form to the appropriate place in the block listing.
  foreach (element_children($variables['form']['nodes']) as $i) {
    $node = &$variables['form']['nodes'][$i];
    // Set special classes needed for table drag and drop.
    $node['weight']['#attributes']['class'] = array('node-weight', 'node-weight');
    $block['weight']['#attributes']['class'] = array('block-weight', 'block-weight');

    $variables['node_listing'][$i] = new stdClass();
    $variables['node_listing'][$i]->row_class = 'node-weight';
    $variables['node_listing'][$i]->weight_select = drupal_render($node['weight']);

    $variables['node_listing'][$i]->node_title = drupal_render($node['title']);
    $variables['node_listing'][$i]->publication_date = drupal_render($node['title']);
    $variables['node_listing'][$i]->author = drupal_render($node['title']);
    $variables['node_listing'][$i]->actions = drupal_render($node['edit']).' ,'. !empty($node['delete']) ? drupal_render($node['delete']) : '';
    $variables['node_listing'][$i]->printed = FALSE;

    $variables['node_listing'][$i]->author = 'coco';
    $variables['node_listing'][$i]->publication_date = '12-04-2013';

  }
  $variables['form_submit'] = drupal_render_children($variables['form']);
}

