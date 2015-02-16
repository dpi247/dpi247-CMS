<?php
$plugin = array (
  'title' => 'Accroche',
  'description' => 'Display a list of the newest nodes',
  'edit form' => 'dpiblocks_accroche_plugin_edit_form',
  'render callback' => 'dpiblocks_accroche_render',
  'admin info' => 'dpiblocks_accroche_admin_info',
  'category' => 'Accroche',
  'group' => 'DPI Core',
  'all contexts' => TRUE 
);

/**
 * Ajout des blocks d'informations pour les accroches ainsi que les accroches réutilisables
 * @param array $plugins
 * @return array
 */
function dpiblocks_accroche_content_type_content_types($plugins) {
  $types = array ();
  $layouts = _dpiblocks_accroche_get_all_teaser ();
  
  foreach ( $layouts as $key => $layout ) {
    $types [$key] = array (
      'title' => check_plain ( $layout [0] ),
      'subtype' => $key,
      'description' => check_plain ( $layout [0] ),
      'icon' => drupal_get_path ( "module", "dpidestinations" ) . '/images/1412086058_93156.ico',
      'category' => 'Accroche',
      'group' => 'DPI Core', 
      'all contexts' => TRUE
    ); 
  }
  
  /* foreach on entity */
  $entities = entity_load('accroche');
  foreach($entities as $aid => $entity){
    $types ['accroche_'.$entity->machine_name] = array (
      'title' => check_plain ( $entity->title ),
      'subtype' => $entity->machine_name,
      'description' => check_plain ( $entity->title ),
      'icon' => drupal_get_path ( "module", "dpiblocks" ) . '/icons/1422908884_kwrite.png',
      'category'     => t('DPI Blocks'),
      'group'        => t('DPI Core'),
      'all contexts' => TRUE
    );
  }

  return $types;
}

/**
 * Formulaire de création et de modification d'une accroche
 * @param array $form
 * @param array $form_state
 * @return $form
 */
function dpiblocks_accroche_plugin_edit_form($form,& $form_state){ 
  form_load_include($form_state, 'inc', 'dpiblocks', 'plugins/content_types/accroche');
  form_load_include($form_state, 'inc', 'dpiblocks', 'includes/custom_accroche.admin');
  
  $entity = _dpiblocks_accroche_get_entity_by_subtype($form_state['pane']->subtype);
  if(isset($entity) && empty($entity)  && isset($form_state['subtype_name'])){
    $entity = new stdClass();
    $entity->layout = $form_state['subtype_name'];
  }
  $form += dpiblocks_get_entity_form('accroche_form', $form_state,$entity);
  
  unset($form['accroche']['revision']);
  unset($form['accroche']['submit']);  
  
  return $form;
}

/**
 * Formulaire de soumission pour la création et la modification d'une accroche
 * @param array $form
 * @param array $form_state
 */
function dpiblocks_accroche_plugin_edit_form_submit($form, &$form_state) {
  $form_state2=array();
  $form_state2['values']=$form_state['values'];  
  //load form include
  form_load_include($form_state2, 'inc', 'dpiblocks', 'plugins/content_types/accroche');
  form_load_include($form_state2, 'inc', 'dpiblocks', 'includes/custom_accroche.admin');
  //submit
  drupal_form_submit('accroche_form', $form_state2);
  $form_state['pane']->subtype = $form_state2['accroche']->machine_name;
  
  /* invalidate cache */
  dpicache_api_regenerate_all_cache_entry_callback('dpiblocks_accroche_feed', 'accroche_'.$form_state2['accroche']->machine_name);
}

/**
 * Retourne les accroches disponibles
 * @return array
 */
function _dpiblocks_accroche_get_all_teaser() {
  return variable_get ( 'dpiblocks_accroche_settings' );
}

/**
 * Retourne le nom de la vue a utilisé
 * @return string
 */
function _dpiblocks_accroche_get_view_name() {
  return 'dpiblocks_accroche_default';
}

/**
 * Retourne un object contenant le rendu de l'accroche
 * @param String $subtype
 * @param array $conf
 * @param array $args
 * @param String $contexts
 * @return stdClass
 */
function dpiblocks_accroche_render($subtype, $conf, $args, $contexts) {
  $elements = dpicache_get_cache_entry('dpiblocks_accroche_feed', $subtype, $conf, $args);
  /* Prepare render element and return it */
  $block = new stdClass ();
  $block->title = (isset ( $elements['entity']->link ) && $elements['entity']->link != "") ? l ( $elements['entity']->title, $elements['entity']->link ) : $elements['entity']->title;
  $block->content = $elements['view'];
  $block->admin_links = array ();
  return $block;
}

/**
 * Définis la categorie pour les différentes accroches lors de l'enregistrement en cache
 * @param string $cachedefinition
 * @param array $arguments
 * @return string
 */
