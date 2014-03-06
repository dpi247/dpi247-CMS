<?php
/**
 * @file
 * unfoldcompanion.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function unfoldcompanion_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_default_target().
 */
function unfoldcompanion_default_target() {
  $items = array();
  $items['main'] = entity_import('target', '{
    "machine_name" : "main",
    "label" : "Main",
    "view_name" : "default_targetview_2",
    "view_display" : "targetview_1",
    "description" : "Main target",
    "available_targetlayouts" : "a:7:{s:17:\\u0022targetblock_small\\u0022;s:17:\\u0022targetblock_small\\u0022;s:18:\\u0022targetblock_medium\\u0022;s:18:\\u0022targetblock_medium\\u0022;s:15:\\u0022targetblock_big\\u0022;s:15:\\u0022targetblock_big\\u0022;s:15:\\u0022targetblock_red\\u0022;s:15:\\u0022targetblock_red\\u0022;s:4:\\u0022full\\u0022;i:0;s:6:\\u0022teaser\\u0022;i:0;s:7:\\u0022package\\u0022;i:0;}",
    "available_templateoverrides" : "",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_targetblock().
 */
function unfoldcompanion_default_targetblock() {
  $items = array();
  $items['2'] = entity_import('targetblock', '{
    "targetblockid" : "2",
    "machine_name" : null,
    "target" : "main",
    "page_manager_handlers_did" : null,
    "node_count_for_targetblockinstance" : "10",
    "use_pager" : "0",
    "offset" : "0",
    "hide_pager" : "0",
    "use_split" : "",
    "split_delta" : "0",
    "did" : "1",
    "pid" : "2",
    "template_rule" : null,
    "node_count_for_targetblock" : "10",
    "pager_type" : "mini",
    "rank_max_value" : "5",
    "rdf_mapping" : []
  }');
  $items['3'] = entity_import('targetblock', '{
    "targetblockid" : "3",
    "machine_name" : null,
    "target" : "main",
    "page_manager_handlers_did" : null,
    "node_count_for_targetblockinstance" : "5",
    "use_pager" : "0",
    "offset" : "0",
    "hide_pager" : "0",
    "use_split" : "",
    "split_delta" : "0",
    "did" : "2",
    "pid" : "3",
    "template_rule" : null,
    "node_count_for_targetblock" : "5",
    "pager_type" : "mini",
    "rank_max_value" : "5",
    "rdf_mapping" : []
  }');
  return $items;
}
