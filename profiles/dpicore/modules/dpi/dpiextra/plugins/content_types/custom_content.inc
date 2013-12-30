<?php 

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('custom content'),
  'no title override' => TRUE,
  'defaults' => array('admin_title' => '', 'title' => '', 'body' => '', 'format' => filter_default_format(), 'substitute' => TRUE),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
  'edit form' => array(
    'dpiextra_custom_content_content_type_view_form'=>'view custom content',
    'dpiextra_custom_content_content_type_edit_form'=>'edit custom content',
    'dpiextra_custom_content_type_revision_form'=>'view revision',
  ),
  'add form' => array(
    'dpiextra_custom_content_content_type_view_form'=>'view custom content',
    'dpiextra_custom_content_content_type_edit_form'=>'edit custom content',
    'dpiextra_custom_content_type_revision_form'=>'view revision',
  ),
);

/**
 * Implementation of hook_content_type_content_types()
 *
 * @return $custom_content
 * all custom content
*/
function dpiextra_custom_content_content_type_content_types(){
  $results = dpiextra_custom_content_get_all();

  foreach($results as $record) {
    //existant content type
    $custom_content[$record->machine_name]= array(
      'title' => t('custom content version : '.$record->title),
      'description' => t(''),
      'category' => t('Dpi extra'),
      'subtype'=> $record->machine_name,
      'add form' => array(
        'dpiextra_custom_content_content_type_view_form'=>'view custom content',
        'dpiextra_custom_content_content_type_edit_form'=>'edit custom content',
        'dpiextra_custom_content_type_revision_form'=>'view revision',
      ),
      'edit form' => array(
        'dpiextra_custom_content_content_type_view_form'=>'view custom content',
        'dpiextra_custom_content_content_type_edit_form'=>'edit custom content',
        'dpiextra_custom_content_type_revision_form'=>'view revision',
      ),
    );
  }

  form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
  //new custom content type
  $custom_content['custom']= array(
    'name'           => 'custom_version',
    'title'          => t('New custom content version'),
    'top level'      => TRUE,
    'category'       => t('Custom'),
    'description'    => t('Create a completely custom piece of custom content.'),
    'edit form' => array(),
    'add form' => 'dpiextra_custom_content_content_type_add_form',
    'edit form' => array(
      'dpiextra_custom_content_content_type_view_form'=>'view custom content',
      'dpiextra_custom_content_content_type_edit_form'=>'edit custom content',
      'dpiextra_custom_content_type_revision_form'=>'view revision',
    ),
    'all contexts'   => TRUE,
  );

  return  $custom_content;
}

/**
 * Implementation of content_type_render()
 *
 * @param $subtype
 * @param $conf
 * @param $panel_args
 * @param $context
 */
function dpiextra_custom_content_content_type_render($subtype, $conf, $panel_args, $context){
  $block = new stdClass();
  //get the custom content entity
  $entity = _dpiextra_custom_content_get_entity_by_subtype($subtype);
  //create the renderable block
  $block->module = 'dpiextra';
  $block->title = t($entity->title);
  $block->content = check_markup($entity->body, $entity->format);

  return $block;
}

