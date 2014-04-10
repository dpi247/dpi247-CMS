<?php 

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Remote HTML'),
  'top level' => TRUE,
  'no title override' => TRUE,
  'defaults' => array('admin_title' => '', 'title' => '', 'body' => '', 'format' => filter_default_format(), 'substitute' => TRUE),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'edit form' => array(
    'dpiblocks_remotehtml_content_type_view_form'=>'view remotehtml',
    'dpiblocks_remotehtml_content_type_edit_form'=>'edit remotehtml',
  ),
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Implementation of hook_content_type_content_types()
 *
 * @return $remotehtml
 * all remotehtml
*/
function dpiblocks_remotehtml_content_type_content_types(){
  $results = dpiblocks_remotehtml_get_all();

  foreach($results as $record) {
    $remotehtml[$record->machine_name]= array(
      'title' => t('Remote HTML : '.$record->title),
      'machine_name' => $record->machine_name,
      'description' => t(''),
      'add form'      => array(
        'dpiblocks_remotehtml_content_type_view_form'=>'view remotehtml',
        'dpiblocks_remotehtml_content_type_edit_form'=>'edit remotehtml',
      ),
      'category' => t('DPI Extras'),
    );
  }
  form_load_include($form_state, 'inc', 'dpiblocks','includes/remotehtml.admin');

  $remotehtml['new_remotehtml']= array(
    'name'           => 'remotehtml',
    'title'          => t('New remote HTML'),
    'top level'      => TRUE,
    'category'       => t('Custom'),
    'description'    => t('Create a completely custom piece of HTML content.'),
    'add form'       => 'dpiblocks_remotehtml_content_type_add_form',
    'all contexts'   => TRUE,
  );

  return  $remotehtml;
}

/**
 * Implementation of add_form()
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiblocks_remotehtml_content_type_add_form($form, &$form_state){
  form_load_include($form_state, 'inc', 'dpiblocks','includes/remotehtml.admin');
  //get the entity form and unset the save button
  $form = dpiblocks_get_entity_form('remotehtml_form', $form_state);
  return $form;
}

/**
 * Implementation of add_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function dpiblocks_remotehtml_content_type_add_form_validate($form, &$form_state){
  if($form_state['clicked_button']['#wizard type'] != 'cancel'){
    $form_state2=array();
    form_load_include($form_state2, 'inc', 'dpiblocks','includes/remotehtml.admin');

    $formFromRemotehtml = drupal_build_form('remotehtml_form',$form_state2);
    $form_state2['values'] = $form_state['values'];
    //unset the wrong token
    unset($formFromRemotehtml['#token']);
    //validate the form
    drupal_validate_form('remotehtml_form', $formFromRemotehtml, $form_state2);
  }
}

/**
 * Implementation of add_form_submit()
 *
 * @param $form
 * @param $form_state
 */
function dpiblocks_remotehtml_content_type_add_form_submit($form, &$form_state){
  $form_state2=array();
  //create a new form_state with just the values
  $form_state2['values'] = $form_state['values'];

  //load form include
  form_load_include($form_state2, 'inc', 'dpiblocks','includes/remotehtml.admin');

  //submit
  drupal_form_submit('remotehtml_form', $form_state2);
  $entity = $form_state2['remotehtml'];
  //change the subtype "custom" by his machine name
  $form_state['pane']->subtype = $entity->machine_name;
}

