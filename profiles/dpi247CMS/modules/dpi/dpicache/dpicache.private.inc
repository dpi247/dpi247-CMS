<?php

/**
 * Fetch cache content from ad hoc storage
 *
 * @param  String $entry_name Name of the dpicache
 * @param  String $entry_hash Hash of called parameters
 * @return void                  Content stored, FALSE otherwise.
 */
function dpicache_fetch_cache($entry_name, $entry_hash)
{
	global $conf;
	$default_cache_table = 'cache_dpicache';
	// Check if a specific storage has been defined for
	// this type of DPIcache (MemCacheDrupal).
	// By default all is stored in cache_dpicache table
	// thanks to DrupalDatabaseCache PHP class.
	// [ see /includes/cache.inc : _cache_get_object() ]
	if (isset($conf['cache_class_dpicache_' . $entry_name])) {
		$bin = 'dpicache_' . $entry_name;
	} else {
		$bin = $default_cache_table;
	}
	$c = cache_get($entry_name . '_' . $entry_hash, $bin);
	if (!$c)
		return FALSE;
	return $c->data;
}

/**
 * Save cache content in ad hoc storage
 *
 * @param  String $entry_name Name of the dpicache
 * @param  String $entry_hash Hash of called parameters
 * @param  void $content Content to store
 * @param  Array $cache_definition Registered definition
 * @return Boolean                  Save success, or not.
 */
function dpicache_save_cache($entry_name, $entry_hash, $content, $cache_definition)
{
	global $conf;


	if (!dpicache_prevent_save_of_cache_entry_for_domain()) {
		$default_cache_table = 'cache_dpicache';
		if (isset($conf['cache_class_dpicache_' . $entry_name])) {
			$bin = 'dpicache_' . $entry_name;
		} else {
			$bin = $default_cache_table;
		}
		cache_set($entry_name . '_' . $entry_hash, $content, $bin, CACHE_PERMANENT);
		return TRUE;

	} else {
		dpi_log('dpicache', "prevent_cache_entry_save_for_domain", t("Cache (re)generated for name = !cache_name, hash = !hash but not saved due to the domain name in the list of prevent domain", array(
			"!cache_name" => $entry_name,
			"!hash" => $entry_hash
		)), array(), $entry_hash, WATCHDOG_DEBUG);


		return FALSE;
	}
}

/**
 * Check if the host is in the dpi_variable_get('dpicache_prevent_save_of_cache_entry_for_domain', FALSE)
 *
 * @return bool return TRUE if we should prevent from saving the cache entry
 * @see dpicache_save_cache()
 *
 */
function dpicache_prevent_save_of_cache_entry_for_domain()
{

	$host = $_SERVER['HTTP_HOST'];

	$forbidden_hosts = explode("\n", dpi_variable_get('dpicache_prevent_save_of_cache_entry_for_domain', FALSE));

	return in_array($host, $forbidden_hosts);


}

/**
 * Delete cache content in ad hoc storage
 *
 * @param  String $entry_name Name of the dpicache
 * @param  String $entry_hash Hash of called parameters
 * @return Boolean               Deletion success, or not.
 */
function dpicache_delete_cache($entry_name, $entry_hash)
{
	global $conf;
	$default_cache_table = 'cache_dpicache';
	if (isset($conf['cache_class_dpicache_' . $entry_name])) {
		$bin = 'dpicache_' . $entry_name;
	} else {
		$bin = $default_cache_table;
	}
	cache_clear_all($entry_name . '_' . $entry_hash, $bin);
}

/**
 * Regenerate caches that need to be regenerated based on the time
 * @param  Integer $time Current timestamp
 * @return Boolean       Success of regenerations
 */
