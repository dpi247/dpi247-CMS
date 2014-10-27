
<?php
/**
 * @author GVD - LBA
 */

/**
 * This function is the callback from menu to delete context
 * 
 * @param string $id
 * @return render(form)
 */
function dpiunivers_delete($id = NULL) {
  return drupal_get_form ( 'dpiunivers_delete_form', $id );
}

/**
 * This function is use to create form delete
 * We add a simple verification
 *
 * @param array $form          
 * @param array $form_state          
 * @param integer $id          
 * @return multitype:string The
 */
function dpiunivers_delete_form($form, &$form_state, $id) {
  if (isset ( $id )) {
    $form ['store'] = array (
      '#type' => 'value',
      '#value' => $id 
    );
  }
  
  $form ['add'] = array (
    '#title' => t ( 'Are you sure that you want to delete univers ?' ),
    '#type' => 'fieldset' 
  );
  
  $form ['add'] ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Submit' ) 
  );
  $form ['add'] ['Cancel'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Cancel' ) 
  );
  
  return $form;
}

/**
 * This function is use to create form submition
 *
 * @param unknown $form          
 * @param unknown $form_state          
 */
function dpiunivers_delete_form_submit($form, $form_state) {
  if ($form_state ['values'] ['op'] == 'Submit') {
    if (dpiunivers_api_delete_univers_by_id ( $form_state ['values'] ['store'] )) {
      drupal_set_message ( 'Data deleted successfully' );
    } else {
      drupal_set_message ( 'Error while deleting data', 'error' );
    }
  }
  drupal_goto ( 'admin/config/dpiunivers/settings' );
}