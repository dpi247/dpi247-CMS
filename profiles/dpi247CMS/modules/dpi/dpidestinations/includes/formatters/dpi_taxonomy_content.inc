<?php
function dpidestinations_field_formatter_settings_form_dpi_taxonomy_content($field, $instance, $view_mode, $form, &$form_state, &$element){
	$settings = $instance['display'][$view_mode]['settings'];

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
    				'select[name="fields['.$field['field_name'].'][settings_edit_form][settings][wrapper_html_tag]"]' => array('value' => '- None -'),
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
    				'select[name="fields['.$field['field_name'].'][settings_edit_form][settings][wrapper_html_tag]"]' => array('value' => '- None -'),
  				),
			),
	);
	
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
    				'select[name="fields['.$field['field_name'].'][settings_edit_form][settings][element_html_tag]"]' => array('value' => '- None -'),
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
    				'select[name="fields['.$field['field_name'].'][settings_edit_form][settings][element_html_tag]"]' => array('value' => '- None -'),
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

	$element['element_html_link_bootstrap_class'] = array(
			'#type' => 'select',
			'#title' => t('Add a boostrap class to the link HTML element'),
			'#description' => t('The class assigned to the link HTML element'),
			'#default_value' => $settings['element_html_link_bootstrap_class'],
			'#options' => array(
					'- None -' => '- None -',
					'optional-break' => 'optional-break',
			),
			'#states' => array(
  				'visible' => array(
    				'select[name="fields['.$field['field_name'].'][settings_edit_form][settings][element_html_link]"]' => array('value' => 'yes'),
  				),
			),
	);
	
	$element['element_html_link_custom_class'] = array(
			'#type' => 'textfield',
			'#title' => t('Add custom class(es) to the link HTML element'),
			'#description' => t('The class assigned to the link HTML element'),
			'#default_value' => $settings['element_html_link_custom_class'],
			'#states' => array(
  				'visible' => array(
    				'select[name="fields['.$field['field_name'].'][settings_edit_form][settings][element_html_link]"]' => array('value' => 'yes'),
  				),
			),
	);

	$element['element_prefixe'] = array(
			'#type' => 'textfield',
			'#title' => t('Add custom prefixe to the element'),
			'#description' => t('The text displayed before the destinations terms'),
			'#default_value' => $settings['element_prefixe'],
	);

	$element['element_suffixe'] = array(
			'#type' => 'textfield',
			'#title' => t('Add custom suffixe to the element'),
			'#description' => t('The text displayed after the destinations terms'),
			'#default_value' => $settings['element_suffixe'],
	);

	$element['separator_option'] = array(
			'#type' => 'textfield',
			'#title' => t('Separator'),
			'#description' => t('The separator to use, including leading and trailing spaces'),
			'#default_value' => $settings['separator_option'],
	);
	
}


function dpidestinations_field_formatter_view_dpi_taxonomy_content($entity_type, $entity, $field, $instance, $langcode, $items, $display, &$element){
	$settings = $display['settings'];
	if (count($items) > 0 && isset($settings['element_prefixe']) && !empty($settings['element_prefixe']) ) {
		// on ajoute  le prefixe
		$element[] = _dpi_taxonomy_content_simple_element_text_render($settings,'element_prefixe');
	}

	foreach ($items as $item) {
		$render=array();
	
		$term_id = $item['tid'];
		$term = taxonomy_term_load($term_id);

		$render = array(
			'#type' => 'markup',
			'#markup' => $term->name,
		);
			
		$render = _dpi_taxonomy_content_element_link_render($settings, $term, $render);
		$render = _dpi_taxonomy_content_element_tag_render($settings,$render);

		$element[] = $render;

		if (isset($settings['separator_option']) && !empty($settings['separator_option'])) {
			// on ajout le séparateur
			$element[] = _dpi_taxonomy_content_simple_element_text_render($settings, 'separator_option');
		}

	}

	if (isset($settings['separator_option']) && !empty($settings['separator_option'])) {
		// Si il y a un séparateur, on supprimer le dernier
		array_pop($element);
	}

	if (count($items) > 0 && isset($settings['element_suffixe']) && !empty($settings['element_suffixe']) ) {
		// on ajoute le suffixe
		$element[] = _dpi_taxonomy_content_simple_element_text_render($settings,'element_suffixe');
	}

	// wrap element
	$element = _dpi_taxonomy_content_wrapper_tag_render($settings,$element);
}


function _dpi_taxonomy_content_element_link_render($settings, $term, $value){

	if($settings['element_html_link']!="no"){
		$classes_link = "";
		if ($settings['element_html_link_bootstrap_class'] != "- None -") {
			$classes_link = $settings['element_html_link_bootstrap_class'] . " " . $settings['element_html_link_custom_class'];
		} else {
			$classes_link = $settings['element_html_link_custom_class'];
		}
		if ($term->vocabulary_machine_name == "sections") {
			$term_link = drupal_get_path_alias('section/'.$term->tid);
		} else {
			$term_link = drupal_get_path_alias('taxonomy/term/'.$term->tid);
		}
		$render = array(
			'#value' => $value,
			'#tag' => 'a',
			'#attributes' => array(
				'href' => $term_link,
				'class' => array($classes_link),
			),
			'#theme_wrappers' => array(0 => 'dpi_render_wrapper'),
		);

		return $render;

	} else {
		return $value;
	}
}

