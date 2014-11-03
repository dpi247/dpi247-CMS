<?php

/**
 * @file
 * This file contains declartion for admin form.
 */

/**
 * Menu callback to configure module settings.
 */
function dpicomments_settings() {
  $form['dpicomments_global_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dpicomments global settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['dpicomments_global_settings']['dpicomments_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include standard dpicomments js file.'),
    '#default_value' => variable_get('dpicomments_add_js', 1),
  );

  $form['dpicomments_comment_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment report abuse widget settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_widget_show'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add report abuse voting widget to comments"),
    '#default_value' => variable_get('dpicomments_comment_widget_show', 0),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Vote text"),
    '#default_value' => variable_get('dpicomments_comment_vote_text', t('Vote')),
    '#description' => t('Enter, for example, <em>Vote</em> or <em>Digg</em>.'),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_undo_vote'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to undo their comment votes"),
    '#default_value' => variable_get('dpicomments_comment_undo_vote', 0),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_undo_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted and user CAN undo his vote"),
    '#default_value' => variable_get('dpicomments_comment_undo_vote_text', ''),
    '#description' => t('Enter, for example, <em>Undugg</em>, <em>Undo vote</em>, or <strong>leave it blank to provide user with "arrow down" icon.</strong>'),
    '#states' => array(
      'visible' => array(
        'input[name="dpicomments_comment_undo_vote"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_nb_abuse_before_seccond_trigger'] = array(
    '#type' => 'textfield',
    '#title' => t("Number of reports of abuse before the comment unpublication"),
    '#size' => 4,
    '#default_value' => variable_get('dpicomments_comment_nb_abuse_before_seccond_trigger', 5),
    '#description' => t('Enter a number.'),
  );

  // Validate form
  $form['#validate'][] = 'dpicomments_settings_validate';

  $form['array_filter'] = array('#type' => 'hidden');

  return system_settings_form($form);
}

/**
 * Validate dpicomments_settings_validate form submissions.
 */
function dpicomments_settings_validate($form, &$form_state) {
  if (!is_numeric($form['dpicomments_comment_widget_settings']['dpicomments_comment_nb_abuse_before_seccond_trigger']['#value'])) {
    form_set_error('Number of reports of abuse before the comment unpublication ', 'Number of reports of abuse before the comment unpublication  must be a number');
  }
}

/**
 * Menu callback; present an administrative comment listing.
 * Base on comment module
 */
function dpicomments_admin($type = 'abusive') {
  $edit = $_POST;

  // On charge comment.admin pour avoir acces à la suppréssion multiple des commentaires
  module_load_include('inc', 'comment', 'comment.admin');

  if (isset($edit['operation']) && ($edit['operation'] == 'delete') && isset($edit['comments']) && $edit['comments']) {
    return drupal_get_form('comment_multiple_delete_confirm');
  }
  else {
    return drupal_get_form('dpicomments_admin_overview', $type);
  }
}


/**
 * Form builder for the comment overview administration form.
 *
 * @param $arg
 *   Current path's fourth component: the type of overview form ('approval' or
 *   'new').
 *
 * @ingroup forms
 * @see comment_admin_overview_validate()
 * @see comment_admin_overview_submit()
 * @see theme_comment_admin_overview()
 */
function dpicomments_admin_overview($form, &$form_state, $arg) {
  // Build an 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  if ($arg == 'abusive_unpublish') {
    $options['publish'] = t('Publish the selected comments');
  }
  else {
    $options['unpublish'] = t('Unpublish the selected comments');
  }
  $options['delete'] = t('Delete the selected comments');

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'publish',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $header = array(
    'subject' => array('data' => t('Subject'), 'field' => 'subject'),
    'author' => array('data' => t('Author'), 'field' => 'name'),
    'posted_in' => array('data' => t('Posted in'), 'field' => 'node_title'),
    'changed' => array('data' => t('Updated'), 'field' => 'c.changed', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  // Load the comments that need to be displayed.
  // From VotingAPI
  $criteria['entity_type'] = 'comment';
  $criteria['tag'] = 'dpicomments_comment_vote';
  $results_voting = votingapi_select_votes($criteria);

  $cids_results = array();
  foreach ($results_voting as $row) {
     $cids_results[] = $row['entity_id'];
  }

  $status = ($arg == 'abusive_unpublish') ? COMMENT_NOT_PUBLISHED : COMMENT_PUBLISHED;

  // Load the comments (and node title) that need to be displayed.
  $query = db_select('comment', 'c')->extend('PagerDefault')->extend('TableSort');
  $query->join('node', 'n', 'n.nid = c.nid');
  $query->addField('n', 'title', 'node_title');
  $query->addTag('node_access');
  $result = $query
    ->fields('c', array('cid', 'subject', 'name', 'changed')) 
    ->condition('c.cid', $cids_results, 'IN')
    ->condition('c.status', $status)
    ->limit(50)
    ->orderByHeader($header)
    ->execute();


  // We collect a sorted list of node_titles during the query to attach to the
  // comments later.
  $cids = array();
  foreach ($result as $row) {
    $cids[] = $row->cid;
    $node_titles[] = $row->node_title;
  }
  $comments = comment_load_multiple($cids);

  // Build a table listing the appropriate comments.
  $options = array();
  $destination = drupal_get_destination();

  foreach ($comments as $comment) {

    // Remove the first node title from the node_titles array and attach to
    // the comment.
    $comment->node_title = array_shift($node_titles);
    $comment_body = field_get_items('comment', $comment, 'comment_body');
    $options[$comment->cid] = array(
      'subject' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $comment->subject,
          '#href' => 'comment/' . $comment->cid,
          '#options' => array('attributes' => array('title' => truncate_utf8($comment_body[0]['value'], 128)), 'fragment' => 'comment-' . $comment->cid),
        ),
      ),
      'author' => theme('username', array('account' => $comment)),
      'posted_in' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $comment->node_title,
          '#href' => 'node/' . $comment->nid,
        ),
      ),
      'changed' => format_date($comment->changed, 'short'),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => 'comment/' . $comment->cid . '/edit',
          '#options' => array('query' => $destination),
        ),
      ),
    );
  }

  $form['comments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No comments available.'),
  );

  $form['pager'] = array('#theme' => 'pager'); 

  return $form;
}

/**
 * Validate dpicomments_admin_overview form submissions.
 */
function dpicomments_admin_overview_validate($form, &$form_state) {
  $form_state['values']['comments'] = array_diff($form_state['values']['comments'], array(0));
  // We can't execute any 'Update options' if no comments were selected.
  if (count($form_state['values']['comments']) == 0) {
    form_set_error('', t('Select one or more comments to perform the update on.'));
  }
}

/**
 * Process dpicomments_admin_overview form submissions.
 *
 * Execute the chosen 'Update option' on the selected comments, such as
 * publishing, unpublishing or deleting.
 */
function dpicomments_admin_overview_submit($form, &$form_state) {
  module_load_include('inc', 'comment', 'comment.admin');
  comment_admin_overview_submit($form, $form_state);
  //url de retour
  if (isset($form['options']['operation']['#options']['publish']))
    $url_ret = 'admin/content/comment/abusive_unpublish';
  else
    $url_ret = 'admin/content/comment/abusive';
  $form_state['redirect'] = $url_ret;
}