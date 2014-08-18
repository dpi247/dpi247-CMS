<?php
// $Id: pub-header.tpl.php,v 1.0 2011/10/12 HNA $

/**
 * Available variable:
 *  - $header_code : contents the code returned by the webservice
 */
if(isset($header_code) && $header_code != ''){
  drupal_add_js($header_code, 'inline');
}

?>