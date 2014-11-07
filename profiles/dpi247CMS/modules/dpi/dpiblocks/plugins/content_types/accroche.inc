<?php
$plugin = array (
  'title' => t ( 'Teaser' ),
  'description' => t ( 'Display a list of the newest nodes' ),
  'edit form' => 'dpiblocks_accroche_plugin_edit_form',
  'render callback' => 'dpiblocks_accroche_render',
  'category' => t ( 'Accroche' ),
  'group' => t ( 'DPI Core' ),
  'all contexts' => TRUE 
);

function dpiblocks_accroche_content_type_content_types($plugins) {
  $types = array ();
  $layouts = _dpiblocks_accroche_get_all_teaser ();
  foreach ( $layouts as $key => $layout ) {
    $types [$key] = array (
      'title' => check_plain ( $key ),
      'subtype' => $key,
      'description' => check_plain ( $layout ),
      'icon' => drupal_get_path ( "module", "dpidestinations" ) . '/images/1412086058_93156.ico',
      'category' => t ( 'Accroche' ),
      'group' => t ( 'DPI Core' ) 
    );
  }
  return $types;
}

function dpiblocks_accroche_plugin_edit_form($form, &$form_state) {
  $form_temp = $form;
  $conf = $form_state ['conf'];
  
  if(isset($conf["destination"])){
    $form_state ['values'] ['accroche'] ['type'] = "destination";
  }elseif(isset($conf["node"])){
    $form_state ['values'] ['accroche'] ['type'] = "node";
  }
  
  $form ['accroche'] = array (
    '#prefix' => '<div id="accroche-form">',
    '#suffix' => '</div>',
    "#tree" => true 
  );
  
  form_load_include ( $form_state, 'inc', 'dpiblocks', 'plugins/content_types/accroche' );
  
  $form ['accroche'] ['title'] = array (
    '#type' => 'textfield',
    '#title' => t ( 'Enter a Title' ),
    '#required' => TRUE,
    '#default_value' => $conf ["title"] 
  );
  
  $form ['accroche'] ['url'] = array (
    '#type' => 'textfield',
    '#title' => t ( 'Enter url for title link' ),
    '#required' => FALSE,
    '#default_value' => $conf ["url"],
    '#description' => t ( 'URL should be : http://www.mysite.com/ask/me' ) 
  );
  
  $form ['accroche'] ['layout'] = array (
    '#type' => 'textfield',
    '#title' => t ( 'Layout choose' ),
    '#default_value' => $form_state ['subtype_name'],
    '#required' => TRUE,
    '#disabled' => TRUE 
  );
  
  $form ['accroche'] ['type'] = array (
    '#type' => 'select',
    '#title' => t ( 'Select type of value' ),
    '#default_value' => $conf ["type"],
    '#options' => array (
      '' => t ( 'Choose a node selection method' ),
      'node' => t ( 'Node' ),
      'destination' => t ( 'Destination' ) 
    ),
    '#description' => t ( 'Choose your type of informations' ),
    '#ajax' => array (
      'callback' => 'dpiblocks_ajax_callback_define_type_form',
      'wrapper' => 'accroche-form',
      'method' => 'replace' 
    ) 
  );
  
  if (! strcmp ( $form_state ['values'] ['accroche'] ['type'], 'node' )) {
    $form ['accroche'] ['node'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Enter node id' ),
      '#description' => t ( 'add ; between each node id' ),
      '#required' => FALSE,
      
      '#autocomplete_path' => 'package/autocomplete/input[name="node"]',
      '#default_value' => $conf ["node"] 
    );
  } elseif (! strcmp ( $form_state ['values'] ['accroche'] ['type'], 'destination' )) {
    $form ['accroche'] ['destination_section'] = array (
      '#type' => 'select',
      '#title' => t ( 'Choose a section' ),
      '#required' => FALSE,
      '#options' => dpidestinations_api_get_all_sections_term_available_as_select_options ( FALSE ),
      '#ajax' => array (
        'callback' => 'dpiblocks_ajax_callback_define_target_from_form',
        'wrapper' => 'destination-target-form',
        'method' => 'replace' 
      ),
      '#default_value' => $conf ["destination"] [0] ["section"] 
    );
    
    if(isset($conf ["destination"] [0] ["section"]) && $conf ["destination"] [0] ["section"]!=""){
      $options =  __dpidestinations_get_selectoption_get_targets_for_sectionterm($conf ["destination"] [0] ["section"]);
    }
    
    $form ['accroche'] ['destination_target'] = array (
      '#type' => 'select',
      '#title' => t ( 'Choose a section' ),
      '#required' => FALSE,
      "#options" => $options,
      '#default_value' => $conf ["destination"] [0] ["target"],
      '#prefix' => '<div id="destination-target-form">',
      '#suffix' => '</div>' 
    );    
    
    $form ['accroche'] ['destination_nb_element'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'Enter number of element that you want' ),
      '#required' => TRUE,
      '#default_value' => $conf ["destination"] [0] ["nb_element"] 
    );
  }
  
  return $form;
}

