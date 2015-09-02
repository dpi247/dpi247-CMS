<?php
/**
 * This file is used to load messages from Drupal's drupal_get_message()
 */
//define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
//$base_url = 'http://'.$_SERVER['HTTP_HOST'];
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT.'/includes/bootstrap.inc';
require_once DRUPAL_ROOT.'/includes/theme.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

load_all_module_using_ctools_plugin();
$variables['display']=NULL;

if(isset($_GET['format'])){
  switch($_GET['format']){
    case "json": //xml Version
        print json_encode(drupal_get_messages($variables['display']));
      break;
    case "xml": //json Version
        print xml_encode(drupal_get_messages($variables['display']));
      break;
    default : //html Version
        print drupal_get_status_messages_with_javascript($variables);
      break;
  }
}else{
  print drupal_get_status_messages_with_javascript($variables);
}



/**
 * Load plugin ctools needed to process elements.
 */
function load_all_module_using_ctools_plugin(){
	/*
	  You should maintain this list manually .
	  All the keys of this call should belong to the array:

	  module_load_include('inc','ctools','includes/plugins');
	  dsm(ctools_get_plugins_info());
	*/
	$modules_that_uses_ctools_plugin_system = array(
		'ctools',
		'dpiblocks',
		'dpicache',
		'dpimport',
		'dpisocial',
		'dpisso',
		'enabootstrap_companion',
		'entityreference',
		'panels',
		'views_bulk_operations',
		'page_manager',
		'addressfield',
	);

	foreach ($modules_that_uses_ctools_plugin_system as $module_name) {
		/*
		 * module_load_include() isn't yet available
     * module_load_include('module', $module_name);
     */
		drupal_load('module', $module_name);
	}
}

/**
 * This function generate HTML Drupal Message
 * @param Array $variables
 * @return String
 */
function drupal_get_status_messages_with_javascript($variables) {
	$display = $variables ['display'];
	$output = '';

	$status_heading = array(
		'status' => t('Status message'),
		'error' => t('Error message'),
		'warning' => t('Warning message'),
	);
	foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= variable_get("dpicache_comment_".$type."_message_start", "");
		$output .= "<div class=\"messages $type\">\n";
		if (!empty($status_heading [$type])) {
			$output .= '<h2 class="element-invisible">' . $status_heading [$type] . "</h2>\n";
		}
		if (count($messages) > 1) {
			$output .= " <ul>\n";
			foreach ($messages as $message) {
				$output .= '  <li>' . $message . "</li>\n";
			}
			$output .= " </ul>\n";
		}
		else {
			$output .= reset($messages);
		}
		$output .= "</div>\n";
    $output .= variable_get("dpicache_comment_".$type."_message_end", "");
	}
	return $output;
}

/**
 * This function create xml document
 * @param mixed $mixed
 * @param dom $domElement
 * @param DOMDocument $DOMDocument
 */
function xml_encode($mixed, $domElement=null, $DOMDocument=null) {
  if (is_null($DOMDocument)) {
    $DOMDocument =new DOMDocument;
    $DOMDocument->formatOutput = true;
    xml_encode($mixed, $DOMDocument, $DOMDocument);
    echo $DOMDocument->saveXML();
  } else {
    if (is_array($mixed)) {
      foreach ($mixed as $index => $mixedElement) {
        if (is_int($index)) {
          if ($index === 0) {
            $node = $domElement;
          } else {
            $node = $DOMDocument->createElement($domElement->tagName);
            $domElement->parentNode->appendChild($node);
          }
        } else {
          $plural = $DOMDocument->createElement($index);
          $domElement->appendChild($plural);
          $node = $plural;
          if (!(rtrim($index, 's') === $index)) {
            $singular = $DOMDocument->createElement(rtrim($index, 's'));
            $plural->appendChild($singular);
            $node = $singular;
          }
        }
        xml_encode($mixedElement, $node, $DOMDocument);
      }
    } else {
      $domElement->appendChild($DOMDocument->createTextNode($mixed));
    }
  }
}