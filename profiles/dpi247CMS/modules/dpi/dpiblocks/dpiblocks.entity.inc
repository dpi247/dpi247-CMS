<?php

/**
 * Implementation of hook_entity_info()
 *
 * @return $entities
 *  An array of definitions
 */
function dpiblocks_entity_info(){
  // Create entities info
  $entities = array();

  // Remote HTML
  $entities['remotehtml'] = array(
    'label' => t('Remote Html'),
    'entity class' => 'RemoteHtmlEntity',
    'controller class' => 'RemotehtmlController',
    'base table' => 'dpiblocks_remotehtml',
    'fieldable' => TRUE,
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'entity keys' => array(
      'id' => 'rid',
      'machine_name' => 'machine_name',
    ),

    'bundles' => array(
      'remotehtml' => array(),
    ),

    'view modes' => array(
      'minimal' => array(
        'label' => t('Minimal'),
        'custom settings' => FALSE,
      ),
      'full' => array(
        'label' => t('Full'),
        'custom settings' => TRUE,
      ),
    ),

    // Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
    'module'=> 'dpiblocks',
    'access callback' => 'remotehtml_access_admin_callback',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/remotehtml',
      // OPTIONAL- See "Customize the User Interface" below if you set.
      'file' => 'includes/remotehtml.admin.inc',
    ),
  );

  // Custom content
  $entities['custom_content'] = array(
    'label' => t('Custom content'),
    'entity class' => 'Custom_contentEntity',
    'controller class' => 'Custom_contentController',
    'base table' => 'dpiblocks_custom_content',
    'revision table' => 'dpiblocks_custom_content_revision',
    'fieldable' => TRUE,
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'entity keys' => array(
      'id'=>'cid',
      'machine_name' => 'machine_name',
      'revision' => 'revision_id',
    ),

    'bundles' => array(
      'custom_content' => array(),
    ),

    'view modes' => array(
      'minimal' => array(
        'label' => t('Minimal'),
        'custom settings' => FALSE,
      ),
      'full' => array(
        'label' => t('Full'),
        'custom settings' => TRUE,
      ),
    ),

    // Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
    'module'=> 'dpiblocks',
    'access callback' => 'custom_content_access_admin_callback',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/custom_content',
      // OPTIONAL- See "Customize the User Interface" below if you set.
      'file' => 'includes/custom_content.admin.inc',
    ),
  );

  // FlowMix
  $entities['flowmix'] = array(
    'label' => t('FlowMix'),
    'entity class' => 'FlowMixEntity',
    'controller class' => 'FlowMixController',
    'base table' => 'dpiblocks_flowmix',
    'fieldable' => TRUE,
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'entity keys' => array(
      'id' => 'fid',
      'machine_name' => 'machine_name',
    ),

    'bundles' => array(
      'flowmix' => array(),
    ),

    'view modes' => array(
      'minimal' => array(
        'label' => t('Minimal'),
        'custom settings' => FALSE,
      ),
      'full' => array(
        'label' => t('Full'),
        'custom settings' => TRUE,
      ),
    ),

    // Since the entity hasn't the same name of the module NEED THIS if you want VIEW intégration
    'module'=> 'dpiblocks',
    'access callback' => 'flowmix_access_admin_callback',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/flowmix',
      // OPTIONAL- See "Customize the User Interface" below if you set.
      'file' => 'includes/flowmix.admin.inc',
    ),
  );

  return $entities;
}

/**
 * Implement the access_admin_callback() for the remote HTML
 *
 * @return boolean
 */
if (!function_exists('remotehtml_access_admin_callback')) {
  function remotehtml_access_admin_callback() {
    return TRUE;
  }
}

/**
 * Implement the access_admin_callback() for the custom content
 *
 * @return boolean
 */
if (!function_exists('custom_content_access_admin_callback')) {
  function custom_content_access_admin_callback() {
    return TRUE;
  }
}

/**
 * Implement the access_admin_callback() for the flowmix
 *
 * @return boolean
 */
if (!function_exists('flowmix_access_admin_callback')) {
  function flowmix_access_admin_callback() {
    return TRUE;
  }
}

/**
 * Load a remotehtml entity with this id
 *
 * @param $tid
 * @param $reset
 */
if (!function_exists('remotehtml_load')) {
  function remotehtml_load($tid = NULL, $reset = FALSE) {
    $tids = (isset ($tid) ? array($tid) : array());

    $remotehtml = remotehtml_load_multiple($tids, $reset);

    return $remotehtml ? reset($remotehtml) : FALSE;
  }
}