function dpicache_regenerate_cache_entries_since_time($time)
{
	if (!$time)
		$time = date();
	// Get caches that need to be regenerated
	$table_cache_entries = 'dpicache_cache_entries';
	$query = db_select($table_cache_entries, 'dce');
	$query->fields('dce', array('name', 'hash', 'data_type', 'creation_date', 'parameters'));
	$query->where('dce.expiration_date > ' . $time . ' AND dce.last_run+dce.update_freq < ' . $time);

	//@todo modification pour éviter d'utiliser la fonction car celle-ci pos eproblème actuellement
	//dpicache_regenerate_cache_entries_process($query);

	/* suppression des caches concernés */
	foreach ($query->execute() as $entry) {
		try {
			$cid = $entry->name . '_' . $entry->hash;
			cache_clear_all($cid, 'cache_dpicache');
			dpi_log('dpicache', "manual_regeneration_of_cache_name", t("Manual cache suppression: @cache_name success !!!", array(
				'@cache_name' => $entry->name . '_' . $entry->hash,
			)), array(), NULL, WATCHDOG_INFO);
		} catch (Exception $e) {
			dpi_log('dpicache', "manual_regeneration_of_cache_name", t("Manual cache suppression: @cache_name success !!!", array(
				'@cache_name' => $entry->name . '_' . $entry->hash,
			)), array(), NULL, WATCHDOG_CRITICAL);
		}
	}

	return TRUE;
}

/**
 * Process the cache generation for each result of the querry object
 *
 * @param  Ressource A querry object htat should contains dpicache_cache_entries rows
 */
function dpicache_regenerate_cache_entries_process($query)
{
	foreach ($query->execute() as $entry) {
		$cache_definition = dpicache_get_cache_entry_definition($entry->name);
		if (!$cache_definition) {
			dpi_log('dpicache', "error", t("Cron : the entry name !cache_name has no definition", array("!cache_name" => $entry->name)), array(), $entry->hash, WATCHDOG_WARNING);
			// @TODO Modifier sa date d'expiration à '-1'
			continue;
		}
		// Get fresh content
		$fresh_content = _dpicache_cache_call_function($cache_definition, unserialize($entry->parameters));
		if (is_string($fresh_content) && strpos($fresh_content, '#ERROR#')) {
			// fail to get cache
			dpi_log('dpicache', "error", t("Error while calling callback function for name = !cache_name, hash = !hash : !error", array("!cache_name" => $entry->name, "!hash" => $entry->hash, "!error" => $fresh_content)), array(), $entry->hash, WATCHDOG_ERROR);
			continue;
		}

		// Save content into cache
		$saved = dpicache_save_cache($entry->name, $entry->hash, $fresh_content, $cache_definition);
		$val_arg = dpicache_get_arguments_values($cache_definition, unserialize($entry->parameters));
		$entry_information = array(
			"name" => $entry->name,
			"hash" => $entry->hash,
			"category" => $val_arg['category'],
			"data_type" => @$cache_definition["data_type"] ?: "string",
			"expiration_date" => intval($entry->creation_date + $val_arg['expiration_date']),
			"update_freq" => $val_arg['update_freq']
		);
		if ($saved)
			$entry_information['last_run'] = time();
		// Update cache entry data
		dpicache_save_cache_entry($entry_information, true);
		dpi_log('dpicache', "regenerate_cold", t("Cache (re)generated for name = !cache_name, hash = !hash", array("!cache_name" => $entry->name, "!hash" => $entry->hash)), array(), $entry->hash, WATCHDOG_DEBUG);
	}
	return TRUE;
}

function dpicache_get_arguments_values($cache_definition, $entry)
{
	$value_return['update_freq'] = $cache_definition["refresh_interval"];

	if (isset($cache_definition["refresh_interval_callback"])) {
		$func = $cache_definition["refresh_interval_callback"];
		$value_return['update_freq'] = $func($cache_definition, $entry);
	}

	$value_return['expiration_date'] = $cache_definition["ttl"];
	if (isset($cache_definition["ttl_callback"])) {
		$func = $cache_definition["ttl_callback"];
		$value_return['expiration_date'] = $func($cache_definition, $entry);
	}

	$value_return['category'] = $cache_definition["category"];
	if (isset($cache_definition["category_callback"])) {
		$func = $cache_definition["category_callback"];
		$value_return['category'] = $func($cache_definition, $entry);
	}
	return $value_return;
}

