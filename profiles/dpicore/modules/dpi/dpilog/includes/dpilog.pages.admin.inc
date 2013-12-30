<?php

module_load_include('inc', 'dpilog','dpilog.private');

function dpilog_form_page_log($lid){

}

function dpilog_page_view_list(){
  $filters=dpilog_page_view_list_build_filters();
  $filters_form=drupal_get_form("dpilog_page_view_list_filter_form");
  $logs_table=dpilog_page_view_list_build_logs_table($filters);

  $render_array['filters_form'] = array(
    '#type' => 'form',
    "form"=>$filters_form,
  );

  $render_array['content'] = array(
    '#type' => 'markup',
    "#markup"=> $logs_table,
  );

  $render_array['pager'] = array(
    '#type' => 'markup',
    "#markup"=>theme('pager', array('tags' => array())),
  );

  $render_array['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'dpilog') . '/css/dpilog-table.css',
    ),
  );

  return $render_array;
}

function dpilog_page_view_list_build_logs_table($filters) {
  $granularity = dpi_variable_get('dpilog_configuration_granularity', array());
  if(isset($granularity['registrations'])) {
    $registrations = $granularity['registrations'];
  }
  else {
    $registrations = array();
  }

  $log_levels = watchdog_severity_levels();
  $header_row = array(
    array('data' => ''),
    array('data' => t('Log Name'), 'field' => 'l.log_name'),
    array('data' => t('Action'), 'field' => 'l.action'),
    array('data' => t('Message'), 'field' => 'l.message'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Severity'), 'field' => 'l.severity'),
    array('data' => t('Thread Id'), 'field' => 'l.thread_id'),
    array('data' => t('ID'), 'field' => 'l.id'),
    array('data' => t('Time'), 'field' => 'l.timestamp', 'sort' => 'desc'),
  );

  $query = dpilog_get_logs_query_based_on_filters($filters);
  $query = $query
    ->extend('PagerDefault')->limit(30)
    ->extend('TableSort')
    ->orderByHeader($header_row);

  $results = $query->execute();
  $table_rows = array();
  foreach($results as $result) {
    $bck_color = $registrations[$result->log_name][$result->action]['color'];

    $message = truncate_utf8(filter_xss($result->message, array()), 150, TRUE, TRUE);
    $message = ' '.l($message, 'admin/dpi247/dpilogs/dpilog/view/log/'.$result->lid);

    $table_rows[] = array(
      array(
        'style' => array("background-color:$bck_color"),
        'class' => array('dpilog-table-row', 'dpilog-table-row-color'),
      ),
      array(
        'data' => $result->log_name,
        'class' => array('dpilog-table-row', 'dpilog-table-row-logname'),
      ),
      array(
        'data' => $result->action,
        'class' => array('dpilog-table-row', 'dpilog-table-row-action'),
      ),
      array(
        'data' => $message,
        'class' => array('dpilog-table-row', 'dpilog-table-row-message'),
      ),
      array(
        'data' => $result->name,
        'class'=>array('dpilog-table-row', 'dpilog-table-row-user'),
      ),
      array(
        'data' => $log_levels[$result->severity],
        'class' => array('dpilog-table-row', 'dpilog-table-row-severity'),
      ),
      array(
        'data' => $result->thread_id,
        'class' => array('dpilog-table-row', 'dpilog-table-row-thread_id'),
      ),
      array(
        'data' => $result->id,
        'class' => array('dpilog-table-row', 'dpilog-table-row-id'),
      ),
      array(
        'data' => date("Y-m-d H:i:s", $result->timestamp),
        'class' => array('dpilog-table-row', 'dpilog-table-row-timestamp'),
      ),
    );
  }

  $content = theme_table(array('header' => $header_row,
    'rows' => $table_rows,
    'attributes' => array('class' => 'table_dpilogs', 'width' => '100%'),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('No data is available'),
  ));

  return $content;
}

function dpilog_page_view_timeline_json(){
  $filters=dpilog_page_view_list_build_filters();

  $query=dpilog_get_logs_query_based_on_filters($filters);
  $results = $query->execute();

  $config_display=dpi_variable_get("dpilog_configuration_display",array());;
  $config_display=$config_display['registrations'];

  $dates=array();
  foreach($results as $result){
    dsm($result);

    if(isset( $config_display[$result->log_name]['display'][$result->action]['display']['color'])){
      $color=$config_display[$result->log_name]['display'][$result->action]['display']['color'];
      $color=substr($color,1);
      $tag=$config_display[$result->log_name]['display'][$result->action]['display']['tag'];
    }

    $message=truncate_utf8(filter_xss($result->message, array()), 150, TRUE, TRUE);
    $title=truncate_utf8(filter_xss($result->log_name.'-'.$result->action, array()), 18, TRUE, TRUE);

    $dates[]=array(
      'startDate'=>date("Y,m,d,H,i,s",$result->timestamp),
      //"endDate"=>date("Y,m,d,H,i,s",$result->timestamp),
      "headline"=> $title,
      "text"=>$message,
      "tag"=>$tag,
      "asset"=>array(
        "media"=>"http://dummyimage.com/420x80/".$color."/e1e2f7.png&text=".$result->log_name.'+'.$result->action,
        //   "media"=>"http://www.lesoir.be/2/sports",
        "thumbnail"=>"http://dummyimage.com/32x32/".$color."/e1e2f7.png&text=".$result->log_name.'+'.$result->action,
        "credit"=>"Credit Name Goes Here",
        "caption"=>"Caption <strong>Am I able to put something here ?</strong> goes here How long could it be ? any long tht I need ? ".$result->message
      ),
    );
  }

  $array=array('timeline'=>
    array(
      'headline'=>'Headline DEfault',
      'type'=>'default',
      'text'=>'<p>Intro body text goes here, some HTML is ok</p>',
      'asset'=>array(
        'media'=>"http://dummyimage.com/420x420/c23023/e1e2f7.png&text=DPI+Logs",
        'credit'=>'dpi247.com all right reserved',
        'caption'=> "Caption goes here",
      ),
      "date"=>$dates,
    ),
  );
  return drupal_json_output($array);
}

function dpilog_page_view_timeline(){
  global $language;
  // Données pour la timeline
  $query = db_select('dpilog_logs', 'l');
  $query->fields('l', array('log_name', 'action','timestamp'));
  $query->orderBy('timestamp', 'DESC');
  $query->range(0,1000);
  $results = $query->execute();
  $data = "";
  foreach($results as $result){
    $data .= "[new Date(".($result->timestamp*1000)."),, '".$result->log_name."', '".$result->action."', 'maclasse'],\n";
  }
  if($data == '')
    return t("No data available");

  // Ressources requises
  $dpilog_path = drupal_get_path('module','dpilog');
  drupal_add_css($dpilog_path.'/css/timeline.css','module', 'all', FALSE);
  drupal_add_js('http://www.google.com/jsapi',array('type'=>'external','scope' => 'header', 'weight' => 1));
  drupal_add_js($dpilog_path .'/js/timeline-min.js',array('scope' => 'header', 'weight' => 15));
  if($language->language && $language->language != 'en')
    drupal_add_js($dpilog_path .'/js/timeline-locales.js',array('scope' => 'header', 'weight' => 16));

  // Javascript pour générer la timeline
  // http://almende.github.io/chap-links-library/timeline.html
  $output = " <script type='text/javascript'>
        var timeline;
        google.load('visualization', '1');

        // Set callback to run when API is loaded
        google.setOnLoadCallback(drawVisualization);

        // Called when the Visualization API is loaded.
        function drawVisualization() {
            // Create and populate a data table.
            var data = new google.visualization.DataTable();
            data.addColumn('datetime', 'start');
            data.addColumn('datetime', 'end');
            data.addColumn('string', 'content');
            data.addColumn('string', 'group');
            data.addColumn('string', 'className');

            data.addRows([".$data."]);

            // specify options
            var options = {
                width: '100%',
                height: '300px',
                style: 'box',
                eventMargin: 5,  // minimal margin between events
                //eventMarginAxis: 0, // minimal margin beteen events and the axis
                showMajorLabels: true,
                axisOnTop: false,
                groupsChangeable : true,
                groupsOnRight: false,
                cluster: true,
                locale: '".($language->language ?: 'en')."'
            };

            // Instantiate our timeline object.
            timeline = new links.Timeline(document.getElementById('mytimeline'));

            // Draw our timeline with the created data and options
            timeline.draw(data, options);
        }
    </script>
    <div id='mytimeline'></div>";

  return $output;
}

function dpilog_page_view_list_build_filters($form,$form_state){
  $filters = isset($_SESSION['dpilog_filters']) ? $_SESSION['dpilog_filters'] : array();

  //$filters['log_name']=array('dpidestinations_extract');

  return $filters;
}

function dpilog_page_view_list_filter_form(){
  $registrations=dpi_variable_get("dpilog_configuration_granularity",array());
  $registrations=$registrations['registrations'];

  $filters=dpilog_page_view_list_build_filters($form,$form_state);
  $actions_options=array();
  foreach($registrations as $registration_name=>$registration){
    $log_name_options[$registration_name]=$registration_name;
    $action_key[$registration_name]=array();
    foreach($registration as $action_name=>$action){
      $action_key[$registration_name]["$registration_name"."[".$action_name."]"]=$action_name;
    }
    $actions_options=$action_key;
  }

  $form=array(
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['default']=array(
    '#type'=>'fieldset',
    '#title'=>'Filter',
    '#collapsible' => TRUE,
    '#collapsed' => empty($_SESSION['dpilog_filters']),
  );

  $form['default']['log_name']=array(
    '#type'=>'select',
    '#options'=>$log_name_options,
    '#title'=>t('Log Name'),
    '#multiple'=>TRUE,
    '#default_value'=>isset($filters['log_name'])?$filters['log_name']:'',
    '#rows'=>10,
  );

  $form['default']['action']=array(
    '#type'=>'select',
    '#options'=>$actions_options,
    '#title'=>t('Action'),
    '#multiple'=>TRUE,
    '#default_value'=>isset($filters['action'])?array_keys($filters['action']):'',
    '#size'=>10,
    // '#attributes' => array('class' => array('container-inline')),
  );

  $form['default']['user']=array(
    '#type'=>'textfield',
    '#title'=>t('User'),
    '#default_value'=>isset($filters['user'])?$filters['user']:'',
  );

  $form['default']['id']=array(
    '#type'=>'textfield',
    '#title'=>t('Id'),
    '#default_value'=>isset($filters['id'])?$filters['id']:'',
    '#size'=>10,
  );

  $form['default']['thread_id']=array(
    '#type'=>'textfield',
    '#title'=>t('Thread id'),
    '#default_value'=>isset($filters['thread_id'])?$filters['thread_id']:'',
    '#size'=>10,
  );

  $form['default']['severity']=array(
    '#type'=>'select',
    "#options"=>watchdog_severity_levels(),
    '#title'=>t('Severity'),
    '#default_value'=>isset($filters['severity'])?$filters['severity']:'',
    '#size'=>8,
    '#multiple'=>TRUE,
  );

  $form['default']['date']=array(
    '#type'=>'fieldset',
  );

  $form['default']['date']['before_timestamp']=array(
    '#title'=>t('before'),
    '#type' => 'date_popup',
    '#default_value'=>isset($filters['before_timestamp'])&&$filters['before_timestamp']!=""?date('Y-m-d h:i',$filters['before_timestamp']):'',
    '#date_format' => 'Y-m-d h:i A',
  );

  $form['default']['date']['after_timestamp']=array(
    '#title'=>t('after'),
    '#type' => 'date_popup',
    '#default_value'=>isset($filters['after_timestamp'])&&$filters['after_timestamp']!=""?date('Y-m-d h:i',$filters['after_timestamp']):'',
    '#date_format' => 'Y-m-d h:i A',
  );

  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'dpilog') . '/css/dpilog-filter.css',
    ),
  );

  $form['default']['actions'] = array('#type' => 'actions');

  $form['default']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Refine'),
  );

  $form['default']['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

function dpilog_page_view_list_filter_form_submit($form, & $form_state){
  $op = $form_state['values']['op'];

  switch ($op) {
    case t('Refine'):
     foreach($form_state['values'] as $name=>$value){
        //if($value!="" && $value!==array()){
          switch($name){
            case 'before_timestamp':
            case 'after_timestamp':
              $_SESSION['dpilog_filters'][$name] = strtotime($value);
              break;
            case 'action':
              foreach($form_state['values'][$name] as $encoded_action){
                $action=substr(strstr($encoded_action, '['), 1, -1);
                $actions[$encoded_action]=$action;
              }
              $_SESSION['dpilog_filters'][$name] = $actions;
              
              break;
            default:
              $_SESSION['dpilog_filters'][$name] = $form_state['values'][$name];
            break;
         // }
        }
     }
     break;
    case t('Reset'):
      $_SESSION['dpilog_filters'] = array();
      break;
  }
}

/**
 * List node administration filters that can be applied.
 *
 * @return
 *   An associative array of filters.
 */
function dpilog_log_filters() {
  // Regular filters
  $filters['status'] = array(
    'title' => t('status'),
    'options' => array(
      '[any]' => t('any'),
      'status-1' => t('published'),
      'status-0' => t('not published'),
      'promote-1' => t('promoted'),
      'promote-0' => t('not promoted'),
      'sticky-1' => t('sticky'),
      'sticky-0' => t('not sticky'),
    ),
  );
  // Include translation states if we have this module enabled
  if (module_exists('translation')) {
    $filters['status']['options'] += array(
      'translate-0' => t('Up to date translation'),
      'translate-1' => t('Outdated translation'),
    );
  }

  $filters['type'] = array(
    'title' => t('type'),
    'options' => array(
      '[any]' => t('any'),
    ) + node_type_get_names(),
  );

  // Language filter if there is a list of languages
  if ($languages = module_invoke('locale', 'language_list')) {
    $languages = array(LANGUAGE_NONE => t('Language neutral')) + $languages;
    $filters['language'] = array(
      'title' => t('language'),
      'options' => array(
        '[any]' => t('any'),
      ) + $languages,
    );
  }
  return $filters;
}

function dpilog_form_page_view_timeline($form,$form_state){
  $form['content'] = array(
    "#markup"=>'totot',
  );

  return $form;
}

function dpilog_form_page_configure_granularity_form($form, &$form_state){
  $config_registrations=dpi_variable_get("dpilog_configuration_granularity",array());
  if(!empty($config_registrations))
    $config_registrations=$config_registrations['registrations'];
  $registrations=_dpilog_get_modules_registration();

  foreach($registrations as $registration_name => $registration){ // cache_entry, cache_error
    $form['registrations'][$registration_name ]=array(
      '#type'=>'fieldset',
      '#title'=>$registration_name,
    );

    foreach($registration["actions"] as $action_name){ // cache_hit, cache_miss...
      // DPI Logs
      $form['registrations'][$registration_name]['dpilog']['actions'][$action_name]=array(
        '#type'=>'select',
        '#options'=>_dpilog_get_dpilogs_actions(),
        '#group'=> $action_name,
        '#default_value'=>@$config_registrations[$registration_name][$action_name]['dpilog'] ?: 'ignore',
      );

      // DPI Watchdogs
      $form['registrations'][$registration_name]['watchdog']['actions'][$action_name]=array(
        '#type'=>'select',
        '#options'=>_dpilog_get_watchdogs_actions(),
        '#group'=> $action_name,
        '#default_value'=>@$config_registrations[$registration_name][$action_name]['watchdog'] ?: 'ignore',
      );

      // DPI Logs
      $form['registrations'][$registration_name]['color']['actions'][$action_name]=array(
        '#type'=>'textfield',
        '#size'=>7,
        '#maxlength' => 7,
        '#group'=> $action_name,
        '#theme'=> 'colorselect',
        '#attributes' => array("class"=>array('colorselect')),
        '#default_value'=>@$config_registrations[$registration_name][$action_name]['color'] ?: '', //_dpilog_random_color(),
      );
    }
  }

  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'dpilog') . '/css/dpilog.css'
    )
  );

  $form["#tree"]=TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'save',
  );

  return $form;
}