function dpiblocks_accroche_get_category_callback($cachedefinition = '', $arguments = array()){
  if(isset($arguments[0]) && is_string($arguments[0])){
    return 'accroche_'.$arguments[0];
  }
  return "dpiblocks_accroche";
}

/**
 * Cette fonction crée le rendu de la vue et rend également la configuration de l'entité accroche selectionnée
 * @param String $subtype
 * @return array(view->render, entity_info)
 */
function dpiblocks_accroche_content_type_generate_feed($subtype){
  $info_entity = _dpiblocks_accroche_get_entity_by_subtype($subtype);
  
  /* Prepare element for view */
  $teasers = _dpiblocks_accroche_get_all_teaser ();
  if (isset ( $info_entity->type ) && $info_entity->type == "node") {
    $string_element = "";
    $vals = explode ( ";", $info_entity->hash_cache_data );
    array_shift($vals); //commence par un ';' donc la 1er valeur est vide
    array_pop($vals); //termine par un ';' donc la dernière valeur est vide
    foreach ($vals as $val){
      if(isset($val) && $val != ""){
        $string_element .= $val."+";
      }
    }
    $string_element = substr($string_element,0,-1);
  } elseif (isset ( $info_entity->type ) && $info_entity->type == "destination") {
    $array_val = unserialize($info_entity->data);
    $string_element = dpiblocks_define_node_element_string($array_val, $info_entity);
  }
  
  /* create view */
  $view_name = _dpiblocks_accroche_get_view_name ();
  $view = views_get_view ( $view_name );
  $view->set_display ( $view_name );
  $view->accroche_view_mode = $teasers [$info_entity->layout] [1];
  
  /* set arguments and execute view */
  $view->set_arguments ( array($string_element) );
  $view->pre_execute ();
  $view->execute ();
  
  return array('view' => $view->render(), 'entity' => $info_entity);
}

/**
 * Cette fonction définit le type de rendu a fournir pour les accroches de types destinations.
 * Renvois une chaine de caractère. Ex: 1+5+6+7, les nombres représentent le nid des noeuds
 * @param array $dest
 * @param array $info_entity
 * @return string
 */
function dpiblocks_define_node_element_string($dest, $info_entity){
  module_load_include('inc','dpidestinations', 'plugins/ctools/content_types/targetblock/targetblock');
  $string = "";
  if(count($dest) > 1){    
    if(isset($info_entity->mixing)){
      $switch = $info_entity->mixing;
      if(strstr($info_entity->mixing, 'custom')!==FALSE){
        $tmp_info = explode('_', $info_entity->mixing);
        $switch = (isset($tmp_info[0])) ? $tmp_info[0] : '';
        $customVal = (isset($tmp_info[1])) ? $tmp_info[1] : '';
      }
      switch($switch){
        case 'alternate':
            _dpiblocks_accroche_get_alternate_mixing_result($string, $info_entity);
          break;
        case 'random':
            _dpiblocks_accroche_get_random_mixing_result($string, $info_entity);
          break;
        case 'custom':
            _dpiblocks_accroche_get_custom_mixing_result($string, $info_entity, $customVal);
          break;
        default:
            _dpiblocks_accroche_get_default_mixing_result($string, $info_entity);
          break;
      }
    }
  } else{
    _dpiblocks_accroche_get_default_mixing_result($string, $info_entity);
  } 
  return substr($string, 0 , -1);
}

/**
 * Cette fonction crée la chaine de caractère a partir des nid des nodes.
 * Cette fonction definit la chaine en suivant l'arrivée des résultats.
 * @param String $string
 * @param stdClass $info_entity
 */
function _dpiblocks_accroche_get_default_mixing_result(& $string, $info_entity){
  $nb_el = $info_entity->nb_max_value;
  foreach ( unserialize($info_entity->data) as $destination ) {
    try {
      $targetview = dpiblocks_create_targetview_from_destination($destination);      
      foreach($targetview->result as $el){
        $string .= $el->nid.'+';
        $nb_el--;
        if(!$nb_el) break;
      }
      if(!$nb_el) break;
    } catch (Exception $e) {
    }
  }
}

/**
 * Cette fonction retourne les résultats en alternant entre chacune des destinations précisées.
 * @param String $string
 * @param stdClass $info_entity
 */
function _dpiblocks_accroche_get_alternate_mixing_result(& $string, $info_entity){
  $results = array();
  //create results
  foreach ( unserialize($info_entity->data) as $destination ) {
    try {
      $targetview = dpiblocks_create_targetview_from_destination($destination);
      $results[] = $targetview->result;
    } catch (Exception $e) {
    }
  }
  //use results
  return _dpiblocks_accroche_merge_table_alternate($string, $results, $info_entity->nb_max_value);
}

