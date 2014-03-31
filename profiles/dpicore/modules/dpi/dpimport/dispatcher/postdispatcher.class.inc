<?php 

class PostDispatcher extends Dispatcher{

  /**
   * Constructor
   *
   * @param Importer $importer
   * @param Array $settings
   */
  public function PostDispatcher($importer, $settings){
    parent::Dispatcher($importer, $settings);
  }

  /**
   * Extract the sources
   *
   * @return array $files
   */
  public function extractSources(){
    $files = $this->getFormFiles();
    $files += $this->getCurlFiles();
    return $files;
  }

  /**
   * Return the files get from a form
   *
   * @return Array $files
   */
  protected function getFormFiles(){
    $files = array();
    foreach ($_FILES as $key => $input) {
      $file = $this->createFileObject($input['name'], $input['tmp_name']);
      if ($this->isZip()){
        $file_path = $this->getFilePathFromZip($file, 'xml');
        $files[] = array('temp_source' => $file_path, 'source' => $file);
      } else {
        $files[] = array('temp_source' => $input['tmp_name'], 'source' => $file);
      }
    }
    return $files;
  }

  /**
   * Return the files get form a post via curl
   *
   * @return Array $files
   */
  function getCurlFiles(){
    $files = array();

    $temp_file_name = 'dpi_curl_input_'.rand();
    $temp_file_path = $this->getTempFolder().'/'.$temp_file_name.'.'.$this->getExtention();

    // Write the file in the temp folder
    $input = @fopen('php://input', 'r');
    $handle = @fopen($temp_file_path, 'w');

    if ($input && $handle) {
      // If input is not empty we write it into handle and add the file
      if (fwrite($handle, fread($input, 1024))){
        while (!feof($input)) {
          $fwrite = fwrite($handle, fread($input, 1024));
          if (!$fwrite)
            break;
        }

        fclose($input);
        fclose($handle);

        // Add the file path to the array of files
        $file = $this->createFileObject($temp_file_name, $temp_file_path);
        if ($this->isZip()){
          $file_path = $this->getFilePathFromZip($file, 'xml');
          $files[] = array('temp_source' => $file_path, 'source' => $file);
        } else {
          $files[] = array('temp_source' => $temp_file_path, 'source' => $file);
        }
      } else {
        // If input is empty, we delete the temp_file_path
        fclose($input);
        fclose($handle);
        file_unmanaged_delete_recursive($temp_file_path);
      }
    }

    return $files;
  }
}
