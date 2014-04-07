<?php
/**
 * @file
 * API documentation for performance hacks.
 */

/**
 * Add a custom cache key for node view render caching.
 *
 * @param $node
 *   The node being cached.
 * @param $view_mode
 *   The view mode being cached.
 *
 * @return
 *   A string to be added to the cache key for this node and view mode.
 */
function hook_performance_hacks_custom_cache_key($node, $view_mode) {
  if ($view_mode == 'full') {
    // Some custom code themes the node different depending on a $_GET param.
    if isset($_GET['foo']) {
      return $_GET['foo']);
    }
  }
} 
