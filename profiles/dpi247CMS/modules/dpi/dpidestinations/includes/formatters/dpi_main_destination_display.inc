<?php
function dpidestinations_field_formatter_settings_form_dpi_main_destination_display($field, $instance, $view_mode, $form, &$form_state, &$element){
	$settings = $instance['display'][$view_mode]['settings'];
	/*
	 $element['wrapper'] = array(
	 		'#type' => 'fieldset',
	 		'#title' => t('Wrapper\'s HTML tag'),
	 		'#collapsible' => TRUE,
	 		'#collapsed' => FALSE
	 );
	*/
	
	$element['wrapper_html_tag'] = array(
			'#type' => 'select',
			'#title' => t('Wrapper\'s Tag'),
			'#description' => t('The element(s)\'s wrapper HTML tag'),
			'#default_value' => $settings['wrapper_html_tag'],
			'#options' => array(
					'- None -' => '- None -',
					'span' => 'span',
					'div' => 'div',
					'p' => 'p',
					'strong' => 'strong',
			),
	);
	
	$element['wrapper_html_tag_bootstrap_class'] = array(
			'#type' => 'select',
			'#title' => t('Add a boostrap class to the wrapper\'s HTML tag'),
			'#description' => t('The class assigned to the element\'s wrapper HTML tag'),
			'#default_value' => $settings['wrapper_html_tag_bootstrap_class'],
			'#options' => array(
					'- None -' => '- None -',
					'optional-break' => 'optional-break',
			),
			'#states' => array(
					'invisible' => array(
							':input[name="wrapper_html_tag"]' => array('value' => '- None -'),
					),
			),
	);
	
	$element['wrapper_html_tag_custom_class'] = array(
			'#type' => 'textfield',
			'#title' => t('Add custom class(es) to the wrapper\'s HTML tag'),
			'#description' => t('The class assigned to the element\'s wrapper HTML tag'),
			'#default_value' => $settings['wrapper_html_tag_custom_class'],
			'#states' => array(
					'invisible' => array(
							':input[name="wrapper_html_tag"]' => array('value' => '- None -'),
					),
			),
	);
	
	/*
	 $element['element'] = array(
	 		'#type' => 'fieldset',
	 		'#title' => t('Element\'s HTML tag'),
	 		'#collapsible' => TRUE,
	 		'#collapsed' => FALSE
	 );
	*/
	
	$element['element_html_tag'] = array(
			'#type' => 'select',
			'#title' => t('Element(s)\'s Tag'),
			'#description' => t('The element(s)\'s HTML tag'),
			'#default_value' => $settings['element_html_tag'],
			'#options' => array(
					'- None -' => '- None -',
					'h1' => 'h1',
					'h2' => 'h2',
					'h3' => 'h3',
					'h4' => 'h4',
					'h5' => 'h5',
					'blockquote' => 'blockquote',
					'span' => 'span',
					'div' => 'div',
					'p' => 'p',
					'small' => 'small',
					'strong' => 'strong',
			),
	);
	
	$element['element_html_tag_bootstrap_class'] = array(
			'#type' => 'select',
			'#title' => t('Bootstrap class(es)'),
			'#description' => t('The class assigned to the element\'s wrapper HTML tag'),
			'#default_value' => $settings['element_html_tag_bootstrap_class'],
			'#options' => array(
					'- None -' => '- None -',
					'gr-meta' => 'gr-meta',
					'badge' => 'badge',
			),
			'#states' => array(
					'invisible' => array(
							':input[name="element_html_tag"]' => array('value' => '- None -'),
					),
			),
	);
	
	$element['element_html_tag_custom_class'] = array(
			'#type' => 'textfield',
			'#title' => t('Custom class(es)'),
			'#description' => t('The custom class assigned to the element\'s HTML tag'),
			'#default_value' => $settings['element_html_tag_custom_class'],
			'#states' => array(
					'invisible' => array(
							':input[name="element_html_tag"]' => array('value' => '- None -'),
					),
			),
	);
	
	$element['element_html_link']=array(
			'#type' => 'select',
			'#title' => t('Output as a link'),
			'#description' => t('Output the current element as a link to the destination page?'),
			'#default_value' => $settings['element_html_link'],
			'#options' => array(
					'yes' => 'Yes',
					'no' => 'No',
			),
	);	
	
}


function dpidestinations_field_formatter_view_dpi_main_destination_display($entity_type, $entity, $field, $instance, $langcode, $items, $display, &$element){
	$settings = $display['settings'];
	$render=array();
	
	$term_id = $items[0]['tid'];
	$term = taxonomy_term_load($term_id);;

	$render = array(
		'#type' => 'markup',
		'#markup' => $term->name,
	);
		
	$render = _dpi_main_destination_display_element_link_render($settings, $term, $render);
	$render = _dpi_main_destination_display_element_tag_render($settings,$render);

	$render = _dpi_main_destination_display_wrapper_tag_render($settings,$render);
	$element[0] = $render;	
	
}

