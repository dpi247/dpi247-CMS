<?php 

class PostDispatcher extends Dispatcher{
  private static $postdispatcher = NULL;
  
  
  /**
  *
  * Constructor
  * @param Importer $importer
  * @param Array $settings
  */
  protected function PostDispatcher($importer, $settings){
    parent::Dispatcher($importer, $settings);
    
  }
  
  /**
   * Return PostDispatcher Singloton
   *
   * @param Importer $importer
   * @param Array $settings
   * @return PostDispatcher
   */
  public static function getPostDispatcher($importer, $settings) {
    if (is_null(self::$postdispatcher)) {
      self::$postdispatcher = new PostDispatcher($importer, $settings);
    }
    return self::$postdispatcher;
  }
  
  /**
   * Extract the sources
   * @return array $files
   */
  public function extractSources(){
  
    $files = $this->getFormFiles();
    $files += $this->getCurlFiles();
    return $files;
  }
  
  /**
   *
   * Return the files get from a form
   * @return Array $files
   */
  protected function getFormFiles(){
    $files = array();
    foreach ($_FILES as $key => $input) {
      if ($this->isZip()){
        $file = $this->createFileObject($input['name'], $input['tmp_name']);
        $file_path = $this->getFilePathFromZip($file, 'xml');
        if ($file_path != NULL){
          $files[] = $file_path;
        }
      } else {
        $files[] = $input['tmp_name'];
      }
    }
    return $files;
  }
  
  /**
   * 
   * Return the files get form a post via curl
   * @return Array $files
   */
  function getCurlFiles(){
    $files = array();
    
    $temp_file_name = 'dpi_curl_input_'.rand();
    $temp_file_path = $this->getTempFolder().'/'.$temp_file_name.'.'.$this->getExtention();
    
    //Write the file in the temp folder
    $input = fopen('php://input', 'r');
    $handle = fopen($temp_file_path, 'w');
    while (!feof($input)) {
      $fwrite = fwrite($handle, fread($input, 1024));
      if (!$fwrite)
      break;
    }
    fclose($input);
    fclose($handle);
    
    //Add the file path to the array of files 
    if ($this->isZip()){
      $file = $this->createFileObject($temp_file_name, $temp_file_path);
      $file_path = $this->getFilePathFromZip($file, 'xml');
      if ($file_path != NULL){
        $files[] = $file_path;
      }
    } else {
      $files[] = $temp_file_path;
    }
        
    return $files;
  }
}