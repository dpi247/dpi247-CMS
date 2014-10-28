<?php 

/**
 * Page callback.
 * @param $entity_type
 * Type of the entity : comment
 * @param $entity_id
 * A comment ID.
 * @param $tag
 *    Tag name to vote for.
 * @param $value
 *    Value of vote, can be only 1 or -1. if another value will be provided, it will be handled as error.
 * A tag for different voting on same entity type
 * Submits the vote request and refreshes the page without JavaScript.
 * Otherwise, it submits the vote request and returns JSON to be parsed by jQuery.
 */
function dpicomments_reportabuse_vote($entity_type, $entity_id, $tag = 'dpicomments_comment_vote') {
  global $user;
  if (!drupal_valid_token($_GET['token'], $entity_id, true)) {
    watchdog('dpicomments', 'Voting form error: Invalid token.');
    return drupal_access_denied();
  }
  $voted = dpicomments_reportabuse_get_votes($entity_type, $entity_id, $user->uid, $tag);
  
  $vote_source = $user->uid ? $user->uid : ip_address();
  
  // If the voter has not already voted.
  if (!$voted) {
    $votes[] = array(
      'entity_type' => $entity_type,
      'entity_id' => $entity_id,
      'value_type' => 'points',
      'value' => 1,
      'tag' => $tag ? $tag : 'dpicomments_' . $entity_type . '_vote',
      'vote_source' => $vote_source,
    );
    votingapi_set_votes($votes);
    $criteria = array(
      'entity_type' => $entity_type,
      'entity_id' => $entity_id,
      'function' => 'sum',
      'tag' => $tag ? $tag : 'dpicomments_' . $entity_type . '_vote',
    );
    $results = votingapi_select_results($criteria);

    module_invoke_all('dpicomments_reportabuse_voted', 'vote', $entity_type, $entity_id, $tag, $score, $user);
    if (isset($_GET['json'])) {
      // Return json to client side, taking into consideration entity type.
      drupal_json_output(theme('dpicomments_reportabuse_json_response__' . $entity_type . '__' . $tag, array('entity_type' => $entity_type, 'entity_id' => $entity_id, 'tag' => $tag, 'score' => $score, 'vote_type' => 'vote')));
    }
    else {
      drupal_set_message(t('Thank you for your vote.'));
      // go to the page where user pressed vote button
      drupal_goto();
    }
  }
}

function dpicomments_reportabuse_undo_vote($entity_type, $entity_id, $tag = 'dpicomments_comment_vote') {
  global $user;
  if (!drupal_valid_token($_GET['token'], $entity_id, true)) {
    watchdog('dpicomments', 'Voting report abuse form error: Invalid token.');
    return drupal_access_denied();
  }

  $voted = dpicomments_reportabuse_get_votes($entity_type, $entity_id, $user->uid, $tag);
  $can_undo_vote = variable_get('dpicomments_' . $entity_type . '_undo_vote', 0);
  // If the voter has already voted and he can undo his vote.
  $criteria = array();
  if ($voted && $can_undo_vote) {
    $criteria['entity_type'] = $entity_type;
    $criteria['entity_id'] = $entity_id;
    $criteria['value_type'] = 'points';
    if ($user->uid == 0) {
      $criteria['vote_source'] = ip_address();
    }
    else {
      $criteria['uid'] = $user->uid;
    }
    
    if (isset($tag) && $tag != "") {
      $criteria['tag'] = $tag;
    }

    $votes = votingapi_select_votes($criteria);
    votingapi_delete_votes($votes);
    votingapi_recalculate_results($entity_type, $entity_id, TRUE);

    $criteria = array(
      'entity_type' => $entity_type,
      'entity_id' => $entity_id,
      'function' => 'sum',
      'tag' => $tag ? $tag : 'dpicomments_' . $entity_type . '_vote',
    );
    $results = votingapi_select_results($criteria);
    $score = isset($results[0]['value']) ? $results[0]['value'] : 0;
    module_invoke_all('dpicomments_reportabuse_voted', 'undo_vote', $entity_type, $entity_id, $tag, $score, $user);
    if (isset($_GET['json'])) {
      drupal_json_output(theme('dpicomments_reportabuse_json_response__' . $entity_type . '__' . $tag, array('entity_type' => $entity_type, 'entity_id' => $entity_id, 'tag' => $tag, 'score' => $score, 'vote_type' => 'undo_vote')));
    }
    else {
      drupal_set_message(t('Thank you for your vote.'));
      // go to the page where user pressed vote button
      drupal_goto();
    }
  }
}

/**
 * Return the number of votes for a given content.
 *
 * @param $entity_type
 * An entity type : comment.
 * @param $entity_id
 * An ID comment.
 * @param $uid
 * A user ID.
 * @param $tag
 * A tag for different voting on the content of same type
 * @return Integer
 * Number of votes the user has cast on this node.
 */
function dpicomments_reportabuse_get_votes($entity_type, $entity_id, $uid, $tag = 'dpicomments_comment_vote') {
  $criteria['entity_type'] = $entity_type;
  $criteria['entity_id'] = $entity_id;
  $criteria['value_type'] = 'points';
  if ($uid == 0) {
    $criteria['vote_source'] = ip_address();
  }
  else {
    $criteria['uid'] = $uid;
  }
  if (isset($tag) && $tag != "") {
    $criteria['tag'] = $tag;
  }
  
  $results = votingapi_select_votes($criteria);

  return count($results);
}

/**
 * Return the total score for a given content.
 *
 * @param $entity_type
 * Comment
 * @param $entity_id
 * A comment ID.
 * @param $tag
 * A tag to identify different votes on the same content type
 * @return Integer
 * The score.
 */
function dpicomments_reportabuse_get_score($entity_type, $entity_id, $tag = 'dpicomments_comment_vote') {
  $criteria['entity_type'] = $entity_type;
  $criteria['entity_id'] = $entity_id;
  $criteria['value_type'] = 'points';
  $criteria['function'] = 'sum';
  if (isset($tag) && $tag != "") {
    $criteria['tag'] = $tag;
  }
  $results = votingapi_select_results($criteria);
  if (empty($results)) {
    return 0;
  }
  else {
    return $results[0]['value'];
  }
}

/**
 * If widget is returned by ajax, and there are ajax_html_ids in the $_GET,
 * then we need to clear destination parameter, reduce query length.
 */
function dpicomments_reportabuse_get_cleared_destination() {
  $clear_destination = &drupal_static(__FUNCTION__);
  if (isset($clear_destination)) {
    return $clear_destination;
  }
  if (isset($_GET['destination'])) {
    $path = $_GET['destination'];
  }
  else {
    $path = $_GET['q'];
    $query = $_GET;
    unset($query['ajax_html_ids']);
    unset($query['ajax_page_state']);
    $query = drupal_http_build_query(drupal_get_query_parameters($query));
    if ($query != '') {
      $path .= '?' . $query;
    }
  }
  $clear_destination = array('destination' => $path);
  return $clear_destination;
}