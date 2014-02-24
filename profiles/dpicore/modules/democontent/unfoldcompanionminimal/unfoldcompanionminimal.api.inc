<?php

/**
 * Check if a date field is empty or not
 *
 * @return bool
 */
function unfoldcompanionminimal_api_check_date_content_not_empty($content, $field_name) {
  if (isset($content[$field_name]) &&
      isset($content[$field_name]['#items'][0]['value']) && !empty($content[$field_name]['#items'][0]['value']) &&
      isset($content[$field_name][0]['#markup']) && !empty($content[$field_name][0]['#markup'])) {
    return TRUE;
  }
  return FALSE;
}
