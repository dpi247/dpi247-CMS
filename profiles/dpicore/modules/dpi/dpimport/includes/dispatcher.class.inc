<?php 

abstract class Dispatcher{
  /**
  * @var Singleton
  * @access private
  * @static
  */
  private static $dispatcher = NULL;
  
  protected $zip;
  protected $temp_folder;
  protected $temp_subdir;
  protected $importer;
  /**
   * 
   * Class Constructor
   * @param Importer $importer
   * @param array $settings
   */
  
  protected function Dispatcher($importer, $settings) {
    $this->importer = $importer;
    $this->zip = isset($settings['zip']) ? $settings['zip'] : NULL;
    $this->temp_folder = isset($settings['temp_folder']) ? $settings['temp_folder'] : NULL;
  }
  
  /**
   * 
   * Absract function, must be implemented by child
   * Extract the sources and return an array of file path (file path to the sources)
   */
  abstract public function extractSources();
  
  /**
   * 
   * Return the singleton dispatcher
   * @param Importer $importer
   * @param array $settings
   * @return Dispatcher
   */
  public static function getDispatcher($importer, $settings) {
    if (is_null(self::$dispatcher)) {
      self::$dispatcher = new Dispatcher($importer, $settings);
    }
    return self::$dispatcher;
  }
  
  /**
   * 
   * Return the zip variable of the class
   * @return boolean
   */
  public function isZip(){
    return $this->zip;
  }
  
  /**
   * 
   * Return the path to the temps folder
   * @return string
   */
  public function getTempFolder(){
    return $this->temp_folder;
  }
  
  public function getTempSubdir(){
    return $this->temp_subdir;
  }
  
  /**
   * 
   * Return the importer variable of the class
   * @return Importer
   */
  public function getImporter(){
    return $this->importer;
  }
  
  /**
   * 
   * Process a XML file source
   * @param string $source
   */
  public function process($source){
    $handle = fopen($source, "r");
    
    if (!$handle) {
      Logger::logError('dispatcher', 0, 'dispatcher_005', t('The XML file (!xml_file) isn\'t readable', array('!xml_file' => $source)));
    } else { 
      $contents = fread($handle, filesize($source));
      //$contents = wallymport_convertEntities($contents);
    
      $dom = new DomDocument();
      $dom->LoadXML($contents);
      
      //checkfile
      $importer = $this->getImporter();
      if ($importer->checkXml($dom)){
        //process file
        $this->processDom($dom);
      }
    }
  }
  
  /**
   * Explore the DOM and call a function for each element
   * @param DOM $dom
   */
  function processDom($dom){
    $importer = $this->getImporter();
    if ($dom->hasChildNodes()){
      //For each child of the dom
      foreach ($dom->childNodes as $child){
        if ($child->nodeName != '#text'){
          $explore_child = TRUE;
          $callback = $importer->getCallback($child);
          $method_name = 'build_'.$callback;
          //Search for a function named with node name, if the function return FALSE, it does not explode the children of the node.
          if (method_exists($importer, $method_name)){
            $explore_child = $importer->$method_name($child);
            break;
          }
          
          if ($explore_child){
            $this->processDom($child);
          }
        }
      }
    }
  }
  
  /**
   * 
   * Extract file from zip
   * @param Object $file
   * @param String $extension
   * @return String $xml_file
   */
  protected function getFilePathFromZip($file, $extension){
    $xml_file = NULL;
    $zip_error = FALSE;
    $error_msg = FALSE;
    // Unzip input file to the temp directory
    $zip = new ZipArchive;
    
    $res = $zip->open($file->uri);
    if ($res === TRUE) {
      $temp_subdir = $this->getTempFolder().'/'.$file->name;
      $this->checkDirectoryRecursive($temp_subdir, 1);
      if (is_dir($temp_subdir)) {
        $this->temp_subdir = $temp_subdir;
        $zip->extractTo($temp_subdir);
        // Search for XMLs
        $import_files = file_scan_directory($temp_subdir, '/.*\.'.$extension.'$/', array('nomask' => '/(\.\.?|CVS)$/', 'recurse' => FALSE, 'key' => 'name'));
        if (count($import_files) == 0) {
          $zip_error = TRUE;
          Logger::logError('dispatcher', 0, 'dispatcher_001', t('No XML file found in the import: !file. The ZIP must contains only files, no folder.', array('!file' => $file->basename)));
        } elseif (count($import_files) > 1) {
          Logger::logError('dispatcher', 0, 'dispatcher_002', t('There is more than one !extension file found in the import: !file', array('!extention' => $extension, '!file' => $file->basename)));
        } else {
          $import_file = reset($import_files);
          $xml_file = $import_file->uri;
        }
        $zip->close();
      } else {
        Logger::logError('dispatcher', 0, 'dispatcher_003', t('Input file !file could not be unzipped to !subdir. Error code !res', array('!file' => $file->filename, '!subdir' => $temp_subdir, '!res' => $res)));
      }
    } else {
      Logger::logError('dispatcher', 0, 'dispatcher_004', t('Cant UnZip file: !file', array('!file' => $file->filename)));
    }    
    return $xml_file;
  }
  