function dpidestinations_field_formatter_settings_summary_dpi_main_destination_display($field, $instance, $view_mode, &$summary){
	$settings = $instance['display'][$view_mode]['settings'];
	$summary = "";
	$summary .= _dpi_main_destination_display_element_tag_field_formatter_settings_summary($settings);
	$summary .= _dpi_main_destination_display_wrapper_tag_field_formatter_settings_summary($settings);
	$summary .= _dpi_main_destination_display_theme_ui_element_link_field_formatter_settings_summary($settings);	

}

function _dpi_main_destination_display_element_link_render($settings, $term, $value){
	if($settings['element_html_link']!="no"){

		$render = array();
		$term_link = drupal_get_path_alias('section/'.$term->tid);
		$render['#value'] = $value;
		$render['#tag'] = 'a';
		$render['#attributes']['href'] = '/'.$term_link;
		$render['#theme_wrappers'] = array(0 => 'dpi_render_wrapper');

		return $render;

	}else{
		return $value;
	}
}

function _dpi_main_destination_display_wrapper_tag_render($settings,$value){
	if (isset($settings['wrapper_html_tag']) && $settings['wrapper_html_tag']!='- None -' && $settings['wrapper_html_tag']!='') {
		$render = array();
		$render['#value'] = $value;
		$render['#tag'] = $settings['wrapper_html_tag'];
		if(isset($settings['wrapper_html_tag_bootstrap_class']) && $settings['wrapper_html_tag_bootstrap_class']!='- None -' && $settings['wrapper_html_tag_bootstrap_class']!=''){
			$render['#attributes']['class'] = array($settings['wrapper_html_tag_bootstrap_class']);
		}
		if(isset($settings['wrapper_html_tag_custom_class']) && $settings['wrapper_html_tag_custom_class']!='- None -' && $settings['wrapper_html_tag_custom_class']!='- '){
			$render['#attributes']['class'][] = $settings['wrapper_html_tag_custom_class'];
		}
		$render['#theme_wrappers'] = array(0 => 'dpi_render_wrapper');

		return $render;

	}else{
		return $value;
	}
}

function _dpi_main_destination_display_element_tag_render($settings,$value){

	if (isset($settings['element_html_tag']) && $settings['element_html_tag']!='- None -' && $settings['element_html_tag']!='') {

		$render = array();
		$render['#value'] = $value;
		$render['#tag'] = $settings['element_html_tag'];
		if(isset($settings['element_html_tag_bootstrap_class']) && $settings['element_html_tag_bootstrap_class']!='- None -' && $settings['element_html_tag_bootstrap_class']!=''){
			$render['#attributes']['class'][] = $settings['element_html_tag_bootstrap_class'];
		}
		if(isset($settings['element_html_tag_custom_class']) && $settings['element_html_tag_custom_class']!='- None -' && $settings['element_html_tag_custom_class']!='- '){
			$render['#attributes']['class'][] = $settings['element_html_tag_custom_class'];
		}
		$render['#theme_wrappers'] = array(0 => 'dpi_render_wrapper');

		return $render;

	}else{
		return $value;
	}
}

function _dpi_main_destination_display_element_tag_field_formatter_settings_summary($settings){
	if (isset($settings['element_html_tag']) && $settings['element_html_tag']!='- None -' && $settings['element_html_tag']!='') {
		$summary = t('Use for each element(s) a "@tag" tag with Bootstrap class(es) "@b_class" and custom class(es) "@c_class"<br/>', array('@tag' => $settings['element_html_tag'],'@b_class' => $settings['element_html_tag_bootstrap_class'], '@c_class' => $settings['element_html_tag_custom_class']));
	}else{
		$summary = t('No HTML tag is specified for the element(s)<br/>');
	}
	return $summary;
}

function _dpi_main_destination_display_wrapper_tag_field_formatter_settings_summary($settings){
	if (isset($settings['wrapper_html_tag']) && $settings['wrapper_html_tag']!='- None -' && $settings['wrapper_html_tag']!=''){
		$summary = t('Use for wrapper a "@tag" tag with Bootstrap class(es) "@b_class" and custom class(es) "@c_class"<br/>', array('@tag' => $settings['wrapper_html_tag'],'@b_class' => $settings['wrapper_html_tag_bootstrap_class'],'@c_class' => $settings['wrapper_html_tag_custom_class']));
	}else{
		$summary = t('No HTML tag is specified for the element(s)\'s wrapper<br/>');
	}
	return $summary;
}

function _dpi_main_destination_display_theme_ui_element_link_field_formatter_settings_summary($settings){
	if ($settings['element_html_link']!="no") {
		$summary = t('The element is outputed as a link to the section page<br/>');
	}else{
		$summary = t('The element is not outputed as a link<br/>');
	}
	return $summary;
}