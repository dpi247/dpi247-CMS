<?php
// $Id: analytic-content.tpl.php,v 1.0 2011/02/18 ODM $

/**
 * Available variable:
 *  - $html_code : contents the code returned by the webservice
 */
if(isset($html_code) && $html_code != ''){
  drupal_add_js($html_code, 'inline');
}
