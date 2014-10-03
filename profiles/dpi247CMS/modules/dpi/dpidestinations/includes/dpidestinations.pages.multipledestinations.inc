<?php
/**
 * @author lba
 */

/**
 * This function create for to add and preview the table of destination
 *
 * @return theme
 */
function dpidestinations_page_multiple_destinations() {
  $render = l ( t ( 'Add multiple link' ), 'admin/dpi247/dpidestinations/destinationmultiple/add' );
  // $render .= theme_table(array());
  return $render;
}

/**
 * This function manage add element
 *
 * @return render
 */
function dpidestinations_page_multiple_destinations_add() {
  return drupal_get_form ( "dpidestinations_form_multiple_destinations_form" );
}

/**
 * This function create form
 *
 * @param array $form          
 * @param array $form_state          
 * @return array()
 */
function dpidestinations_form_multiple_destinations_form($form, &$form_state) {
  $form = array ();
  
  // Configurations to retrieve elements in the form, necessary for the "add new" and "remove" operations
  $form['wallyflowpublish']['#tree'] = TRUE;
  $form['wallyflowpublish']['field_destinations']['#type'] = 'fieldset';
  foreach (wallytoolbox_element_children_numeric($form['wallyflowpublish']['field_destinations']) as $dest_id) {
    $form['wallyflowpublish']['field_destinations'][$dest_id]['#type_name'] = 'wallyflowpublish';
  }
  $form['wallyflowpublish']['field_destinations']['field_destinations_add_more']['#submit'][0] = 'cckdestinations_add_more_submit_proxy';
  $form['wallyflowpublish']['field_destinations']['field_destinations_add_more']['#ahah']['path'] = str_replace('//', '/', 'cckdestinations/js_add_more/wallyflowpublish/field_destinations/wallyflowpublish/field_destinations');
  $form['wallyflowpublish']['field_destinations']['#parents'] = array('wallyflowpublish', 'field_destinations');
  $form['wallyflowpublish']['#field_info']['field_destinations']['display_settings']['parent'] = 'wallyflowpublish';
  $form['wallyflowpublish']['field_destinations']['#dependency'] = array('edit-wallyflowpublish-type' => array('destinations'));
  $form['wallyflowpublish']['field_destinations']['#process'] = array('ctools_dependent_process');
  $form['wallyflowpublish']['field_destinations']['#input'] = TRUE; //Force process of the fieldset to enable ctools dependent
  $form['wallyflowpublish']['field_destinations']['#prefix'] .= '<div id="edit-wallyflowpublish-field-destinations-wrapper">';
  $form['wallyflowpublish']['field_destinations']['#suffix'] = '</div>'.$form['wallyflowpublish']['field_destinations']['#suffix'];
  
  
  $form ['submit'] = array (
    '#type' => 'submit',
    '#value' => t ( 'Add' ) 
  );
  
  return $form;
}

function dpidestinations_form_multiple_destinations_form_validate($form, &$form_state) {

}

function dpidestinations_form_multiple_destinations_form_submit($form, $form_state) {

}

/**
 * Get numeric keys of a structured array tree element.
 */
function wallytoolbox_element_children_numeric($element) {
  return array_filter(array_keys((array) $element), 'wallytoolbox_element_child_numeric');
}

/**
 * Check if the key is a numeric child.
 */
function wallytoolbox_element_child_numeric($key) {
  return is_numeric($key);
}