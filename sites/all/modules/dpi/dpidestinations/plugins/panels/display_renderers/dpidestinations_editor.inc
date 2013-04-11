<?php

$plugin = array(
  'renderer' => 'dpidestinations_renderer_editor',
);
/**
 * @file
 * Class file to control the main Panels editor.
 */

class dpidestinations_renderer_editor extends panels_renderer_editor {

  /**
   *
   *
   */
  
  
  
  /**
   * AJAX command to present a dialog with a list of available content.
   */
  function ajax_select_content($region = NULL, $category = NULL, $group = 'dpi-core') {
    if (!array_key_exists($region, $this->plugins['layout']['regions'])) {
      ctools_modal_render(t('Error'), t('Invalid input'));
    }
  
    $title = t('Add content to !s', array('!s' => $this->plugins['layout']['regions'][$region]));
    
    $categories = $this->get_categories($this->cache->content_types);
    $groups = $this->get_groups($this->cache->content_types);
    $categories=$groups[$group]['categories'];
    
    if (empty($groups)) {
      $output = t('There are no content types you may add to this display.');
    }
    else {
      $output = theme('dpidestinations_add_content_modal', array('renderer' => $this, 'groups' => $groups,'categories'=>$categories, 'category' => $category,'group' => $group, 'region' => $region));
    }
    $this->commands[] = ctools_modal_command_display($title, $output);
  }
  
  /**
   * Create a list of groups from all of the content type.
   *
   * @return array
   *   An array of groups. Each entry in the array contains an arry of categories 
   *   with 'title' as the printable title of the category, and 'content'
   *   of all content in the category. Each item in the 'content'
   *   array contain the array plugin definition so that it can be later
   *   found in the content array. They will be keyed by the title so that they
   *   can be sorted.
   */
  function get_groups($content_types){
    $groups = array();
    $group_names = array();
    
    
    
    foreach ($content_types as $type_name => $subtypes) {
      foreach ($subtypes as $subtype_name => $content_type) {
        list($group_key, $group) = $this->get_group($content_type);
    
        if (empty($groups[$group_key])) {
          $groups[$group_key] = array(
            'title' => $group,
            'categories' => array(),
          );
          $group_names[$group_key] = $group;
          $groups[$group_key]['contents'][$type_name]=$subtypes;
          
        }
    
        $content_title = filter_xss_admin($content_type['title']);
    
        // Ensure content with the same title doesn't overwrite each other.
        while (isset($groups[$group_key]['content'][$content_title])) {
          $content_title .= '-';
        }
        $groups[$group_key]['content'][$type_name][$content_title] = $content_type;
        $groups[$group_key]['content'][$type_name][$content_title]['type_name'] = $type_name;
        $groups[$group_key]['content'][$type_name][$content_title]['subtype_name'] = $subtype_name;
      }
    }
    foreach($groups as $group_key=>$group){
      $groups[$group_key]['categories']=$this->get_categories($groups[$group_key]['content']);
      unset($groups[$group_key]['content']);
      
    }
    
    // Now sort
    natcasesort($group_names);
    foreach ($group_names as $group => $name) {
      $output[$group] = $groups[$group];
    }
    return $output;
    
    
  }
  /**
   * Return the group name and the group key of a given content
   * type.
   *
   */
  
  function get_group($content_type){
      if (isset($content_type['group'])) {
        if (is_array($content_type['group'])) {
          list($group, $weight) = $content_type['group'];
        }
        else {
          $group = $content_type['group'];
        }
      }
      else {
        $group = t('Drupal-core');
      }
    
      return array(preg_replace('/[^a-z0-9]/', '-', strtolower($group)), $group);
    }
}