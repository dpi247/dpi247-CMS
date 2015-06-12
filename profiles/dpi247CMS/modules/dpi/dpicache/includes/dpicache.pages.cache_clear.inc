<?php



function dpicache_pages_form_cache_clear_form($form, &$form_state)
{
	$form = array();

	$form['info'] = array(
		'#markup' => 'This panels is use to manage cache.'
	);

	$form['dpicache_use'] = array(
		'#title' => t('Caching system settings'),
		'#description' => 'Check this box if you want to bypass cache from dpicache',
		'#type' => 'fieldset',
	);


	$form['dpicache_use']['dpicache_utilisation'] = array(
		'#title' => t('bypass dpicache'),
		'#type' => 'checkbox',
		'#default_value' => dpi_variable_get('dpicache_disable_dpicache_utilisation', FALSE),
	);

	$form['dpicache_use']['dpicache_prevent_save_of_cache_entry_for_domain'] = array(
		'#title' => t('Prevent save of cache entry for thoses domain'),
		'#description' => 'You can prevent dpicache from saving a cache entry if the domain used during cache\'s  building matches one of those lines.<br/> Please use one line per domain and do not include the scheme (http://) ',
		'#type' => 'textarea',
		'#default_value' => dpi_variable_get('dpicache_prevent_save_of_cache_entry_for_domain', FALSE),

	);

	$form['dpicache_use']['dpicache_status'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);

	$form['dpicache_accumulation'] = array(
		'#title' => t('Settings for caching with accumulation'),
		'#description' => 'Check this box if you want to activate accumulation system',
		'#type' => 'fieldset',
	);

	$form['dpicache_accumulation']['accumulation'] = array(
		'#title' => t('Activate accumulation système'),
		'#type' => 'checkbox',
		'#default_value' => dpi_variable_get('dpicache_disable_dpicache_accumulation', FALSE),
	);

	$form['dpicache_accumulation']['dpicache_accumulation_cache'] = array(
		'#type' => 'submit',
		'#value' => 'Save Settings',
	);

	$form['dpicache_cache_varnish'] = array(
		'#title' => t('Settings to clear varnish cache'),
		'#description' => 'This button is for clearing varnish cache',
		'#type' => 'fieldset',
	);

	$form['dpicache_cache_varnish']['varnish_clean_cache'] = array(
		'#type' => 'submit',
		'#value' => 'Clear varnish cache',
	);

	$form['dpicache_cache_submiting'] = array(
		'#title' => t('Settings for clear dpicache cache'),
		'#description' => 'This button is for clearing cache from dpicache module',
		'#type' => 'fieldset',
	);

	$form['dpicache_cache_submiting']['dpicache_clear_cache'] = array(
		'#type' => 'submit',
		'#value' => 'Clear cache from dpicache',
	);

	$form['dpicache_cache_entries'] = array(
		'#title' => t('Settings for Dpicache entries'),
		'#description' => 'This button delete all entries from `dpicache_cache_entries` table',
		'#type' => 'fieldset',
	);

	$form['dpicache_cache_entries']['dpicache_table_entries'] = array(
		'#type' => 'submit',
		'#value' => 'Clar dpicache entries table',
	);


	return $form;
}

/**
 *
 * @param unknown $form
 * @param unknown $form_state
 */
function dpicache_pages_form_cache_clear_form_submit($form, &$form_state)
{
	switch ($form_state['triggering_element']['#parents'][0]) {
		case 'dpicache_accumulation_cache' :
			dpi_variable_set('dpicache_disable_dpicache_accumulation', $form_state['values']['accumulation']);
			drupal_set_message("Settings saved");
			break;
		case 'dpicache_status':
			dpi_variable_set('dpicache_disable_dpicache_utilisation', $form_state['values']['dpicache_utilisation']);
			dpi_variable_set('dpicache_prevent_save_of_cache_entry_for_domain', $form_state['values']['dpicache_prevent_save_of_cache_entry_for_domain']);

			drupal_set_message("Settings saved");


			break;
		case 'varnish_clean_cache':
			dpicache_api_varnish_purge_all_page();
			drupal_set_message("Varnish Cache Clear");
			break;
		case 'dpicache_table_entries':
			if (db_truncate('dpicache_cache_entries')->execute() == 0) {
				drupal_set_message("The table dpicache_cache_entries is truncated");
			} else {
				drupal_set_message("An error is occured", 'error');
			}
			break;
		case 'dpicache_clear_cache':
			drupal_flush_all_caches();
			drupal_set_message("Cache Clear");
			break;
	}
}
