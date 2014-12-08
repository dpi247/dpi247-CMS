<?php

/**
 * @file
 * Theme file, contains implementations of theme functions and preprocessors.
 */

/**
 * Preprocess function for widget template file. Provides variables for default widget.
 * @param  $variables
 * @return void
 */
function template_preprocess_dpicomments_reportabuse_widget(&$variables) {
  $voted = $variables['voted'];
  $logged_in = $variables['logged_in'];
  $score = $variables['score'];
  $vote_link = $variables['vote_link'];
  $undo_vote_link = $variables['undo_vote_link'];
  $link_query = $variables['link_query'];
  $can_vote = $variables['can_vote'];
  $can_undo_vote = $variables['can_undo_vote'];
  $vote_text = $variables['vote_text'];
  $undo_vote_text = $variables['undo_vote_text'];

  // if (!$logged_in && !$can_vote) {
  //   $variables['widget_message'] =  l(t('Log in<br />to vote'), 'user', array('html' => TRUE));
  // }

  if ($variables['voted']) { // User already voted.
    // is the user can undo his vote then provide him with link
    if ($can_undo_vote) {
      if ($undo_vote_text != "") {
        $variables['widget_message'] = l($undo_vote_text, $undo_vote_link, array('query' => $link_query, 'attributes' => array('class' => array('reportabuse-link'))));
      }
      else {
        // if we don't have text for undo action, add "arrow-down" link after score.
        $variables['use_arrow_down'] = TRUE;
        $variables['widget_message'] = l(t('Undo vote'), $undo_vote_link, array('query' => $link_query, 'attributes' => array('class' => array('reportabuse-link'))));
      }
    }
  }
  elseif ($can_vote) {
    // User is eligible to vote.
    $variables['widget_message'] = l($vote_text, $vote_link, array('query' => $link_query, 'attributes' => array('class' => array('reportabuse-link'))));
  }
}

/**
 * Theming function for json response.
 * @param  $variables
 * @return array
 *
 * Returns an array of variables which will be send back to browser, after ajax request.
 * @see dpicomments_reportabuse_vote(), dpicomments_reportabuse_undo_vote()
 */
function theme_dpicomments_reportabuse_json_response($variables) {
  switch ($variables['entity_type']) {
    case 'comment':
      return array('widget' => drupal_render(dpicomments_reportabuse_build_comment_jquery_widget($variables['entity_id'], $variables['tag'])));
      break;
  }
}

/**
 * Theming function for comment wrapper
 * @param  $variables
 * @return string
 *
 * Returns theming comment wrapper
 */
function theme_dpicomments_comment_wrapper($variables){
  $element=$variables['element'];
  $pager=$element['pager'];
  // unset($element['pager']);

  foreach(element_children($element) as $key){
    $render = drupal_render($element[$key]);
    // Bien dégueu, mais lorsqu'un commentaire est indenté,
    // La balise div ouvrante (div suplémentaire pour class=indented) est dans le commentaire indenté
    // Et la balise fermante dans le commentaire suivant ...
    // Du coup le ul/li ne fonctionne pas.
    if (substr($render, 0, 5) == "<div>") {
      // Si on ouvre la div, on la fere dans le meme render.
      $render.="</div>";
    }
    elseif (substr($render, 0, 6) == "</div>") {
      // Si le render du commentaire commence par une balise fermante, on la supprime
      $render = substr($render, 6);
    }
    
    if (strlen($render)>0) {
      $items[]=$render;
    }
  }

  $item_list['#items']= $items;
  $item_list['#type']='ul';
  $item_list['#theme']='item_list';

  $output['item_list']=array(
    'type'=>'markup',
    '#markup'=>drupal_render($item_list),
    );

  if ($element['show_more_comment_link']) {
    $output['links']=array(
      'type'=>'markup',
      '#markup'=>l('Plus de commentaires','node/'.$element['nid'].'/comments'),
      );
  }

  return drupal_render($output);
}
