<?php
module_load_include('inc', 'dpilog','dpilog.private');

function dpilog_form_page_view_list($form,$form_state){

  $registrations=variable_get("dpilog_configuration_granularity",array());
  $display=variable_get("dpilog_configuration_display",array());;
  
  $form['content'] = array(
    "#markup"=>'totot',
    );
  return $form;
}


function dpilog_form_page_view_timeline($form,$form_state){

  $form['content'] = array(
    "#markup"=>'totot',
    );
  return $form;
}


function dpilog_form_page_configure_granularity_form($form, &$form_state){
  $config_registrations=dpi_variable_get("dpilog_configuration_granularity",array());
  $config_registrations=$config_registrations['registrations'];
  dsm($config_registrations);
  $registrations=_dpilog_get_modules_registration();
  foreach($registrations as $registration_name => $registration){
    
    $vertical_tab_id=$registration_name.'-vertical_tab';
    
    $form['registrations'][$registration_name ]=array(
      '#type'=>'fieldset',
      '#title'=>$registration_name,
      );
    $form['registrations'][$registration_name ][$vertical_tab_id]=array(
      '#type'=>'vertical_tabs',
      '#title'=>'Options',
    
    );
    $form['registrations'][$registration_name ][ $vertical_tab_id]['dpilog'] = array(
      '#type' => 'fieldset',
      '#title'=>'dpilog',
    
    );
    $form['registrations'][$registration_name ][ $vertical_tab_id]['watchdog'] = array(
      '#type' => 'fieldset',
      '#title'=>'watchdog',
    
    );
  
    foreach($registration["actions"] as $action_name){
      foreach(array_keys(_dpilog_get_log_levels()) as $level){
        $form['registrations'][$registration_name ][ $vertical_tab_id]['dpilog']['actions'][$action_name][$level]=array(
          '#type'=>'select',
          '#options'=>_dpilog_get_log_methods(),
          '#group'=> $vertical_tab_id,
          '#default_value'=>$config_registrations[$registration_name][$action_name]['dpilog'][$level],
          
          );
        $watchdog_options=_dpilog_get_log_methods();
        //We skip the delayed process untill we implement a custom a dpi watchdog replacement module
        unset($watchdog_options[1]);
        $form['registrations'][$registration_name ][ $vertical_tab_id]['watchdog']['actions'][$action_name][$level]=array(
          '#type'=>'select',
          '#options'=>$watchdog_options,
          '#group'=> $vertical_tab_id,
          '#default_value'=>$config_registrations[$registration_name][$action_name]['watchdog'][$level],
          
        );
        
        
      }
      
    }
  }
  
  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'dpilog') . '/css/dpilog.css',
    ),
    
  );
  $form["#tree"]=TRUE;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'save', 
    );
  return $form;
}

function dpilog_form_page_configure_granularity_form_submit($form, &$form_state){

  $conf['registrations']=array();
  $values=$form_state['values'];
  foreach($values['registrations'] as $registration_name =>$registration){

    foreach($values['registrations'][$registration_name][$registration_name.'-vertical_tab']['dpilog']['actions'] as $action_name =>$action){
      $actions[$action_name]['dpilog']=$action;
    }
    foreach($values['registrations'][$registration_name][$registration_name.'-vertical_tab']['watchdog']['actions'] as $action_name =>$action){
      $actions[$action_name]['watchdog']=$action;
    }
    $conf['registrations'][$registration_name]=$actions;
  }
  dpi_variable_set('dpilog_configuration_granularity',$conf);

}



function dpilog_form_page_configure_display_form($form, &$form_state){
  $registrations=_dpilog_get_modules_registration();
  foreach($registrations as $registration_name => $registration){

    $vertical_tab_id=$registration_name.'-vertical_tab';

    $form['registrations'][$registration_name ]=array(
      '#type'=>'fieldset',
      '#title'=>$registration_name,
    );
    $form['registrations'][$registration_name ][$vertical_tab_id]=array(
      '#type'=>'vertical_tabs',
      '#title'=>'Options',

    );
    
    $form['registrations'][$registration_name ][ $vertical_tab_id]['display'] = array(
      '#type' => 'fieldset',
      '#title'=>'Display',

    );
    foreach($registration["actions"] as $action_name){
      foreach(array_keys(_dpilog_get_log_levels()) as $level){
       

        $form['registrations'][$registration_name ][ $vertical_tab_id]['display']['actions'][$action_name]['timeline']['tag']=array(
          '#type'=>'select',
          '#title' => t('Timeline Tag'),
          "#description"=>t('Define the row in the timeline view'),
          '#options'=>array(1,2,3,4,5,6),
          '#group'=> $vertical_tab_id,
          "#weight"=>3,

        );

        $form['registrations'][$registration_name ][ $vertical_tab_id]['display']['actions'][$action_name]['timeline']['color']=array(
          '#type'=>'textfield',
          '#title' => t('Timeline Color'),
          "#description"=>t('Define the color of the log in the timeline view'),
          '#group'=> $vertical_tab_id,
          "#weight"=>3,
          "#size"=>5,
        );
      }

    }
  }

  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'dpilog') . '/css/dpilog.css',
    ),

  );
  $form["#tree"]=TRUE;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'save',
  );
  return $form;
}


function dpilog_form_page_configure_display_form_submit($form, &$form_state){

  $conf['registrations']=array();
  $values=$form_state['values'];
  foreach($values['registrations'] as $registration_name =>$registration){

    foreach($values['registrations'][$registration_name][$registration_name.'-vertical_tab']['display']['actions'] as $action_name =>$action){
      $display_actions[$action_name]=$action;
    }
    $conf['registrations'][$registration_name]['display']=$display_actions;
  }
  dpi_variable_set('dpilog_configuration_display',$conf);

}


function dpilog_form_page_flush($form,$form_state){

   $form['content'] = array(
    "#markup"=>'totot',
    );
  return $form;
}