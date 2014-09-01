<?php

if ($nid) {
  $temp_node_hours_stats = array();
  $param_name_id = ($param_i == 10) ? 'param'.$param_i : 'param0'.$param_i;
  $node_stats_db = db_select('dpistat_node_counter', 'd')
    ->fields('d', array($param_name_id, 'totalcount'))
    ->condition('d.nid', $nid)
    ->groupBy('d.'.$param_name_id)
    ->execute();

  $data = array();
  foreach ($node_stats_db as $stat) {
    $data[] = array(
      ($stat->{$param_name_id} == '') ? t('Not set') : $stat->{$param_name_id},
      (int)$stat->totalcount,
    );
  }

  if ($display_title == FALSE) {
    $titleChart = '';
  } else {
    $titleChart = t($params_callback[$params_callback['current_callback']]['name']);
  }

  $chart_params = array(
    'data' => $data,
    'title' => $titleChart,
    'header_name' => $param_name_id,
    'header_unit' => 'views count',
    'width' => $width,
    'height' => $height,
    'jsprint' => $jsprint,
  );
  $rendered_chart = theme('dpicommons_pie_chart', $chart_params);

  print $rendered_chart ? $rendered_chart : t('Chart error.');
}
