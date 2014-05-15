<?php 

class FolderDispatcher extends Dispatcher {

  protected $source_folder;

  /**
   * Constructor
   *
   * @param Importer $importer
   * @param Array $settings
   */
  public function FolderDispatcher($importer, $settings){
    parent::Dispatcher($importer, $settings);
    $this->source_folder = isset($settings['source_folder']) ? $settings['source_folder'] : NULL;
  }

  /**
   * Return the source folder
   *
   * @return String $source_folder
   */
  public function getSourceFolder(){
    return 'importer://'.$this->source_folder;
  }

  /**
   * Extract the sources
   *
   * @return array $files
   */
  public function extractSources($file_name = NULL) {
    $files = array();

    if (!$file_name) {
      $file_name = arg(3);
    }

    if ($file_name != NULL){
      // Get one file
      $file = $this->getFile($file_name);
      if ($file != NULL){
        $files = array($file);
      }
    } else {
      // Get entire folder's files
      $files = $this->getAllFiles();
    }
    return $files;
  }

  /**
   * Return the file path and dezip if needed
   *
   * @param String $file_name
   *
   * @return String $file_path
   */
  protected function getFile($file_name){
    $file_path = NULL;

    if (file_exists($this->getSourceFolder()."/".$file_name)){
      $file = $this->createFileObject($file_name, $this->getSourceFolder()."/".$file_name);
      if ($this->isZip()){
        $file_path = array('temp_source' => $this->getFilePathFromZip($file, 'xml'), 'source' => $file);
      } else {
        $file_path = array('temp_source' => $this->getSourceFolder()."/".$file_name, 'source' => $file);
      }
    } else {
      Logger::logError('dispatcher', 0, 'dispatcher_009', t('The file specified does not exists : !filepath', array('!filepath' => $this->getSourceFolder()."/".$file_name, )));
    }
    return $file_path;
  }

  /**
   * Return all the files of the source folder
   *
   * @return Array
   */
  protected function getAllFiles(){
    $source_folder = $this->getSourceFolder();
    $mask = '/.*\.'.$this->getExtention().'$/';
    $options = array(
      'recurse' => FALSE,
      'key' => 'name',
    );
    $files = file_scan_directory($source_folder, $mask, $options);
    $file_paths = NULL;
    if (!empty($files)){
      foreach ($files as $file){
        if ($this->isZip()){
          $file_paths[] = array('temp_source' => $this->getFilePathFromZip($file, 'xml'), 'source' => $file);
        } else {
          $file_paths[] = array('temp_source' => $file->uri, 'source' => $file);
        }
      }
    } else {
      Logger::logError('dispatcher', 0, 'dispatcher_008', t('There is no file in this folder : !folder', array('!folder' => $source_folder, )));
    }
    return $file_paths;
  }

  /**
   * Form to complete the settings of the folderdispatcher
   *
   * @param Array $form_state
   * @param Array $values
   */
  public function getSettingsForm(&$form_state, $values){
    $form = parent::getSettingsForm($form_state, $values);
    $form['dispatcher']['source_folder'] = array(
      '#type' => 'textfield',
      '#title' => t('Source directory path'),
      '#description' => t('This must be an existing and writable path. The Stream Wrapper is importer://'),
      '#default_value' => isset($values['source_folder']) ? $values['source_folder'] : NULL,
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Validation of the settings form
   *
   * @param Array $form_state
   */
  public function validateSettingsForm(&$form_state){
    parent::validateSettingsForm($form_state);
    $path = 'importer://'.$form_state['values']['source_folder'];
    if (!file_prepare_directory($path)) {
      form_set_error('source_folder', t('Source directory path : The path provided is not an existing writable directory.'));
    }
  }

  /**
   * Submission of the settings form
   *
   * @param Array $values
   *
   * @return Array $dispatcher_settings
   */
  public function submitSettingsForm($values){
    $dispatcher_settings = parent::submitSettingsForm($values);
    $dispatcher_settings['source_folder'] = $values['source_folder'];
    return $dispatcher_settings;
  }

  /**
   * Initialize settings form
   *
   * @return Array $setting
   */
  public function initializeForm(){
    $settings = parent::initializeForm();
    $settings['source_folder'] = $this->source_folder;
    return $settings;
  }

  /**
   * Return the log timeout in second
   *
   * @return Interger $lock_timeout
   */
  public function getLockTimeout(){
    $lock_timeout = $this->lock_timeout;
    $file_name = arg(3);

    // In case of proceeding a folder, the lock timeout is bigger
    if ($file_name == NULL){
      $lock_timeout = 60 * 5; // 5minutes
    }
    return $lock_timeout;
  }
}
