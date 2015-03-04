<?php 

/**
 * This function defines default rules for node regeneration caches
 *
 * @return multitype:RulesPlugin
 */


function dpidestinations_default_rules_configuration() {
  $configs = array ();
  $rule = '{ "rules_refresh_content_rules" : {
              "LABEL" : "Refresh content rules",
              "PLUGIN" : "reaction rule",
              "OWNER" : "rules",
              "REQUIRES" : [ "dpidestinations" ],
              "ON" : {
                "dpidestinations_rules_event_package_create" : [],
                "dpidestinations_rules_event_package_edit" : [],
                "dpidestinations_rules_event_package_deleted" : []
              },
              "DO" : [
                { "regenerate_node" : { "node" : [ "node" ] } },
                { "refresh_node_cash" : { "node" : [ "node" ] } },
                { "regenerate_accroche_cache" : { "node" : [ "node" ] } }
              ]
            }
          }';
  $configs ['rules_regenerate_cache_node'] = rules_import ( $rule );
  return $configs;
}