/**
 * Get registered infos associated with a cache type
 * @param  String $cache_name Name of a dpicache.
 * @return Array             Infos about it. False otherwise.
 */
function dpicache_get_cache_entry_definition($cache_name)
{
	if (!$cache_name)
		return FALSE;
	$infos = dpicache_get_all_cache_entries_definitions();
	if (isset($infos[$cache_name])) {
		return $infos[$cache_name];
	}
	return FALSE;
}

/**
 * Call all the hook_dpicache_cache_entry_register
 */
function dpicache_get_all_cache_entries_definitions()
{
	$infos = &drupal_static(__FUNCTION__);
	if (!isset($infos)) {
		$infos = module_invoke_all('dpicache_cache_entry_register');
		// @TODO ajouter un drupal_alter
	}
	return $infos;
}

/**
 * Call defined callbacked in registered dpicache entry
 * with parameters, and return result.
 *
 * @param  Array $dpicache_definition A dpicache entry.
 * @param  Array $parameters All parameters to pass to callback.
 * @return void                        Return of callback function. A string
 *               beginning with "#ERROR#" is returned if we are unable to
 *               call the callback function.
 */
function _dpicache_cache_call_function($dpicache_definition, $parameters = array())
{
	if (_dpicache_validate_cache_entry($dpicache_definition) === FALSE) {
		return FALSE;
	}
	$out = '';
	try {
		$out = call_user_func_array($dpicache_definition['callback'], $parameters);
	} catch (Exception $e) {
		watchdog('dpicache', "dpicache call function : Exception : @ex.",
			array('@ex' => $e->getMessage()), WATCHDOG_ERROR);
		return "#ERROR# Exception occurred.";
	}
	return $out;
}

/**
 * Validate registered DPIcache definition.
 *
 * @param  Array $dpicache_definition DPICache definition
 * @return Boolean                     Definition is valid, or not.
 */
function _dpicache_validate_cache_entry($dpicache_definition = array())
{
	if (!$dpicache_definition || !is_array($dpicache_definition)) {
		watchdog('dpicache', "dpicache validate : no valid dpicache definition provided",
			array(), WATCHDOG_WARNING);
		return FALSE;
	}
	if (!isset($dpicache_definition["callback"])) {
		watchdog('dpicache', "dpicache validate : no valid callback function provided in !def",
			array('!def' => '<pre>' . print_r($dpicache_definition, true) . '</pre>'), WATCHDOG_WARNING);
		return FALSE;
	}
	$callback = $dpicache_definition["callback"];
	if (isset($dpicache_definition["file"])) {
		module_load_include($dpicache_definition["file"]['type'], $dpicache_definition["file"]['module'], $dpicache_definition["file"]['name']);
	}
	if (!$callback || !function_exists($callback)) {
		watchdog('dpicache', "dpicache validate : callback function @func does not exist in !def.",
			array('!def' => '<pre>' . print_r($dpicache_definition, true) . '</pre>', '@func' => $callback),
			WATCHDOG_WARNING);
		return FALSE;
	}
	return TRUE;
}

/**
 * Create/Update database with up to date data from registered dpicaches
 * @param  Array $options dpicache_cache_entries' table's fields
 * @param  Boolean $update If false=Creation, true=Update database
 * @return Boolean          Save success, or not.
 */
