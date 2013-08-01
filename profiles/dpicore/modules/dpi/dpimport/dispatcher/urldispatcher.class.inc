<?php 

class UrlDispatcher extends Dispatcher{
  
  private static $urldispatcher = NULL;
  
  protected $source_url;
  
  /**
   *
   * Constructor
   * @param Importer $importer
   * @param Array $settings
   */
  protected function UrlDispatcher($importer, $settings){
    parent::Dispatcher($importer, $settings);
    $this->source_url = isset($settings['source_url']) ? $settings['source_url'] : NULL;
  }
  
  /**
   * Return UrlDispatcher Singloton
   *
   * @param Importer $importer
   * @param Array $settings
   * @return UrlDispatcher
   */
  public static function getUrlDispatcher($importer, $settings) {
    if (is_null(self::$urldispatcher)) {
      self::$urldispatcher = new UrlDispatcher($importer, $settings);
    }
    return self::$urldispatcher;
  }
  
  /**
   *
   * Return the source folder
   * @return String $source_folder
   */
  public function getSourceUrl(){
    return $this->source_url;
  }
  
  /**
   * Extract the sources
   * @return array $files
   */
  public function extractSources(){
    
    $files = array();
    $file = $this->getFile();
    if ($file != NULL){
      $files = array($file);
    }
    return $files;
  }
  
  /**
   *
   * Return the file path and dezip if needed
   * @return String $file_path
   */
  protected function getFile(){
    
    $file_path = '';
    
    $request = drupal_http_request($this->getSourceUrl(), array('timeout' => _dpimport_variable_get('dpicommons_http_request_timeout', 3)));
    
    if ($request->code == 200){
      $extention = $this->getExtention();
      
      $file_name = 'urldispatcher_'.rand().'.'.$extention;
      $file_path = $this->getTempFolder().'/'.$file_name;
      
      //Save data into temp folder
      if (file_put_contents ($file_path, $request->data) === FALSE){
        Logger::logError('Dispatcher', 0, 106, t('Can\'t copy source: !source into file', array('!source' => $this->getSourceUrl())));
      }
      
      if ($this->isZip()){
        $file = $this->createFileObject($file_name, $file_path);
        $file_path = $this->getFilePathFromZip($file, 'xml');
      }
    } else {
      Logger::logError('Dispatcher', 0, 107, t('Can\'t get source : !source error : !error', array('!source' => $this->getSourceUrl(), '!error' => $request->error)));
    }
    return $file_path;
  }
  
  /**
  *
  * Form to complete the settings of the urldispatcher
  * @param Array $form_state
  * @param Array $values
  */
  public function getSettingsForm(&$form_state, $values){
    $form = parent::getSettingsForm($form_state, $values);
    $form['dispatcher']['source_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Source URL'),
      '#description' => t('The URL to process.'),
      '#default_value' => isset($values['source_url']) ? $values['source_url'] : NULL,
      '#required' => TRUE,
    );
    return $form;
  }
  
  /**
   *
   * Validation of the settings form
   * @param Array $form_state
   */
  public function validateSettingsForm(&$form_state){
    parent::validateSettingsForm($form_state);
    $path = $form_state['values']['source_url'];
    if (!_dpimport_checkurlresponse($path)){
      form_set_error('source_url', t('The source URL does not responds.'));
    }
  }
  
  /**
  *
  * Submission of the settings form
  * @param Array $values
  * @return Array $dispatcher_settings
  */
  public function submitSettingsForm($values){
    $dispatcher_settings = parent::submitSettingsForm($values);
    $dispatcher_settings['source_url'] = $values['source_url'];
    return $dispatcher_settings;
  }
  
  /**
  *
  * Initialize settings form
  * @return Array $setting
  */
  public function initializeForm(){
    $settings = parent::initializeForm();
    $settings['source_url'] = $this->getSourceUrl();
    return $settings;
  }
}