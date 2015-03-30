<?php 
/**
 * This function take table of block informations
 * @return output
 */
function dfpmodule_positions_blocks() {
  $data = array ();
  $query = db_select('zz_pub_UMI' , 'p');
  $content = $query->condition('builder' , 'dfp', '=')->fields('p', array('position'))->execute();
  $i=0;
  foreach ($content as $row){
    $data [] = array (
      $i,
      $row->position
    );
    $i++;
  }
  $rows = array(
      'header' => array(
        'N°', 'Position',
      ),
      'rows' => $data,
    );
  
  
  $output = theme('table',$rows);
  $val_form = drupal_get_form('dfpmodule_positions_refresh_form');
  $output .= drupal_render($val_form);
  return $output;
}

/**
 * 
 * @return multitype:string The
 */
function dfpmodule_positions_refresh_form(){
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Refresh table' )
  );
  return $form;
}

/**
 * This function refresh information to the zz_pub_UMI table
 */
function dfpmodule_positions_refresh_form_submit(){
  $url = variable_get('dfpmodule_url_service_position', '');
  if($url!=''){
    $transaction = db_transaction();
    try {
      $query = db_delete('zz_pub_UMI')->condition('builder', 'dfp')->execute();
      $content = drupal_http_request($url, array(), 'GET');
      if($content->code == '200'){
        $blocks = json_decode($content->data);
        for($i=0;$i<count($blocks);$i++){
          $query = db_insert('zz_pub_UMI')->fields(array('builder' => 'dfp', 'position' => $blocks[$i]))->execute();
        }
        drupal_set_message("Positions refreshed !", 'status');
      }else{
        drupal_set_message("Web services is not avalaible !", 'error');
      }
    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('sql rollback', $e);
    }
  }
}

/**
 * This function create form to set URL
 * @return get_form
 */
function dfpmodule_settings_url() {
  $form = drupal_get_form ( 'dfp_settings_url_form' );
  $output = drupal_render($form);
  $output .= dfpmodule_positions_blocks();
  return $output;
}

/**
 * This function define all informations of the form
 * @return multitype:string The multitype:string The
 */
function dfp_settings_url_form() {
  $form ['url_ws_position'] = array (
    '#title' => t ( 'URL PATH for position' ),
    '#type' => 'textfield',
    '#description' => t ( 'Please enter an URL to take information about blocks position' ),
    '#default_value' => variable_get ( 'dfpmodule_url_service_position', NULL )
  );
  $form ['url_ws_details'] = array (
    '#title' => t ( 'URL PATH for details' ),
    '#type' => 'textfield',
    '#description' => t ( 'Please enter an URL to take information about block' ),
    '#default_value' => variable_get ( 'pub_dfp_webserv', NULL )
  );
  $form ['url_ws_header_script'] = array (
    '#title' => t ( 'URL PATH for header script' ),
    '#type' => 'textfield',
    '#description' => t ( 'Please enter an URL to take information about block' ),
    '#default_value' => variable_get ( 'dfpmodule_url_ws_header_script', NULL )
  );
  $form ['pub_dfp_website'] = array (
    '#title' => t ( '"site" param in the url' ),
    '#type' => 'textfield',
    '#description' => t ( 'Please enter a site name use in argument in the url for a block detail call' ),
    '#default_value' => variable_get ( 'pub_dfp_website', NULL ),
    '#required'=>TRUE,
  );  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' )
  );
  return $form;
}

/**
 * This function validate URL form
 * @param unknown $form
 * @param unknown $form_state
 */
function dfp_settings_url_form_validate($form, &$form_state) {
  if (! isset ( $form_state ['values'] ['url_ws_position'] ) || $form_state ['values'] ['url_ws_position'] == "") {
    form_set_error ( 'url', t ( 'Thx to complete field !' ) );
  }
}

/**
 * This function set url
 * @param unknown $form
 * @param unknown $form_state
 */
function dfp_settings_url_form_submit($form, &$form_state) {
  variable_set ( 'dfpmodule_url_service_position', $form_state ['values'] ['url_ws_position'] );
  variable_set ( 'pub_dfp_webserv', $form_state ['values'] ['url_ws_details'] );
  variable_set ( 'dfpmodule_url_ws_header_script', $form_state ['values'] ['url_ws_header_script'] );
  variable_set ( 'pub_dfp_website', $form_state ['values'] ['pub_dfp_website'] );
  drupal_set_message('Settings registered !' , 'status');
}