<?php

/**
 * Theme function that display to tabs (custom content and revision
 *
 * @param $variables
 * @return string
 */
function theme_custom_content_form($variables) {
	
	ctools_include('ajax');
	$form = $variables['form'];
	
	$content = '';


  	$content .= '<div>';
  	$content .= '<ul class = "tabs primary">';
  	$content .= '<li id = "custom" class = "active">'.ctools_ajax_text_button(t('Custom Content'), 'dpiextra/custom_content/form/custom/ajax', t('Custom content')).'</li>';
  	$content .= '<li id = "version">'.ctools_ajax_text_button(t('Revision'), 'dpiextra/custom_content/form/version/ajax', t('Revision')).'</li>';
  	$content .= '</ul>';
  	 
  	$content .= '<div id = "dpiextra_tabs">';
  	$content .= '<div id = "custom_form">';
  	//$content .= drupal_render($form['cid']);
  	$content .= drupal_render_children($form);
  	$content .= '</div>';
  	$content .= '<div id = "version_form" style = "display:none;">';
  	$versions = dpiextra_custom_content_type_getrevision($form['cid']['#value']);
  	$content .= theme('dpiextra_custom_content_type_listversion',$versions);
  	$content .= '</div>';
  	$content .= '</div>';
  	$content .= '</div>';

  return $content;
}

/**
 * Theme function that display a list of all revision
 * 
 * @param $versions
 * @return $content
 * The html list
 */
function theme_dpiextra_custom_content_type_listversion($versions) {
	if ($versions != NULL){
		$content = '<ul>';
		foreach ($versions  as $version){
			
			$content .= '<li class = "dpiextra_version_list" id = "wallyextra_version_'.$version->revision_id.'">'.ctools_ajax_text_button('Version : '.$version->revision_id.' Date : '.format_date($version->date), 'dpiextra/custom_content/getversion/'.$version->revision_id.'/ajax', t('Choose a version')).'</li>';
		}
		$content .= '</ul>';
		$content .= '<div id = "dpiextra_version"></div>';
	} else {
		$content = t('There is no old version of this content type');
	}
	return $content;
}
