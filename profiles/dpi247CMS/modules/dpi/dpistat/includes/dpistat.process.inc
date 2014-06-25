<?php

/**
 * Cron actual process. Get all recorded statistics and merge them in appropriate tables.
 */
function dpistat_process_cron() {
  $current_tstamp = time();

  // Reset counters if needed
  dpistat_counters_reset($current_tstamp);

  // Count the number of terms and nodes
  $term_counts = $node_counts = array();

  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  $entries_count = 0;
  $cron_memory_limit = dpi_variable_get('dpistat_stats_cron_memory_limit', 75);
  // Check if the cache_dpistat_tempdata table is in DB or in Memcached
  if ($memcache_bins = variable_get('memcache_bins', FALSE) && array_key_exists('cache_dpistat_tempdata', $memcache_bins)) {
    // The stats processing may take some time, we need to be sure we don't edit entries which are under process
    $cached_lock = cache_get('dpistat_lock');
    if ($cached_lock && isset($cached_lock->data) && !empty($cached_lock->data) && is_numeric($cached_lock->data)) {
      $lock = $cached_lock->data;
    }
    else {
      $lock = 1;
    }
    // Increment lock
    $next_lock = $lock + 1;
    cache_set('dpistat_lock', $next_lock);

    // Get cached stats entries and clear them
    $cached_entries_index = cache_get('dpistat_entries_index_'.$lock);
    if ($cached_entries_index && isset($cached_entries_index->data) && !empty($cached_entries_index->data)) {
      $ser_entries_index = $cached_entries_index->data;
    } else {
      $ser_entries_index = '';
    }
    cache_clear_all('dpistat_entries_index_'.$lock, 'cache');

    // Process entries
    if (!empty($ser_entries_index)) {
      $entries_index = explode('--', $ser_entries_index);
      foreach ($entries_index as $entry_id) {
        if (!dpicommons_is_memory_limit_reached($cron_memory_limit)) {
          $res = (array)cache_get($entry_id, 'cache_dpistat_tempdata');
          $res['cid'] = $entry_id;
          dpistat_process_entry($res, $term_counts, $node_counts, $current_tstamp);
          $entries_count++;
        } else {
          print t('Memory over %memory_limit%, process stopped. %entries_count entries processed', array('%memory_limit' => $cron_memory_limit, '%entries_count' => $entries_count)).'<br>';
          break;
        }
      }
    }
  }
  else {
    // Collect all temp datas
    $db_datas = db_select('cache_dpistat_tempdata', 'c')
      ->fields('c', array('cid', 'data', 'created'))
      ->execute();

    // Process entries
    foreach ($db_datas as $res) {
      if (!dpicommons_is_memory_limit_reached($cron_memory_limit)) {
        $res = (array)$res;
        dpistat_process_entry($res, $term_counts, $node_counts, $current_tstamp);
      } else {
        print t('Memory over %memory_limit%, process stopped. %entries_count entries processed', array('%memory_limit' => $cron_memory_limit, '%entries_count' => $entries_count)).'<br>';
        break;
      }
    }
  }

  // Update tables with new datas
  dpistat_counters_update($term_counts, $node_counts);

  // Flush old datas if needed
  dpistat_counters_clean($current_tstamp);
}

/**
 * Reset every counter that need to be resetted, for example the day count each day.
 */