/**
 * Implementation of edit_form()
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiblocks_remotehtml_content_type_edit_form($form, &$form_state){
  form_load_include($form_state, 'inc', 'dpiblocks','includes/remotehtml.admin');
  //get the entity with the machine_name
  $entity = _dpiblocks_remotehtml_get_entity_by_subtype($form_state['subtype']['machine_name']);
  //get the entity form and unset the save button
  $form = dpiblocks_get_entity_form('remotehtml_form', $form_state, $entity);

  //unset finish and cancel buttons
  unset($form['buttons']['return']);
  unset($form['buttons']['cancel']);

  //add previous button Back
  $form['buttons']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#next' => 'step1',
    '#wizard type' => 'next',
  );

  //add next button Save
  $form['buttons']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#next' => 'step1',
    '#wizard type' => 'next',
  );

  return $form;
}

/**
 * Implementation of edit_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function dpiblocks_remotehtml_content_type_edit_form_validate($form, &$form_state){
  if($form_state['clicked_button']['#value'] == t('Save')){
    $form_state2=array();
    form_load_include($form_state2, 'inc', 'dpiblocks','includes/remotehtml.admin');
     
    $formFromRemotehtml = drupal_build_form('remotehtml_form',$form_state2,$entity);
     
    $form_state2['values'] = $form_state['values'];
    //unset the wrong token
    unset($formFromRemotehtml['#token']);
    //validate the form
    drupal_validate_form('remotehtml_form', $formFromRemotehtml, $form_state2);
  }
}

/**
 * Implementation of edit_form_submit()
 *
 * @param $form
 * @param $form_state
 */
function dpiblocks_remotehtml_content_type_edit_form_submit($form, $form_state){
  if($form_state['clicked_button']['#value'] == t('Save')){
    $form_state2=array();
    //create a new form_state with just the values
    $form_state2['values']=$form_state['values'];
    $entity = _dpiblocks_remotehtml_get_entity_by_subtype($form_state['subtype']['machine_name']);
    //load form include
    form_load_include($form_state2, 'inc', 'dpiblocks','includes/remotehtml.admin');
    //submit
    drupal_form_submit('remotehtml_form', $form_state2,$entity);
  }
}

/**
 * Implementation of view_form()
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiblocks_remotehtml_content_type_view_form($form,&$form_state){
  //get the entity
  $entity= _dpiblocks_remotehtml_get_entity_by_subtype($form_state['subtype']['machine_name']);

  $form['machine_name'] = array(
    '#markup' => '<h4>'.t('Machine name').' : '.$entity->machine_name.'</h4>'
  );

  $form['title'] = array(
    '#markup' => '<p>'.t('Title').' : '.$entity->title.'</p>'
  );

  $form['html_adresse'] = array(
    '#markup' => '<p>'.t('URL adress').' : '.$entity->html_adresse.'</p>'
  );

  $form['refresh_time'] = array(
    '#markup' => '<p>'.t('Refresh time (in minutes)').' : '.$entity->refresh_time.'</p>'
  );

  //add the modify button
  $form['modify'] = array(
    '#type' => 'submit',
    '#value' => t('Modify'),
    '#next' => 'step2',
    '#wizard type' => 'next',
  );

  unset($form['buttons']);

  //add the finish button
  $form['buttons']['return'] = array(
    '#type' => 'submit',
    '#value' => t('Finish'),
    '#wizard type' => 'finish',
  );

  //add the cancel button
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#wizard type' => 'cancel',
  );

  return $form;
}

/**
 * Implementation of view_form_submit()
 *
 * @param $form
 * @param $form_state
 */
function dpiblocks_remotehtml_content_type_view_form_submit($form,&$form_state){
}

/**
 * Implementation of content_type_render()
 *
 * @param $subtype
 * @param $conf
 * @param $panel_args
 * @param $context
 *
 * @return stdClass
 * return the block
 */
function dpiblocks_remotehtml_content_type_render($subtype, $conf, $panel_args, $context){
  $entity = _dpiblocks_remotehtml_get_entity_by_subtype($subtype);
  //create new block
  $block = new stdClass();
  $block->module = 'dpiblocks';
  $block->title = $entity->title;
  //get html of the url
  $block->content = file_get_contents($entity->html_adresse);

  return $block;
}

/**
 * This function allow to get the remotehtml entity by machine_name
 *
 * @param $machine_name
 * The machine name
 *
 * @return $entity
 * return the entity
 */
function _dpiblocks_remotehtml_get_entity_by_subtype($machine_name){
  //create a new remotehtml controller
  $remotehtmlController = new RemotehtmlController('remotehtml');
  //return the remotehtml entity
  return $remotehtmlController->load_by_machine_name($machine_name);
}
