<?php 

abstract class Importer{
  
  protected $type; 
  protected $version;
  protected $name;
  protected $dispatcher_class;
  protected $dispatcher_settings;
  protected $dispatcher;
  
  function Importer($settings, $plugin){
    $this->type = isset($settings->type) ? $settings->type : NULL;
    $this->version = isset($settings->version) ? $settings->version : NULL;
    $this->name = isset($settings->name) ? $settings->name : NULL;
    $this->dispatcher_class = isset($settings->dispatcher) ? $settings->dispatcher : NULL;
    $this->dispatcher_settings = isset($settings->dispatcher_settings) ? unserialize($settings->dispatcher_settings) : array();
    
    if ($this->getDispatcherClass() != NULL){
      $dispatcher_class = $this->getDispatcherClass();
      $dispatcher = new $dispatcher_class($this, $this->getDispatcherSettings());
      $this->dispatcher = $dispatcher;
    }
    
  }
  
  /**
   * 
   * Return the type of the importer
   * @return String $type
   */
  public function getType(){
    return $this->type;
  }
  
  /**
  *
  * Return the version of the importer
  * @return Integer $version
  */
  public function getVersion(){
    return $this->version;
  }
  
  /**
  *
  * Return the dispatcher class of the importer
  * @return String $dispatcher_class
  */
  public function getDispatcherClass(){
    return $this->dispatcher_class;
  }
  
  /**
  *
  * Return the dispatcher settings of the importer
  * @return Array $dispatcher_settings
  */
  public function getDispatcherSettings(){
    return $this->dispatcher_settings;
  }
  
  /**
  *
  * Return the dispatcher of the importer
  * @return Dispatcher $dispatcher
  */
  public function getDispatcher(){
    $dispatcher = $this->dispatcher;
    return $dispatcher;
  }
  
  /**
  *
  * Return the name of the importer
  * @return String $name
  */
  public function getName(){
    $name = $this->name;
    return $name;
  }
  
  /**
   * 
   * Check if the XML is valid
   * @param Php DOM $dom
   */
  public function checkXml($dom){
    return TRUE;
  }
  /**
   * 
   * Render de result of the import
   */
  public function render(){
    return Logger::printLog();
  }
  
  /**
  *
  * Form to complete the settings of the importer
  * @param Array $form_state
  * @param Array $values
  */
  public function getSettingsForm(&$form_state, $values){
    $form = array();
    
    return $form;
  }
  
  /**
  *
  * Validation of the settings form
  * @param Array $form_state
  */
  public function validateSettingsForm(&$form_state){
    
  }
  
  /**
  *
  * Submission of the settings form
  * @param Array $values
  * @return Array $dispatcher_settings
  */
  public function submitSettingsForm($values){
    $importer_settings = array();
    return $importer_settings;
  }
  
  /**
   * 
   * Insert new importer settings in to the db
   * @param Array $settings
   */
  public function insertImporterSettings($settings){
    $result = db_insert('dpimport_importer')
    ->fields($settings)
    ->execute();
    return $result;
  }
  
  /**
  *
  * Update importer settings in to the db
  * @param Array $settings
  */
  public function updateImporterSettings($settings, $iid){
    $result = db_update('dpimport_importer')
    ->fields($settings)
    ->condition('iid', $iid)
    ->execute();
    return $result;
  }
  
  /**
  *
  * Delete importer settings in to the db
  * @param Array $settings
  */
  public function deleteImporterSettings($iid){
    $result = db_delete('dpimport_importer')
    ->condition('iid', $iid)
    ->execute();
    return $result;
  }
  
  /**
   * 
   * Initialize settings form
   * @param Array $importer_settings
   */
  public function initializeForm(){
    return array();
  }
  
  /**
   * 
   * Determine the next callback function from the Dom element
   * @param DOM $dom
   * @return String $callback
   */
  public function getCallback($dom){
    $callback = strtolower($dom->nodeName);
    return $callback;
  }
  
