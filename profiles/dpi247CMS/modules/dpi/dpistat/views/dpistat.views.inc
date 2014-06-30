<?php
/**
 * @file
 * Provide views data and handlers for DPI Statistics module
 */

/**
 * Implementation of hook_views_data()
 */
function dpistat_views_data() {
  // Basic table information.

  $data = array(
    'dpistat_node_counter' => array(),
    'dpistat_term_counter' => array(),
    'dpistat_node_hourly_counter' => array(),
  );

  // ----------------------------------------------------------------
  // dpistat_node_counter table

  $data['dpistat_node_counter']['table'] = array(
    'group' => t('DPI Stat Node statistics'),
    'join' => array(
      // ...to the node table
      'node' => array(
        'left_field' => 'nid',
        'field' => 'nid',
      ),
    ),
  );

  // title
  $data['dpistat_node_counter']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the node.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param01
  $data['dpistat_node_counter']['param01'] = array(
    'title' => t('Custom parameter 01'),
    'help' => t('The Custom parameter 01 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param02
  $data['dpistat_node_counter']['param02'] = array(
    'title' => t('Custom parameter 02'),
    'help' => t('The Custom parameter 02 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param03
  $data['dpistat_node_counter']['param03'] = array(
    'title' => t('Custom parameter 03'),
    'help' => t('The Custom parameter 03 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param04
  $data['dpistat_node_counter']['param04'] = array(
    'title' => t('Custom parameter 04'),
    'help' => t('The Custom parameter 04 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param05
  $data['dpistat_node_counter']['param05'] = array(
    'title' => t('Custom parameter 05'),
    'help' => t('The Custom parameter 05 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param06
  $data['dpistat_node_counter']['param06'] = array(
    'title' => t('Custom parameter 06'),
    'help' => t('The Custom parameter 06 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param07
  $data['dpistat_node_counter']['param07'] = array(
    'title' => t('Custom parameter 07'),
    'help' => t('The Custom parameter 07 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param08
  $data['dpistat_node_counter']['param08'] = array(
    'title' => t('Custom parameter 08'),
    'help' => t('The Custom parameter 08 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param09
  $data['dpistat_node_counter']['param09'] = array(
    'title' => t('Custom parameter 09'),
    'help' => t('The Custom parameter 09 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param10
  $data['dpistat_node_counter']['param10'] = array(
    'title' => t('Custom parameter 10'),
    'help' => t('The custom parameter 10 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // totalcount
  $data['dpistat_node_counter']['totalcount'] = array(
    'title' => t('Total views'),
    'help' => t('The total number of times the node has been viewed.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // daycount
  $data['dpistat_node_counter']['daycount'] = array(
    'title' => t('Views today'),
    'help' => t('The total number of times the node has been viewed today.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // weekcount
  $data['dpistat_node_counter']['weekcount'] = array(
    'title' => t('Views this week'),
    'help' => t('The total number of times the node has been viewed this week.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // monthcount
  $data['dpistat_node_counter']['monthcount'] = array(
    'title' => t('Views this month'),
    'help' => t('The total number of times the node has been viewed this month.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // yearcount
  $data['dpistat_node_counter']['yearcount'] = array(
    'title' => t('Views this year'),
    'help' => t('The total number of times the node has been viewed this year.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // timestamp
  $data['dpistat_node_counter']['timestamp'] = array(
    'title' => t('Most recent view'),
    'help' => t('The most recent time the node has been viewed.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'click sortable' => TRUE,
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // ----------------------------------------------------------------
  // dpistat_term_counter table

  $data['dpistat_term_counter']['table'] = array(
    'group' => t('DPI Stat Term statistics'),
    'join' => array(
      // ...to the node table
      'term_data' => array(
        'left_field' => 'tid',
        'field' => 'tid',
      ),
    ),
  );

  // totalcount
  $data['dpistat_term_counter']['totalcount'] = array(
    'title' => t('Total views'),
    'help' => t('The total number of times the term has been viewed.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // daycount
  $data['dpistat_term_counter']['daycount'] = array(
    'title' => t('Views today'),
    'help' => t('The total number of times the term has been viewed today.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // weekcount
  $data['dpistat_term_counter']['weekcount'] = array(
    'title' => t('Views this week'),
    'help' => t('The total number of times the term has been viewed this week.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // monthcount
  $data['dpistat_term_counter']['monthcount'] = array(
    'title' => t('Views this month'),
    'help' => t('The total number of times the term has been viewed this month.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // yearcount
  $data['dpistat_term_counter']['yearcount'] = array(
    'title' => t('Views this year'),
    'help' => t('The total number of times the term has been viewed this year.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // timestamp
  $data['dpistat_term_counter']['timestamp'] = array(
    'title' => t('Most recent view'),
    'help' => t('The most recent time the term has been viewed.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ----------------------------------------------------------------
  // dpistat_node_hourly_counter table

  $data['dpistat_node_hourly_counter']['table'] = array(
    'group' => t('DPI Stat Node hourly statistics'),
    'join' => array(
      // ...to the node table
      'node' => array(
        'left_field' => 'nid',
        'field' => 'nid',
      ),
    ),
  );

  // title
  $data['dpistat_node_hourly_counter']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the node.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param01
  $data['dpistat_node_hourly_counter']['param01'] = array(
    'title' => t('Custom parameter 01'),
    'help' => t('The custom parameter 01 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param02
  $data['dpistat_node_hourly_counter']['param02'] = array(
    'title' => t('Custom parameter 02'),
    'help' => t('The custom parameter 02 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param03
  $data['dpistat_node_hourly_counter']['param03'] = array(
    'title' => t('Custom parameter 03'),
    'help' => t('The custom parameter 03 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param04
  $data['dpistat_node_hourly_counter']['param04'] = array(
    'title' => t('Custom parameter 04'),
    'help' => t('The Custom parameter 04 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param05
  $data['dpistat_node_hourly_counter']['param05'] = array(
    'title' => t('Custom parameter 05'),
    'help' => t('The Custom parameter 05 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param06
  $data['dpistat_node_hourly_counter']['param06'] = array(
    'title' => t('Custom parameter 06'),
    'help' => t('The Custom parameter 06 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param07
  $data['dpistat_node_hourly_counter']['param07'] = array(
    'title' => t('Custom parameter 07'),
    'help' => t('The Custom parameter 07 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param08
  $data['dpistat_node_hourly_counter']['param08'] = array(
    'title' => t('Custom parameter 08'),
    'help' => t('The Custom parameter 08 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param09
  $data['dpistat_node_hourly_counter']['param09'] = array(
    'title' => t('Custom parameter 09'),
    'help' => t('The Custom parameter 09 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // param10
  $data['dpistat_node_hourly_counter']['param10'] = array(
    'title' => t('Custom parameter 10'),
    'help' => t('The custom parameter 10 for the stats.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // hourlycount
  $data['dpistat_node_hourly_counter']['count'] = array(
    'title' => t('Hourly views'),
    'help' => t('The total number of times the node has been viewed each hour the past 48 hours.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // timestamp
  $data['dpistat_node_hourly_counter']['timestamp'] = array(
    'title' => t('Hour viewed'),
    'help' => t('The hour within which the node was counted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
