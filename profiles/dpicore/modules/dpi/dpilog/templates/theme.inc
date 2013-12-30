<?php

function theme_colorselect(&$variables){
  $id = $variables['element']['#id'];
  $variables["element"]["#attributes"]["onchange"] = "document.getElementById('".$id."_colorzone').style.backgroundColor = this.value;";
  $variables["element"]["#attributes"]["onkeypress"] = "document.getElementById('".$id."_colorzone').style.backgroundColor = this.value;";
  return theme_textfield($variables)."<div style='background-color:".$variables['element']['#value']."' id='".$id."_colorzone' class='colorselect_zone'></div>";
}

function theme_dpilog_form_page_configure_granularity_form(&$variables){
  $header = array('Action', 'DPI Log', 'Watchdog', 'Color');

  foreach(element_children($variables["form"]['registrations']) as $registration_name){
    $main_table=array();
    $main_table['header']=$header;
    foreach(element_children($variables["form"]['registrations'][$registration_name]['dpilog']['actions']) as $action_name ){
      $main_table['rows'][$action_name] = array(
        $action_name,
        drupal_render($variables["form"]['registrations'][$registration_name]['dpilog']['actions'][$action_name]),
        drupal_render($variables["form"]['registrations'][$registration_name]['watchdog']['actions'][$action_name]),
        drupal_render($variables["form"]['registrations'][$registration_name]['color']['actions'][$action_name]),
      );

      $variables["form"]['registrations'][$registration_name]['table']=array(
        "#markup"=>theme('table',$main_table),
        "#weight"=>-1,
      );
    }
  }

  return drupal_render_children($variables["form"]);
}

function theme_dpilog_form_page_configure_display_form(&$variables){
  $display_header = array('Action', 'Tag', 'color');

  foreach(element_children($variables["form"]['registrations']) as $registration_name){
    $vertical_tab_id=$registration_name.'-vertical_tab';

    $display_table=array();
    $display_table['header']=$display_header;

    foreach(element_children($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['display']['actions']) as $action_name ){
      $display_table['rows'][$action_name] = array(
        $action_name,
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['display']['actions'][$action_name]['display']['color']),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['display']['actions'][$action_name]['display']['tag']),
      );

      $variables["form"]['registrations'][$registration_name][$vertical_tab_id]['display']['table']=array(
        "#markup"=>theme('table',$display_table),
        "#weight"=>-1,
      );
    }
  }

  return drupal_render_children($variables["form"]);
}
