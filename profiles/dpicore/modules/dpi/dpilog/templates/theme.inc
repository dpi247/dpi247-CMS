<?php


function theme_dpilog_form_page_configure_granularity_form(&$variables){

  $header = array('Action', 'EMERGENCY', 'ALERT', 'CRITICAL', 'ERROR', 'WARNING', 'NOTICE', 'INFO', 'DEBUG');

  foreach(element_children($variables["form"]['registrations']) as $registration_name){
    $vertical_tab_id=$registration_name.'-vertical_tab';
    $dpilog_table=array();
    $watchdog_table=array();
    $dpilog_table['header']=$header;
    $watchdog_table['header']=$header;
    $display_table['header']=$display_header;
    foreach(element_children($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions']) as $action_name ){
      $dpilog_table['rows'][$action_name] = array(
        $action_name,

        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][0]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][1]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][2]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][3]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][4]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][5]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][6]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['actions'][$action_name][7]),
      );
      $watchdog_table['rows'][$action_name] = array(
        $action_name,
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][0]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][1]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][2]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][3]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][4]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][5]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][6]),
        drupal_render($variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['actions'][$action_name][7]),

      );


      $variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['dpilog']['table']=array(
        "#markup"=>theme('table',$dpilog_table),
        "#weight"=>-1,

      );

       
      $variables["form"]['registrations'][$registration_name ][$vertical_tab_id]['watchdog']['table']=array(
        "#markup"=>theme('table',$watchdog_table),
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