<?php

$plugin = array(
  'renderer' => 'dpidestinations_renderer_viewer',
);

/**
 * @file
* Class file to control the main Panels editor.
*/
class dpidestinations_renderer_viewer extends panels_renderer_standard {

  /**
   * Render the interior contents of a single pane.
   *
   * This method retrieves pane content and produces a ready-to-render content
   * object. It also manages pane-specific caching.
   *
   * @param stdClass $pane
   *   A Panels pane object, as loaded from the database.
   * @return stdClass $content
   *   A renderable object, containing a subject, content, etc. Based on the
   *   renderable objects used by the block system.
   */
  function render_pane_content(&$pane) {
    ctools_include('context');
    // TODO finally safe to remove this check?
    if (!is_array($this->display->context)) {
      watchdog('panels', 'renderer::render_pane_content() hit with a non-array for the context', $this->display, WATCHDOG_DEBUG);
      $this->display->context = array();
    }

    $content = FALSE;
    $caching = !empty($pane->cache['method']) && empty($this->display->skip_cache);
    if ($caching && ($cache = panels_get_cached_content($this->display, $this->display->args, $this->display->context, $pane))) {
      $content = $cache->content;
    }
    else {
      if ($caching) {
        // This is created before rendering so that calls to drupal_add_js
        // and drupal_add_css will be captured.
        $cache = new panels_cache_object();
      }

      $pane->configuration['pane']=$pane;
      $content = ctools_content_render($pane->type, $pane->subtype, $pane->configuration, array(), $this->display->args, $this->display->context);

      if (empty($content)) {
        return;
      }

      foreach (module_implements('panels_pane_content_alter') as $module) {
        $function = $module . '_panels_pane_content_alter';
        $function($content, $pane, $this->display->args, $this->display->context, $this, $this->display);
      }
      if ($caching && isset($cache)) {
        $cache->set_content($content);
        panels_set_cached_content($cache, $this->display, $this->display->args, $this->display->context, $pane);
        $content = $cache->content;
      }
    }

    // Pass long the css_id that is usually available.
    if (!empty($pane->css['css_id'])) {
      $content->css_id = check_plain($pane->css['css_id']);
    }

    // Pass long the css_class that is usually available.
    if (!empty($pane->css['css_class'])) {
      $content->css_class = check_plain($pane->css['css_class']);
    }

    return $content;
  }

}