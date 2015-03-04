<?php 

/**
 * This function defines default rules for node regeneration caches
 *
 * @return multitype:RulesPlugin
 */
function dpiservices_default_rules_configuration() {
  $configs = array ();
  $rule = '{ "rules_refresh_json_content" : {
                  "LABEL" : "Refresh_json_content",
                  "PLUGIN" : "reaction rule",
                  "OWNER" : "rules",
                  "REQUIRES" : [ "dpiservices", "dpidestinations" ],
                  "ON" : {
                    "dpidestinations_rules_event_package_create" : [],
                    "dpidestinations_rules_event_package_deleted" : [],
                    "dpidestinations_rules_event_package_edit" : []
                  },
                  "DO" : [ { "regenerate_json_package_cache" : [] } ]
                }
            }';
  $configs ['rules_regenerate_cache_json_package'] = rules_import ( $rule );
  return $configs;
}