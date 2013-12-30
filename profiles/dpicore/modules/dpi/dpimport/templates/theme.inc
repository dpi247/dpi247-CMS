<?php 

/**
 * Recursively render the log
 *
 * @param unknown_type $variables
 */
function theme_dpimport_log_recursif($variables){
  $children_html = '';
  $content = '<ul>';
  foreach ($variables['children'] as $child){
    $content .= '<li><b>'.t('Type').' : '.$child->type.'</b></li>';
    $content .= '<li>'.t('Status').' : '.$child->status.'</li>';
    $content .= '<li>'.t('Code').' : '.$child->code.'</li>';
    $content .= '<li>'.t('Message').' : '.filter_xss($child->message).'</li>';
    if ($child->children != NULL){
      $content .= '<li>'.t('Children').' : '.theme('dpimport_log_recursif', array('children' => $child->children)).'</li>';
    }
  }
  $content .= '</ul>';
  return $content;
}
