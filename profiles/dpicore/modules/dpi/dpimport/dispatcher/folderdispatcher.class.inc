<?php 

class FolderDispatcher extends Dispatcher{
  
  
  private static $folderdispatcher = NULL;
  
  protected $source_folder;
  
  /**
   * 
   * Constructor
   * @param Importer $importer
   * @param Array $settings
   */
  protected function FolderDispatcher($importer, $settings){
    parent::Dispatcher($importer, $settings);
    $this->source_folder = isset($settings['source_folder']) ? $settings['source_folder'] : NULL;
  }
  
  /**
   * Return FolderDispatcher Singloton
   *
   * @param Importer $importer
   * @param Array $settings
   * @return FolderDispatcher
   */
  public static function getFolderDispatcher($importer, $settings) {
    if (is_null(self::$folderdispatcher)) {
      self::$folderdispatcher = new FolderDispatcher($importer, $settings);
    }
    return self::$folderdispatcher;
  }
  
  /**
   * 
   * Return the source folder
   * @return String $source_folder
   */
  public function getSourceFolder(){
    return $this->source_folder;
  }
  
  /**
   * Extract the sources
   * @return array $files
   */
  public function extractSources(){
    
    $files = array();
    
    $file_name = arg(3);
    
    if ($file_name != NULL){
      //Get one file
      $file = $this->getFile($file_name);
      if ($file != NULL){
        $files = array($file);
      }
    } else {
      //Get entire folder's files
      $files = $this->getAllFiles();
    }
    return $files;
  }
  
  /**
   * 
   * Return the file path and dezip if needed
   * @param String $file_name
   * @return String $file_path
   */
  protected function getFile($file_name){
    $file_path = NULL;
    if ($this->isZip()){
      $file = $this->createFileObject($file_name, $this->getSourceFolder()."/".$file_name);
      $file_path = $this->getFilePathFromZip($file, 'xml');
    } else {
      if (file_exists($this->getSourceFolder()."/".$file_name)){
        $file_path = $this->getSourceFolder()."/".$file_name;
      }
    }
    return $file_path;
  }
  
  /**
   * 
   * Return all the files of the source folder
   * @return Array
   */
  protected function getAllFiles(){
    return array();
  }
  
  /**
   * 
   * Form to complete the settings of the folderdispatcher
   * @param Array $form_state
   * @param Array $values
   */
  public function getSettingsForm(&$form_state, $values){
    $form = parent::getSettingsForm($form_state, $values);
    $form['dispatcher']['source_folder'] = array(
      '#type' => 'textfield',
      '#title' => t('Source directory path'),
      '#description' => t('This must be an existing and writable path.'),
      '#default_value' => isset($values['source_folder']) ? $values['source_folder'] : NULL,
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
    $path = $form_state['values']['source_folder'];
    $dir_exist = file_prepare_directory($path);
    if (!$dir_exist) {
      form_set_error('source_folder', t('Source directory path : The path provided is not an existing writable directory.'));
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
    $dispatcher_settings['source_folder'] = $values['source_folder'];
    return $dispatcher_settings;
  }
  
  /**
  *
  * Initialize settings form
  * @return Array $setting
  */
  public function initializeForm(){
    $settings = parent::initializeForm();
    $settings['source_folder'] = $this->getSourceFolder();
    return $settings;
  }
}