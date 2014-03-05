<?php

/**
 * Implementation of hook_preprocess_field_extra_field
 * Add template suggestions on field names and view mode for extra fields theming
 */
function unfoldcompanionminimal_preprocess_field_extra_field(&$vars) {
  if (isset($vars['fields']) && is_array($vars['fields']) && !empty($vars['fields'])) {
    $combined_fields_name = array();
    foreach ($vars['fields'] as $field_name => $field) {
      $combined_fields_name[] = $field_name;
    }
    if (!empty($combined_fields_name)) {
      $imploded_fields_name = implode('_', $combined_fields_name);
      $vars['theme_hook_suggestions'][] = 'field_extra_field__'.$imploded_fields_name;
      $vars['theme_hook_suggestions'][] = 'field_extra_field__'.$imploded_fields_name.'__'.$vars['view_mode'];
    }
  }
}