function dpiblocks_ajax_callback_define_type_form($form, $form_state) {
  return $form ['accroche'];
}

function dpiblocks_ajax_callback_define_target_from_form($form, $form_state) {
  $form ["accroche"] ["destination_target"] ["#options"] = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $form_state ["values"] ["accroche"] ["destination_section"] );
  return $form ["accroche"] ["destination_target"];
}

function dpiblocks_accroche_plugin_edit_form_validate($form, &$form_state) {
  if (isset ( $form_state ['values'] ["accroche"] ['type'] ) && $form_state ['values'] ["accroche"] ['type'] == "node") {
    if (! isset ( $form_state ['values'] ["accroche"] ['node'] ) || $form_state ['values'] ["accroche"] ['node'] == "")
      form_set_error ( "accroche][node", "Pleaser enter a value into the node field" );
  } elseif (isset ( $form_state ['values'] ["accroche"] ['type'] ) && $form_state ['values'] ["accroche"] ['type'] == "destination") {
    if (! isset ( $form_state ['values'] ["accroche"] ['destination_section'] ) || $form_state ['values'] ["accroche"] ['destination_section'] == "")
      form_set_error ( "accroche][destination_section", "Select a section value" );
    if (! isset ( $form_state ['values'] ["accroche"] ['destination_target'] ) || $form_state ['values'] ["accroche"] ['destination_target'] == "")
      form_set_error ( "accroche][destination_target", "Please select a target" );
    if (! isset ( $form_state ['values'] ["accroche"] ['destination_nb_element'] ) || ! is_numeric ( $form_state ['values'] ["accroche"] ['destination_nb_element'] ))
      form_set_error ( "accroche][destination_nb_element", "Enter a numeric value" );
  } else {
    form_set_error ( "accroche][type", "Pleaser choose a section" );
  }
}

/**
 * Content type submit handler.
 */
function dpiblocks_accroche_plugin_edit_form_submit($form, &$form_state) {
  $form_state ['conf'] ['title'] = $form_state ['values'] ["accroche"] ['title'];
  $form_state ['conf'] ['url'] = $form_state ['values'] ["accroche"] ['url'];
  $form_state ['conf'] ['layout'] = $form_state ['values'] ["accroche"] ['layout'];
  $form_state ['conf'] ['type'] = $form_state ['values'] ["accroche"] ['type'];
  if (isset ( $form_state ['values'] ["accroche"] ['type'] ) && $form_state ['values'] ["accroche"] ['type'] == "destination") {
    $form_state ['conf'] ['destination'][0] = array(
      'section' => $form_state ["values"] ["accroche"] ["destination_section"],
      'target' => $form_state ["values"] ["accroche"] ["destination_target"],
      'nb_element' => $form_state ["values"] ["accroche"] ['destination_nb_element'],
    );
  } else {
    $form_state ['conf'] ['node'] = $form_state ['values'] ["accroche"] ['node'];
  }
}

/**
 * This function return different kind of teaser
 *
 * @return multitype:string
 */
function _dpiblocks_accroche_get_all_teaser() {
  return array (
    'accroche_small' => 'targetblock_small',
    'accroche_big' => 'targetblock_big',
    'accroche_medium' => 'targetblock_medium',
    'accroche_color_red' => 'targetblock_red',
    'accroche_background_red' => 'targetblock_red',
  );
}

function _dpiblocks_accroche_get_view_name() {
  return 'dpiblocks_accroche_default';
}

function dpiblocks_accroche_render($subtype, $conf, $args, $contexts) {
  
  /* Prepare element for view */
  $elements = array ();
  $teasers = _dpiblocks_accroche_get_all_teaser();
  $elements ["layout"] = $teasers[$conf ["layout"]];
  if(isset($conf["type"]) && $conf["type"]=="node"){
    $elements ["node_id"] = explode ( ";", $conf ["node"] );
  }elseif(isset($conf["type"]) && $conf["type"]=="destination"){
    $elements ["destination"] = $conf["destination"];
  }
  
  /*@todo 
   *     la view est déjà prête à recevoir plusieurs destination il suffit de renvoyer
   *     un array sous le format
   *     array(
   *       array( "section" => 1, "target" => "main_content", "nb_element" => 5 ),
   *       array( "section" => 3, "target" => "secodnary_content", "nb_element" => 2 ),
   *     )
   */
  
  /* create view */
  $view_name = _dpiblocks_accroche_get_view_name ();
  $view = views_get_view ( $view_name );
  $view->set_display ( 'dpiblocks_accroche_default' );
  /* unset pager */
  $pager = $view->display_handler->get_option ( 'pager' );
  $pager ['type'] = 'none';
  $view->display_handler->set_option ( 'pager', $pager );
  
  /* set arguments and execute view */
  $view->set_arguments ( $elements );
  $view->pre_execute ();
  $view->execute ();
  
  /* Prepare render element and return it */
  $block = new stdClass ();
  $block->title = (isset ( $conf ["url"] )) ? l ( $conf ["title"], $conf ["url"] ) : $conf ["title"];
  $block->content = $view->render ();
  $block->admin_links = array ();
  return $block;
}