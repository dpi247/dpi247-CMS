<?php

/**
 * Implementation of hook_theme
 * */
function theunfold_theme() {
  $path = drupal_get_path('theme', 'theunfold');

  return array(
    'theme_package_top_items' => array(
      'arguments' => array('topItems' => array()),
    ),
    'theme_package_publication_date_and_comments_count' => array(
      'arguments' => array('date' => array(), 'comment_count' => array())
    ),
    'comment_form' => array(
      'arguments' => array('form' => array()),
      'render element' => 'form',
      'template' => 'comment-form',
      'path' => $path.'/templates/comment',
    ),
  );
}

function theunfold_preprocess_page(&$vars) {
  _theunfold_set_js();

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

/**
 * Hook preprocess_page
 */
function theunfold_process_page(&$vars) {
}

/**
 * Hook process_node
 */
function theunfold_process_node(&$vars) {
  if ($vars['type'] == 'package') {
    if (isset($vars['content']['field_displaytitle']) && !empty($vars['content']['field_displaytitle'])) {
      // This handles the title_prefix, title_suffix and title_attributes
      // These are usually used for the title, we report theme on the displaytitle

      $rendered_title_prefix = isset($vars['title_prefix']) ? render($vars['title_prefix']) : '';
      if (isset($vars['title_attributes']) && !empty($vars['title_attributes'])) {
        $attributes_prefix_tag = '<span '.$vars['title_attributes'].' >';
      } else {
        $attributes_prefix_tag = '';
      }
      if (!isset($vars['content']['field_displaytitle'][0]['#prefix'])) {
        $vars['content']['field_displaytitle'][0]['#prefix'] = '';
      }
      $vars['content']['field_displaytitle'][0]['#prefix'] = $rendered_title_prefix . $attributes_prefix_tag . $vars['content']['field_displaytitle'][0]['#prefix'];

      $rendered_title_suffix = isset($vars['title_suffix']) ? render($vars['title_suffix']) : '';
      if (isset($vars['title_attributes']) && !empty($vars['title_attributes'])) {
        $attributes_suffix_tag = '</span>';
      } else {
        $attributes_suffix_tag = '';
      }
      if (!isset($vars['content']['field_displaytitle'][0]['#suffix'])) {
        $vars['content']['field_displaytitle'][0]['#suffix'] = '';
      }
      $vars['content']['field_displaytitle'][0]['#suffix'] = $vars['content']['field_displaytitle'][0]['#suffix'] . $attributes_suffix_tag . $rendered_title_suffix;
    }
  }
}


/**
 * To allow contextual link on node
 * see: http://drupal.stackexchange.com/questions/12528/contextual-link-for-individual-node
 *
 */
function theunfold_node_view_alter(&$build) {
  if(isset($build['#node'])) {
    $node = $build['#node'];
    if(!empty($node->nid)) {
      $build['#contextual_links']['node'] = array('node', array($node->nid));
    }
  }
}

/**
 * Hook preprocess_node
 */
function theunfold_preprocess_node(&$vars) {
  if (strstr($vars['view_mode'], 'targetblock_')) {
    $node_url = $vars['node_url'];
    if (!isset($vars['content']['#prefix'])) {
      $vars['content']['#prefix'] = '';
    }
    if (!isset($vars['content']['#suffix'])) {
      $vars['content']['#suffix'] = '';
    }
    $vars['content']['#prefix'] = '<a href="'.$node_url.'">'.$vars['content']['#prefix'];
    $vars['content']['#suffix'] .= '</a>';
  }
}

function theunfold_preprocess_field(&$variables) {
  $element = $variables['element'];
  switch ($element['#field_name']) {
    /*
    case 'field_textchapo':
      $variables['classes_array'][] = 'heading';
      break;
*/
    case 'field_textbody':
    case 'field_linkslists':
      $variables['classes_array'][] = 'article-body';
      break;
/*
    case 'field_textbarette':
      $variables['classes_array'][] = 'cat';
      break;
*/
    case 'field_copyright':
      $variables['classes_array'][] = 'meta';
      break;
  }
}

function theunfold_preprocess_dpicontenttypes_image_render_context(&$variables, $base_theme) {
  if (isset($variables['context'])) {
    $atom = $variables['atom'];
    $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);
    $title_value = $atom_wrapper->field_displaytitle->value();
    $variables['title'] = check_plain($title_value['value']);
    $summary_value = $atom_wrapper->field_byline->value();
    $variables['summary'] = check_plain($summary_value['value']);
  }
}

/**
 * Theme function to render the html of the media box
 */
function theunfold_theme_package_top_items($vars) {
  drupal_add_js(drupal_get_path('theme', 'theunfold').'/scripts/mylibs/pagination.js');

  $content = '';
  if (isset($vars['topItems']) && is_array($vars['topItems']) && count($vars['topItems']) != 0) {
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
 * _theunfold_set_js : Set all necessary javascripts
 */
function _theunfold_set_js() {
  $path = drupal_get_path('theme', 'theunfold');

  // Put Scripts at the Bottom
  // @info: Look at hook_closure
  $js_options = array(
    //'type' => 'theme',
    'scope' => 'footer',
  );
  
  
  /*
  drupal_add_js('http://cdn.jquerytools.org/1.2.7/full/jquery.tools.min.js', $js_options);
  drupal_add_js($path.'/scripts/mylibs/script.js', $js_options);
  drupal_add_js($path.'/scripts/mylibs/swipe.js', $js_options);
  drupal_add_js($path.'/scripts/mylibs/ticker.js', $js_options);
  drupal_add_js($path.'/scripts/mylibs/jquery.easing.1.3.js', $js_options);
  drupal_add_js($path.'/scripts/mylibs/jquery.elastislide.js', $js_options);
*/
  
  
  
  // Add js to the header
  drupal_add_js($path.'/scripts/vendor/modernizr-2.6.1.min.js');
  drupal_add_js("
    (function(doc) {
    var addEvent = 'addEventListener',
        type = 'gesturestart',
        qsa = 'querySelectorAll',
        scales = [1, 1],
        meta = qsa in doc ? doc[qsa]('meta[name=viewport]') : [];

      function fix() {
        meta.content = 'width=device-width,minimum-scale=' + scales[0] + ',maximum-scale=' + scales[1];
        doc.removeEventListener(type, fix, true);
      }

      if ((meta = meta[meta.length - 1]) && addEvent in doc) {
        fix();
        scales = [.25, 1.6];
        doc[addEvent](type, fix, true);
      }

    }(document));
", 'inline');
/*
  $vars['scripts'] = drupal_get_js(); // necessary in D7?
  $vars['closure'] = theme('closure'); // necessary in D7?
*/
}
