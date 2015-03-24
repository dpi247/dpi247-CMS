<?php
abstract class Dispatcher {
  /**
   *
   * @var Singleton
   * @access private
   * @static
   *
   */
  protected $zip;
  protected $text_format;
  protected $cron;
  protected $cron_frequency;
  protected $temp_folder;
  protected $temp_subdir;
  protected $done_folder;
  protected $error_folder;
  protected $ttl_folder;
  protected $importer;
  protected $lock_timeout;
  
  /**
   * Class Constructor
   *
   * @param Importer $importer          
   * @param array $settings          
   */
  public function Dispatcher($importer, $settings) {
    $this->importer = $importer;
    $this->zip = isset ( $settings ['zip'] ) ? $settings ['zip'] : 0;
    $this->text_format = isset ( $settings ['text_format'] ) ? $settings ['text_format'] : NULL;
    $this->cron = isset ( $settings ['cron'] ) ? $settings ['cron'] : 0;
    $this->cron_frequency = isset ( $settings ['cron_frequency'] ) ? $settings ['cron_frequency'] : 10;
    $this->temp_folder = isset ( $settings ['temp_folder'] ) ? $settings ['temp_folder'] : NULL;
    $this->done_folder = isset ( $settings ['temp_folder'] ) ? $settings ['done_folder'] : NULL;
    $this->error_folder = isset ( $settings ['temp_folder'] ) ? $settings ['error_folder'] : NULL;
    $this->ttl_folder = isset ( $settings ['ttl_folder'] ) ? $settings ['ttl_folder'] : 30;
    $this->lock_timeout = 30;
  }
  
  /**
   * Absract function, must be implemented by child
   * Extract the sources and return an array of file path (file path to the sources)
   */
  abstract public function extractSources();
  
  /**
   * Return the zip variable of the class
   *
   * @return boolean
   */
  public function isZip() {
    return $this->zip;
  }
  
  /**
   * Return the cron variable of the class
   *
   * @return boolean
   */
  public function isCron() {
    return $this->cron;
  }
  
  /**
   * Return the path to the temps folder
   *
   * @return string
   */
  public function getTempFolder() {
    return 'importer://' . $this->temp_folder;
  }
  
  /**
   * Return the path to the temp subdir folder
   *
   * @return string
   */
  public function getTempSubdir() {
    return $this->temp_subdir;
  }
  
  /**
   * Return the path to the done folder
   *
   * @return string
   */
  public function getDoneFolder() {
    return 'importer://' . $this->done_folder;
  }
  
  /**
   * Return the path to the error folder
   *
   * @return string
   */
  public function getErrorFolder() {
    return 'importer://' . $this->error_folder;
  }
  
  /**
   * Return the value of the TTL folder (in days)
   *
   * @return string
   */
  public function getTTLFolder() {
    return $this->ttl_folder;
  }
  
  /**
   * Return the importer variable of the class
   *
   * @return Importer
   */
  public function getImporter() {
    return $this->importer;
  }
  
  /**
   * Process a XML file source
   *
   * @param string $source          
   */
  public function process($source) {
    $handle = fopen ( drupal_realpath ( $source ), "r" );
    
    if (! $handle) {
      Logger::logError ( 'dispatcher', 0, 'dispatcher_005', t ( 'The XML file (!xml_file) isn\'t readable', array (
        '!xml_file' => $source 
      ) ) );
    } else {
      $contents = fread ( $handle, filesize ( $source ) );
      // $contents = wallymport_convertEntities($contents);
      
      $dom = new DomDocument ();
      $dom->LoadXML ( $contents );
      
      // Checkfile
      $importer = $this->getImporter ();
      if ($importer->checkXml ( $dom )) {
        // Allow to alter the XML before treatment
        module_invoke_all ( 'dpimport_xml_preprocess', $dom, $importer );
        
        // Process file
        $this->processDom ( $dom );
      }
    }
  }
  
