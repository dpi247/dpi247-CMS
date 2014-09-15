<?php

/**
 * This function return a json from nid
 * 
 * @param integer $nid
 * @return array()
 */
function dpiservices_dpijson_packages_retrieve($nid) {
  $node = node_load($nid);
  
  $packages_return = array(
    'nid' => $node->nid,
    'comment' => $node->comment,
    'title' => $node->title,
    'creationDate' => $node->created,
    'pubDate' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_editorialpublicationdate', 'value'),
    'type' => $node->type,
    'layout' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_destinations', 'layout'),
    'chapo' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_textchapo', 'value'),
    'freeaccess' => (dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_restrictedaccess', 'value'))? "true" : "false",
    'body' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_textbody', 'value'),
    'foretitle' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_textforetitle', 'value'),
    'subtitle' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_textsubtitle', 'value'),
    'barette' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_textbarette', 'value'),
    'byline' => dpiservices_dpijson_packages_get_field_element_by_search_value('node',$node, 'field_byline', 'value'),
    'authors' => dpiservices_dpijson_packages_get_one_element_from_item_from_node('node', $node, 'field_authors', 'tid', 'name'),
    'qualities' => '[@todo]',
    'url' =>  drupal_get_path_alias('node/'.$nid),
    'uri' => dpi_variable_get('dpicommons_product', '').'.'.dpi_variable_get('dpicommons_environment', '').'/node/'.$nid,
    'destination' => dpiservices_dpijson_packages_get_all_element_from_item_from_node($node, 'field_destinations'),
    'object_relations' => array(
      dpiservices_dpijson_packages_get_all_related_object_from_node($node)
    ),
    'object_definitions' => array(
      dpiservices_dpijson_packages_get_all_related_object_definitions_from_node($node)
    ),
    'object_properties' => array(
      dpiservices_dpijson_packages_get_all_related_object_properties_from_node($node)
    ),
    'taxonomy' => array(
        'concepts' => array(
          dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid('node', $node, 'field_concepts')
        ),
        'entities' => array(
          dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid('node', $node, 'field_entities')
        ),
        'freetags' => array(
          dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid('node', $node, 'field_freetags')
        ),
        'IPTC' => array(
          dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid('node', $node, 'field_iptc')
        ),
        'locations' => array(
          dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid('node', $node, 'field_locations')
        ),
        'persons' => array(
          dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid('node', $node, 'field_persons')
        ),
      ),
  );  
  
  return $packages_return;
}

/**
 * 
 * 
 * @param unknown $node
 * @param unknown $field_name
 * @param unknown $search_value
 * @return mixed
 */
function dpiservices_dpijson_packages_get_field_element_by_search_value($type, $node, $field_name, $search_value){
  $element = current(field_get_items($type, $node, $field_name));
  return $element[$search_value];
}

/**
 * 
 * 
 * @param unknown $node
 * @param unknown $field_name
 * @param unknown $search_value
 * @param unknown $get_item
 * @return multitype:NULL
 */
function dpiservices_dpijson_packages_get_one_element_from_item_from_node($type, $node, $field_name, $search_value, $get_item){
  $authors = array();
  foreach (field_get_items($type, $node, $field_name) as $items){
    $authors[] = taxonomy_term_load($items[$search_value])->$get_item;
  }
  return $authors;
}

/**
 * 
 * 
 * @param unknown $node
 * @param unknown $field_name
 * @return multitype:multitype:unknown
 */
function dpiservices_dpijson_packages_get_all_element_from_item_from_node($node, $field_name){
  $table_value = array();
  foreach (field_get_items('node', $node, $field_name) as $items){
    $element = array();
    foreach ($items as $key => $value){
      $element[$key] = $value;
    }
    $table_value[] = $element;
  }
  return $table_value;
}

/**
 * 
 * 
 * @param unknown $node
 * @return multitype:multitype:string mixed unknown multitype:mixed  multitype:unknown  NULL Ambigous <boolean, mixed> multitype:Ambigous <multitype:string , multitype:multitype:unknown Ambigous <>  >
 */
function dpiservices_dpijson_packages_get_all_related_object_from_node($node){
  $table_value = array();
  foreach (field_get_items('node', $node, 'field_embededobjects') as $item){
    $table_value[] = dpiservices_dpijson_packages_generate_related_object($item['sid'], $node->nid, $item['inline']);
  }
  return $table_value;
}

function dpiservices_dpijson_packages_get_all_related_object_definitions_from_node($node){
  $table_value = array();
  foreach (field_get_items('node', $node, 'field_embededobjects') as $item){
    $table_value[] = dpiservices_dpijson_packages_generate_related_object_definitions($item['sid'], $node->nid, $item['inline']);
  }
  return $table_value;
}

function dpiservices_dpijson_packages_get_all_related_object_properties_from_node($node){
  $table_value = array();
  foreach (field_get_items('node', $node, 'field_embededobjects') as $item){
    $table_value[] = dpiservices_dpijson_packages_generate_related_object_properties($item['sid'], $node->nid, $item['inline']);
  }
  return $table_value;
}

/**
 * 
 * 
 * @param unknown $sid
 * @param unknown $nid
 * @param unknown $inline
 * @return multitype:string mixed unknown multitype:mixed  multitype:unknown  NULL Ambigous <boolean, mixed> multitype:Ambigous <multitype:string , multitype:multitype:unknown Ambigous <>  >
 */
function dpiservices_dpijson_packages_generate_related_object($sid, $nid, $inline){
  $atom = scald_atom_load($sid);  

  $formatted_atom = array(
    'type' => $atom->type,    
    'relation_type' => $atom->provider,
    'uri' => dpi_variable_get('dpicommons_product', '').'.'.dpi_variable_get('dpicommons_environment', '').'/atom/'.$sid,
  );
  
  return $formatted_atom;
}

function dpiservices_dpijson_packages_generate_related_object_definitions($sid, $nid, $inline){
  $atom = scald_atom_load($sid);
  $all_crop_informations = dpiservices_dpijson_packages_generate_crop_informations($sid, $nid);
  
  $val_returned = array(
    dpi_variable_get('dpicommons_product', '').'.'.dpi_variable_get('dpicommons_environment', '').'/atom/'.$sid => array(
      'preset value'
    )
  );
  return $val_returned;
}

/**
 * 
 * 
 * @param unknown $sid
 * @param unknown $nid
 * @param unknown $inline
 * @return multitype:multitype:string mixed unknown multitype:unknown  multitype:Ambigous <multitype:string>  NULL
 */
function dpiservices_dpijson_packages_generate_related_object_properties($sid, $nid, $inline){
  $atom = scald_atom_load($sid);
  $all_crop_informations = dpiservices_dpijson_packages_generate_crop_informations($sid, $nid);
  
  $val_returned = array(
    dpi_variable_get('dpicommons_product', '').'.'.dpi_variable_get('dpicommons_environment', '').'/atom/'.$sid => array(
      'type' => $atom->type,
      'caption' => dpiservices_dpijson_packages_get_field_element_by_search_value('scald_atom', $atom, 'field_caption', 'value'),
      'credit' => dpiservices_dpijson_packages_get_field_element_by_search_value('scald_atom', $atom, 'field_copyright', 'value'),
      'width' => dpiservices_dpijson_packages_get_field_element_by_search_value('scald_atom', $atom, 'scald_thumbnail', 'width'),
      'height' => dpiservices_dpijson_packages_get_field_element_by_search_value('scald_atom', $atom, 'scald_thumbnail', 'height'),
      //crop
      'crops' => array(
        $all_crop_informations[0]
      ),
      'crop_definitions' => array(
        $all_crop_informations[1]
      ),
      'externalReference' => dpiproperties_load($nid)->external_reference,
      'filesize' => dpiservices_dpijson_packages_get_field_element_by_search_value('scald_atom', $atom, 'scald_thumbnail', 'filesize'),
      'inline_object' => $inline,
      'url' => dpiservices_dpijson_packages_get_field_element_by_search_value('scald_atom', $atom, 'scald_thumbnail', 'uri'),
      'uri' => dpi_variable_get('dpicommons_product', '').'.'.dpi_variable_get('dpicommons_environment', '').'/atom/'.$sid,
    )
  );
  
  return $val_returned;
}

/**
 * 
 * 
 * @param unknown $sid
 * @param unknown $nid
 * @return multitype:multitype:string  multitype:multitype:unknown Ambigous <>
 */
function dpiservices_dpijson_packages_generate_crop_informations($sid, $nid){
  $gen_value = array();
  $crop_value = array();
  $crop_definition_value = array();
  
  /* First, get crop information */
  foreach (_dpicontenttypes_get_saved_cropings_for_entity_and_atom('node', $nid, $sid) as $key => $value){
    $crop_value['_'.$value[0].'_'.$value[1].'_'.$value[2].'_'.$value[3]] = $key;
    $crop_definition_value[$key] = array(
      'start_width' => $value[0],
      'start_height' => $value[1],
      'width' => $value[2],
      'height' => $value[3],
    );    
  }
  
  /* Create table with 2 values */
  $gen_value[] = $crop_value;
  $gen_value[] = $crop_definition_value;
  
  return $gen_value;
}

/**
 * 
 * 
 * @param unknown $type
 * @param unknown $node
 * @param unknown $field_name
 * @return multitype:multitype:string NULL
 */
function dpiservices_dpijson_packages_generate_taxonomy_name_and_uri_by_tid($type, $node, $field_name){
  $term = array();  
  foreach (field_get_items($type, $node, $field_name) as $item){
    $term_load = taxonomy_term_load($item['tid']);
    $term[] = array(
      'name' => $term_load->name,
      'uri' => dpi_variable_get('dpicommons_product', '').'.'.dpi_variable_get('dpicommons_environment', '').'/taxonomy/term/'.$term_load->tid,
    );
  }
  return $term;
}