function dpistat_counters_reset($current_tstamp) {
  // Reset node counters
  $db_last_tstamp_query = db_select('dpistat_node_counter', 'd')
    ->fields('d', array('timestamp'))
    ->orderBy('d.timestamp', 'DESC')
    ->range(0, 1)
    ->execute();
  $last_tstamp = 0;
  foreach ($db_last_tstamp_query as $db_last_tstamp) {
    $last_tstamp = $db_last_tstamp->timestamp;
    break;
  }
  if ($placeholders = dpistat_get_placeholders($last_tstamp, $current_tstamp)) {
    db_update('dpistat_node_counter')
      ->fields($placeholders)
      ->execute();
    $string_placeholders = array();
    foreach ($placeholders as $column => $value) {
      $string_placeholders[] = $column.' = '.$value;
    }
    $string_placeholder = implode(', ', $string_placeholders);
    print t('Updated : !placeholder for !table', array('!placeholder' => $string_placeholder, '!table' => 'dpistat_node_counter')).'<br/>';
  }

  // Reset term counters
  $db_last_tstamp_query = db_select('dpistat_term_counter', 'd')
    ->fields('d', array('timestamp'))
    ->orderBy('d.timestamp', 'DESC')
    ->range(0, 1)
    ->execute();
  $last_tstamp = 0;
  foreach ($db_last_tstamp_query as $db_last_tstamp) {
    $last_tstamp = $db_last_tstamp->timestamp;
    break;
  }
  if ($placeholders = dpistat_get_placeholders($last_tstamp, $current_tstamp)) {
    db_update('dpistat_term_counter')
      ->fields($placeholders)
      ->execute();
    $string_placeholders = array();
    foreach ($placeholders as $column => $value) {
      $string_placeholders[] = $column.' = '.$value;
    }
    $string_placeholder = implode(', ', $string_placeholders);
    print t('Updated : !placeholder for !table', array('!placeholder' => $string_placeholder, '!table' => 'dpistat_term_counter')).'<br/>';
  }
}

/**
 * Merge an entry with previously processed statistics, or log an error.
 */
function dpistat_process_entry($res, &$term_counts, &$node_counts, $current_tstamp) {
  $dtstamp = $res['created'];
  if ($dtstamp && ($dtstamp < $current_tstamp)) {
    list($type, $a) = explode('_', $res['cid'], 2);
    $data = is_string($res['data']) ? unserialize($res['data']) : $res['data'];

    switch ($type) {
      case 'dpistat' :
        $trunc_dtstamp = $dtstamp - (date('i', $dtstamp) * 60 + date('s', $dtstamp));

        // Create merged arrays containing all structured datas
        dpistat_merge_datas($term_counts, $node_counts, $data, $trunc_dtstamp);
        break;

      case 'error' :
        $mapping_errors = array(
          E_ERROR => WATCHDOG_ERROR,
          E_WARNING => WATCHDOG_WARNING,
          E_PARSE => WATCHDOG_ERROR,
        );
        $vars = array(
          '%message' => $data['message'],
          '%file' => $data['file'],
          '%line' => $data['line'],
        );
        dpi_log('dpistat', 'record', 'PHP : %message in %file on line %line.', $vars, $mapping_errors[$data['type']]);
        break;
    }
  }

  if ($dtstamp < $current_tstamp) {
    cache_clear_all($res['cid'], 'cache_dpistat_tempdata');
  }
}

function dpistat_merge_datas(&$term_counts, &$node_counts, $data, $trunc_dtstamp) {
  if (isset($data['terms'])) {
    //explode terms to get an array of terms
    $terms = explode(' ', $data['terms']);
    //foreach term add 1 in the array
    foreach ($terms as $term) {
      if (isset($term_counts[$trunc_dtstamp][$term])) {
        $term_counts[$trunc_dtstamp][$term]++;
      } else {
        $term_counts[$trunc_dtstamp][$term] = 1;
      }
    }
  }

  for ($i = 1; $i <= 10; $i++) {
    $id = ($i == 10) ? 'param'.$i : 'param0'.$i;
    $param_array[$id] = $data[$id];
  }
  $param_key = implode('_', $param_array);

  $nid = $data['nid'];
  $param_exist = FALSE;
  if (isset($node_counts[$trunc_dtstamp][$nid])) {
    foreach ($node_counts[$trunc_dtstamp][$nid] as $key => $cur_node_count) {
      if ($cur_node_count['param_key'] == $param_key) {
        $param_exist = TRUE;
        $node_counts[$trunc_dtstamp][$nid][$key]['node_count']++;
        break;
      }
    }
  }
  if (!$param_exist || !isset($node_counts[$trunc_dtstamp][$nid])) {
    $new_entry = array(
      'param_key' => $param_key,
      'node_count' => 1,
      'data' => $data,
    );
    $node_counts[$trunc_dtstamp][$nid][] = $new_entry;
  }
}

