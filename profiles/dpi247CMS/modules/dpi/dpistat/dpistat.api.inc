<?php

/**
 * Get day, week, month, year and total counts for a node.
 *
 * @param int $nid
 *   Node ID
 *
 * @return array
 *   The counts
 */
function dpistat_api_node_get_counts($nid) {
  $return_counts = array();
  $db_query = db_select('dpistat_node_counter', 'd');
  $db_query->addExpression('SUM(d.totalcount)', 'totalcount');
  $db_query->addExpression('SUM(d.daycount)', 'daycount');
  $db_query->addExpression('SUM(d.weekcount)', 'weekcount');
  $db_query->addExpression('SUM(d.monthcount)', 'monthcount');
  $db_query->addExpression('SUM(d.yearcount)', 'yearcount');
  $db_query->condition('d.nid', $nid);
  $db_results = $db_query->execute();
  foreach ($db_results as $db_result) {
    $return_counts = (array)$db_result;
    break;
  }
  return $return_counts;
}

/**
 * Get day, week, month, year or total counts for a node.
 *
 * It is possible to add parameters to the query.
 *
 * @param string $column
 *   Value to get ("day" or "week" for example)
 * @param array $param
 *   Array of parameters to add to the query.
 *   Each parameter must be an array with two mandatory keys : column and value,
 *   and an optional key : op, which is set to "=" by default.
 *
 * @return array
 *   The counts
 */
function dpistat_api_node_get_count_by_params($column, $params) {
  $return_counts = array();
  
  $db_query = db_select('dpistat_node_counter', 'd');
  $db_query->addExpression('SUM(d.'.$column.')', $column);
  foreach ($params as $param) {
    $param += array('op' => '=');
    $db_query->condition('d.'.$param['column'], $param['value'], $param['op']);
  }
  $db_results = $db_query->execute();
  foreach ($db_results as $db_result) {
    $return_counts = (array)$db_result;
    break;
  }
}

/**
 * Get hourly counts for a node for the last 48h.
 *
 * @param int $nid
 *   Node ID
 *
 * @return array
 *   The counts
 */
function dpistat_api_node_get_hourly_counts($nid) {
  return dpistat_api_get_hourly_counts_with_params(array(array('column' => 'nid', 'value' => $nid)));
}

/**
 * Get hourly counts for a node for the last 48h
 *
 * It is possible to add parameters to the query.
 *
 * @param array $param
 *   Array of parameters to add to the query.
 *   Each parameter must be an array with two mandatory keys : column and value,
 *   and an optional key : op, which is set to "=" by default.
 *
 * @return array
 *   The counts
 */
function dpistat_api_get_hourly_counts_with_params($params = array()) {
  $return_counts = array();

  $db_query = db_select('dpistat_node_hourly_counter', 'd');
  $db_query->addExpression('SUM(d.count)', 'count');
  $db_query->fields('d', array('timestamp'));
  foreach ($params as $param) {
    $param += array('op' => '=');
    $db_query->condition('d.'.$param['column'], $param['value'], $param['op']);
  }
  $db_query->groupBy('d.timestamp');
  $db_query->orderBy('d.timestamp', 'ASC');
  $db_results = $db_query->execute();
  foreach ($db_results as $db_result) {
    $return_counts = (array)$db_result;
    break;
  }

  return $return_counts;
}
