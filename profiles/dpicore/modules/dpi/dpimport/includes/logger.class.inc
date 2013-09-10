<?php 
/**
 * 
 * Static class for log management
 * @author asl
 *
 */
class Logger{
  private static $error = array();
  private static $warning = array();
  public static $execution = NULL;
  private static $currentexecution = NULL;
  
  /**
   * 
   * Add a Warning to the warning list
   * @param String $type
   * @param Integer $status
   * @param String $code
   * @param String $message
   */
  public static function logWarning($type, $status, $code, $message){
    self::$warning[] = new LoggerLog($type, $status, $code, $message);
    self::logExecution($type, FALSE, $status, $code, $message);
  }
  
  /**
   * 
   * Add an Error to the error list
   * @param String $type
   * @param Integer $status
   * @param String $code
   * @param String $message
   */
  public static function logError($type, $status, $code, $message){
    self::$error[] = new LoggerLog($type, $status, $code, $message);
    self::logExecution($type, FALSE, $status, $code, $message);
  }
  
  /**
   * 
   * Create an ExecutionLog and add it to the current execution log. Set current if specified
   * @param String $type
   * @param Boolean $current
   * @param Integer $status
   * @param String $code
   * @param String $message
   */
  public static function logExecution($type, $current = FALSE, $status = 1, $code = '000', $message = 'OK'){
    if (self::$execution == NULL){
      self::initializeExecution();
    }
    $executionlog = new LoggerLog($type, $status, $code, $message);
    self::$currentexecution->addChildren($executionlog);
    if ($current){
      self::$currentexecution = $executionlog;
    }
  }
  
  /**
   * 
   * Initialize Execution
   */
  private static function initializeExecution(){
    self::$execution = new LoggerLog('base', 1, '000', 'OK');
    self::$currentexecution = self::$execution;
  }
  
  /**
   * 
   * Set the currentexecution to the previous execution
   */
  public static function setCurrentToPreviousExecution(){
    if (self::$currentexecution->getParent() !== NULL){
      self::$currentexecution = self::$currentexecution->getParent();
    }
  }
  
  /**
   * 
   * Return execution
   */
  public static function getLogExecution(){
    return self::$execution;
  }
  /**
   * 
   * Return the array of errors
   */
  public static function getLogError(){
    return self::$error;
  }
  /**
   * 
   * Return TRUE if an error is logged
   * @return boolean $error_exists
   */
  public static function error(){
    $error_exists = FALSE;
    foreach (self::$error as $loggerlog){
      if ($loggerlog->getStatus == 0){
        $error_exists = TRUE;
        break;
      }
    }
    return $error_exists;
  }
  public static function printLog(){
    return 'print log render';
  }
}
/**
 * 
 * Class representing a Log
 * @author asl
 *
 */
class LoggerLog{
  
  public $type;
  public $status;
  public $code;
  public $message;
  public $children = NULL;
  public $parent = NULL;
  /**
   * 
   * Constructor
   * @param String $type
   * @param Integer $status
   * @param String $code
   * @param String $message
   */
  function LoggerLog($type, $status, $code, $message){
    $this->type = $type;
    $this->status = $status;
    $this->code = $code;
    $this->message = $message;
  }
  /**
   * 
   * Add a children to the log
   * @param LoggerLog $executionlog
   */
  public function addChildren($executionlog){
    $this->children[] = $executionlog;
    $executionlog->setParent($this);
  }
  /**
   * 
   * Set parent to $parent
   * @param LoggerLog $parent
   */
  public function setParent($parent){
    $this->parent = $parent;
  }
  
  /**
   * 
   * Return parent
   */
  public function getParent(){
    return $this->parent;
  }
  /**
   * 
   * Return status
   * @return number
   */
  public function getStatus(){
    return $this->status;
  }
}