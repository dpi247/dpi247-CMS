<?php
/**
 * @file
 * unfoldcompanion.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function unfoldcompanion_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
  );

  // Exported role: contributor.
  $roles['contributor'] = array(
    'name' => 'contributor',
    'weight' => 2,
  );

  // Exported role: journalist.
  $roles['journalist'] = array(
    'name' => 'journalist',
    'weight' => 3,
  );

  // Exported role: webmaster.
  $roles['webmaster'] = array(
    'name' => 'webmaster',
    'weight' => 2,
  );

  return $roles;
}
