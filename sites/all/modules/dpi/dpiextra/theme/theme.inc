<?php

/**
 * Theme function that display to tabs (custom content and revision
 *
 * @param $variables
 * @return string
 */
function theme_dpiextra_custom_content_type_revision_form($variables) {
	
	ctools_include('ajax');
	$form = $variables['form'];
  	$content .= '<div>';
  	$content .= '<button type="button" id = "version" class = "active">'.ctools_ajax_text_button(t('Display all Revision'), 'dpiextra/custom_content/form/version/ajax', t('Revision')).'</button>';
  	$content .= '<div id = "version_form" style = "display:none;">';
  	//get revisions
  	$versions = dpiextra_custom_content_type_getrevision($form['machine_name']['#value']);
  	$content .= theme('dpiextra_custom_content_type_listversion',$versions);
  	$content .= '</div>';
  	$content .= '<div id = "custom_form">';
  	$content .= drupal_render_children($form);
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
		$cpt = 1;
		foreach ($versions  as $version){
			//construct link for each revision
			$revision = entity_revision_load('custom_content', $version->revision_id);
			//if the revision is not the default revision
			if(!entity_revision_is_default('custom_content',$revision)){
				$content .= '<li class = "dpiextra_version_list" id = "dpiextra_version_'.$version->revision_id.'">'.ctools_ajax_text_button('Version : '.$cpt.' Date : '.format_date($version->date), 'dpiextra/custom_content/getversion/'.$version->revision_id.'/ajax', t('Choose a revision')).' 
						----------'
						.ctools_ajax_text_button('Delete', 'dpiextra/custom_content/delete_revision/'.$version->revision_id.'/ajax', t('Delete a revision')).'</li>';
			}
			else
			{
				$content .= '<p class = "dpiextra_version_list" id = "dpiextra_version_'.$version->revision_id.'">default version is : the version '.$cpt.' Date : '.format_date($version->date).'</p>';
			}
			$cpt++;
		}
		$content .= '</ul>';
		$content .= '<div id = "dpiextra_version"></div>';
	} else {
		$content = t('There is no old version of this content type');
	}
	return $content;
}