/**
 * Load a custom content entity with this id
 *
 * @param $tid
 * @param $reset
 */
if (!function_exists('custom_content_load')) {
  function custom_content_load($tid = NULL, $reset = FALSE) {
    $tids = (isset ($tid) ? array($tid) : array());

    $custom_content = custom_content_load_multiple($tids, $reset);

    return $custom_content ? reset($custom_content) : FALSE;
  }
}

/**
 * Load a flowmix entity with this id
 *
 * @param $tid
 * @param $reset
 */
if (!function_exists('flowmix_load')) {
  function flowmix_load($tid = NULL, $reset = FALSE) {
    $tids = (isset ($tid) ? array($tid) : array());

    $flowmix = flowmix_load_multiple($tids, $reset);

    return $flowmix ? reset($flowmix) : FALSE;
  }
}

/**
 * Retrieve multiple remotehtml entities
 *
 * @param $tids
 *  Their ids
 * @param $conditions
 * @param $reset
 *
 * @return object
 *  Return the entities
 */
if (!function_exists('remotehtml_load_multiple')) {
  function remotehtml_load_multiple($tids = array(), $conditions = array(), $reset = FALSE) {
    return entity_load('remotehtml', $tids, $conditions, $reset);
  }
}

/**
 * Retrieve multiple custom content entities
 *
 * @param $tids
 *  Their ids
 * @param $conditions
 * @param $reset
 *
 * @return object
 *  Return the entities
 */
if (!function_exists('custom_content_load_multiple')) {
  function custom_content_load_multiple($tids = array(), $conditions = array(), $reset = FALSE) {
    return entity_load('custom_content', $tids, $conditions, $reset);
  }
}

/**
 * Retrieve multiple flowmix entities
 *
 * @param $tids
 *  Their ids
 * @param $conditions
 * @param $reset
 *
 * @return object
 *  Return the entities
 */
if (!function_exists('flowmix_load_multiple')) {
  function flowmix_load_multiple($tids = array(), $conditions = array(), $reset = FALSE) {
    return entity_load('flowmix', $tids, $conditions, $reset);
  }
}

/**
 * Return the name of the given remote HTML entity
 *
 * @param $remotehtml
 *  The given entities (remote HTML)
 */
if (!function_exists('remotehtml_page_title')) {
  function remotehtml_page_title($remotehtml) {
    return $remotehtml->machine_name;
  }
}

/**
 * Return the name of the given custom content entity
 *
 * @param $custom_content
 *  The given entities (custom_content)
 */
if (!function_exists('custom_content_page_title')) {
  function custom_content_page_title($custom_content) {
    return $custom_content->title;
  }
}

/**
 * Return the name of the given FlowMix entity
 *
 * @param $remotehtml
 *  The given entities (remote HTML)
 */
if (!function_exists('flowmix_page_title')) {
  function flowmix_page_title($flowmix) {
    return $flowmix->machine_name;
  }
}

/**
 * Delete single remote HTML entity.
 *
 * @param $remotehtml
 *  The entity
 */
if (!function_exists('remotehtml_delete')) {
  function remotehtml_delete($remotehtml) {
    entity_delete('remotehtml', entity_id('remotehtml' ,$remotehtml));
  }
}

/**
 * Delete single custom content entity.
 *
 * @param $custom_content
 *  The entity
 */
if (!function_exists('custom_content_delete')) {
  function custom_content_delete($custom_content) {
    entity_delete('custom_content', entity_id('custom_content' ,$custom_content));
  }
}

/**
 * Delete single FlowMix entity.
 *
 * @param $flowmix
 *  The entity
 */
if (!function_exists('flowmix_delete')) {
  function flowmix_delete($flowmix) {
    entity_delete('flowmix', entity_id('flowmix', $flowmix));
  }
}

/**
 * Delete multiple remote HTML entities.
 *
 * @param $remotehtml_ids
 *  Their ids
 */
if (!function_exists('remotehtml_delete_multiple')) {
  function remotehtml_delete_multiple($remotehtml_ids) {
    entity_delete_multiple('remotehtml', $remotehtml_ids);
  }
}

/**
 * Delete multiple custom content entities.
 *
 * @param $custom_content_ids
 *  Their ids
 */
if (!function_exists('custom_content_delete_multiple')) {
  function custom_content_delete_multiple($custom_content_ids) {
    entity_delete_multiple('custom_content', $custom_content_ids);
  }
}

