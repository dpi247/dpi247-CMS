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
function dpiservices_dpi_packages_retrieve($nid, $product_name = null) {
  global $present_definition;
  $present_definition = array ();

  $node = node_load ( $nid );

  $packages_return = array (
    'nid' => $node->nid,
    'comment' => $node->comment,
    'title' => $node->title,
    'creationDate' => $node->created,
    'pubDate' => dpiservices_packages_get_field_element_by_search_value ( 'node', $node, 'field_editorialpublicationdate', 'value' ),
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
    'uri' => dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/node/' . $nid,
    'destination' => dpiservices_dpi_packages_get_all_element_from_item_from_node ( $node, 'field_destinations' ),
    'mainSection' => '@todo',
    'rightOfReplyTitle' => "",
    'rightofReplyBody' => "",
    'object_relations' => array (
      dpiservices_dpi_packages_get_all_related_object_from_node ( $node )
    ),
    'object_definitions' => array (
      dpiservices_dpi_packages_get_all_related_object_definitions_from_node ( $node , $product_name )
    ),
    'taxonomy' => array (
      'concepts' => array (
        dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid ( 'node', $node, 'field_concepts' )
      ),
      'entities' => array (
        dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid ( 'node', $node, 'field_entities' )
      ),
      'freetags' => array (
        dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid ( 'node', $node, 'field_freetags' )
      ),
      'IPTC' => array (
        dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid ( 'node', $node, 'field_iptc' )
      ),
      'locations' => array (
        dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid ( 'node', $node, 'field_locations' )
      ),
      'persons' => array (
        dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid ( 'node', $node, 'field_persons' )
      )
    )
  );
  $present_definition = null;
  return $packages_return;
}

/**
 * This function is use to create a table of destination for a node
 *
 * @param node $node
 * @param string $field_name
 * @return array
 */
function dpiservices_dpi_packages_get_all_element_from_item_from_node($node, $field_name) {
  $table_value = array ();
  foreach ( field_get_items ( 'node', $node, $field_name ) as $items ) {
    $element = array ();
    foreach ( $items as $k => $v ) {
      if ($k == 'tid') {
        $var = taxonomy_term_load ( $v );
        $element ["code"] = $v;
        $element ['name'] = $var->name;
      }
    }
    $table_value [] = $element;
  }
  return $table_value;
}

/**
 * This function is use to create a table with link and image object
 *
 * @param node $node
 * @return array
 */
function dpiservices_dpi_packages_get_all_related_object_from_node($node) {
  $table_value = array ();
  
  foreach ( field_get_items ( 'node', $node, 'field_embededobjects' ) as $item ) {
    $table_value [] = dpiservices_dpi_packages_generate_related_object ( $item ['sid'], $node->nid, $item ['inline'] );
  }
  
  foreach ( field_get_items ( 'node', $node, 'field_linkslists' ) as $item ) {
    $table_value [] = dpiservices_dpi_packages_generate_related_object ( $item ['sid'], $node->nid, $item ['inline'] );
  }
  
  return $table_value;
}

/**
 * This function is use to create a table of related object with type, relation and URI
 *
 * @param integer $sid
 * @param integer $nid
 * @param string $inline
 * @return array
 */
function dpiservices_dpi_packages_generate_related_object($sid, $nid, $inline) {
  $atom = scald_atom_load ( $sid );

  $formatted_atom = array (
    'object_type' => $atom->type,
    'relation_type' => $atom->provider,
    'uri' => dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/atom/' . $sid
  );

  return $formatted_atom;
}

/**
 * This function is use to create a table with all related object informations 
 * 
 * @param node $node
 * @return array
 */
function dpiservices_dpi_packages_get_all_related_object_definitions_from_node($node, $product_name = NULL) {
  $table_value = array ();
  foreach ( field_get_items ( 'node', $node, 'field_embededobjects' ) as $item ) {
    if(($var = dpiservices_dpi_packages_generate_related_object_definitions ( $item ['sid'], $node->nid, $item ['inline'], $product_name )) != FALSE)
      $table_value [] = $var;
  }
  return $table_value;
}


/**
 * This function is use to create a table with information about one related object
 *
 * @param integer $sid
 * @param integer $nid
 * @param string $inline
 * @return array:false
 */
function dpiservices_dpi_packages_generate_related_object_definitions($sid, $nid, $inline, $product_name = null) {
  $atom = scald_atom_load ( $sid );
  $all_crop_informations = dpiservices_packages_generate_crop_informations ( $sid, $nid, $product_name );
  
  global $present_definition;
  if (! in_array ( dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/atom/' . $sid, $present_definition )) {
    $present_definition[] = dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/atom/' . $sid;
    $val_returned = array (
      dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/atom/' . $sid => array (
        'nid' => $nid,
        'type' => $atom->type,
        'caption' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'field_caption', 'value' ),
        'credit' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'field_copyright', 'value' ),
        'width' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'scald_thumbnail', 'width' ),
        'height' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'scald_thumbnail', 'height' ),
        'crops' => array (
          $all_crop_informations [0]
        ),
        'crop_definitions' => array (
          $all_crop_informations [1]
        ),
        'externalReference' => dpiproperties_load ( $nid )->external_reference,
        'filesize' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'scald_thumbnail', 'filesize' ),
        'inline_object' => $inline,
        'url' => dpiservices_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'scald_thumbnail', 'uri' ),
        'uri' => dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/atom/' . $sid
      )
    );
  }
  
  return (isset($val_returned)) ? $val_returned : FALSE;
}

/**
 * This function is use to create a table with information for a taxonomy term.
 * It return name and URI
 *
 * @param string $type
 * @param object $node
 * @param string $field_name
 * @return array
 */
function dpiservices_dpi_packages_generate_taxonomy_name_and_uri_by_tid($type, $node, $field_name) {
  $term = array ();
  foreach ( field_get_items ( $type, $node, $field_name ) as $item ) {
    $term_load = taxonomy_term_load ( $item ['tid'] );
    $term [] = array (
      'name' => $term_load->name,
      'uri' => dpi_variable_get ( 'dpicommons_product', '' ) . '.' . dpi_variable_get ( 'dpicommons_environment', '' ) . '/taxonomy/term/' . $term_load->tid
    );
  }
  return $term;
}
