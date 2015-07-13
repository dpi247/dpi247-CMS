<?php
/**
 * @file
 * ESI handlers for ESI Block.
 */





/**
 * Implements hook_block_view_alter().
 */
function dpicache_block_view_alter(&$data, $block) {
  // The region isn't known in hook_block_view().  This is the first hook where
  // the region is provided.

  if ($block->module == 'dpicache') {
    // Build a URL which contains all the necessary data.
    $url = url(dpicache_block_url($block), array('absolute' => TRUE));
    $data['content'] = array(
        '#type' => 'esi',
        '#url' => $url,
    );


    if(variable_get('dpicache_esi_add_url_in_comment',FALSE)){
      $data['content']["#prefix"]="<!-- esi url: $url -->";
    }
  }
}


/**
 * Prepare an ESI block for rendering.
 * Defined in hook_esi_component_info().
 *
 * @see dpicache_esi_component_info().
 */
function dpicache_block__esi_block_prepare($component_key) {
  list($theme, $region, $module, $delta) = explode(':', $component_key);

  $block = block_load($module, $delta);
  // Validate that the module/delta combination is valid.
  if (empty($block->esi_enabled)) {
    return FALSE;
  }

  $block->theme = $theme;
  $block->region = $region;

  // Check for optional arguments (page, user/role cache control).
  $args = array_slice(func_get_args(), 1);
  if (count($args)) {
    // Check for user/role contexts.
    if (preg_match('/CACHE=(USER|ROLE)/', $args[count($args)-1], $matches)) {
      $block->esi_cache_context = $matches[1];
      array_pop($args);
    }
    // Check for a page context.
    if (count($args) && $page = base64_decode($args[0])) {
      $block->esi_page_context = $page;
    }
  }

  // Allow other modules to alter the context information here.
  // @see hook_esi_block_context_alter().
  drupal_alter('esi_block_context', $block);

  // Restore the original context.
  dpicache_block__restore_context($block);

  return $block;
}

/**
 * Restore the original context that was used when a block was displayed.
 */
function dpicache_block__restore_context($block) {
  // Restore the theme.
  global $theme;
  $theme = $block->theme;

  // Restore the page context.
  if (!empty($block->esi_page_context)) {
    $_SERVER['REQUEST_URI'] = $block->esi_page_context;
    $_GET['q'] = $block->esi_page_context;
    drupal_static_reset('drupal_get_destination');
  }
}

/**
 * Render the HTML for a single block.
 * Defined in hook_esi_component_info().
 *
 * @see dpicache_esi_component_info()
 */
function dpicache_block__esi_block_render($block) {
  $build = array();

  // Reproduce functionality of _block_render_blocks().
  $array = module_invoke($block->module, 'block_view', $block->delta);

  // Allow modules to modify the block before it is viewed, via either
  // hook_block_view_alter() or hook_block_view_MODULE_DELTA_alter().
  drupal_alter(array('block_view', "block_view_{$block->module}_{$block->delta}"), $array, $block);

  if (isset($array) && is_array($array)) {
    foreach ($array as $k => $v) {
      $block->$k = $v;
    }
  }

  if (isset($block->content) && $block->content) {
    // Normalize to the drupal_render() structure.
    if (is_string($block->content)) {
      $block->content = array('#markup' => $block->content);
    }
    // Override default block title if a custom display title is present.
    if ($block->title) {
      // Check plain here to allow module generated titles to keep any
      // markup.
      $block->subject = $block->title == '<none>' ? '' : check_plain($block->title);
    }
    if (!isset($block->subject)) {
      $block->subject = '';
    }

    // Reproduce functionality of _block_get_renderable_array().
    $key = "{$block->module}_{$block->delta}";

    $build[$key] = $block->content;
    unset($block->content);
    if ($key != 'system_main' && $key != 'system_help') {
      $build[$key]['#contextual_links']['block'] = array('admin/structure/block/manage', array($block->module, $block->delta));
    }
    $build[$key] += array(
      '#block' => $block,
      // @TODO: is weight really an issue here?
      // '#weight' => ++$weight,
    );
    $build[$key]['#theme_wrappers'][] ='block';
  }

  esi_block_set_http_headers($block);

  return $build;
}

/**
 * Set HTTP headers to control caching of ESI fragments.
 */
