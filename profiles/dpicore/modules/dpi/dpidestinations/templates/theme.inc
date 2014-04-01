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
      $vars['columns'][$content['name']] .= theme('panels_add_content_link', array('renderer' => $vars['renderer'], 'region' => $vars['region'], 'content_type' => $content_type));
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






/**
 * Theme the 'placeholder' pane, which is shown on an active IPE when no panes
 * live in that region.
 *
 * @param string $region_id
 * @param string $region_title
 */
function theme_dpidestinations_ipe_placeholder_pane($vars) {
  $region_id = $vars['region_id'];
  $region_title = $vars['region_title'];

  $output = '<div class="dpidestinations-ipe-placeholder-content">';
  $output .= "<h3>$region_title</h3>";
  $output .= '</div>';
  return $output;
}

function template_preprocess_dpidestinations_ipe_pane_wrapper(&$vars) {
  $pane = $vars['pane'];
  $display = $vars['display'];
  $renderer = $vars['renderer'];

  $content_type = ctools_get_content_type($pane->type);
  $subtype = ctools_content_get_subtype($content_type, $pane->subtype);
  $vars['links'] = array();

  if (ctools_content_editable($content_type, $subtype, $pane->configuration)) {
    $vars['links']['edit'] = array(
      'title' => isset($content_type['edit text']) ? '<span>' . $content_type['edit text'] . '</span>' : '<span>' . t('Settings') . '</span>',
      'href' => $renderer->get_url('edit-pane', $pane->pid),
      'html' => TRUE,
      'attributes' => array(
        'class' => array('ctools-use-modal', 'dpidestinations-ipe-hide-bar'),
        'title' => isset($content_type['edit text']) ? $content_type['edit text'] : t('Settings'),
        // 'id' => "pane-edit-panel-pane-$pane->pid",
      ),
    );
  }

  // Add option to configure style in IPE
  if (user_access('administer panels styles')) {
    $vars['links']['style'] = array(
      'title' => '<span>' . t('Style') . '</span>',
      'href' => $renderer->get_url('style-type', 'pane', $pane->pid),
      'html' => TRUE,
      'attributes' => array(
        'class' => array('ctools-use-modal', 'dpidestinations-ipe-hide-bar'),
        'title' => t('Style'),
      ),
    );
  }

  // Deleting is managed entirely in the js; this is just an attachment point
  // for it
  $vars['links']['delete'] = array(
    'title' => '<span>' . t('Delete') . '</span>',
    'href' => '#',
    'html' => TRUE,
    'attributes' => array(
      'class' => 'pane-delete',
      'id' => "pane-delete-panel-pane-$pane->pid",
      'title' => t('Delete'),
    ),
  );

  $context = array(
    'pane' => $pane,
    'display' => $display,
    'renderer' => $renderer
  );
  drupal_alter('dpidestinations_ipe_pane_links', $vars['links'], $context);

}

function theme_dpidestinations_ipe_pane_wrapper($vars) {
  $output = $vars['output'];
  $pane = $vars['pane'];

  $attributes = array(
    'class' => 'dpidestinations-ipe-linkbar',
  );

  $links = theme('links', array('links' => $vars['links'], 'attributes' => $attributes));

  if (!empty($pane->locks['type']) && $pane->locks['type'] == 'immovable') {
    $links = '<div class="dpidestinations-ipe-dragbar dpidestinations-ipe-nodraghandle clearfix">' . $links . '</div>';
  }
  else {
    $links = '<div class="dpidestinations-ipe-dragbar dpidestinations-ipe-draghandle clearfix">' . $links . '<span class="dpidestinations-ipe-draghandle-icon"><span class="dpidestinations-ipe-draghandle-icon-inner"></span></span></div>';
  }

  $handlebar = '<div class="dpidestinations-ipe-handlebar-wrapper dpidestinations-ipe-on">' . $links . '</div>';

  return $handlebar . $output;
}

function theme_dpidestinations_ipe_region_wrapper($vars) {
  return $vars['controls'] . $vars['output'];
}

function template_preprocess_dpidestinations_ipe_add_pane_button(&$vars) {
  $region_id = $vars['region_id'];
  $display = $vars['display'];
  $renderer = $vars['renderer'];
  $vars['links'] = '';

  // Add option to configure style in IPE
  if (user_access('administer panels styles')) {
    $vars['links']['style'] = array(
      'title' => '<span>' . t('Region style') . '</span>',
      'href' => $renderer->get_url('style-type', 'region', $region_id),
      'html' => TRUE,
      'attributes' => array(
        'class' => array('ctools-use-modal', 'dpidestinations-ipe-hide-bar', 'style'),
        'title' => t('Region style'),
      ),
    );
  }

  // Add option to add items in the IPE
  $vars['links']['add-pane'] = array(
    'title' => '<span>' . t('Add new pane') . '</span>',
    'href' => $renderer->get_url('select-content', $region_id),
    'attributes' => array(
      'class' => array('ctools-use-modal', 'add', 'dpidestinations-ipe-hide-bar'),
      'title' => t('Add new pane'),
    ),
    'html' => TRUE,
  );

  $context = array(
    'region_id' => $region_id,
    'display' => $display,
    'renderer' => $renderer,
  );
  drupal_alter('dpidestinations_ipe_region_links', $vars['links'], $context);

}

function theme_dpidestinations_ipe_add_pane_button($vars) {
  $attributes = array(
    'class' => array('dpidestinations-ipe-linkbar', 'inline'),
  );

  $links = theme('links', array('links' => $vars['links'], 'attributes' => $attributes));

  return '<div class="dpidestinations-ipe-newblock dpidestinations-ipe-on">' . $links . '</div>';
}

function theme_dpidestinations_ipe_toolbar($vars) {
  $buttons = $vars['buttons'];

  $output = "<div id='dpidestinations-ipe-control-container' class='clearfix'>";
  foreach ($buttons as $key => $ipe_buttons) {
    $output .= "<div id='dpidestinations-ipe-control-$key' class='dpidestinations-ipe-control'>";

    // Controls in this container will appear when the IPE is not on.
    $output .= '<div class="dpidestinations-ipe-button-container clearfix">';
    foreach ($ipe_buttons as $button) {
      $output .= is_string($button) ? $button : drupal_render($button);
    }
    $output .= '</div>';

    // Controls in this container will appear when the IPE is on. It is usually
    // filled via AJAX.
    $output .= '<div class="dpidestinations-ipe-form-container clearfix"></div>';
    $output .= '</div>';
  }

  $output .= "</div>";

  return $output;
}

