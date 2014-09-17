<?php
/**
 * @author lba
 */
module_load_include('inc', 'dpiservices', 'dpiservices.api');

/**
 * This function return a json from nid
 *
 * @param integer $nid
 * @return array()
 */
function dpiservices_wally_package_retrieve($nid) {
  $node = node_load ( $nid );

  $packages_return = array (
    'nid' => $node->nid,
    'comment' => $node->comment,
    'title' => $node->title,
    'creationDate' => $node->created,
    'pubDate' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_editorialpublicationdate', 'value' ),
    'editorialUpdate' => $node->revision_timestamp,
    'updateDate' => $node->changed,
    'type' => $node->type,
    'layout' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_destinations', 'layout' ),
    'chapo' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_textchapo', 'value' ),
    'freeaccess' => (dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_restrictedaccess', 'value' )) ? "true" : "false",
    'body' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_textbody', 'value' ),
    'foretitle' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_textforetitle', 'value' ),
    'subtitle' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_textsubtitle', 'value' ),
    'barette' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_textbarette', 'value' ),
    'byline' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_byline', 'value' ),
    'authors' => dpiservices_packages_get_one_element_from_item_from_node ( 'node', $node, 'field_authors', 'tid', 'name' ),
    'qualities' => '@todo',
    'url' => drupal_get_path_alias ( 'node/' . $nid ),
    'relatedObjects' => array (
      dpiservices_wally_packages_get_all_related_object_from_node ( $node )
    ),
  );
  
  return $packages_return;
}

/**
 * This function get inforamtion about related object (linklist - image)
 *
 * @param node $node
 * @return array
 */
function dpiservices_wally_packages_get_all_related_object_from_node($node) {
  $table_value = array ();
  
  foreach ( field_get_items ( 'node', $node, 'field_embededobjects' ) as $item ) {
    $table_value [] = dpiservices_wally_packages_generate_related_object ( $item ['sid'], $node->nid, $item ['inline'] );
  }
  
  foreach ( field_get_items ( 'node', $node, 'field_linkslists' ) as $item ) {
    $table_value [] = dpiservices_wally_packages_generate_related_object ( $item ['sid'], $node->nid, $item ['inline'] );
  }
  
  return $table_value;
}

/**
 * This function get information about one related object and return an array with
 * nid, type, caption, credit and crop informations
 * 
 * @param integer $sid
 * @param integer $nid
 * @param integer $inline
 * @return array
 */
function dpiservices_wally_packages_generate_related_object($sid, $nid, $inline) {
  $atom = scald_atom_load ( $sid );
  $all_crop_informations = dpiservices_packages_generate_crop_informations ( $sid, $nid );

  $formatted_atom = array (
    'nid' => $nid,
    'type' => $atom->type,
    'caption' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'field_caption', 'value' ),
    'credit' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'field_copyright', 'value' ),
    'crop' => $all_crop_informations[1],
  );

  return $formatted_atom;
}