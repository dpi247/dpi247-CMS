<?php

/**
 * Implements hook_field_formatter_info().
 *
 * We need to tell Drupal that we have one or more formatters
 * for this field. The default one will display all the information of the quadruplet
 * that define a destination
 * each other will display only one information of the quadruplet
 *
 * @see fdpidestinations_field_formatter_view()
 */
function dpidestinations_field_formatter_info() {
  return array(
    // This formatter just displays the hex value in the color indicated.
    'destination_default_formatter' => array(
      'label' => t('Simple text-based formatter of the destination'),
      'field types' => array('destination'),
    ),
		'dpi_main_destination_display' => array(
			'label' => t('DPI Main Destination term'),
			'field types' => array('destination'),
			'settings'  => array(
				'wrapper_html_tag' => '- None -',
				'wrapper_html_tag_bootstrap_class' => '- None -',
				'wrapper_html_tag_custom_class' => '',
				'element_html_tag' => '- None -',
				'element_html_tag_bootstrap_class' => '- None -',
				'element_html_tag_custom_class' => '',
				'element_html_link' => 'no',
				'element_html_link_custom_class' => '',
			),				
		),
		'dpi_taxonomy_content' => array(
			'label' => t('DPI Taxonomy content'),
			'field types' => array('destination', 'taxonomy_term_reference'),
			'settings'  => array(
				'wrapper_html_tag' => '- None -',
				'wrapper_html_tag_bootstrap_class' => '- None -',
				'wrapper_html_tag_custom_class' => '',
				'element_html_tag' => '- None -',
				'element_html_tag_bootstrap_class' => '- None -',
				'element_html_tag_custom_class' => '',
				'element_html_link' => 'no',
				'element_html_link_bootstrap_class' => '- None -',
				'element_html_link_custom_class' => '',
				'element_prefixe' => '',
				'element_suffixe' => '',
				'separator_option' => '',
        'nb_elements' => 0
			),				
		),
  );
}

/**
 * Implementation of hook_field_formatter_settings_form
 * Specify the form elements for a formatter's settings.
 * This hook is only invoked if hook_field_formatter_settings_summary() returns a non-empty value.
 */
function dpidestinations_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
	$display = $instance['display'][$view_mode];
	$element = array();
	switch ($display['type']) {
		case 'dpi_main_destination_display':
			module_load_include('inc','dpidestinations','includes/formatters/dpi_main_destination_display');
			dpidestinations_field_formatter_settings_form_dpi_main_destination_display($field, $instance, $view_mode, $form, $form_state, $element);
			break;
		case 'dpi_taxonomy_content':
			module_load_include('inc','dpidestinations','includes/formatters/dpi_taxonomy_content');
			dpidestinations_field_formatter_settings_form_dpi_taxonomy_content($field, $instance, $view_mode, $form, $form_state, $element);
			break;
	}
	return $element;
}

/**
 * Implements hook_field_formatter_view().
 *
 * Two formatters are implemented.
 * - field_example_simple_text just outputs markup indicating the color that
 *   was entered and uses an inline style to set the text color to that value.
 * - field_example_color_background does the same but also changes the
 *   background color of div.region-content.
 *
 * @see field_example_field_formatter_info()
 */
function dpidestinations_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  
  switch ($display['type']) {
    // This formatter simply outputs the field as text and with a color.
    case 'destination_default_formatter':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'span',
          '#value' => $item['tid'].'-'.$item['target'].'-'.$item['layout'].'-'.$item['rank'],
        );
      }
      break;

    case 'dpi_main_destination_display':
		module_load_include('inc','dpidestinations','includes/formatters/dpi_main_destination_display');
		dpidestinations_field_formatter_view_dpi_main_destination_display($entity_type, $entity, $field, $instance, $langcode, $items, $display, $element);
		break;
	case 'dpi_taxonomy_content':
		module_load_include('inc','dpidestinations','includes/formatters/dpi_taxonomy_content');
		dpidestinations_field_formatter_view_dpi_taxonomy_content($entity_type, $entity, $field, $instance, $langcode, $items, $display, $element);
		break;
	}
	return $element;
}

/**
 * Implementation of hook_field_formatter_settings_settings_summary
 * Return a short summary for the current formatter settings of an instance.
 * If an empty result is returned, the formatter is assumed to have no configurable settings, and no UI will be provided to display a settings form.
 */
function dpidestinations_field_formatter_settings_summary($field, $instance, $view_mode) {
	$display = $instance['display'][$view_mode];
	$summary = '';
	switch ($display['type']) {
		case 'dpi_main_destination_display':
			module_load_include('inc','dpidestinations','includes/formatters/dpi_main_destination_display');
			dpidestinations_field_formatter_settings_summary_dpi_main_destination_display($field, $instance, $view_mode, $summary);
			break;
		case 'dpi_taxonomy_content':
			module_load_include('inc','dpidestinations','includes/formatters/dpi_taxonomy_content');
			dpidestinations_field_formatter_settings_summary_dpi_taxonomy_content($field, $instance, $view_mode, $summary);
			break;	
	}
	return $summary;
}