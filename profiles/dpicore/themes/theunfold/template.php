<?php

function theunfold_preprocess_page(& $vars){
    if (isset($vars['main_menu'])) {
      $vars['primary_nav'] = theme('links__system_main_menu', array(
        'links' => $vars['main_menu'],
        'attributes' => array(
          'class' => array('links', 'inline', 'main-menu'),
        ),
        'heading' => array(
          'text' => t('Main menu'),
          'level' => 'h2',
          'class' => array('element-invisible'),
        )
      ));
    }
    else {
      $vars['primary_nav'] = FALSE;
    }
    if (isset($vars['secondary_menu'])) {
      $vars['secondary_nav'] = theme('links__system_secondary_menu', array(
        'links' => $vars['secondary_menu'],
        'attributes' => array(
          'class' => array('links', 'inline', 'secondary-menu'),
        ),
        'heading' => array(
          'text' => t('Secondary menu'),
          'level' => 'h2',
          'class' => array('element-invisible'),
        )
      ));
    }
    else {
      $vars['secondary_nav'] = FALSE;
    }
}

function theunfold_process_page($vars){
 // dsm($variables);
 
  
}



/*
 * Hook preprocess node
*/
function theunfold_preprocess_node(&$vars){
         
  $type = $vars["type"];
  $node = &$vars['node'];

  /*
  
  switch ($node->type){
    case 'package':
      // We build data for node template teaser mode
      if ($node->teaser) {
        $vars["widget"] = theunfold_widget_prepare_article_summary_node($node);
      }
      // We build data for redacblock views
      if (isset($view)) {
        $current_display = &$view->display[$view->current_display];
        if ($current_display->display_options['row_plugin'] == 'redacblock_row') {
          $vars["widget"] = theunfold_widget_prepare_article_summary_node($node);
        }
      }


      // build data for node page
      if ($node->nid == arg(1) || $node->preview ) {

        // We unset the body, theunfold_preprocess_node_build will create a new one.
        unset($vars["body"]);
        $vars = array_merge($vars, theunfold_preprocess_node_build($node));

        $merged_medias = array();
        if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
          if (module_exists('cciinlineobjects')) {
            cciinlineobjects_flag_inline_embed_objects($node);
          }

          if ($node->preview) {
            foreach ($node->field_embededobjects_nodes as $delta => $embed) {
              // Fake nid in case of preview
              $node->field_embededobjects_nodes[$delta]->nid = $delta;
            }
          }

          foreach ($node->field_embededobjects_nodes as $embed) {
            $merged_medias += theunfold_preprocess_node_build_embedded_photos($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_videos($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_audios($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_links($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_documents($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_text($embed);
          }
        }


        $topItems = array();
        $bottomItems = array();
        theunfold_preprocess_node_article_dispatch_top_bottom($node, $merged_medias, $topItems, $bottomItems, $bearItems);
        $linkslist = theunfold_get_sorted_links($node);

        $vars['top_html'] = theme('theme_package_top_items', array('topItems' => $topItems));
        $vars['bottom_html'] = theme('theme_package_bottom_items', array('bottomItems' => $bottomItems));
        $vars['related_html'] = theme('theme_package_related_items',
            array('linkslist' => $linkslist,
              'bearItems' => $bearItems,
              'freeTags'  => $vars['field_free_tags'],
              'persons'  => $vars['field_persons'],
            )
        );
      }
      break;

    case 'wally_photoobject':
    case 'wally_videoobject':
    case 'wally_audioobject':
    case 'wally_digitalobject':
    case 'wally_linktype':
      $vars['widget'] = theunfold_widget_prepare_media_node($node);
      break;
    case 'wally_textobject':
      unset($vars["body"]);
      $vars += theunfold_preprocess_node_build_textobject($node);
      break;
    case 'wally_linkslistobject':
      $vars['widget'] = theunfold_widget_prepare_linkslist_node($node);
      break;
  }
  */
  //if (isset($vars['view'])) { $view = &$vars['view']; }
  
}