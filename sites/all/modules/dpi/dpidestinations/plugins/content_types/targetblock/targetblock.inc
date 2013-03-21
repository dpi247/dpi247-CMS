<?php
/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('TargetBlock'),
  'description' => t('Crafted links to create content (nodes) for a given group.'),
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
  'category' => t('Target'),
  'defaults' => array(
   ),
);


/**
 * Render callback.
 */
function dpidestinations_targetblock_content_type_render($subtype, $conf, $args, $context) {
  if (empty($context->data)) {
    return FALSE;
  }
}


/**
 * Edit form.
 */
function dpidestinations_targetblock_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
}


/**
 * Edit form submit callback.
 */
function dpidestinations_targetblock_content_type_edit_form_submit($form, &$form_state) {
  //$form_state['conf']['field_name'] = $form_state['values']['field_name'];
  //$form_state['conf']['types'] = array_filter($form_state['values']['types']);
}