function dpicache_save_cache_entry($options = array(), $update = FALSE)
{
	if (!$options)
		return FALSE;
	$table_cache_entries = 'dpicache_cache_entries';
	try {
		// Insert or update
		$query = db_merge($table_cache_entries);
		$query
			->key(array('name' => $options['name'], 'hash' => $options['hash']))
			->insertFields(array(
				'name' => @$options['name'],
				'hash' => @$options['hash'],
				'creation_date' => @$options['creation_date'],
				'parameters' => @$options['parameters'],
				'category' => $options['category'],
				'data_type' => $options['data_type'],
				'expiration_date' => $options['expiration_date'],
				'last_run' => $options['last_run'],
				'update_freq' => $options['update_freq'],
			))
			->updateFields(array(
				'category' => $options['category'],
				'data_type' => $options['data_type'],
				'expiration_date' => $options['expiration_date'],
				'last_run' => $options['last_run'],
				'update_freq' => $options['update_freq'],
			))
			->execute();
		return TRUE;
	} catch (Exception $e) {
		dpi_log('dpicache', "error", t("Unable to save or update dpicache entry : !error / !options", array("!error" => $e->getMessage(), "!options" => print_r($options, 1))), array(), @$options['hash'], WATCHDOG_ERROR);
		watchdog('dpicache', 'Save entry exception : %s', array('%s' => $e->getMessage()), WATCHDOG_ERROR);
		return FALSE;
	}
}

/**
 * Clean the cold cache entries table
 * by deleting out of date entries
 *
 * @param  Integer $time Current Timestamp
 * @return Boolean       Cleanup success, or not.
 */
function dpicache_cron_cleanup($time)
{
	if (!$time)
		$time = date();
	// Get caches that need to be deleted
	$table_cache_entries = 'dpicache_cache_entries';
	$query = db_select($table_cache_entries, 'dce');
	$query->fields('dce', array('name', 'hash'));
	$query->condition('dce.expiration_date', $time, '<=');
	$entries = array();
	foreach ($query->execute() as $entry) {
		$entries[] = array('name' => $entry->name, 'hash' => $entry->hash);
	}
	if (empty($entries))
		return TRUE;
	// Now, delete out of date caches
	foreach ($entries as $entry) {
		dpicache_delete_cache($entry['name'], $entry['hash']);
		// ... and associated entries
		$num_deleted = db_delete($table_cache_entries)
			->condition('name', $entry['name'])
			->condition('hash', $entry['hash'])
			->execute();
		dpi_log('dpicache', "regenerate_cold", t("This out of date entry has been deleted : name = !cache_name, hash = !hash", array("!cache_name" => $entry['name'], "!hash" => $entry['hash'])), array(), $entry['hash'], WATCHDOG_DEBUG);
	}
	return TRUE;
}

/**
 * Get hash for given cache name and arguments.
 * Arguments MUST be an array of strings.
 *
 * @param  String $cache_name Name of the DPIcache.
 * @param  Array $args Array of callback function arguments.
 * @return String             The hash.
 */
function _dpicache_get_hash($cache_name, $args = array())
{
	$hash = sha1($cache_name . '--' . implode('-', $args));
	return $hash;
}


/*
 * Sync block roles with esi role visibility
 * 
 * The dpicache esi block uses it's own visibility to show block.
 * The main reason to that is to keep the page that contains the esi tag the same for 
 * everybody.
 * 
 * If we rely on Drupal's block role this page would differ based on role ... 
 * E.g : esi tag will appear or not...
 * And thus we use the concept of esi block role visibility ...
 * 
 * In the admin this is transparent for the user 
 * but in fact we collect the roles setup and put them on a custom column of the block page
 * and we let the real role visibility blank
 * 
 * Now, when we activate/desactive the ESI from dpicache, we need to synchronize that with Drupal's role block...
 * 
 *  
 * @see _dpicache_esi_sync_esi_visibility_to_drupal_roles()
 * @see dpicache_block_admin_configure_submit()
 * @see dpicache_form_block_admin_configure_alter()
 * 
 */