function esi_block_set_http_headers($block) {
  // Nginx follows rfc2616 section 14.9.1 correctly and will not cache if
  // header is set to private.
  // The example Varnish VCL does not follow the RFC, and for ajax private
  // should be used.

  // $user_cache_control_header = variable_get('esi_mode', ESI_MODE) == ESI__CONFIG_SSI ? 'public' : 'private';
  $user_cache_control_header = 'private';
  $ttl = $block->esi_ttl;

  $headers = array();
  if ($block->cache == DRUPAL_NO_CACHE) {
    $headers[] = array('Cache-Control', 'must-revalidate, max-age=0');
  }
  elseif ($block->cache & DRUPAL_CACHE_PER_ROLE || $block->cache & DRUPAL_CACHE_PER_USER) {
    $headers[] = array('Cache-Control', "$user_cache_control_header, max-age=$ttl");
    $headers[] = array('X-BLOCK-CACHE', ($block->cache & DRUPAL_CACHE_PER_USER ? 'USER' : 'ROLE'));
  }
  else {
    $headers[] = array('Cache-Control', "public, max-age=$ttl");
  }

  // Allow other modules to alter the headers.
  // @see hook_esi_block_cache_headers_alter().
  drupal_alter('esi_block_cache_headers', $headers);

  foreach($headers as $header) {
    drupal_add_http_header($header[0], $header[1]);
  }
}

/**
 * Flush the ESI block caches.
 * Defined in hook_esi_component_info().
 *
 * @see esi_block_esi_component_info()
 */
function dpicache_block__esi_block_flush() {
  // @TODO.

}



/**
 * Build the URL to use for this ESI component.  The URL must contain all the
 * relevant information required to restore the original context of this block.
 *
 * @param Object $block.
 * A populated block object (as made available in hook_block_view_alter())
 * containing as a minimum the keys:
 * - cache
 * - module
 * - delta
 * - region
 * - theme
 *
 * @return String
 * The internal URL. Generate a fully-qualified path by running through url().
 */
function dpicache_block_url($block) {
  // ESI 6.x-1.x and 6.x-2.x used the URL patterns:
  // Default:                esi/block/theme:region:module:delta
  // Cache-per-page:         esi/block/theme:region:module:delta/[base64($_GET['q'])]
  // Cache-per-role:         esi/block/theme:region:module:delta/CACHE=ROLE
  // Cache-per-role ID:      esi/block/theme:region:module:delta/CACHE=[rolehash]
  // Cache-per-page-role:    esi/block/theme:region:module:delta/[base64($_GET['q'])]/CACHE=ROLE
  // Cache-per-page-role ID: esi/block/theme:region:module:delta/[base64($_GET['q'])]/CACHE=[rolehash]
  // Cache-per-user:         esi/block/theme:region:module:delta/CACHE=USER
  // Cache-per-user ID:      esi/block/theme:region:module:delta/CACHE=[userhash]
  // Cache-per-user-page:    esi/block/theme:region:module:delta/[base64($_GET['q'])]/CACHE=USER
  // Cache-per-user-page ID: esi/block/theme:region:module:delta/[base64($_GET['q'])]/CACHE=[userhash]


  // Get the original module/delta.
  list($module, $delta) = dpicache__new_delta($block->delta);
  // Build the "theme:region:module:delta" key.
  $component_key = implode(':', array(
      $block->theme,
      $block->region,
      $module,
      $delta,
  ));
  $url = "esi/block/{$component_key}";

  // Use the $block->cache parameter (as defined in the database) to determine
  // the caching rules for this block (per-user, per-role, etc).
  // The cache configuration is defined in hook_block_info(), and may be
  // altered through hook_block_info_alter().  The alter hook is the correct
  // method to specify a custom cache configuration which is different from
  // that defined in the original hook_block_info().
  // If the block changes per page, encode the page URL in the ESI URL.
  if ($block->cache & DRUPAL_CACHE_PER_PAGE) {
    $url .= '/' . base64_encode($_GET['q']);
  }

  if ($block->cache != DRUPAL_NO_CACHE) {
    // DRUPAL_CACHE_PER_ROLE and DRUPAL_CACHE_PER_USER are mutually exclusive.
    // DRUPAL_CACHE_PER_USER takes precedence.
    // Do not inject the actual roles or user data here; this string must not
    // contain any personalisation, if the current page is to be cacheable.
    if ($block->cache & DRUPAL_CACHE_PER_USER) {
      $url .= '/CACHE=USER';
    }
    elseif ($block->cache & DRUPAL_CACHE_PER_ROLE) {
      $url .= '/CACHE=ROLE';
    }
  }
  // Allow other modules to alter the ESI URL (or respond to it).
  // @see hook_dpicache_block_url_alter().
  drupal_alter('dpicache_block_url', $url);

  return $url;
}









