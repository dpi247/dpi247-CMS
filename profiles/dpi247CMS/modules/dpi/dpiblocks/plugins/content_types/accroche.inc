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

function dpiblocks_accroche_plugin_edit_form($form, &$form_state) {
  $form_temp = $form;
  $conf = $form_state ['conf'];
  if (isset ( $conf ["destination"] )) {
    $form_state ['values'] ['accroche'] ['type'] = "destination";
  } elseif (isset ( $conf ["node"] )) {
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
    $nb = 1;
    
    // watchdog('debug dpi', '<pre>'.print_r($form_state,1).'</pre>');
    /* si je rebuild && si il s'agit du obuton add more */
    if (isset ( $form_state ['values'] ['accroche'] ['destination'] ) && isset ( $form_state ['triggering_element'] ) && $form_state ['triggering_element'] ["#type"] == "button") {
      $nb = count ( $form_state ['values'] ['accroche'] ['destination'] ) + 1;
    } elseif (isset ( $form_state ['values'] ['accroche'] ['destination'] )) {
      $nb = count ( $form_state ['values'] ['accroche'] ['destination'] );
    } elseif (isset($conf ["destination"])) {
      $nb = count($conf ["destination"]);
    }
    
    /* boucle */
    for($i = 0; $i < $nb; $i ++) {
      $form ['accroche'] ['destination'] [$i] = array (
        '#prefix' => '<div id="destination-add-form" class="container-inline" >',
        '#suffix' => '</div>' 
      );
      
      $form ['accroche'] ['destination'] [$i] ['destination_section'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a section' ),
        '#required' => TRUE,
        '#options' => dpidestinations_api_get_all_sections_term_available_as_select_options ( FALSE ),
        '#ajax' => array (
          'callback' => 'dpiblocks_ajax_callback_define_target_from_form',
          'wrapper' => 'accroche-form',
          'method' => 'replace' 
        ),
        '#default_value' => (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] )) ? $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] : isset ( $conf ["destination"] [$i] ["section"] ) ? $conf ["destination"] [$i] ["section"] :  '' 
      );
      
      if (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] )) {
        $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_section'] );
      } elseif (isset ( $conf ["destination"] [$i] ["section"] ) && $conf ["destination"] [$i] ["section"] != "") {
        $options = __dpidestinations_get_selectoption_get_targets_for_sectionterm ( $conf ["destination"] [$i] ["section"] );
      } else {
        $options = array ();
      }
      
      $form ['accroche'] ['destination'] [$i] ['destination_target'] = array (
        '#type' => 'select',
        '#title' => t ( 'Choose a section' ),
        '#required' => FALSE,
        "#options" => $options,
        '#default_value' => (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_target'] )) ? $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_target'] : isset ( $conf ["destination"] [$i] ["target"] ) ? $conf ["destination"] [$i] ["target"] : '' 
      );
      
      $form ['accroche'] ['destination'] [$i] ['destination_nb_element'] = array (
        '#type' => 'textfield',
        '#title' => t ( 'Enter number of element that you want' ),
        '#required' => TRUE,
        '#default_value' => (isset ( $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_nb_element'] )) ? $form_state ['values'] ['accroche'] ['destination'] [$i] ['destination_nb_element'] :isset ( $conf ["destination"] [$i] ["nb_element"] ) ? $conf ["destination"] [$i] ["nb_element"] :  '' 
      );
    }
    
    $form ['accroche'] ['add_more'] = array (
      '#type' => 'button',
      '#value' => 'Add more',
      '#ajax' => array (
        'callback' => 'dpiblocks_ajax_callback_define_get_destination',
        'wrapper' => 'accroche-form',
        'method' => 'replace' 
      ) 
    );
  }
  
  return $form;
}

function dpiblocks_ajax_callback_define_type_form($form, $form_state) {
  return $form ['accroche'];
}

function dpiblocks_ajax_callback_define_target_from_form($form, $form_state) {
  // $i = $form_state['triggering_element']['#parents'][2];
  return $form ["accroche"];
}

function dpiblocks_ajax_callback_define_get_destination($form, $form_state) {
  return $form ['accroche'];
}

function dpiblocks_accroche_plugin_edit_form_validate($form, &$form_state) {
  if (isset ( $form_state ['values'] ["accroche"] ['type'] ) && $form_state ['values'] ["accroche"] ['type'] == "node") {
    if (! isset ( $form_state ['values'] ["accroche"] ['node'] ) || $form_state ['values'] ["accroche"] ['node'] == "")
      form_set_error ( "accroche][node", "Pleaser enter a value into the node field" );
  } elseif (isset ( $form_state ['values'] ["accroche"] ['type'] ) && $form_state ['values'] ["accroche"] ['type'] == "destination") {
    for($i = 0; $i < count ( $form_state ['values'] ['accroche'] ['destination'] ); $i ++) {
      if (! isset ( $form_state ['values'] ["accroche"] ['destination'] [$i] ['destination_section'] ) || $form_state ['values'] ["accroche"] ['destination'] [$i] ['destination_section'] == "")
        form_set_error ( "accroche] ['destination'] [$i][destination_section", "Select a section value" );
      if (! isset ( $form_state ['values'] ["accroche"] ['destination'] [$i] ['destination_target'] ) || $form_state ['values'] ["accroche"] ['destination'] [$i] ['destination_target'] == "")
        form_set_error ( "accroche] ['destination'] [$i][destination_target", "Please select a target" );
      if (! isset ( $form_state ['values'] ["accroche"] ['destination'] [$i] ['destination_nb_element'] ) || ! is_numeric ( $form_state ['values'] ["accroche"] ['destination'] [$i] ['destination_nb_element'] ))
        form_set_error ( "accroche] ['destination'] [$i][destination_nb_element", "Enter a numeric value" );
    }
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
    for($i = 0; $i < count ( $form_state ["values"] ["accroche"] ['destination'] ); $i ++) {
      $form_state ['conf'] ['destination'] [$i] = array (
        'section' => $form_state ["values"] ["accroche"] ['destination'] [$i] ["destination_section"],
        'target' => $form_state ["values"] ["accroche"] ['destination'] [$i] ["destination_target"],
        'nb_element' => $form_state ["values"] ["accroche"] ['destination'] [$i] ['destination_nb_element'] 
      );
    }
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