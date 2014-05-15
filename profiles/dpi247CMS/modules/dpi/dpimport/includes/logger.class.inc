<?php 

/**
 * Static class for log management
 *
 * @author asl
 */
class Logger{
  private static $error = array();
  private static $warning = array();
  public static $execution = NULL;
  private static $currentexecution = NULL;

  /**
   * Add a Warning to the warning list
   *
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
   * Add an Error to the error list
   *
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
   * Create an ExecutionLog and add it to the current execution log. Set current if specified
   *
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
   * Initialize Execution
   */
  private static function initializeExecution(){
    self::$execution = new LoggerLog('base', 1, '000', 'OK');
    self::$currentexecution = self::$execution;
  }

  /**
   * Set the currentexecution to the previous execution
   */
  public static function setCurrentToPreviousExecution(){
    if (self::$currentexecution->getParent() !== NULL){
      self::$currentexecution = self::$currentexecution->getParent();
    }
  }

  /**
   * Return execution
   */
  public static function getLogExecution(){
    if (self::$execution == NULL){
      self::initializeExecution();
    }
    return self::$execution;
  }

  public static function getStatusOfLastLogExecution(){
    $status = 1;
    if (self::error()){
      $logexecution = self::getLogExecution();
      $last_element = end($logexecution->children);

      $status = $last_element->globalStatus();

    }
    return $status;
  }

  /**
   * Return the array of errors
   */
  public static function getLogError(){
    return self::$error;
  }

  /**
   * Return TRUE if an error is logged
   * @return boolean $error_exists
   */
  public static function error(){
    $error_exists = FALSE;
    foreach (self::$error as $loggerlog) {
      if ($loggerlog->getStatus() == 0) {
        $error_exists = TRUE;
        break;
      }
    }
    return $error_exists;
  }

  /**
   * Return the log themed in html
   */
  public static function printLog(){
    if (Logger::error()){
      //If an error appears during the import, the global status is set to 0
      $base = new LoggerLog('dpimport', 0, 'dpimport_error', t('There was an error during the import'));
    } else {
      //If no error appears during the import, the global status is set to 1
      $base = new LoggerLog('dpimport', 1, 'dpimport_success', t('The import was proceeded with success'));
    }
    //The execution log is attached to the base log.
    $execution = Logger::getLogExecution();
    $base->addChildren($execution);
    return theme('dpimport_log_recursif', array('children' => array($base)));
  }
}

/**
 * Class representing a Log
 *
 * @author asl
 */
class LoggerLog {

  public $type;
  public $status;
  public $code;
  public $message;
  public $children = NULL;
  private $parent = NULL;

  /**
   * Constructor
   *
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
   * Add a children to the log
   *
   * @param LoggerLog $executionlog
   */
  public function addChildren($executionlog){
    $this->children[] = $executionlog;
    $executionlog->setParent($this);
  }

  /**
   * Set parent to $parent
   *
   * @param LoggerLog $parent
   */
  public function setParent($parent){
    $this->parent = $parent;
  }

  /**
   * Return parent
   */
  public function getParent(){
    return $this->parent;
  }

  /**
   * Return status
   *
   * @return number
   */
  public function getStatus(){
    return $this->status;
  }

  /**
   * Return global status of a LoggerLog
   *
   * @return Integer $status
   */
  public function globalStatus(){
    if ($this->status == 0){
      $status = 0;
    } else {
      $status = $this->getChildrenStatus();
    }
    return $status;
  }

  public function getChildrenStatus(){
    $status = 1;
    foreach ($this->children as $logger){
      if ($logger->status == 0){
        $status = 0;
      } else {
        if ($logger->children != NULL){
          $status = $logger->getChildrenStatus();
        }
      }
    }
    return $status;
  }
}
