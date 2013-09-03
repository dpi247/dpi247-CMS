<?php 

abstract class Importer{
  
  protected $type; 
  protected $version;
  protected $name;
  protected $dispatcher_class;
  protected $dispatcher_settings;
  
  function Importer($settings, $plugin){
    $this->type = isset($settings->type) ? $settings->type : NULL;
    $this->version = isset($settings->version) ? $settings->version : NULL;
    $this->name = isset($settings->name) ? $settings->name : NULL;
    $this->dispatcher_class = isset($settings->dispatcher) ? $settings->dispatcher : NULL;
    $this->dispatcher_settings = isset($settings->dispatcher_settings) ? unserialize($settings->dispatcher_settings) : array();
  }
  //fonction que la class enfant est obligée d'implémenter
  // abstract protected function getValue();
  //fonction commune
  public function getType(){
    return $this->type;
  }
  public function getVersion(){
    return $this->version;
  }
  public function getDispatcherClass(){
    return $this->dispatcher_class;
  }
  public function getDispatcherSettings(){
    return $this->dispatcher_settings;
  }
  public function getDispatcher(){
    $dispatcher_class = $this->getDispatcherClass();
    $dispatcher_function = 'get'.$dispatcher_class;
    $dispatcher = $dispatcher_class::$dispatcher_function($this, $this->getDispatcherSettings());
    return $dispatcher;
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
}