function _dpi_taxonomy_content_wrapper_tag_render($settings,$value){
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
		return array($value);
	}
}

function _dpi_taxonomy_content_element_tag_render($settings,$value){

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

function _dpi_taxonomy_content_simple_element_text_render($settings, $elmt) {
	if (array_key_exists($elmt, $settings)) {
		$render = array(
			'#type' => 'markup',
			'#markup' => $settings[$elmt],
		);
		return $render;
	}
	return array();
}

function dpidestinations_field_formatter_settings_summary_dpi_taxonomy_content($field, $instance, $view_mode, &$summary){
	$settings = $instance['display'][$view_mode]['settings'];
	$summary = "";
	$summary .= _dpi_taxonomy_content_wrapper_tag_field_formatter_settings_summary($settings);
	$summary .= _dpi_taxonomy_content_element_tag_field_formatter_settings_summary($settings);
	$summary .= _dpi_taxonomy_content_theme_ui_element_link_field_formatter_settings_summary($settings);
	$summary .= _dpi_taxonomy_content_theme_ui_element_prefix_field_formatter_settings_summary($settings);
	$summary .= _dpi_taxonomy_content_theme_ui_element_suffix_field_formatter_settings_summary($settings);
	$summary .= _dpi_taxonomy_content_theme_ui_element_separator_field_formatter_settings_summary($settings);	

}

function _dpi_taxonomy_content_wrapper_tag_field_formatter_settings_summary($settings){
	if (isset($settings['wrapper_html_tag']) && $settings['wrapper_html_tag']!='- None -' && $settings['wrapper_html_tag']!=''){
		$summary = t('Use for wrapper a "@tag" tag with Bootstrap class(es) "@b_class" and custom class(es) "@c_class"<br/>', array('@tag' => $settings['wrapper_html_tag'],'@b_class' => $settings['wrapper_html_tag_bootstrap_class'],'@c_class' => $settings['wrapper_html_tag_custom_class']));
	}else{
		$summary = t('No HTML tag is specified for the element(s)\'s wrapper<br/>');
	}
	return $summary;
}


function _dpi_taxonomy_content_element_tag_field_formatter_settings_summary($settings){
	if (isset($settings['element_html_tag']) && $settings['element_html_tag']!='- None -' && $settings['element_html_tag']!='') {
		$summary = t('Use for each element(s) a "@tag" tag with Bootstrap class(es) "@b_class" and custom class(es) "@c_class"<br/>', array('@tag' => $settings['element_html_tag'],'@b_class' => $settings['element_html_tag_bootstrap_class'], '@c_class' => $settings['element_html_tag_custom_class']));
	}else{
		$summary = t('No HTML tag is specified for the element(s)<br/>');
	}
	return $summary;
}


function _dpi_taxonomy_content_theme_ui_element_link_field_formatter_settings_summary($settings){
	if ($settings['element_html_link']!="no") {
		$summary = t('The element is outputed as a link to the taxonomy page with Bootstrap class(es) "@b_class" and custom class(es) "@c_class"<br/>', array('@b_class' => $settings['element_html_link_bootstrap_class'], '@c_class' => $settings['element_html_link_custom_class']));
	}else{
		$summary = t('The element is not outputed as a link<br/>');
	}
	return $summary;
}

function _dpi_taxonomy_content_theme_ui_element_prefix_field_formatter_settings_summary($settings) {
	if (isset($settings['element_prefixe']) && $settings['element_prefixe']!='- None -' && $settings['element_prefixe']!='') {
		$summary = t('Use prefix ' . $settings['element_prefixe'] . '<br/>');
	}else{
		$summary = t('Don\'t use a prefix<br/>');
	}
	return $summary;
}

function _dpi_taxonomy_content_theme_ui_element_suffix_field_formatter_settings_summary($settings) {
	if (isset($settings['element_suffixe']) && $settings['element_suffixe']!='- None -' && $settings['element_suffixe']!='') {
		$summary = t('Use suffix ' . $settings['element_suffixe'] . '<br/>');
	}else{
		$summary = t('Don\'t use a prefix<br/>');
	}
	return $summary;	
}

function _dpi_taxonomy_content_theme_ui_element_separator_field_formatter_settings_summary($settings) {
	if (isset($settings['separator_option']) && $settings['separator_option']!='- None -' && $settings['separator_option']!='') {
		$summary = t('Elements separated by '. $settings['separator_option'] .'<br/>');
	}else{
		$summary = t('Don\'t use a separator<br/>');
	}
	return $summary;	
}