function dpilog_form_page_configure_granularity_form_validate(&$form, &$form_state) {
  foreach($form_state['values']['registrations'] as $registration_name => $registration){
    $colors = $registration["color"]["actions"];
    foreach($colors as $cat => $color_value){
      if($color_value != '' && !preg_match('/^#[0-9A-F]{3}([0-9A-F]{3})*$/i', $color_value)){
        form_set_error('registrations]['.$registration_name.'][color][actions]['.$cat, t('Color value must be a hexadecimal value, or empty.'));
      }
    }
  }
}

function dpilog_form_page_configure_granularity_form_submit($form, &$form_state){
  $conf['registrations']=array();
  $values=$form_state['values'];
  foreach($values['registrations'] as $registration_name =>$registration){
    foreach($values['registrations'][$registration_name]['dpilog']['actions'] as $action_name =>$action){
      $actions[$action_name]['dpilog']=$action;
    }
    foreach($values['registrations'][$registration_name]['watchdog']['actions'] as $action_name =>$action){
      $actions[$action_name]['watchdog']=$action;
    }
    foreach($values['registrations'][$registration_name]['color']['actions'] as $action_name =>$color){
      $actions[$action_name]['color']=$color;
    }
    $conf['registrations'][$registration_name]=$actions;
    $actions=array();
  }
  dpi_variable_set('dpilog_configuration_granularity',$conf);
}

function dpilog_form_page_flush($form,$form_state){
  $form['content'] = array(
    "#markup"=>'totot',
  );
   
  return $form;
}