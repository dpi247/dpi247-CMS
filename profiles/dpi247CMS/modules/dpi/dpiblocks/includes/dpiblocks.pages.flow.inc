<?php

/**
 * Gives informations about existing flow plugins
 */
function dpiblocks_page_flow_plugins() {
  ctools_include('plugins');

  $module = 'dpiblocks';
  $type = 'flow';

  $owner = $module;
  $api = $type;

  // Give informations about plugins provided by modules
  dsm(ctools_get_plugins($module, $type), t('All flow plugins'));

  // Give informations about the type definition of plugins
  dsm(ctools_plugin_get_info($module, $type), t('General flow plugins informations'));

  // Which module implements this plugin with which version?
  dsm(ctools_plugin_api_info($owner, $api, 0, 99), t('API Informations'));

  return '';
}
