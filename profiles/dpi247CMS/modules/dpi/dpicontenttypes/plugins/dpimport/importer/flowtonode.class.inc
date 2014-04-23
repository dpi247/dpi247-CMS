<?php

require_once 'packagehelper.class.inc';

abstract class FlowToNode extends PackageHelper {

  /**
   * Form to complete the settings of the importer
   *
   * @param Array $form_state
   * @param Array $values
   */
  public function getSettingsForm(&$form_state, $values) {
    $form = array();

    $form['importer_settings']['#tree'] = TRUE;

    $form['importer_settings']['maxitem'] = array (
      '#type'             => 'textfield',
      '#title'            => t('Max item to fetch for this feed'),
      '#description'      => t('Set to 0 for no limitation'),
      '#default_value'    => isset($values['maxitem']) ? $values['maxitem'] : 20,
    );

    // Add the destination field
    $items = array();
    if (isset($form_state['values']['importer_settings']['destination'])) {
      $items = $form_state['values']['importer_settings']['destination'];
    } elseif (isset($values['destination'])) {
      $items = $values['destination'];
    }
    if (isset($items['add_more'])) {
      unset($items['add_more']);
    }
    $langcode = 'und';
    $field = array(
      'field_name' => 'field_destinations',
      'type' => 'destination',
      'cardinality' => 1,
      'columns' => array('tid' => '', 'target' => '', 'layout' => '', 'rank' => '', 'virtual_rank' => ''),
    );
    $instance = array(
      'label' => '',
      'description' => '',
      'widget' => array('type' => 'destination_default_widget', 'module' => 'dpidestinations'),
      'entity_type' => 'node',
      'bundle' => 'package',
      'required' => TRUE,
    );
    $form_copy = $form;
    $form_copy['#parents'] = array();
    $form_copy['#entity'] = new stdClass();
    $form_state_copy = $form_state;
    $form_state_copy['programmed'] = NULL;
    $form_state_copy['destinationsfield'] = array();
    foreach ($items as $delta => $item) {
      $form_state_copy['destinationsfield'][$item['element_key']] = $item;
    }
    $form['importer_settings']['destination'] = field_multiple_value_form($field, $instance, $langcode, $items, $form_copy, $form_state_copy);

    return $form;
  }
}