  /**
   * 
   * Call the create_or_load_node function based on the callback
   * @param DOM $dom
   * @return Object $node
   */
  protected function createOrLoadEntity($dom){
    $callback = $this->getCallback($dom);
    $create_or_load_method = 'createOrLoadEntity'.$callback;
    if (method_exists($this, $create_or_load_method)){
      $entity = $this->$create_or_load_method($dom);
    } else {
      $entity = NULL;
      Logger::logError('importer', 0, 'unknowmethod_001', t('The method !method doesn\'t exist.', array('!method' => $create_or_load_method)));
    }
    return $entity;
  }
  
  /**
   * 
   * Return a boolean form the xml value (int, string or boolean)
   * @param unknown_type $xmlboolean
   */
  protected function xmlbooleantophpboolean($xmlboolean){
    $phpboolean = NULL;
    if ($xmlboolean === FALSE) $phpboolean = FALSE;
    elseif ($xmlboolean === TRUE) $phpboolean = TRUE;
    elseif (strtolower($xmlboolean) == "true") $phpboolean = TRUE;
    elseif (strtolower($xmlboolean) == "false") $phpboolean = FALSE;
    elseif ($xmlboolean == 0) $phpboolean = FALSE;
    elseif ($xmlboolean == 1) $phpboolean = TRUE;
    return $phpboolean;
  }
  /**
   * 
   * Filter xss if the module filter is enabled else, use php function strip_tags
   * @param unknown_type $text
   */
  protected function filterHtmlTags($text){
    if (module_exists('filter')) {
      $filtered_text = filter_xss($text, array());
    } else {
      $filtered_text = strip_tags($text);
    }
    return $filtered_text;
  }
  
  /**
   * 
   * Create a file from uri
   * @param String $uri
   * @return Object $file
   */
  protected function createFile($file_uri, $destination){
    $dispatcher = $this->getDispatcher();
    $uri = parse_url($file_uri);
    $file = NULL;
    switch ($uri['scheme']){
      case 'embed':
        $source = $dispatcher->getTempSubdir().'/'.$uri['host'];
        if ($data = file_get_contents($source)){
          $file = file_save_data($data, $destination.'/'.$uri['host']);
        } else {
          Logger::logWarning('importer', 2, 'fileupload', t('The file (!file) can not be uploaded', array('!file' => $uri['host'])));
        }
        break;
      case 'http':
        if ($data = drupal_http_request($file_uri, array('timeout' => _dpimport_variable_get('dpicommons_http_request_timeout', 3)))){
          list($file_type, $file_ext) = explode('/', $data->headers['content-type']);
          $file_name = 'http'.rand().'.'.$file_ext;
          $file = file_save_data($data->data, $destination.'/'.$file_name);
        } else {
          Logger::logWarning('importer', 2, 'fileupload', t('The file (!file) can not be uploaded', array('!file' => $uri['host'])));
        }
        break;
    }
    return $file;
  }
  /**
   * return an save entity created by a create or load function, a setAttribute function and a process children
   * @param DOM $dom
   * @param String $entity_type
   */
  protected function contructEntity($dom, $entity_type){
    $dispatcher = $this->getDispatcher();
    $callback = $this->getCallback($dom);
    
    $entity = $this->createOrLoadEntity($dom);
    if (!Logger::error()){
      //Initialize the entity
      $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
      
      //Set attributes
      $attributes_method = 'set'.$callback.'Attributes';
      if (method_exists($this, $attributes_method)){
        $this->$attributes_method($dom, $entity_wrapper, $entity);
      }
      
      //Process children
      $dispatcher->processChildren($dom, $entity_wrapper, $entity);
      if (!Logger::error()){
        entity_save($entity_type, $entity);
      } else {
        $entity = NULL;
      }
    } 
    
    return $entity;
  }
}

