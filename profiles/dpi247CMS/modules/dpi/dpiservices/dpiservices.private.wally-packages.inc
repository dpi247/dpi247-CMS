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
function dpiservices_wally_package_retrieve($nid, $sid = null, $target = null) {
  return dpicache_get_cache_entry ( 'dpiservices_wallypackages_render', $nid, $sid = null, $target = null);
}

/**
 * This function process json array
 *
 * @param integer $nid
 * @param string $sid
 * @param string $target
 * @return array
 */
function dpiservices_wally_package_retrieve_process($nid, $sid = null, $target = null) {
  $node = node_load ( $nid );

  $packages_return = array (
    'nid' => $node->nid,
    'comment' => $node->comment,
    'title' => $node->title,
    'creationDate' => $node->created,
    'pubDate' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_editorialpublicationdate', 'value' ),
    'editorialUpdate' => $node->revision_timestamp,
    'updateDate' => $node->changed,
    'type' => $node->type,
    'layout' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_destinations', 'layout' ),
    'chapo' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_textchapo', 'value' ),
    'freeaccess' => (dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_restrictedaccess', 'value' )) ? "true" : "false",
    'body' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_textbody', 'value' ),
    'foretitle' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_textforetitle', 'value' ),
    'subtitle' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_textsubtitle', 'value' ),
    'barette' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_textbarette', 'value' ),
    'byline' => dpiservices_api_packages_get_field_element_by_search_value ( 'node', $node, 'field_byline', 'value' ),
    'authors' => dpiservices_api_packages_get_one_element_from_item_from_node ( 'node', $node, 'field_authors', 'tid', 'name' ),
    'qualities' => array(),
    'url' => drupal_get_path_alias ( 'node/' . $nid ),
    'mainDestination' => $target,
    'mainSection' => $sid,
    'relatedObjects' => dpiservices_wally_packages_get_all_related_object_from_node ( $node ),
  );

  drupal_alter('dpiservices_wally_package_info', $packages_return);

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
  if(count($table_value)==0){
    $table_value=dpiservices_wally_packages_get_default_related_object_from_node($node);
  }

  foreach ( field_get_items ( 'node', $node, 'field_linkslists' ) as $item ) {
    $table_value [] = dpiservices_wally_packages_generate_related_object ( $item ['sid'], $node->nid, $item ['inline'] );
  }



  return $table_value;
}

function dpiservices_wally_packages_get_default_related_object_from_node($node){

  $image=array(
    'nid'=>rand(),
    'type'=>'image',
    'caption'=>FALSE,
    'credit'=>FALSE,
    

  );
  $background_index=rand(1,14);
  $background_folder="public://default_background/";
  $background_path=$background_folder.$background_index.".jpg";
  foreach(image_styles() as $preset_name=>$preset_info){
    $image['crop'][$preset_name]['url']=image_style_url($preset_name, $background_path);
  }
  return $image;

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
  if($atom = scald_atom_load ( $sid )){
    $all_crop_informations = dpiservices_api_packages_generate_crop_links ( $sid, $nid, $atom );

    $formatted_atom = array (
      'nid' => $atom->sid,
      'type' => $atom->type,
    );

    if($atom->type == "image"){
      $formatted_atom = $formatted_atom + array(
        'caption' => dpiservices_api_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'field_caption', 'value' ),
        'credit' => dpiservices_api_packages_get_field_element_by_search_value ( 'scald_atom', $atom, 'field_copyright', 'value' ),
        'crop' => $all_crop_informations,
      );
    }else{
      $formatted_atom = $formatted_atom + array(
        'title' => $atom->title,
        'links' => dpiservices_wally_get_atoms_links_list($atom),
      );
    }

    return $formatted_atom;
  }else{
    return null;
  }
}

/**
 * This function return all links info for an atom sclad_linklist
 *
 * @param scald_atom $atom
 * @return array
 */
function dpiservices_wally_get_atoms_links_list($atom){
  $return = array();
  foreach(field_get_items('scald_atom', $atom, 'field_linkitems') as $link){
    $return[] = array(
      'nid' => $atom->sid,
      'type' => $atom->type,
      'path' => null,
      'title' => $link['title'],
      'url' => $link['url'],
    );
  }
  return $return;
}