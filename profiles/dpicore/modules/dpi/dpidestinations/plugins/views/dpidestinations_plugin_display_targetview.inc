<?php

/**
 * The plugin that handles a panel_pane.
 */
class dpidestinations_plugin_display_targetview extends views_plugin_display {


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
  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
  
    $option_form=parent::options_form($form, $form_state);
    if($form_state['section']=="row_plugin"){
      foreach($form['row_plugin']["#options"] as $key =>$value){
        if($key!='targetblock_node_row' and $key!='fields'){
          unset($form['row_plugin']["#options"][$key]);
        }
        
      }
    }
    return $options;
  }
  
}
