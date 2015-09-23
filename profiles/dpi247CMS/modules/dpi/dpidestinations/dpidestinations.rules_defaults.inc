<?php 

/**
 * This function defines default rules for node regeneration caches
 *
 * @return multitype:RulesPlugin
 */


function dpidestinations_default_rules_configuration() {
  $configs = array ();
  $rule = '{ "rules_regenerate_cache_node" : {
              "LABEL" : "Refresh content rules",
              "PLUGIN" : "reaction rule",
              "OWNER" : "rules",
              "REQUIRES" : [ "dpidestinations", "dpiblocks" ],
              "ON" : {
                "dpidestinations_rules_event_package_create" : [],
                "dpidestinations_rules_event_package_edit" : [],
                "dpidestinations_rules_event_package_deleted" : []
              },
              "DO" : [
                { "refresh_node_cash" : { "node" : [ "node" ] } },
                { "regenerate_accroche_cache" : { "node" : [ "node" ] } },
                { "regenerate_node_drupal_content" : { "node" : [ "node" ] } },
                { "regenerate_node_varnish_content" : { "node" : [ "node" ] } }
              ]
            }
          }';
  $configs ['rules_regenerate_cache_node'] = rules_import ( $rule );
  $rule = '{ "rules_invalidation_cache_targetblock" : {
              "LABEL" : "Invalidation cache targetblock",
              "PLUGIN" : "reaction rule",
              "OWNER" : "rules",
              "REQUIRES" : [ "dpidestinations" ],
              "ON" : {
                "targetblock_insert" : [],
                "targetblock_update" : [],
                "targetblock_delete" : []
              },
              "DO" : [
                { "regenerate_targetblock_destination" : { "node" : [ "targetblock" ] } }
              ]
            }
          }';
  $configs ['rules_regenerate_cash_targetblock'] = rules_import ( $rule );
  return $configs;
}