  /**
  * Checks whether a directory exists and is writable.
  *
  * Furthermore, the directory can optionally be created if it does not exist,
  * and/or be set to writable if it is currently not. Directories need to have
  * execute permission to be considered a directory by FTP servers.
  *
  * @param $directory
  *   A string representing the directory path.
  * @param $mode
  *   An optional bitmask containing the actions, if any, to be carried out on
  *   the directory. Any combination of the actions FILE_CREATE_DIRECTORY and
  *   FILE_MODIFY_PERMISSIONS is allowed.
  * @param $form_item
  *   An optional string containing the name of a form item that any errors
  *   will be attached to. Useful when the function validates a directory path
  *   entered as a form value. An error will consequently prevent form submit
  *   handlers from running, and instead display the form along with the
  *   error messages.
  *
  * @return
  *   FALSE if the directory does not exist or is not writable, even after
  *   any optional actions have been carried out. Otherwise, TRUE is returned.
  */
  public function checkDirectoryRecursive($directory, $mode = 0, $form_item = NULL){
    $directory = rtrim($directory, '/\\');
    
    // Check if directory exists.
    if (!is_dir($directory)) {
      if (($mode & FILE_CREATE_DIRECTORY) && @mkdir($directory, 0775, TRUE)) {
        drupal_set_message(t('The directory %directory has been created.', array('%directory' => $directory)));
        @chmod($directory, 0775); // Necessary for non-webserver users.
      }
      else {
        if ($form_item) {
          form_set_error($form_item, t('The directory %directory does not exist.', array('%directory' => $directory)));
        }
        return FALSE;
      }
    }
    
    // Check to see if the directory is writable.
    if (!is_writable($directory)) {
      if (($mode & FILE_MODIFY_PERMISSIONS) && @chmod($directory, 0775)) {
        drupal_set_message(t('The permissions of directory %directory have been changed to make it writable.', array('%directory' => $directory)));
      }
      else {
        form_set_error($form_item, t('The directory %directory is not writable', array('%directory' => $directory)));
        watchdog('file system', 'The directory %directory is not writable, because it does not have the correct permissions set.', array('%directory' => $directory), WATCHDOG_ERROR);
        return FALSE;
      }
    }
    
    if ((variable_get('file_public_path', conf_path() . '/files') == $directory || file_directory_temp() == $directory) && !is_file("$directory/.htaccess")) {
      $htaccess_lines = "SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006\nOptions None\nOptions +FollowSymLinks";
      if (($fp = fopen("$directory/.htaccess", 'w')) && fputs($fp, $htaccess_lines)) {
        fclose($fp);
        chmod($directory .'/.htaccess', 0664);
      }
      else {
        $variables = array('%directory' => $directory, '!htaccess' => '<br />'. nl2br(check_plain($htaccess_lines)));
        form_set_error($form_item, t("Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables));
        watchdog('security', "Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables, WATCHDOG_ERROR);
      }
    }
    return TRUE;
  }
  
  /**
   * 
   * Create a file Object form file name and file path
   * @param String $file_name
   * @param String $file_path
   * @return Object $file
   */
  protected function createFileObject($file_name, $file_path){
    $file = new stdClass();
    $file->uri = $file_path;
    $file->filename = $file_name;
    $file->name = str_replace('.'.$this->getExtention(), '', $file_name);
    return $file;
  }
  
  /**
  *
  * Return Dispatcher extention
  * @return string
  */
  protected function getExtention(){
    $ext = 'xml';
    if ($this->isZip()){
      $ext = 'zip';
    }
    return $ext;
  }
  
  /**
   * 
   * Form to complete the settings of the dispatcher
   * @param Array $form_state
   * @param Array $values
   */
  public function getSettingsForm(&$form_state, $values){
    $form = array();
    $form['dispatcher']['zip'] = array(
      '#type' => 'select',
      '#title' => t('Are the data zipped?'),
      '#description' => t('Indicates if the data to import are zipped'),
      '#options' => array(0 => 'No', 1 => 'Yes'),
      '#default_value' => isset($values['zip']) ? $values['zip'] : NULL,
      '#required' => TRUE,
    );
    $form['dispatcher']['temp_folder'] = array(
      '#type' => 'textfield',
      '#title' => t('Temporary directory path'),
      '#description' => t('This must be an existing and writable path.'),
      '#default_value' => isset($values['temp_folder']) ? $values['temp_folder'] : NULL,
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
    $path = $form_state['values']['temp_folder'];
    $dir_exist = file_prepare_directory($path);
    if (!$dir_exist) {
      form_set_error('temp_folder', t('Temporary directory path : The path provided is not an existing writable directory.'));
    }
  }
  
  /**
  *
  * Submission of the settings form
  * @param Array $values
  * @return Array $dispatcher_settings
  */
  public function submitSettingsForm($values){
    $dispatcher_settings = array(
      'zip' => $values['zip'],
      'temp_folder' => $values['temp_folder'],
    );
    return $dispatcher_settings;
  }
  
  /**
  *
  * Initialize settings form
  * @return Array $settings
  */
  public function initializeForm(){
    return array(
      'zip' => $this->isZip(),
      'temp_folder' => $this->getTempFolder(),
    );
  }
}
  
