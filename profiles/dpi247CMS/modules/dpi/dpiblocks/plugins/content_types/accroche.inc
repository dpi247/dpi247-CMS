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
    $types [$entity->machine_name] = array (
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

function dpiblocks_accroche_plugin_edit_form($form,& $form_state){  
  if(!isset($form_state['conf']['view_mode']))
    $form_state['conf']['view_mode'] = $form_state["subtype_name"];
  
  form_load_include($form_state, 'inc', 'dpiblocks', 'plugins/content_types/accroche');
  form_load_include($form_state, 'inc', 'dpiblocks', 'includes/custom_accroche.admin');
  
  $entity = _dpiblocks_accroche_get_entity_by_subtype($form_state['pane']->subtype);
  $form += dpiblocks_get_entity_form('accroche_form', $form_state,$entity);
  
  unset($form['accroche']['revision']);
  unset($form['accroche']['submit']);  
  
  return $form;
}

/**
 * Content type submit handler.
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
}

/**
 * This function return different kind of teaser
 *
 * @return multitype:string
 */
function _dpiblocks_accroche_get_all_teaser() {
  return variable_get ( 'dpiblocks_accroche_settings' );
}

function _dpiblocks_accroche_get_view_name() {
  return 'dpiblocks_accroche_default';
}

function dpiblocks_accroche_render($subtype, $conf, $args, $contexts) {
  $info_entity = _dpiblocks_accroche_get_entity_by_subtype($subtype);
  
  /* Prepare element for view */
  $teasers = _dpiblocks_accroche_get_all_teaser ();
  if (isset ( $info_entity->layout ) && $info_entity->layout == "node") {
    $string_element = "";
    $vals = explode ( ";", $info_entity->data );
    foreach ($vals as $val){
      if(isset($val) && $val != ""){
        $string_element .= $val."+";
      }
    }
    $string_element = substr($string_element,0,-1);
  } elseif (isset ( $info_entity->layout ) && $info_entity->layout == "destination") {
    $array_val = unserialize($info_entity->data);
    $string_element = dpiblocks_define_node_element_string($array_val);
  }
  /* create view */
  $view_name = _dpiblocks_accroche_get_view_name ();
  $view = views_get_view ( $view_name );
  $view->set_display ( $view_name );
  $view->accroche_view_mode = $teasers [$conf ["view_mode"]] [1];

  /* set arguments and execute view */
  $view->set_arguments ( array($string_element) );
  $view->pre_execute ();
  $view->execute ();

  /* Prepare render element and return it */
  $block = new stdClass ();
  $block->title = (isset ( $info_entity->link ) && $info_entity->link != "") ? l ( $info_entity->title, $info_entity->link ) : $info_entity->title;
  $block->content = $view->render ();
  $block->admin_links = array ();
  return $block;
}

function dpiblocks_define_node_element_string($dest){
  module_load_include('inc','dpidestinations', 'plugins/ctools/content_types/targetblock/targetblock');
  $string = "";  
  foreach ( $dest as $destination ) {
    try {
      $targetBlockController = new TargetBlockController ( 'targetblock' );
      $panel_display = dpidestinations_api_get_panelsdisplay_for_sectionterm(intval ( $destination ["destination_section"] ));
      $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta ($panel_display->did, $destination ["destination_target"], 0 );
      $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
      $target=$targetblock_wrapper->target->value();
      $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview ( intval ( $destination ["destination_section"] ), $destination ["destination_target"], $targetblock );
      $val_view = $targetview->result;
      $cpt = $destination ["destination_nb_element"];
      if ($cpt > count ( $val_view ))
        $cpt = count ( $val_view );
      for($i = 0; $i < $cpt; $i ++) {
        $string .= $val_view [$i]->nid.'+';
      }
    } catch (Exception $e) {
    }
  }
  return substr($string, 0 , -1);
}

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

function _dpiblocks_accroche_get_entity_by_subtype($machine_name){
  // Create a new custom content controller
  $customContentController = new AccrocheController('accroche');
  // Return the custom_content entity
  return $customContentController->load_by_machine_name($machine_name);
}