function dpistat_counters_update($term_counts, $node_counts) {
  // Insert or update term counts
  foreach ($term_counts as $timestamp => $terms) {
    foreach ($terms as $tid => $term_count) {
      $term_updates = array(
        'daycount' => $term_count,
        'weekcount' => $term_count,
        'monthcount' => $term_count,
        'yearcount' => $term_count,
        'totalcount' => $term_count,
        'timestamp' => $timestamp,
      );
      db_merge('dpistat_term_counter')
        ->key(array('tid' => $tid))
        ->fields($term_updates)
        ->expression('daycount', 'daycount + :inc', array(':inc' => $term_count))
        ->expression('weekcount', 'weekcount + :inc', array(':inc' => $term_count))
        ->expression('monthcount', 'monthcount + :inc', array(':inc' => $term_count))
        ->expression('yearcount', 'yearcount + :inc', array(':inc' => $term_count))
        ->expression('totalcount', 'totalcount + :inc', array(':inc' => $term_count))
        ->execute();
    }
  }

  // Insert or update node counts
  foreach ($node_counts as $timestamp => $nodes) {
    foreach ($nodes as $nid => $node_datas) {
      foreach ($node_datas as $node_data) {
        $data = $node_data['data'];
        $node_count = $node_data['node_count'];

        $node_updates = array(
          'daycount' => $node_count,
          'weekcount' => $node_count,
          'monthcount' => $node_count,
          'yearcount' => $node_count,
          'totalcount' => $node_count,
          'timestamp' => $timestamp,
          'title' => $data['title'],
        );
        $merge_keys = array(
          'nid' => $nid,
          'param01' => $data['param01'],
          'param02' => $data['param02'],
          'param03' => $data['param03'],
          'param04' => $data['param04'],
          'param05' => $data['param05'],
          'param06' => $data['param06'],
          'param07' => $data['param07'],
          'param08' => $data['param08'],
          'param09' => $data['param09'],
          'param10' => $data['param10'],
        );
        db_merge('dpistat_node_counter')
          ->key($merge_keys)
          ->fields($node_updates)
          ->expression('daycount', 'daycount + :inc', array(':inc' => $node_count))
          ->expression('weekcount', 'weekcount + :inc', array(':inc' => $node_count))
          ->expression('monthcount', 'monthcount + :inc', array(':inc' => $node_count))
          ->expression('yearcount', 'yearcount + :inc', array(':inc' => $node_count))
          ->expression('totalcount', 'totalcount + :inc', array(':inc' => $node_count))
          ->execute();

        $node_updates = array(
          'count' => $node_count,
          'title' => $data['title'],
        );
        $merge_keys += array('timestamp' => $timestamp);
        db_merge('dpistat_node_hourly_counter')
          ->key($merge_keys)
          ->fields($node_updates)
          ->expression('count', 'count + :inc', array(':inc' => $node_count))
          ->execute();
      }
    }
  }
}

function dpistat_counters_clean($current_tstamp) {
  // Time limit of 48h
  $limit_tstamp = $current_tstamp - 49*60*60;
  db_delete('dpistat_node_hourly_counter')
    ->condition('timestamp', $limit_tstamp, '<')
    ->execute();
}

/**
 * Internal function to get the placeholder to put in the query
 * reseting some of the count fields depending on the difference of
 * time between two timestamps
 */
function dpistat_get_placeholders($last_tstamp, $current_tstamp) {
  if (!is_numeric($last_tstamp) || !is_numeric($current_tstamp)) {
    return FALSE;
  }

  $placeholder = array();

  if (date('z', $current_tstamp) != date('z', $last_tstamp)) {
    $placeholder['daycount'] = '0';
  }

  if (date('W', $current_tstamp) != date('W', $last_tstamp)) {
    $placeholder['weekcount'] = '0';
  }

  if (date('m', $current_tstamp) != date('m', $last_tstamp)) {
    $placeholder['monthcount'] = '0';
  }

  if (date('Y', $current_tstamp) != date('Y', $last_tstamp)) {
    $placeholder['yearcount'] = '0';
  }

  return empty($placeholder) ? FALSE : $placeholder;
}