/**
 * Cette fonction retourne les résultats en alternant les valeurs en fonction de la variables résults.
 * @param String $string
 * @param Array $results
 * @param integer $nb_max_value
 */
function _dpiblocks_accroche_merge_table_alternate(& $string, $results, $nb_max_value){
  $nb_table = count($results);
  $val_max = 0;
  for ($i = 0; $i < $nb_table; $i++){
    $val_max += count($results[$i]);
  }
  if($val_max > $nb_max_value)
    $val_max = $nb_max_value;
  $tmp_val = 0;
  for($i=0;$i<$val_max && $tmp_val<$val_max;$i++){
    for($j=0;$j<$nb_table && $tmp_val<$val_max;$j++){
      if(isset($results[$j][$i]) && $tmp = $results[$j][$i]){
        $string .= $tmp->nid.'+';
        $tmp_val++;
      }
    }
  }
}

/**
 * Cette fonction retourne les resultats de manière aléatoire.
 * @param String $string
 * @param stdClass $info_entity
 */
function _dpiblocks_accroche_get_random_mixing_result(& $string, $info_entity){
  $results = array();
  foreach ( unserialize($info_entity->data) as $destination ) {
    try {
      $targetview = dpiblocks_create_targetview_from_destination($destination);
      $results = array_merge($results, $targetview->result);      
    } catch (Exception $e) {
    }
  }
  if($info_entity->nb_max_value > count($results)){
    $nb_slice = count($results);
  }else{
    $nb_slice = $info_entity->nb_max_value;
  }
  if(shuffle($results) && $slice_array = array_slice($results, 0, $nb_slice)){
    foreach($slice_array as $node){
      $string .= $node->nid.'+';
    }
  }  
}

/**
 * Cette fonction retourne les résultats en suivant le schéma précisé dans la variable $customVal.
 * @param String $string
 * @param stdClass $info_entity
 * @param String $customVal
 */
function _dpiblocks_accroche_get_custom_mixing_result(& $string, $info_entity, $customVal){
  $results = array();
  //create results
  foreach ( unserialize($info_entity->data) as $destination ) {
    try {
      $targetview = dpiblocks_create_targetview_from_destination($destination);
      $results[] = $targetview->result;
    } catch (Exception $e) {
    }
  }
  //use results
  return _dpiblocks_accroche_merge_table_custom($string, $results, $customVal);
}

/**
 * Cette fonction retourne les résultats en suivant les valeur fournie dans results.
 * @param String $string
 * @param Array $results
 * @param String $customVal
 */
function _dpiblocks_accroche_merge_table_custom(& $string, $results, $customVal){
  $tab_val = explode(',', $customVal);
  foreach($tab_val as $destinationLine){
    if(isset($results[$destinationLine-1]) && !empty($results[$destinationLine-1])){
      $val = current($results[$destinationLine-1]); 
      $string .= $val->nid.'+';
      array_shift($results[$destinationLine-1]);
    }
  }
}

/**
 * Cette fonction retourne la targetview associée à la destination passé en paramètre.
 * @param stdClass $destination
 */
function dpiblocks_create_targetview_from_destination($destination){
  $targetBlockController = new TargetBlockController ( 'targetblock' );
  $panel_display = dpidestinations_api_get_panelsdisplay_for_sectionterm(intval ( $destination ["destination_section"] ));
  $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta ($panel_display->did, $destination ["destination_target"], 0 );
  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
  $target=$targetblock_wrapper->target->value();
  return _dpidestinations_targetblock_content_type_helper_get_global_targetview ( intval ( $destination ["destination_section"] ), $destination ["destination_target"], $targetblock );
}

/**
 * Cette fonction retourne les informations sur le content pane.
 * @param String $subtype
 * @param Array $conf
 * @param String $contexts
 * @return stdClass
 */
function dpiblocks_accroche_admin_info($subtype, $conf, $contexts) {
  $info_entity = _dpiblocks_accroche_get_entity_by_subtype($subtype);
  $block = new stdClass ();
  $block->title = $info_entity->override_title ? $info_entity->override_title : $info_entity->title;
  $block->content = t ( 'Accroche key : @accroche_key<br />Title : @title<br />Type : @type', array (
    '@accroche_key' => $subtype,
    '@title' => $info_entity->title,
    "@type" => $info_entity->layout 
  ) );
  return $block;
}

/**
 * Cette fonction retourne l'entité accroche passé associé au machine name associé à la chaine de caractère passé en paramètre.
 * @param String $machine_name
 * @return stdClass
 */
function _dpiblocks_accroche_get_entity_by_subtype($machine_name){
  // Create a new accroche controller
  $customContentController = new AccrocheController('accroche');
  // Return the accroche entity
  return $customContentController->load_by_machine_name($machine_name);
}
