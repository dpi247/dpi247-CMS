<?php
/*
 * Reorder process
 * 
 * Technical documentation: <a href="/node/193">link text</a> 
 * 
 * 
 */

function dpidestinations_targetview_page_reorder($term,$targetblock){

  dsm($targetblock);
  $targetview=_dpidestinations_targetviewreorder_initialize_view($targetblock,$term);
  
  $targetview->pre_execute();
  $targetview->execute();
  
  
  $nodes=$targetview->result;

  dsm($nodes);
  return drupal_get_form('dpidestinations_targetview_page_reorder_form',$nodes,$term->tid ,$targetblock->target);
  
}


function dpidestinations_targetview_page_reorder_form($form, &$form_state, $nodes, $destination_term, $destination_target){
  $form['#tree'] = TRUE;
  
  
  $weight_delta = round(count($nodes) / 2);
  foreach($nodes as $node){
    $form['nodes'][$node->nid]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => 0,
      '#delta' => $weight_delta,
      '#title_display' => 'invisible',
      '#title' => t('Weight for @block block', array('@block' => $block['info'])),
      '#attributes'=> array('class' => array('node-weight')),
    );
    $form['nodes'][$node->nid]['title'] = array(
     // '#markup' => 'markup',
      '#markup' => $node->node_title,
    );
  
    $form['nodes'][$node->nid]['edit'] = array(
      // '#markup' => 'markup',
      '#markup' => $node->node_title,
    );
  }
  
  $form['destination_tid'] = array(
    '#type' => 'value',
    '#value' => $destination_tid,
  );
  $form['destination_target'] = array(
    '#type' => 'value',
    '#value' => $destination_target,
  );
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Re order'),
  );
  
  return $form;
  
}


function dpidestinations_targetview_page_reorder_form_submit(& $form, &$form_state){
  dsm($form_id);
  dsm($form);
  dsm($form_state);
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
function template_preprocess_dpidestination_targetview_page_reorder_form(&$variables) {

  // Add each block in the form to the appropriate place in the block listing.
  foreach (element_children($variables['form']['nodes']) as $i) {
    $node = &$variables['form']['nodes'][$i];

    // Set special classes needed for table drag and drop.
    $node['weight']['#attributes']['class'] = array('node-weight', 'node-weight');
    $block['weight']['#attributes']['class'] = array('block-weight', 'block-weight-' . $region);
    
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