  /**
   * Explore the DOM and call a function for each element
   *
   * @param DOM $dom          
   */
  function processDom($dom) {
    $importer = $this->getImporter ();
    if ($dom->hasChildNodes ()) {
      // For each child of the dom
      foreach ( $dom->childNodes as $child ) {
        if ($child->nodeName != '#text') {
          $explore_child = TRUE;
          $callback = $importer->getCallback ( $child );
          $method_name = 'build' . $callback;
          // Search for a function named with node name, if the function return FALSE, it does not explode the children of the node.
          if (method_exists ( $importer, $method_name )) {
            $explore_child = $importer->$method_name ( $child );
            if (! $explore_child) {
              break;
            }
          }
          
          if ($explore_child) {
            $this->processDom ( $child );
          }
        }
      }
    }
  }
  
  /**
   * Process the children of the currant dom and call a function for each one.
   *
   * @param DOM $dom          
   * @param Object $object          
   * @param Object $object2          
   */
  function processChildren($dom, $object, $object2) {
    $importer = $this->getImporter ();
    
    foreach ( $dom->childNodes as $child ) {
      $name = strtolower ( $child->nodeName );
      if ($name != '#text') {
        $callback = $importer->getCallback ( $child );
        $method_name = 'build' . $callback;
        if (method_exists ( $importer, $method_name )) {
          if($method_name == 'builddestinations'){
            $importer->$method_name ( $child, $object, $object2, $dom );
          }else{
            $importer->$method_name ( $child, $object, $object2 );
          }          
        }
      }
    }
  }
  
  /**
   * Process the children of the currant dom and call a function for each one.
   * Return an array with the return of each function
   *
   * @param DOM $dom          
   * @param Object $object          
   */
  function processList($dom, $object, $object2) {
    $list = array ();
    $importer = $this->getImporter ();
    foreach ( $dom->childNodes as $child ) {
      $name = strtolower ( $child->nodeName );
      if ($name != '#text') {
        $callback = $importer->getCallback ( $child );
        $method_name = 'build' . $callback;
        if (method_exists ( $importer, $method_name )) {
          $list [] = $importer->$method_name ( $child, $object, $object2 );
        }
      }
    }
    return $list;
  }
  