/**
 * Implementation of edit_form() for a new custom content block
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiextra_custom_content_content_type_add_form($form,$form_state){
  $form = dpiextra_get_entity_form('custom_content_form', $form_state);
  unset($form['revision']);
  return $form;
}

/**
 * Implementation of edit_form_validate() for a new custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_add_form_validate($form, &$form_state){
  $form_state2=array();
  form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
  //build form
  $formFromCustomContent = drupal_build_form('custom_content_form',$form_state2,$entity);

  $form_state2['values'] = $form_state['values'];
  //unset the wrong token
  unset($formFromCustomContent['#token']);
  //validate the form
  drupal_validate_form('custom_content_form', $formFromCustomContent, $form_state2);
}

/**
 * Implementation of edit_form_submit() for a new custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_add_form_submit($form,&  $form_state){
  $form_state2=array();
  $form_state2['values']=$form_state['values'];
  //load form include
  form_load_include($form_state2, 'inc', 'dpiextra','includes/custom_content.admin');
  //submit
  drupal_form_submit('custom_content_form', $form_state2);
  $form_state['pane']->subtype = $form_state2['custom_content']->machine_name;
}

/**
 * Implementation edit_form() for edit a custom content block
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiextra_custom_content_content_type_edit_form($form,$form_state){
  form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
  //get the entity
  $entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
  $form = dpiextra_get_entity_form('custom_content_form', $form_state,$entity);

  //unset the buttons next and cancel
  unset($form['buttons']['next']);
  unset($form['buttons']['cancel']);

  $form['buttons']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#next' => 'step1',
    '#wizard type' => 'next',
  );

  $form['buttons']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#next' => 'step1',
    '#wizard type' => 'next',
  );

  return $form;
}

/**
 * Implementation edit_form_validate() for edit a custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_edit_form_validate($form, &$form_state){
  if($form_state['clicked_button']['#value'] == 'Save'){
    $form_state2=array();
    form_load_include($form_state2, 'inc', 'dpiextra','includes/remotehtml.admin');
    //build form
    $formFromCustomContent = drupal_build_form('custom_content_form',$form_state2,$entity);

    $form_state2['values'] = $form_state['values'];
    //unset the wrong token
    unset($formFromCustomContent['#token']);
    //validate the form
    drupal_validate_form('custom_content_form', $formFromCustomContent, $form_state2);
  }
}

/**
 * Implementation edit_form_submit() for edit a custom content block
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_edit_form_submit($form,$form_state){
  if($form_state['clicked_button']['#value'] == 'Save'){
    $form_state2=array();
    $form_state2['values']=$form_state['values'];
    //get entity
    $entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);
    //We Need the CID for the "drupal_write_record"
    $form_state2['values']['cid'] = $entity->cid;
    //load form include
    form_load_include($form_state2, 'inc', 'dpiextra','includes/custom_content.admin');
    //submit
    drupal_form_submit('custom_content_form', $form_state2,$entity);
  }
}

/**
 * Implementation of content_type_form()
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiextra_custom_content_content_type_view_form($form, $form_state){
  form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
  //get the entity
  $entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);

  $form['machine_name'] = array(
    '#markup' => '<h4>Machine name : '.$entity->machine_name.'</h4>'
  );

  $form['title'] = array(
    '#markup' => '<p>Title : '.$entity->title.'</p>'
  );

  $form['body'] = array(
    '#markup' => '<p>Content : '.$entity->body.'</p>'
  );

  //Buttons
  $form['modify'] = array(
    '#type' => 'submit',
    '#value' => t('Modify'),
    '#next' => 'step2',
    '#wizard type' => 'next',
  );

  unset($form['buttons']);
  $form['buttons']['return'] = array(
    '#type' => 'submit',
    '#value' => 'Finish',
    '#wizard type' => 'finish',
  );

  $form['buttons']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Revision'),
    '#next' => 'step3',
    '#wizard type' => 'next',
  );

  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#wizard type' => 'cancel',
  );

  return $form;
}

/**
 * Implementation of content_type_edit_form_submit()
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_content_type_view_form_submit($form,&$form_state){
}

/**
 * Implementation of edit_form() to display revision
 *
 * @param $form
 * @param $form_state
 *
 * @return $form
 */
function dpiextra_custom_content_type_revision_form($form, $form_state){
  form_load_include($form_state, 'inc', 'dpiextra','includes/custom_content.admin');
  $entity= _dpiextra_custom_content_get_entity_by_subtype($form_state['subtype']['subtype']);

  $form['machine_name']['#value'] = $entity->machine_name;
  //unset buttons return and cancel
  unset($form['buttons']['return']);
  unset($form['buttons']['cancel']);
  unset($form['buttons']['next']);
  //change the step of the previous button
  $form['buttons']['previous']['#next'] = 'step1';

  return $form;
}

/**
 * Implementation of edit_form_submit() to display revision
 *
 * @param $form
 * @param $form_state
 */
function dpiextra_custom_content_type_revision_form_submit($form, $form_state){
}

/**
 * this function get all custom content from db
 *
 * @return $results
 * the results from db
 */
function dpiextra_custom_content_get_all(){
  //get all remotehtml
  $results = db_query('SELECT * FROM {dpiextra_custom_content}');
  return $results;
}

/**
 * This function allow to get the custom content entity by machine_name
 *
 * @param $machine_name
 * The machine name
 *
 * @return $entity
 * return the entity
 */
function _dpiextra_custom_content_get_entity_by_subtype($machine_name){
  //create a new custom content controller
  $customContentController = new Custom_contentController('custom_content');
  //return the custom_content entity
  return $customContentController->load_by_machine_name($machine_name);
}
