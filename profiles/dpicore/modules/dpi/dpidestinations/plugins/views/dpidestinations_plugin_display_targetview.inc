<?php

/**
 * The plugin that handles a panel_pane.
 */
class dpidestinations_plugin_display_targetview extends views_plugin_display {
  /**
   * If this variable is true, this display counts as a panel pane. We use
   * this variable so that other modules can create alternate pane displays.
   */


  /**
   * Render this display.
   */
  function render() {
    return theme($this->theme_functions(), array('view' => $this->view));
  }



  /**
   * Get the instance of a plugin, for example style or row.
   *
   * @param string $type
   *   The type of the plugin.
   * @param string $name
   *   The name of the plugin defined in hook_views_plugins.
   *
   * @return views_plugin|FALSE
   */
  function get_plugin($type = 'style', $name = NULL) {
    if($type=='row'){
      static $cache = array();
      if (!isset($cache[$type][$name])) {
        $option_name = $type . '_plugin';
        $options = $this->get_option($type . '_options');
        if (!$name) {
          $name = $this->get_option($option_name);
        }
        $plugin = views_get_plugin($type, $name);

        if (!$plugin) {
          return;
        }
        $plugin->init($this->view, $this->display, $options);

        $cache[$type][$name] = $plugin;
      }
      return $cache[$type][$name];
    }
    else{
      return parent::get_plugin($type,$name);
    }
  }
  

}
