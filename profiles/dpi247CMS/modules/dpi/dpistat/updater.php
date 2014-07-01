<?php

if (isset($_GET['nid']) && is_numeric($_GET['nid'])) {
  function dpistat_cache_entry($cid, $datas) {
    $cache_inc = variable_get('cache_inc', './includes/cache.inc');

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

      // Using Memcached we have no way to get all cache entries starting with "dpistat_",
      // so we have to maintain an index of these entries.
      $cached_entries_index = cache_get('dpistat_entries_index_'.$lock);
      if ($cached_entries_index && isset($cached_entries_index->data) && !empty($cached_entries_index->data)) {
        $entries_index = $cached_entries_index->data.'--'.$cid;
      }
      else {
        $entries_index = $cid;
      }
      cache_set('dpistat_entries_index_'.$lock, $entries_index);
    }

    cache_set($cid, $datas, 'cache_dpistat_tempdata');
  }

  // Log errors
  function dpistat_updater_shutdown() {
    if ($last_error = error_get_last()) {
      if (function_exists('cache_set') && $last_error['type'] <= E_PARSE) {
        dpistat_cache_entry(uniqid('error_'), $last_error);
      }
    }
  }

  register_shutdown_function('dpistat_updater_shutdown');

  // Ceci n'est pas encore utilisé dans la v1... le but est de créer les trending topics.
  if (isset($_GET['terms'])){
    // On s'assure que tous les termes ici sont bien des id numériques et on recrée $safe_terms avec les valeurs vérifiées.
    $term_array = explode(' ', $_GET['terms']);
    if (is_array($term_array)) {
      $safe_terms = array();
      foreach ($term_array as $term) {
        if (is_numeric($term)) {
          $safe_terms[] = $term;
        }
      }
      $terms = implode(' ', $safe_terms);
    }
  }

  $dpi_install_root = $_SERVER['DOCUMENT_ROOT'];
  chdir($dpi_install_root);

  /**
   * Root directory of Drupal installation.
   */
  define('DRUPAL_ROOT', getcwd());

  include_once ('./includes/bootstrap.inc');
  drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);
  // Memcached needs less bootstrap than DB cache
  if ($memcache_bins = variable_get('memcache_bins', FALSE) && array_key_exists('cache_dpistat_tempdata', $memcache_bins)) {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_PAGE_CACHE);
  }
  else {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
  }

  $datas = array();
  $datas['nid'] = $_GET['nid'];
  $datas['title'] = $_GET['title'];
  for ($i = 1; $i <= 10; $i++) {
    $id = ($i == 10) ? 'param'.$i : 'param0'.$i;
    $datas[$id] = $_GET[$id];
  }
  if (isset($terms) && $terms != '') {
    $datas['terms'] = $terms;
  }

  $cid = uniqid('dpistat_'.$_GET['nid'].'_');

  dpistat_cache_entry($cid, $datas);
}
