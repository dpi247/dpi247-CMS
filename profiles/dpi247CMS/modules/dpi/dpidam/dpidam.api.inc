<?php

/**
 * Get all DAM search profiles
 *
 * @return array
 *   An associative array containing the search profiles, the keys are the PIDs of each profile
 */
function dpidam_api_get_search_profiles() {
  $db_profiles = db_select('dpidam_search_profiles', 'dsp')
    ->fields('dsp')
    ->execute();
  $profiles = array();
  foreach ($db_profiles as $db_profile) {
    $profiles[$db_profile->pid] = (array)$db_profile;
    $profiles[$db_profile->pid]['settings'] = unserialize($profiles[$db_profile->pid]['settings']);
  }
  return $profiles;
}

/**
 * Get a DAM search profile
 *
 * @param int|string $pid
 *   The PID or the machine name of the wanted profile
 *
 * @return array
 *   The wanted profile
 */
function dpidam_api_get_search_profile($pid) {
  $db_select_profiles = db_select('dpidam_search_profiles', 'dsp')
    ->fields('dsp');
  if (is_numeric($pid)) {
    $db_select_profiles->condition('dsp.pid', $pid);
  } else {
    $db_select_profiles->condition('dsp.name', $pid);
  }
  $db_profiles = $db_select_profiles->execute();

  $profile = FALSE;
  foreach ($db_profiles as $db_profile) {
    $profile = (array)$db_profile;
    $profile['settings'] = unserialize($profile['settings']);
    break;
  }
  return $profile;
}

/**
 * Check wheter a DAM search profile exists or not
 *
 * @param int|string $pid
 *   The PID or the machine name of the wanted profile
 *
 * @return bool
 */
function dpidam_api_check_search_profile_exists($pid) {
  $db_select_profiles = db_select('dpidam_search_profiles', 'dsp')
    ->fields('dsp', array('pid'));
  if (is_numeric($pid)) {
    $db_select_profiles->condition('dsp.pid', $pid);
  } else {
    $db_select_profiles->condition('dsp.name', $pid);
  }
  $db_profiles = $db_select_profiles->execute();

  $profile_exists = FALSE;
  foreach ($db_profiles as $db_profile) {
    $profile_exists = TRUE;
    break;
  }
  return $profile_exists;
}

/**
 * Get the default DAM search profile
 *
 * @return array
 *   The default search profile
 */
function dpidam_api_get_default_search_profile() {
  $db_profiles = db_select('dpidam_search_profiles', 'dsp')
    ->fields('dsp')
    ->condition('dsp.default_profile', 1)
    ->execute();

  $profile = FALSE;
  foreach ($db_profiles as $db_profile) {
    $profile = (array)$db_profile;
    break;
  }
  return $profile;
}

/**
 * Get the default DAM search profile
 */
function dpidam_api_get_loaded_default_search_profile() {
  static $default_profile = NULL;

  if (!$default_profile) {
    $db_profiles = db_select('dpidam_search_profiles', 'dsp')
      ->fields('dsp')
      ->condition('default_profile', 1)
      ->execute();
    foreach ($db_profiles as $db_profile) {
      $default_profile = (array)$db_profile;
      break;
    }
    if (!$default_profile) {
      $db_query_min_profile = db_select('dpidam_search_profiles', 'dsp');
      $db_query_min_profile->addExpression('MIN(dsp.pid)', 'min_pid');
      $db_min_profile = $db_query_min_profile->execute();
      $min_profile = FALSE;
      foreach ($db_min_profile as $db_min_profile) {
        $min_profile = $db_min_profile->min_pid;
        break;
      }
      if ($min_profile) {
        $db_profiles = db_select('dpidam_search_profiles', 'dsp')
          ->fields('dsp')
          ->condition('dsp.pid', $min_profile)
          ->execute();
        foreach ($db_profiles as $db_profile) {
          $default_profile = (array)$db_profile;
          break;
        }
      }
    }

    if ($default_profile && is_string($default_profile['settings'])) {
      $default_profile['settings'] = unserialize($default_profile['settings']);
    }
  }

  return $default_profile;
}

/**
 * Get all sources
 *
 * @return array
 *   An associative array containing the sources, the keys are the SIDs of each source
 */
function dpidam_api_get_sources() {
  $db_sources = db_select('dpidam_source', 'ds')
    ->fields('ds')
    ->execute();
  $sources = array();
  foreach ($db_sources as $db_source) {
    $sources[$db_source->sid] = $db_source;
  }
  return $sources;
}

/**
 * Get a source
 *
 * @param int|string $pid
 *   The SID or the machine name of the wanted source
 *
 * @return array
 *   The wanted source
 */
function dpidam_api_get_source($sid) {
  $db_select_sources = db_select('dpidam_source', 'ds')
    ->fields('ds');
  if (is_numeric($sid)) {
    $db_select_sources->condition('ds.sid', $sid);
  } else {
    $db_select_sources->condition('ds.name', $sid);
  }
  $db_sources = $db_select_sources->execute();

  $source = FALSE;
  foreach ($db_sources as $db_source) {
    $source = $db_source;
    break;
  }
  return $source;
}

/**
 * Check wheter a source exists or not
 *
 * @param int|string $sid
 *   The SID or the machine name of the wanted source
 *
 * @return bool
 */
function dpidam_api_check_source_exists($sid) {
  $db_select_sources = db_select('dpidam_source', 'ds')
    ->fields('ds', array('sid'));
  if (is_numeric($sid)) {
    $db_select_sources->condition('ds.sid', $sid);
  } else {
    $db_select_sources->condition('ds.name', $sid);
  }
  $db_sources = $db_select_sources->execute();

  $source_exists = FALSE;
  foreach ($db_sources as $db_source) {
    $source_exists = TRUE;
    break;
  }
  return $source_exists;
}
