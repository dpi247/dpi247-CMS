<?php

if ($nid) {
  $temp_node_hours_stats = dpistat_api_node_get_hourly_counts($nid);

  $current_tstamp = time() - date('i')*60 - date('s');
  $last_tstamp = $current_tstamp - 48*60*60;

  $data = array();
  $data[] = array(t('Hours'), t('Views'));
  for ($tstamp = $last_tstamp; $tstamp <= $current_tstamp; $tstamp += 60*60) {
    $tstamp_views = isset($temp_node_hours_stats[$tstamp]) ? (int)$temp_node_hours_stats[$tstamp] : 0;
    $tstamp_hour = ($tstamp - $current_tstamp)/(60*60);
    $data[] = array($tstamp_hour, $tstamp_views);
  }

  if ($display_title == FALSE) {
    $titleChart = '';
  } else {
    $titleChart = t('Views for 48h');
  }

  $chart_params = array(
    'data' => $data,
    'title' => $titleChart,
    'pointSize' => 3,
    'AxeXTitle' => t('Hours since now'),
    'width' => $width,
    'height' => $height,
    'jsprint' => $jsprint,
  );
  $rendered_chart = theme('dpicommons_line_chart', $chart_params);

  print $rendered_chart ? $rendered_chart : t('Chart error.');
}
