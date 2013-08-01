<?php 
/**
 * 
 * Static class for log management
 * @author asl
 *
 */
class Logger{
  private static $error = array();
  public static function logWarning(){
    ;
  }
  public static function logError($type, $status, $code, $message){
    self::$error[$type][] = array('status' => $status, 'code' => $code, 'message' => $message);
  }
  public static function getLogError(){
    return self::$error;
  }
  public static function printLog(){
    return 'print log render';
  }
}