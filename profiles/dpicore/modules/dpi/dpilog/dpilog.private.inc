<?php
function dpilog_get_logs_query_based_on_filters( $filters){



  $query = db_select('dpilog_logs', 'l');
  $query->join('users', 'u', 'l.user = u.uid'); //JOIN node with users
  $query->fields('l', array('lid','log_name', 'action','message','user', 'timestamp','id','severity','thread_id'));
  $query->fields('u', array('name'));
  $query = $query
  ->extend('PagerDefault')->limit(30);


  foreach($filters as $filter_name=>$filter_value){
    if($filter_value!='' && $filter_value!=array()){
      switch($filter_name){
        case 'log_name':
          $query->condition('l.log_name', $filter_value,'IN');
          break;
        case 'action':
          $query->condition('l.action', $filter_value, 'IN');
          break;
        case 'id':
          $query->condition('l.id', $filter_value,'=');
          break;
        case 'user':
          $query->condition('l.user', $filter_value,'=');
          break;
        case 'thread_id':
          $query->condition('l.thread_id', $filter_value,'=');
          break;
        case 'before_timestamp':
          $query->condition('l.timestamp', $filter_value,'<');
          break;
        case 'after_timestamp':
          $query->condition('l.timestamp', $filter_value,'>');
          break;
        case 'id':
          $query->condition('l.id', $filter_value,'IN');
          break;
        case 'severity':
          $query->condition('l.severity', $filter_value,'IN');
          break;
      }
    }
  }
  return  $query;
}

// We dont plan to use Mysql delayed because of: https://drupal.org/node/772554

function _dpilog_log($log_name,$action,$message,$severity=DPILOG_SEVERITY_DEFAULT,$uid=NULL, $id=NULL,$variables=array(),$keep_backtrace=FALSE){

  $granularity=dpi_variable_get('dpilog_configuration_granularity',array());
  $registrations=$granularity['registrations'];
  if(isset($registrations[$log_name]) and isset($registrations[$log_name][$action])){

    $dpilog_method=$registrations[$log_name][$action]['dpilog'][$severity];
    $watchdog_method=$registrations[$log_name][$action]['watchdog'][$severity];


    $dpilog_structure=NULL;

    if($dpilog_method>0){
      $dpilog_structure=_dpilog_format_dpilog_structure($log_name, $action, $message, $severity, $uid,$id,$variables,$keep_backtrace);
      if($dpilog_method==1){
        _dpilog_delayed_save($dpilog_structure,'dpilog');
      }
      elseif($dpilog_method==2){

        drupal_write_record('dpilog_logs', $dpilog_structure);
      }
    }
     
    if($watchdog_method>0){
      $watchdog_structure=_dpilog_format_watchdog_structure($log_name, $action, $message, $severity, $uid,$id,$variables,$keep_backtrace,$dpilog_structure);
      if($watchdog_method==1){
        //NOT implemented
      }
      elseif($watchdog_method=2){
        watchdog($watchdog_structure->type,$watchdog_structure->message,$watchdog_structure->variables,$watchdog_structure->severity,$watchdog_structure->link);
      }
    }

  }


}
function _dpilog_format_dpilog_structure($log_name,$action,$message,$severity=DPILOG_SEVERITY_DEFAULT,$uid=NULL, $id=NULL,$variables=array(),$keep_backtrace=FALSE){
  global $dpilog_thread_id;
  global $user;

  if(!isset($uid)){
    $uid=$user->uid;
  }
  if(!isset($dpilog_log->timestamp)){
    $timestamp=time();

  }

  $dpilog_log=new stdClass();
  $dpilog_log->log_name=$log_name;
  $dpilog_log->action=$action;

  $dpilog_log->user=$uid;
  $dpilog_log->message=$message;
  $dpilog_log->id=$id;
  $dpilog_log->variables=serialize($variables);
  $dpilog_log->severity= $severity;
  $dpilog_log->timestamp=$timestamp;
  if($keep_backtrace){
    $dpilog_log->trace=serialize(debug_backtrace());
  }
  else{
    $dpilog_log->trace=NULL;
  }
  $dpilog_log->thread_id=$dpilog_thread_id;

  return $dpilog_log;

}

function _dpilog_get_modules_registration(){

  $definitions=array();
  $registrations=module_invoke_all('dpilog_register');
  return $registrations;

}


function _dpilog_get_log_levels(){

  return array(
    WATCHDOG_DEBUG     => 'dblog-debug',
    WATCHDOG_INFO      => 'dblog-info',
    WATCHDOG_NOTICE    => 'dblog-notice',
    WATCHDOG_WARNING   => 'dblog-warning',
    WATCHDOG_ERROR     => 'dblog-error',
    WATCHDOG_CRITICAL  => 'dblog-critical',
    WATCHDOG_ALERT     => 'dblog-alert',
    WATCHDOG_EMERGENCY => 'dblog-emerg',
    );

}

function _dpilog_get_log_methods(){

  return array(
    0=>t("Do nothing"),
    1=>t("Delayed Save"),
    2=>t("Direct Save"),
  );

}
function _dpilog_format_watchdog_structure($log_name, $action, $message, $severity, $uid,$id,$variables,$keep_backtrace,$dpilog_structure){

  $watchdog->type=$log_name.'-'.$action;
  $watchdog->message=$message;
  $watchdog->variables=$variables;
  $watchdog->severity=$severity;
  $watchdog->link=NULL;
  if(isset($dpilog_structure->lid)){
    $watchdog->link=l('dpilog','admin/dpilog/log_message/'.$dpilog_structure->lid). ' '.l('dpilog','admin/dpilog/log_message/'.$dpilog_structure->lid);
  }
  return $watchdog;
}

function _dpilog_delayed_save($dpilog_structure,$structure){

  global $dpilog_delayed_save;

  $dpilog_delayed_save[$structure][]=$dpilog_structure;


}



function _dpilog_random_color_part() {
  return str_pad( dechex( mt_rand( 0, 255 ) ), 2, '0', STR_PAD_LEFT);
}

function _dpilog_random_color() {
  return "#"._dpilog_random_color_part() . _dpilog_random_color_part() . _dpilog_random_color_part();
}
