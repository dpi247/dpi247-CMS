<?php

/**
 * Implementation of hook_theme
 * */
function theunfold_theme(&$var) {
  return array(
    'theme_package_linkslists' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_bears_items' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_bottom_items' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_top_items' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_related_items' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_by' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_freetags' => array(
      'arguments' => array('vars' => NULL),
    ),
    'theme_package_persons' => array(
      'arguments' => array('vars' => NULL),
    ),
  );
}

function theunfold_node_load() {
  dsm(func_get_args(), 'args');
}

function theunfold_preprocess_page(&$vars) {
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
  } else {
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
  } else {
    $vars['secondary_nav'] = FALSE;
  }
}

function theunfold_process_page($vars) {
  // dsm($variables);
}

/**
 * Hook preprocess node
 */
function theunfold_preprocess_node(&$vars) {
  $type = $vars['type'];

  if ($type == 'package') {
    $node = &$vars['node'];
    // build data for node page
    if ($node->nid == arg(1) || $node->preview) {
      $content = $vars['content'];
dsm($content, 'content');
      $allItems = theunfold_preprocess_node_article_dispatch_embeds($node, $content);

      $vars['top_html'] = theme('theme_package_top_items', array('topItems' => $allItems['top']));
      $vars['bottom_html'] = theme('theme_package_bottom_items', array('bottomItems' => $allItems['bottom']));
      $vars['related_html'] = theme('theme_package_related_items',
        array(
          'linkslist' => $allItems['linkslists'],
          'bearItems' => $allItems['bears'],
          'freeTags'  => $vars['field_free_tags'],
          'persons'  => $vars['field_persons'],
        )
      );
    }

    return;
  }




      // build data for node page
      if ($node->nid == arg(1) || $node->preview) {

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
      
      
      return;
      
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

}

function theunfold_preprocess_node_article_dispatch_embeds($node, $content) {
  // @todo : Pour les embeds bottom, essayer d'avoir tout sauf 'image', 'video', 'links_list' et 'bears'
  $allItems = array(
    'top' => dpicontenttypes_api_get_embeds_view($node, $content, array('image', 'video'), FALSE, FALSE),
    'bottom' => dpicontenttypes_api_get_embeds_view($node, $content, array('audio', 'twitter'), FALSE, FALSE),
    'linkslists' => dpicontenttypes_api_get_embeds_view($node, $content, array('links_list'), FALSE, FALSE),
    'bears' => dpicontenttypes_api_get_embeds_view($node, $content, array('bear'), FALSE, FALSE),
  );
  return $allItems;
}

function theunfold_preprocess_dpicontenttypes_image_render_context(&$variables, $base_theme) {
  if (isset($variables['context'])) {
    $atom = $variables['atom'];
    $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);
    $title_value = $atom_wrapper->field_displaytitle->value();
    $variables['title'] = $title_value['safe_value'];
    $summary_value = $atom_wrapper->field_byline->value();
    $variables['summary'] = $summary_value['safe_value'];
  }
}

/**
 * Theme function to render the html of the media box
 */
function theunfold_theme_package_top_items($vars) {
  drupal_add_js(drupal_get_path('theme', 'theunfold').'/scripts/mylibs/pagination.js');

  $content = '';
  if (count($vars['topItems']) != 0) {
    $content .= '<div class="block-slidepic media">';
    $content .= '<ul class="page-inner">';
    foreach ($vars['topItems'] as $item) {
      $content .= '<li>';
      $content .= '<figure>';
      $content .= $item;
      $content .= '</figure>';
      $content .= '</li>';
    }
    $content .= '</ul>';
    $content .= '<div class="pagination"></div>';
    $content .= '</div>';
  }

  return $content;
}

/**
 * Theme function to render the html of the bottom media
 */
function theunfold_theme_package_bottom_items($vars) {
  $bottom_html = '';
  if (count($vars['bottomItems'])) {
    $bottom_html .= '<div class="bottom-items">';
    $bottom_html .= '<ul class="article-group">';
    foreach ($vars['bottomItems'] as $id => $item) {
      $bottom_html .= '<li class="article-inline small">';
      $bottom_html .= $item;
      $bottom_html .= '</li>';
    }
    $bottom_html .= '</ul>';
    $bottom_html .= '</div>';
  }

  return $bottom_html;
}
