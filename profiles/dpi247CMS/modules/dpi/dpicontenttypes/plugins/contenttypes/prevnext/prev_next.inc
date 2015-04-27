<?php
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
    'title' => t('Navigation inter-articles'),
    'description' => t('Affiche un lien vers l\'article précédent et suivant du node en cours'),  
    'edit form' => 'dpicontenttypes_prev_next_articles_edit_form',
    'render callback' => 'dpicontenttypes_prev_next_articles_render',
    'category' => 'Naviguation',    
    'group' => 'DPI Core',
    'required context' => new ctools_context_required(t('Node'), 'node'),  
);

/**
 * This function create the block use to do naviguation
 * @param Array $plugins
 */
function dpicontenttypes_accroche_content_type_content_types($plugins) {
  $types = array ();  
  $types ["node_naviguation"] = array (
    'title' => t('Navigation inter-articles'),
    'description' => t('Affiche un lien vers l\'article précédent et suivant du node en cours'),
    'icon' => drupal_get_path ( "module", "dpicontenttypes" ) . '/icons/navigation.ico',
    'category' => 'Naviguation',
    'group' => 'DPI Core', 
    'required context' => new ctools_context_required(t('Node'), 'node'),  
  );   
  return $types;
}

/**
 * Declares edit form
 * @param array $form
 * @param array $form_state
 * @return array $form
 */
function dpicontenttypes_prev_next_articles_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $prev_next = array(
    'show_prev' => t('Previous'),
    'show_next' => t('Next'), 
  );

  $form['prev_next'] = array(
    '#title' => t('Show'),
    '#type' => 'checkboxes',
    '#default_value' => $conf['prev_next'],
    '#description' => t('Select element'),
    '#options' => $prev_next,
  );
 
  return $form;
}

/**
 * Validate form
 * @param array $form
 * @param array $form_state
 */
function dpicontenttypes_prev_next_articles_edit_form_validate(&$form, &$form_state) {
  $form_state['conf']['prev_next'] = $form_state['values']['prev_next'];
}

/**
 * Declares and returns the object block rendered as expected by the render API
 * @param string $subtype
 * @param array $conf
 * @param array $panel_args
 * @param array $context
 * @return object $block
 */
function dpicontenttypes_prev_next_articles_render($subtype, $conf, $panel_args, &$context) {  
  drupal_add_css(drupal_get_path('module', 'dpicontenttypes').'/css/prev_next.css');
  drupal_add_js(drupal_get_path('module', 'dpicontenttypes').'/js/prev_next.js');
  
  //@todo mettre dpi cache si possible ?
  $rendered_list_links = dpicontenttypes_get_prev_links_content($context, $conf);
  
  $block = new stdClass();
  $block->content = $rendered_list_links;
  return $block;
}

/**
 * This function generate the content of prev next and return the theme associate
 * @param array $context
 * @param array $conf
 * @return theme
 */
function dpicontenttypes_get_prev_links_content($context, $conf){
  $node_prev_infos = dpicontenttypes_prev_next_articles_get_infos_node($context, $conf, FALSE);
  $node_next_infos = dpicontenttypes_prev_next_articles_get_infos_node($context, $conf, TRUE);
  
  return theme('dpidestinations_prev_next', array('prev' => $node_prev_infos, 'next' => $node_next_infos));
}

/**
 * Return the node informations for previous or next article
 * @param $context the current context
 * @param $conf the configuration ctools
 * @param $next if true, return the next article, else the previous
 * @return array() node informations (title and link)
 */
function dpicontenttypes_prev_next_articles_get_infos_node($context, $conf, $next = TRUE) {  
  if (($next && !$conf['prev_next']['show_next']) || (!$next && !$conf['prev_next']['show_prev'])) {
    // dans la conf du bloc, on a choisi d'afficher qu'un seul des deux articles (prédcedent et suivant)
    return array();
  }
  
  $language = $context->data->language;
  $terms = field_get_items('node', $context->data, 'field_destinations');
  if(isset($terms) && is_array($terms)){
    $term = current($terms);  
    if (isset($term['tid']) && isset($term['virtual_rank']) && isset($term['target'])) {
      $results = dpicontenttypes_prev_next_articles_build_query($term['tid'], $term['target'], $term['virtual_rank'], $next);
      if (isset($results) && $results!=null) {
        $link = 'node/'.$results->entity_id;
        return array('title'=>$results->title, 'link'=> '/'.$link, 'alias' => '/'.drupal_get_path_alias($link));
      }
    }
  }  
  
  return array();
}

/**
 * Return the query result for previous or next node
 * 
 * @param $tid the current tid
 * @param $target node target
 * @param $timerank node timerank
 * @param $next if true, return the next article, else the previous
 * @return array() : query results
 */
function dpicontenttypes_prev_next_articles_build_query($tid, $target, $timerank, $next = TRUE) {
  $op = $next?'<':'>';
  $order = $next?'DESC':'ASC';
  $query = db_select('field_data_field_destinations', 'd');
  $query->join('node', 'n', 'n.nid = d.entity_id');
  $query
    ->fields('d', array('entity_id'))
    ->fields('n', array('title'))
    ->condition('d.field_destinations_virtual_rank', $timerank, $op)
    ->condition('d.field_destinations_tid', $tid, '=')
    ->condition('d.field_destinations_target', $target, 'LIKE')
    ->condition('d.delta', 0, '=')
    ->condition('n.status', 1, '=')
    ->orderBy('d.field_destinations_virtual_rank', $order)
    ->range(0, 1);  

  return $query->execute()->fetchObject();
}