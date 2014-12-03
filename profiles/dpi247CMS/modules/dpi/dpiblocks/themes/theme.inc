<?php

/**
 * Theme function that display to tabs (custom content and revision
 *
 * @param $variables
 *
 * @return string
 */
function theme_dpiblocks_custom_content_type_revision_form($variables) {
  ctools_include('ajax');
  $form = $variables['form'];
  $content .= '<div>';
  $content .= '<button type="button" id = "version" class = "active">'.ctools_ajax_text_button(t('Display all Revision'), 'dpiblocks/custom_content/form/version/ajax', t('Revision')).'</button>';
  $content .= '<div id = "version_form" style = "display:none;">';
  // Get revisions
  $versions = dpiblocks_custom_content_type_getrevision($form['machine_name']['#value']);
  $content .= theme('dpiblocks_custom_content_type_listversion', $versions);
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
 *
 * @return $content
 *  The HTML list
 */
function theme_dpiblocks_custom_content_type_listversion($versions) {
  if ($versions != NULL){
    $content = '<ul>';
    $cpt = 1;
    foreach ($versions  as $version){
      // Construct link for each revision
      $revision = entity_revision_load('custom_content', $version->revision_id);
      // If the revision is not the default revision
      if(!entity_revision_is_default('custom_content',$revision)){
        $content .= '<li class = "dpiblocks_version_list" id = "dpiblocks_version_'.$version->revision_id.'">'.ctools_ajax_text_button('Version : '.$cpt.' Date : '.format_date($version->date), 'dpiblocks/custom_content/getversion/'.$version->revision_id.'/ajax', t('Choose a revision')).'
            ----------'
            .ctools_ajax_text_button('Delete', 'dpiblocks/custom_content/delete_revision/'.$version->revision_id.'/ajax', t('Delete a revision')).'</li>';
      }
      else {
        $content .= '<p class = "dpiblocks_version_list" id = "dpiblocks_version_'.$version->revision_id.'">default version is : the version '.$cpt.' Date : '.format_date($version->date).'</p>';
      }
      $cpt++;
    }
    $content .= '</ul>';
    $content .= '<div id = "dpiblocks_version"></div>';
  } else {
    $content = t('There is no old version of this content type');
  }
  return $content;
}

/**
 * Theme the FlowMix main form
 */
function theme_dpiblocks_flowmix_content_type_form_summary_form($variables) {
  $form = $variables['form'];

  foreach(element_children($form['flows']) as $key) {
    $rows[] = array(
      drupal_render($form['flows'][$key]['title']),
      drupal_render($form['flows'][$key]['edit_action']).drupal_render($form['flows'][$key]['delete_action']),
    );
  }

  if(count($form['flows'])) {
    $table_output .= theme('table', array('header' => array ('titre', 'action'), 'rows' => $rows));
    $button_add_flow_output .= drupal_render($form['buttons']['add_flow']);
  }

  $output_after = drupal_render_children($form);

  $output .= $table_output;
  $output .= $button_add_flow_output;
  $output .= $output_after;

  return $output;
}