  /**
   * Extract file from zip
   *
   * @param Object $file          
   * @param String $extension          
   *
   * @return String $xml_file
   */
  protected function getFilePathFromZip($file, $extension) {
    $xml_file = NULL;
    $zip_error = FALSE;
    $error_msg = FALSE;
    // Unzip input file to the temp directory
    $zip = new ZipArchive ();
    $res = $zip->open ( drupal_realpath ( $file->uri ) );
    if ($res === TRUE) {
      $temp_subdir = $this->getTempFolder () . '/' . $file->name;
      $this->checkDirectoryRecursive ( $temp_subdir, 1 );
      if (is_dir ( $temp_subdir )) {
        $this->temp_subdir = $temp_subdir;
        $zip->extractTo ( $temp_subdir );
        // Search for XMLs
        $import_files = file_scan_directory ( $temp_subdir, '/.*\.' . $extension . '$/', array (
          'nomask' => '/(\.\.?|CVS)$/',
          'recurse' => FALSE,
          'key' => 'name' 
        ) );
        if (count ( $import_files ) == 0) {
          $zip_error = TRUE;
          Logger::logError ( 'dispatcher', 0, 'dispatcher_001', t ( 'No XML file found in the import: !file. The ZIP must contains only files, no folder.', array (
            '!file' => $file->name 
          ) ) );
        } elseif (count ( $import_files ) > 1) {
          Logger::logError ( 'dispatcher', 0, 'dispatcher_002', t ( 'There is more than one !extension file found in the import: !file', array (
            '!extention' => $extension,
            '!file' => $file->name 
          ) ) );
        } else {
          $import_file = reset ( $import_files );
          $xml_file = $import_file->uri;
        }
        $zip->close ();
      } else {
        Logger::logError ( 'dispatcher', 0, 'dispatcher_003', t ( 'Input file !file could not be unzipped to !subdir. Error code !res', array (
          '!file' => $file->filename,
          '!subdir' => $temp_subdir,
          '!res' => $res 
        ) ) );
      }
    } else {
      Logger::logError ( 'dispatcher', 0, 'dispatcher_004', t ( 'Cant UnZip file: !file', array (
        '!file' => $file->filename 
      ) ) );
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
   * @param $directory A
   *          string representing the directory path.
   * @param $mode An
   *          optional bitmask containing the actions, if any, to be carried out on
   *          the directory. Any combination of the actions FILE_CREATE_DIRECTORY and
   *          FILE_MODIFY_PERMISSIONS is allowed.
   * @param $form_item An
   *          optional string containing the name of a form item that any errors
   *          will be attached to. Useful when the function validates a directory path
   *          entered as a form value. An error will consequently prevent form submit
   *          handlers from running, and instead display the form along with the
   *          error messages.
   *          
   * @return FALSE if the directory does not exist or is not writable, even after
   *         any optional actions have been carried out. Otherwise, TRUE is returned.
   */
  public function checkDirectoryRecursive($directory, $mode = 0, $form_item = NULL) {
    $directory = rtrim ( $directory, '/\\' );
    
    // Check if directory exists.
    if (! is_dir ( $directory )) {
      if (($mode & FILE_CREATE_DIRECTORY) && @mkdir ( $directory, 0775, TRUE )) {
        drupal_set_message ( t ( 'The directory %directory has been created.', array (
          '%directory' => $directory 
        ) ) );
        @chmod ( $directory, 0775 ); // Necessary for non-webserver users.
      } else {
        if ($form_item) {
          form_set_error ( $form_item, t ( 'The directory %directory does not exist.', array (
            '%directory' => $directory 
          ) ) );
        }
        return FALSE;
      }
    }
    
    // Check to see if the directory is writable.
    if (! is_writable ( $directory )) {
      if (($mode & FILE_MODIFY_PERMISSIONS) && @chmod ( $directory, 0775 )) {
        drupal_set_message ( t ( 'The permissions of directory %directory have been changed to make it writable.', array (
          '%directory' => $directory 
        ) ) );
      } else {
        form_set_error ( $form_item, t ( 'The directory %directory is not writable', array (
          '%directory' => $directory 
        ) ) );
        watchdog ( 'file system', 'The directory %directory is not writable, because it does not have the correct permissions set.', array (
          '%directory' => $directory 
        ), WATCHDOG_ERROR );
        return FALSE;
      }
    }
    
    if ((variable_get ( 'file_public_path', conf_path () . '/files' ) == $directory || file_directory_temp () == $directory) && ! is_file ( "$directory/.htaccess" )) {
      $htaccess_lines = "SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006\nOptions None\nOptions +FollowSymLinks";
      if (($fp = fopen ( "$directory/.htaccess", 'w' )) && fputs ( $fp, $htaccess_lines )) {
        fclose ( $fp );
        chmod ( $directory . '/.htaccess', 0664 );
      } else {
        $variables = array (
          '%directory' => $directory,
          '!htaccess' => '<br />' . nl2br ( check_plain ( $htaccess_lines ) ) 
        );
        form_set_error ( $form_item, t ( "Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables ) );
        watchdog ( 'security', "Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables, WATCHDOG_ERROR );
      }
    }
    return TRUE;
  }
  
  /**
   * Create a file Object form file name and file path
   *
   * @param String $file_name          
   * @param String $file_path          
   *
   * @return Object $file
   */
  protected function createFileObject($file_name, $file_path) {
    $file = new stdClass ();
    $file->uri = $file_path;
    $file->filename = $file_name;
    $file->name = str_replace ( '.' . $this->getExtention (), '', $file_name );
    return $file;
  }
  
  /**
   * Return Dispatcher extention
   *
   * @return string
   */
  protected function getExtention() {
    $ext = 'xml';
    if ($this->isZip ()) {
      $ext = 'zip';
    }
    return $ext;
  }
  
  /**
   * Form to complete the settings of the dispatcher
   *
   * @param Array $form_state          
   * @param Array $values          
   */
  public function getSettingsForm(&$form_state, $values) {
    $form = array ();
    
    $form ['dispatcher'] ['zip'] = array (
      '#type' => 'select',
      '#title' => t ( 'Are the data zipped?' ),
      '#description' => '',
      '#options' => array (
        t ( 'No' ),
        t ( 'Yes' ) 
      ),
      '#default_value' => isset ( $values ['zip'] ) ? $values ['zip'] : 0,
      '#required' => TRUE 
    );
    
    $all_formats = filter_formats();
    $els = array();
    if($all_formats && is_array($all_formats)){
      foreach($all_formats as $format){
        $els[$format->format] = $format->name;
      }
    }
    
    $form ['dispatcher'] ['text_format'] = array (
      '#type' => 'select',
      '#title' => t ( 'Choose text format for textfield' ),
      '#description' => '',
      '#options' => $els,
      '#default_value' => isset ( $values ['text_format'] ) ? $values ['text_format'] : 0,
      '#required' => TRUE 
    );
    
    $form ['dispatcher'] ['cron'] = array (
      '#type' => 'select',
      '#title' => t ( 'Are the data imported by a CRON job?' ),
      '#options' => array (
        t ( 'No' ),
        t ( 'Yes' ) 
      ),
      '#default_value' => isset ( $values ['cron'] ) ? $values ['cron'] : 0,
      '#required' => TRUE 
    );
    
    $form ['dispatcher'] ['cron_frequency'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Cron frequency (minutes)' ),
      '#description' => t ( 'Time between two cron runs' ),
      '#default_value' => isset ( $values ['cron_frequency'] ) ? $values ['cron_frequency'] : 10,
      '#states' => array (
        'visible' => array (
          ':input[name="cron"]' => array (
            'checked' => TRUE 
          ) 
        ) 
      ) 
    );
    
    /*
     * $form['dispatcher']['temp_folder'] = array( '#type' => 'textfield', '#title' => t('Temporary directory path'), '#description' => t('This must be an existing and writable path. The Stream Wrapper is importer://'), '#default_value' => isset($values['temp_folder']) ? $values['temp_folder'] : NULL, '#required' => TRUE, ); $form['dispatcher']['done_folder'] = array( '#type' => 'textfield', '#title' => t('Done directory path'), '#description' => t('This must be an existing and writable path. The Stream Wrapper is importer://'), '#default_value' => isset($values['done_folder']) ? $values['done_folder'] : NULL, '#required' => TRUE, ); $form['dispatcher']['error_folder'] = array( '#type' => 'textfield', '#title' => t('Error directory path'), '#description' => t('This must be an existing and writable path. The Stream Wrapper is importer://'), '#default_value' => isset($values['error_folder']) ? $values['error_folder'] : NULL, '#required' => TRUE, );
     */
    
    $form ['dispatcher'] ['ttl_folder'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Time to live of the files in day' ),
      '#description' => t ( 'The files older than this period will be deleted.' ),
      '#default_value' => isset ( $values ['ttl_folder'] ) ? $values ['ttl_folder'] : 30,
      '#required' => TRUE 
    );
    
    return $form;
  }
  
  /**
   * Validation of the settings form
   *
   * @param Array $form_state          
   */
  public function validateSettingsForm(&$form_state) {
    // check valide folder is not necessery, we create the file.
  }
  
  /**
   * Submission of the settings form
   *
   * @param Array $values          
   *
   * @return Array $dispatcher_settings
   */
  public function submitSettingsForm($values, $form_state_multistep) {
    $default_path = $_SERVER ["DOCUMENT_ROOT"] . "/sites/default/importer/" . $form_state_multistep["default"] ["name"];
    $old = umask ( 0 );
    if (@mkdir ( $default_path . "/done", 0777, true )) {
      if (mkdir ( $default_path . "/tmp", 0777, true )) {
        if (! mkdir ( $default_path . "/error", 0777, true )) {
          rmdir ( $default_path . "/tmp" );
          rmdir ( $default_path . "/done" );
          drupal_set_message ( "File cannot be created", "error" );
        } else {
          drupal_set_message ( "Creating : " . $default_path . "/tmp" );
          drupal_set_message ( "Creating : " . $default_path . "/done" );
          drupal_set_message ( "Creating : " . $default_path . "/error" );
        }
      } else {
        drupal_set_message ( "File cannot be created", "error" );
        rmdir ( $pathname . "/tmp" );
      }
    }
    umask ( $old );
    
    $dispatcher_settings = array (
      'zip' => $values ['zip'],
      'text_format' => $values ['text_format'],
      'cron' => $values ['cron'],
      'cron_frequency' => $values ['cron_frequency'],
      'temp_folder' =>  $form_state_multistep ["default"] ["name"] . "/tmp",
      'done_folder' => $form_state_multistep ["default"] ["name"] . "/done",
      'error_folder' =>$form_state_multistep ["default"] ["name"] . "/error",
      'ttl_folder' => $values ['ttl_folder'] 
    );
    
    return $dispatcher_settings;
  }
  
  /**
   * Initialize settings form
   *
   * @return Array $settings
   */
  public function initializeForm() {
    return array (
      'zip' => $this->isZip (),
      'text_format' => $this->text_format,
      'cron' => $this->isCron (),
      'cron_frequency' => $this->cron_frequency,
      'temp_folder' => $this->temp_folder,
      'done_folder' => $this->done_folder,
      'error_folder' => $this->error_folder,
      'ttl_folder' => $this->ttl_folder 
    );
  }
  
  /**
   * Move source to the done/error folder
   *
   * @param Array $source          
   */
  public function moveSource($source) {
    if (Logger::getStatusOfLastLogExecution () == 0) {
      $destination_folder = $this->getErrorFolder ();
    } else {
      $destination_folder = $this->getDoneFolder ();
    }
    
    // Move file to done/error repository
    if ($file = @file_move ( $source ['source'], $destination_folder . '/' . $source ['source']->filename, FILE_EXISTS_RENAME )) {
      // set the file as permanent
      $file->status = 1;
      file_save ( $file );
    } else {
      Logger::logError ( 'importer', 2, 'filemove', t ( 'The file !file can not be moved.', array (
        '!file' => $source ['source']->uri 
      ) ) );
    }
    
    // Delete temp folder
    if ($this->isZip ()) {
      $temp_folder = $this->getTempFolder () . '/' . $source ['source']->name;
      file_unmanaged_delete_recursive ( $temp_folder );
    }
  }
  
  /**
   * Delete the files older than ttl
   */
  public function cleanDirectories() {
    // ttl in second
    $ttl = $this->getTTLFolder () * 24 * 60 * 60;
    
    // Select the files from the error and done folder that are older than the ttl
    $folder_or = db_or ();
    $folder_or->condition ( 'uri', '%' . $this->getDoneFolder () . '%', 'LIKE' );
    $folder_or->condition ( 'uri', '%' . $this->getErrorFolder () . '%', 'LIKE' );
    
    $result = db_select ( 'file_managed', 'f' )->fields ( 'f', array (
      'fid' 
    ) )->condition ( $folder_or )->condition ( 'timestamp', time () - $ttl, '<' )->execute ()->fetchAssoc ();
    
    // Delete the files
    if ($result != NULL) {
      foreach ( $result as $fid ) {
        $file = file_load ( $fid );
        if (! file_delete ( $file )) {
          // @todo mettre un watchdog? pas de log error car on est en cron
        }
      }
    }
  }
  
  /**
   * Return the log timeout in second
   *
   * @return Interger $lock_timeout
   */
  public function getLockTimeout() {
    return $this->lock_timeout;
  }
}
