<?php

/**
 * Determine if a provider exists for this uri and in this case, return the provider (otherwise set provider to null)
 *
 * @return
 *   An array with the provider informations.
 *   following items:
 *   - "name": The machine name of the provider.
 *   - "type": The type of the provider (photo/video/audio/etc)
 */
function hook_dpicontenttypes_provider($uri) {
  return array(
    'module_name' => array('name' => 'provider_name', 'type' => 'provider_type'),
  );
}
