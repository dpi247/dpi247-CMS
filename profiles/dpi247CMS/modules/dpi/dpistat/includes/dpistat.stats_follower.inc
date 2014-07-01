<?php 

/**
 * Return the HTML of the stats for a node
 */
function dpistat_show_stats($nid) {
  $cached_content = cache_get('dpistat_stats_content_'.$nid, 'cache');
  $refresh_time = dpi_variable_get('dpistat_stats_content_refresh_time', 15);

  if (true||!is_object($cached_content) || !isset($cached_content->data) || empty($cached_content->data) || ($cached_content->created + $refresh_time*60) < time()) {
    // This cache doesn't exists or is too old, create it
    $show_stats_48h_params = array(
      'nid' => $nid,
      'width' => 'auto',
      'height' => 'auto',
      'display_title' => TRUE,
      'jsprint' => TRUE,
    );
    $last_48h = theme('dpistat_show_stats_48h', $show_stats_48h_params);
    $params = array();
    for ($i = 1; $i <= 10; $i++) {
      if ($params_callback = variable_get('dpistat_param_'.$i, FALSE)) {
        $show_stats_param_params = array(
          'nid' => $nid,
          'param_i' => $i,
          'params_callback' => $params_callback,
          'width' => 'auto',
          'height' => 'auto',
          'display_title' => TRUE,
          'jsprint' => TRUE,
        );
        $params[] = theme('dpistat_show_stats_param', $show_stats_param_params);
      }
    }

    $content = $last_48h.implode('', $params);

    cache_set('dpistat_stats_content_'.$nid, $content, 'cache');
  } else {
    // The object exists in cache and is not too old
    $content = $cached_content->data;
  }

  print $content;
}

/**
 * Callback to set the value of the dpistat_display_follower variable
 */
function dpistat_set_display_follower($val) {
  cache_clear_all('dpistat_stats_content_', 'cache', TRUE);
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  dpi_variable_set('dpistat_display_follower', dpicommons_toboolean($val));
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : '';
  drupal_goto($redirect);
}
