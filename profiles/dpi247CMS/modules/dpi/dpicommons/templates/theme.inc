<?php

/**
 * Theming fonction for DPI vertical tabs.
 * It's based on Drupal's vertical tab
 * except it works for any HTML content,
 * not just forms.
 * 
 * @param  Array $variables  An array like this
 *   array(
 *     array(
 *       'title' => "Tab title",
 *       'subtitle' => "Tab subtitle",
 *       'content' => "Pane HTML content"
 *     ),
 *     [...]
 *   )
 * @return String The HTML rendered vertical tabs.
 */
function theme_dpi_vertical_tabs($variables){
  if(!is_array($variables))
    return '';
  if(isset($variables['data']) && !empty($variables['data'])){
    // si on appelle theme(xxx,array($data))
    $data = $variables['data'];
  }
  else {
    // si on appelle theme(xxx, $data)
    $data = $variables;
    unset($data['data']);
  }
  // Add the CSS and JS
  $dpilog_path = drupal_get_path('module', 'dpicommons');
  drupal_add_css($dpilog_path.'/css/dpi_vertical_tabs.css','module', 'all', FALSE);
  drupal_add_js($dpilog_path .'/js/dpi_vertical_tabs.js',array('scope' => 'header', 'weight' => 5));

  $out  = "<div class='dpi_vertical_tabs clearfix'>\n";
  // Tabs
  $out .= "<ul class='dpi_vertical_tabs_menu'>\n";
  foreach($data as $index => $entry){
    $out .= "\t<li id='dpi_vertical_tabs_option_".$index."' class='dpi_vertical_tabs_option'>";
    $out .= "<a href='#dpi_vertical_tabs_content_".$index."'>";
    $out .= "<strong class='dpi_vertical_tabs_title'>".strip_tags($entry['title'])."</strong>";
    if(!isset($entry['subtitle']))
      $entry['subtitle'] = "&nbsp;";
    $out .= "<span class='dpi_vertical_tabs_subtitle'>".strip_tags($entry['subtitle'])."</span>";
    $out .= "</a>";
    $out .= "</li>\n";
  }
  $out .= "</ul>\n";
  // Panes
  $out .= "<div class='dpi_vertical_tabs_main_content'>\n";
  foreach($data as $index => $entry){
    $out .= "\t<div id='dpi_vertical_tabs_content_".$index."' class='dpi_vertical_tabs_content'>";
    $out .= "<h2 class='dpi_vertical_tabs_pane_title'>".strip_tags($entry['title'])."</h2>\n";
    $out .= $entry['content'];
    $out .= "</div>\n";
  }
  $out .= "</div>";
  $out .= "</div>";

  return $out;
}