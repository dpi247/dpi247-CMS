<?php

/**
 * Define a cache entry type.
 *
 * If an cache entry type is not defined,
 * it will be impossible to use the functionnalities
 * of the dpicache module, like the hot and cold
 * cache refreshing processes.
 *
 * @return array
 *   An array of arrays defining the cache entries.
 *     - callback : the function to call to refresh the cache entry
 *     - plugin : the kind of storage
 *     - category : a group classification
 *     - category_callback : the function to call to refresh a category
 *     - data_type : the type of datas to be cached
 *     - ttl : validity time (in seconds) for a cache entry
 *     - ttl_callback : the function's name that return time
 *     - refresh_interval : the time (in seconds) between two cold refreshes for a cache entry
 *     - refresh_interval_callback : the function's name that return interval
 *     - bypass : for development purpose, allows to always bypass the cache
 *     - file : an array defining a file to include
 *       - module : module to which the file belongs
 *       - type : extension of the file
 *       - name : path of the file, relative to the module directory, without the extension
 */
function hook_dpicache_cache_entry_register() {
  return array(
    'flowmix_render' => array(
      'callback' => 'flowmix_get_render', // function to call
      'plugin' => 'dpicache_memcache', // which storage?
      'category' => 'flowmix_render', // group
      'data_type' => "string", // the callback returns a string
      'ttl' => 7*24*60*60, // (in seconds) keep a cache for one week
      'refresh_interval' => 5*60 //(in seconds) auto-refresh cache every 5 minutes
    ),
    // file key if your callback is not auto loaded (not in .module)
    //'file'=>array(
    //  "module"=>"dpidestinations",
    //  "type"=>"inc",
    //  "name"=>"plugins/ctools/content_types/targetblock/targetblock"
    //),
    // use bypass to bypass cache for debugging purpose.
    //'bypass'=>TRUE,
  );
}