/**
 * Delete multiple FlowMix entities.
 *
 * @param $flowmix_ids
 *  Their ids
 */
if (!function_exists('flowmix_delete_multiple')) {
  function flowmix_delete_multiple($flowmix_ids) {
    entity_delete_multiple('flowmix', $flowmix_ids);
  }
}

/**
 * Save a remote HTML entity
 *
 * @param $remotehtml
 */
if (!function_exists('remotehtml_save')) {
  function remotehtml_save(&$remotehtml) {
    return entity_save('remotehtml', $remotehtml);
  }
}

/**
 * Save a custom content entity
 *
 * @param $custom_content
 */
if (!function_exists('custom_content_save')) {
  function custom_content_save(&$custom_content) {
    return entity_save('custom_content', $custom_content);
  }
}

/**
 * Save a flowmix entity
 *
 * @param $flowmix
 */
if (!function_exists('flowmix_save')) {
  function flowmix_save(&$flowmix) {
    return entity_save('flowmix', $flowmix);
  }
}

/**
 * Generate an array for rendering the given remote HTML entity
 *
 * @param $remotehtml
 *  The given entities
 * @param $view_mode
 */
if (!function_exists('remotehtml_view')) {
  function remotehtml_view($remotehtml, $view_mode = 'full') {
    return $remotehtml->view();
  }
}

/**
 * Generate an array for rendering the given custom content entity
 *
 * @param $custom_content
 *  The given entities
 * @param $view_mode
 */
if (!function_exists('custom_content_view')) {
  function custom_content_view($custom_content, $view_mode = 'full') {
    return $custom_content->view();
  }
}

/**
 * Generate an array for rendering the given flowmix entity
 *
 * @param $flowmix
 *  The given entities
 * @param $view_mode
 */
if (!function_exists('flowmix_view')) {
  function flowmix_view($flowmix, $view_mode = 'full') {
    return $flowmix->view();
  }
}

/**
 * @param $remotehtml
 * @param $view_mode
 */
if (!function_exists('remotehtml_page_view')) {
  function remotehtml_page_view($remotehtml, $view_mode = 'full') {
    $remotehtml->content = array();

    // Build fields content.
    field_attach_prepare_view('remotehtml', array($remotehtml->tid => $remotehtml), $view_mode);
    entity_prepare_view('remotehtml', array($remotehtml->tid => $remotehtml));
    $remotehtml->content += field_attach_view('remotehtml', $remotehtml, $view_mode);

    return $remotehtml->content;
  }
}

/**
 * @param $custom_content
 * @param $view_mode
 */
if (!function_exists('custom_content_page_view')) {
  function custom_content_page_view($custom_content, $view_mode = 'full') {
    $custom_content->content = array();

    // Build fields content.
    field_attach_prepare_view('custom_content', array($custom_content->tid => $custom_content), $view_mode);
    entity_prepare_view('custom_content', array($custom_content->tid => $custom_content));
    $custom_content->content += field_attach_view('custom_content', $custom_content, $view_mode);

    return $custom_content->content;
  }
}

/**
 * @param $flowmix
 * @param $view_mode
 */
if (!function_exists('flowmix_page_view')) {
  function flowmix_page_view($flowmix, $view_mode = 'full') {
    $flowmix->content = array();

    // Build fields content.
    field_attach_prepare_view('flowmix', array($flowmix->tid => $flowmix), $view_mode);
    entity_prepare_view('flowmix', array($flowmix->tid => $flowmix));
    $flowmix->content += field_attach_view('flowmix', $flowmix, $view_mode);

    return $flowmix->content;
  }
}

/**
 * This function get all Remote HTML blocks from DB
 *
 * @return $allRemotehtml
 *  Return all Remote HTML blocks
 */
function dpiblocks_remotehtml_get_all() {
  $results = array();
  $db_results = db_select('dpiblocks_remotehtml', 'dr')
    ->fields('dr')
    ->execute();
  foreach ($db_results as $db_result) {
    $results[$db_result->rid] = $db_result;
  }
  return $results;
}

/**
 * This function get all Remote HTML blocks from DB
 *
 * @return $allRemotehtml
 *  Return all Remote HTML blocks
 */
function dpiblocks_flowmix_get_all() {
  $results = array();
  $db_results = db_select('dpiblocks_flowmix', 'df')
    ->fields('df')
    ->execute();
  foreach ($db_results as $db_result) {
    $results[$db_result->fid] = $db_result;
  }
  return $results;
}