function _dpicache_esi_sync_drupal_roles_to_esi_visibility()
{
	global $theme;
	$query = db_select('block_role', 'br');
	// Add extra detail to this query object: a condition, fields and a range
	$query->condition('b.esi_enabled', 1, '=');
	$query->condition('b.theme', $theme, '=');
	$query->fields('br', array('module', 'delta', 'rid'));
	$query->leftJoin('block', 'b', 'b.delta = br.delta AND b.module = br.module');
	$results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
	foreach ($results as $result) {
		$writes[$result['module']][$result['delta']][$result['rid']] = $result['rid'];
		$deletes[$result['module']][$result['delta']][$result['rid']] = $result['rid'];

	}
	foreach ($results as $result) {
		foreach (user_roles() as $rid => $role) {
			if (!isset($write[$result['module']][$result['delta']][$rid])) {
				$writes[$result['module']][$result['delta']][$rid] = 0;

			}
		}
	}
	foreach ($writes as $module => $write) {
		foreach ($write as $delta => $roles) {


			$block_updated = db_update('block')
				->fields(array(
					'esi_roles' => serialize($roles),
				))
				->condition('module', $module, '=')
				->condition('delta', $delta, '=')
				->execute();

		}
	}
	//remove entry from block roles
	foreach ($deletes as $module => $delete) {
		foreach ($delete as $delta => $roles) {
			foreach ($roles as $rid)

				$num_deleted = db_delete('block_role')
					->condition('module', $module, '=')
					->condition('delta', $delta, '=')
					->condition('rid', $rid, '=')
					->execute();
		}
	}
}


/*
 * Sync esi role visibility with block roles with 
 *
 *
 *
 * @see _dpicache_esi_sync_drupal_roles_to_esi_visibility()
 * @see dpicache_block_admin_configure_submit()
 * @see dpicache_form_block_admin_configure_alter()
 *
 */
function _dpicache_esi_sync_esi_visibility_to_drupal_roles()
{
	global $theme;
	$query = db_select('block', 'b');
	// Add extra detail to this query object: a condition, fields and a range
	$query->condition('b.esi_enabled', 1, '=');
	$query->condition('b.theme', $theme, '=');
	$query->fields('b', array('module', 'delta', 'esi_roles'));
	$results = $query->execute();
	foreach ($results as $result) {
		$esi_roles = unserialize($result->esi_roles);
		foreach ($esi_roles as $rid => $esi_role) {
			if ($esi_role) {
				$block_role = new stdClass();
				$block_role->module = $result->module;
				$block_role->delta = $result->delta;
				$block_role->rid = $rid;
				drupal_write_record('block_role', $block_role);
			}
		}
	}


}




/**
 * Get a list of possible max age (ttl) choices.
 *
 * @param optional Int $current_max_age
 * The defined max age might not amongst the pre-defined options.  Adding the
 * current max age as a parameter allows it to be added to the list of options,
 * ensuring the current configuration doesn't get overridden.
 *
 * @return Array
 * Array of potential max-age choices. The key is the TTL (in seconds) and the
 * value is the human-readable description of that TTL.
 */
function _dpicache_esi_max_age_options($current_max_age = NULL) {
	if (is_null($current_max_age)) {
		$current_max_age = variable_get('dpicache_esi_default_tll_block', ESI_DEFAULT_TTL);
	}



	$options = drupal_map_assoc(
		array(0, 5, 15, 30, 60, 120, 180, 240, 300, 600, 900, 1200, 1800, 3600, 7200, 14400, 28800, 43200, 64800, 86400, 86400*2, 86400*3, 86400*4, 86400*5, 86400*6, 86400*7),
		'format_interval');

	// If the given max age isn't one of our options, add the current max age as a custom option.
	if (!isset($options[$current_max_age])) {
		$options[$current_max_age] = t('Custom: @time', array('@time' => format_interval($current_max_age)), array('context' => 'Cache Duration'));
		ksort($options);
	}
	$options[0] = '<' . t('none', array(), array('context' => 'Cache Duration')) . '>';

	$options[-1] = '<' . t('Default TTL', array(), array('context' => 'Cache Duration')) . '>';
	ksort($options);


	return $options;
}