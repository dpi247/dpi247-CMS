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
  return $types;
}

function dpiblocks_accroche_plugin_edit_form($form,$form_state){
  form_load_include($form_state, 'inc', 'dpiblocks','includes/custom_accroche.admin');
  $form = dpiblocks_get_entity_form('accroche_form', $form_state,$entity);
  dsm(function_exists('dpiblocks_ajax_callback_define_type_entity_form'), 'exist');
  unset($form['revision']);
  return $form;
}

/**
 * Content type submit handler.
 */
function dpiblocks_accroche_plugin_edit_form_submit($form, &$form_state) {
  dsm($form_state);  
//   $form_state ['conf'] ['title'] = $form_state ['values'] ["accroche"] ['title'];
//   $form_state ['conf'] ['url'] = $form_state ['values'] ["accroche"] ['url'];
//   $form_state ['conf'] ['layout'] = $form_state ['values'] ["accroche"] ['layout'];
//   $form_state ['conf'] ['type'] = $form_state ['values'] ["accroche"] ['type'];
//   if (isset ( $form_state ['values'] ["accroche"] ['type'] ) && $form_state ['values'] ["accroche"] ['type'] == "destination") {
//     for($i = 0; $i < count ( $form_state ["values"] ["accroche"] ['destination'] ); $i ++) {
//       $form_state ['conf'] ['destination'] [$i] = array (
//         'section' => $form_state ["values"] ["accroche"] ['destination'] [$i] ["destination_section"],
//         'target' => $form_state ["values"] ["accroche"] ['destination'] [$i] ["destination_target"],
//         'nb_element' => $form_state ["values"] ["accroche"] ['destination'] [$i] ['destination_nb_element'] 
//       );
//     }
//   } else {
//     $form_state ['conf'] ['node'] = $form_state ['values'] ["accroche"] ['node'];
//   }
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
  
  /* Prepare element for view */
  $elements = array ();
  $teasers = _dpiblocks_accroche_get_all_teaser ();
  $elements ["layout"] = $teasers [$conf ["layout"]] [1];
  if (isset ( $conf ["type"] ) && $conf ["type"] == "node") {
    $elements ["node_id"] = explode ( ";", $conf ["node"] );
    $string_element = "";
    $vals = explode ( ";", $conf ["node"] );
    foreach ($vals as $val){
      if(isset($val) && $val != ""){
        $string_element .= $val."+";
      }
    }
    $string_element = substr($string_element,0,-1);
  } elseif (isset ( $conf ["type"] ) && $conf ["type"] == "destination") {
    $elements ["destination"] = $conf ["destination"];
    $string_element = dpiblocks_define_node_element_string($conf["destination"]);
  }
  /* create view */
  $view_name = _dpiblocks_accroche_get_view_name ();
  $view = views_get_view ( $view_name );
  $view->set_display ( $view_name );
  $view->accroche_view_mode = $elements ["layout"];

  /* set arguments and execute view */
  $view->set_arguments ( array($string_element) );
  $view->pre_execute ();
  $view->execute ();

  /* Prepare render element and return it */
  $block = new stdClass ();
  $block->title = (isset ( $conf ["url"] ) && $conf ["url"] != "") ? l ( $conf ["title"], $conf ["url"] ) : $conf ["title"];
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
      $panel_display = dpidestinations_api_get_panelsdisplay_for_sectionterm(intval ( $destination ["section"] ));
      $targetblock = $targetBlockController->load_targetblock_by_did_target_and_splitdelta ($panel_display->did, $destination ["target"], 0 );
      $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);
      $target=$targetblock_wrapper->target->value();
      $targetview = _dpidestinations_targetblock_content_type_helper_get_global_targetview ( intval ( $destination ["section"] ), $destination ["target"], $targetblock );
      $val_view = $targetview->result;
      $cpt = $destination ["nb_element"];
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
  $block = new stdClass ();
  $block->title = $conf ['override_title'] ? $conf ['override_title_text'] : $conf ["title"];
  $block->content = t ( 'Accroche key : @accroche_key<br />Title : @title<br />Type : @type', array (
    '@accroche_key' => $subtype,
    '@title' => $conf ["title"],
    "@type" => $conf ["type"] 
  ) );
  return $block;
}

function _dpiblocks_accroche_get_entity_by_subtype($machine_name){
  // Create a new custom content controller
  $customContentController = new Custom_contentController('accroche');
  // Return the custom_content entity
  return $customContentController->load_by_title($machine_name);
}
