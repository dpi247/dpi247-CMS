<?php

/**
 * Transform a node into a DPIXML
 *
 * @param object $node
 *
 * @return
 *   The created DOM object and an array of files to add in the zip
 */
function dpiexport_node_to_dpixml($node, &$files_to_zip) {
  $node_wrapper = entity_metadata_wrapper('node', $node);
  $dpiproperties = dpiproperties_load_by_type_and_id('node', $node->nid);

  $dom = new DomDocument('1.0', 'UTF-8');

  $packages_node = $dom->appendChild($dom->createElement('Packages'));
  $packages_node->setAttribute('xmlns', 'http://rossel.audaxis.com');
  $packages_node->setAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
  $packages_node->setAttribute('xsi:schemaLocation', 'http://rossel.audaxis.com http://rossel.audaxis.com/xsd/packages.1.21.xsd');

  $package_node = $packages_node->appendChild($dom->createElement('Package'));
  // Package Type
  $package_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', 'ArticlePackageType');

  // Set package fields
  dpiexport_node_to_dpixml_fill_package($dom, $package_node, $node, $node_wrapper, $dpiproperties);

  // Set main story
  dpiexport_node_to_dpixml_fill_article_package_type($dom, $package_node, $node, $node_wrapper, $dpiproperties);

  return $dom;

  if (isset($package_array['EmbeddedContent'])){
    $embeddedcontent_node = $package_node->appendChild($valid_dom->createElement('EmbeddedContent'));
    if (isset($package_array['EmbeddedContent']['EmbeddedObjects']['Object'])){
      $embeddedobjects_node = $embeddedcontent_node->appendChild($valid_dom->createElement('EmbeddedObjects'));
      foreach ($package_array['EmbeddedContent']['EmbeddedObjects']['Object'] as $object_array){
        $object_node = $embeddedobjects_node->appendChild($valid_dom->createElement('Object'));
        $object_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $object_array['type']);
        _wallytoolbox_arraytowallyxml_fillObjectType($valid_dom, $object_node, $object_array);
        switch ($object_array['type']){
          case 'TextObjectType' :
            _wallytoolbox_arraytowallyxml_fillTextObjectType($valid_dom, $object_node, $object_array);
            break;
          case 'AudioObjectType':
            _wallytoolbox_arraytowallyxml_fillAudioObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
            break;
          case 'VideoObjectType':
            _wallytoolbox_arraytowallyxml_fillVideoObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
            break;
          case 'PhotoObjectType':
            _wallytoolbox_arraytowallyxml_fillPhotoObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
            break;
          case 'PollObjectType':
            _wallytoolbox_arraytowallyxml_fillPollObjectType($valid_dom, $object_node, $object_array);
            break;
        }
      }
    }
    if (isset($package_array['EmbeddedContent']['EmbeddedObjects']['Link'])){
      if (!$embeddedobjects_node) {
        $embeddedobjects_node = $embeddedcontent_node->appendChild($valid_dom->createElement('EmbeddedObjects'));
      }
      foreach ($package_array['EmbeddedContent']['EmbeddedObjects']['Link'] as $link_array){
        $link_node = $embeddedobjects_node->appendChild($valid_dom->createElement('Link'));
        if (isset($link_array['EmbeddedObjectsOrder']) && $link_array['EmbeddedObjectsOrder'] != NULL && $link_array['EmbeddedObjectsOrder'] != ''){
          $link_node->setAttribute('EmbeddedObjectsOrder', $link_array['EmbeddedObjectsOrder']['value']);
        }
        $link_node->appendChild($valid_dom->createElement('ExternalReference', $link_array['ExternalReference']['value']));
        $link_node->appendChild($valid_dom->createElement('Title', $link_array['Title']['value']));
        $link_node->appendChild($valid_dom->createElement('Description', $link_array['Description']['value']));
        $link_node->appendChild($valid_dom->createElement('URI', $link_array['URI']['value']));
      }
    }
    if (isset($package_array['EmbeddedContent']['LinksLists']['LinksList'])){
      $linkslists_node = $embeddedcontent_node->appendChild($valid_dom->createElement('LinksLists'));
      foreach ($package_array['EmbeddedContent']['LinksLists']['LinksList'] as $linkslist_array){
        $linkslist_node = $linkslists_node->appendChild($valid_dom->createElement('LinksList'));
        _wallytoolbox_arraytowallyxml_filllinkslist($valid_dom, $linkslist_node, $linkslist_array);
      }
    }
  }

  return $valid_dom;
}

/**
 * Set the package elements
 */
function dpiexport_node_to_dpixml_fill_package($dom, &$package_node, $node, $node_wrapper, $dpiproperties) {
  // @todo : What is Package ID ?
  //$package_node->setAttribute('PackageID', '');
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialpublicationdate', 'value', 'now')) {
    $package_node->setAttribute('PublicationDate', $date);
  }
  $package_node->setAttribute('CreationDate', date('Y-m-d', $node->created).'T'.date('H:i:s', $node->created));
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialupdatedate', 'value', 'now')) {
    $package_node->setAttribute('LastUpdateDate', $date);
  }
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_autopublishdate', 'value')) {
    $package_node->setAttribute('EmbargoDate', $date);
  }
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_autopublishdate', 'value2')) {
    $package_node->setAttribute('UnPublishDate', $date);
  }
  $package_node->setAttribute('Language', $node->language);
  $package_node->setAttribute('Version', $node->vid);
  $package_node->setAttribute('Action', 'CreateUpdate');
  $package_node->setAttribute('ForceLastUpdate', 'true');

  $package_node->appendChild($dom->createElement('ProductID', $dpiproperties->product_id));
  if ($newsroom = dpi_variable_get('ccimapping_newsroom', FALSE)) {
    $package_node->appendChild($dom->createElement('Newsroom', $newsroom));
  }

  if (isset($node_wrapper->field_displaytitle) && $field_value = $node_wrapper->field_displaytitle->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $title_node = $package_node->appendChild($dom->createElement('TextTitle'));
    $title_node->appendChild($dom->createCDATASection($value));
  } else {
    $package_node->appendChild($dom->createElement('PackageTitle', $node->title));
  }

  $package_node->appendChild($dom->createElement('InternalReference', 'Drupal_NID:'.$node->nid));
  $package_node->appendChild($dom->createElement('ExternalReference', $dpiproperties->external_reference));

  $destinations_node = $package_node->appendChild($dom->createElement('Destinations'));
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
  foreach ($node_wrapper->field_destinations->value() as $dest) {
    $term = taxonomy_term_load($dest['tid']);
    $term_path = dpicommons_taxonomy_get_path_by_tid_or_term($term->tid);
    $destination_node = $destinations_node->appendChild($dom->createElement('Destination'));
    $destination_node->appendChild($dom->createElement('DestinationPath', $term_path));
    $destination_node->setAttribute('DestinationRank', $dest['rank']);
    $destination_node->setAttribute('Position', $dest['target']);
    $destination_node->setAttribute('DocumentLayout', $dest['layout']);
  }

  // @todo : Update when channels are handled in dpi247
  $channels_node = $package_node->appendChild($dom->createElement('Channels'));
  $channel_node = $channels_node->appendChild($dom->createElement('Channel'));
  $channel_node->setAttribute('Name', 'web');

  $comments_translate = array(
    0 => 'Disabled',
    1 => 'Read Only',
    2 => 'Read-Write',
  );
  $package_node->appendChild($dom->createElement('CommentsAllowed', $comments_translate[$node->comment]));

  $freeaccess_node = $package_node->appendChild($dom->createElement('FreeAccess'));
  if (isset($node_wrapper->field_restrictedaccess) && $node_wrapper->field_restrictedaccess->value()) {
    $freeaccess_node->setAttribute('xsi:nil', 'false');
  } else {
    $freeaccess_node->setAttribute('xsi:nil', 'true');
  }

  if (isset($node_wrapper->field_autopublish) && $node_wrapper->field_autopublish->value()) {
    $package_node->appendChild($dom->createElement('AutoPublish', 'true'));
  }

  if (isset($node_wrapper->field_packagelayout) && $field_value = $node_wrapper->field_packagelayout->value()) {
    $package_node->appendChild($dom->createElement('PackageLayout', $field_value->name));
  }

  if (isset($node_wrapper->field_externalurl) && $field_value = $node_wrapper->field_externalurl->value()) {
    $package_node->appendChild($dom->createElement('ExternalURI', $field_value['url']));
  } else {
    $externaluri_node = $package_node->appendChild($dom->createElement('ExternalURI'));
    $externaluri_node->setAttribute('xsi:nil', 'true');
  }

  $package_node->appendChild($dom->createElement('Source', $dpiproperties->source));
}

/**
 * Set the article package elements and main story
 */
function dpiexport_node_to_dpixml_fill_article_package_type($dom, &$package_node, $node, $node_wrapper, $dpiproperties) {
  $mainstory_node = $package_node->appendChild($dom->createElement('MainStory'));
  $mainstory_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', 'TextObjectType');

  dpiexport_node_to_dpixml_fill_object_type($dom, $mainstory_node, $node, $node_wrapper, $dpiproperties);
  dpiexport_node_to_dpixml_fill_text_object_type($dom, $mainstory_node, $node, $node_wrapper, $dpiproperties);
}

/**
 * Set the Object elements
 */
function dpiexport_node_to_dpixml_fill_object_type($dom, &$object_node, $node, $node_wrapper, $dpiproperties) {
  // @todo : What is ObjectID ?
  //$object_node->setAttribute('ObjectID', $object_array['ObjectID']);
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialpublicationdate', 'value', 'now')) {
    $object_node->setAttribute('PublicationDate', $date);
  }
  $object_node->setAttribute('CreationDate', date('Y-m-d', $node->created).'T'.date('H:i:s', $node->created));
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialupdatedate', 'value', 'now')) {
    $object_node->setAttribute('LastUpdateDate', $date);
  }

  $object_node->appendChild($dom->createElement('PackageTitle', $node->title));
  $object_node->appendChild($dom->createElement('InternalReference', 'Drupal_NID:'.$node->nid));
  $object_node->appendChild($dom->createElement('ExternalReference', $dpiproperties->external_reference_2));

  if (isset($node_wrapper->field_copyright) && $field_value = $node_wrapper->field_copyright->value()) {
    $object_node->appendChild($dom->createElement('Copyright', $field_value));
  }

  dpiexport_node_to_dpixml_fill_taxonomies($dom, $object_node, $node, $node_wrapper, $dpiproperties);
}

/**
 * Set the Text Object elements
 */
function dpiexport_node_to_dpixml_fill_text_object_type($dom, &$mainstory_node, $node, $node_wrapper, $dpiproperties) {
  if (isset($node_wrapper->field_textbarette) && $field_value = $node_wrapper->field_textbarette->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_barette_node = $mainstory_node->appendChild($dom->createElement('TextBarette'));
    $text_barette_node->appendChild($dom->createCDATASection($value));
  }
  if (isset($node_wrapper->field_textforetitle) && $field_value = $node_wrapper->field_textforetitle->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_foretitle_node = $mainstory_node->appendChild($dom->createElement('TextForeTitle'));
    $text_foretitle_node->appendChild($dom->createCDATASection($value));
  }
  if (isset($node_wrapper->field_displaytitle) && $field_value = $node_wrapper->field_displaytitle->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_title_node = $mainstory_node->appendChild($dom->createElement('TextTitle'));
    $text_title_node->appendChild($dom->createCDATASection($value));
  } else {
    $mainstory_node->appendChild($dom->createElement('TextTitle', $node->title));
  }
  if (isset($node_wrapper->field_textsubtitle) && $field_value = $node_wrapper->field_textsubtitle->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_subtitle_node = $mainstory_node->appendChild($dom->createElement('TextSubTitle'));
    $text_subtitle_node->appendChild($dom->createCDATASection($value));
  }
  if (isset($node_wrapper->field_textchapo) && $field_value = $node_wrapper->field_textchapo->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_chapo_node = $mainstory_node->appendChild($dom->createElement('TextChapo'));
    $text_chapo_node->appendChild($dom->createCDATASection($value));
  }
  if (isset($node_wrapper->field_textbody) && $field_value = $node_wrapper->field_textbody->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_body_node = $mainstory_node->appendChild($dom->createElement('TextBody'));
    $text_body_node->appendChild($dom->createCDATASection($value));
  }
  if (isset($node_wrapper->field_byline) && $field_value = $node_wrapper->field_byline->value()) {
    $value = html_entity_decode($field_value['safe_value'], ENT_COMPAT, 'UTF-8');
    $text_byline_node = $mainstory_node->appendChild($dom->createElement('ByLine'));
    $text_byline_node->appendChild($dom->createCDATASection($value));
  }
}

function dpiexport_node_to_dpixml_fill_taxonomies($dom, &$object_node, $node, $node_wrapper, $dpiproperties) {
  return;
  $temp_object = array();

  $temp_object['Locations']['Location'] = array();
  if ($object->field_locations && is_array($object->field_locations) && !empty($object->field_locations)) {
    foreach ($object->field_locations as $location) {
      if ($location['value']) {
        $temp_object['Locations']['Location'][$location['value']] = $location['value'];
        unset($object->taxonomy[$location['value']]);
      }
    }
  }
  if ($object->field_locations_nodes && is_array($object->field_locations_nodes) && !empty($object->field_locations_nodes)) {
    foreach ($object->field_locations_nodes as $location) {
      $tid = $location->field_locationtaxonomy[0]['value'];
      if (in_array($tid, $temp_object['Locations']['Location']))
        $temp_object['Locations']['Location'][$tid] = wallytoolbox_destinations_flow_set_locationtax($location, $tid);
      else
        $temp_object['Locations']['Location'][] = wallytoolbox_destinations_flow_set_locationtax($location, $tid);
    }
  }
  $temp_object['Entities']['Entity'] = array();
  if ($object->field_entities && is_array($object->field_entities) && !empty($object->field_entities)) {
    foreach ($object->field_entities as $entity) {
      if ($entity['value']) {
        $temp_object['Entities']['Entity'][$entity['value']] = $entity['value'];
        unset($object->taxonomy[$entity['value']]);
      }
    }
  }
  if ($object->field_entities_nodes && is_array($object->field_entities_nodes) && !empty($object->field_entities_nodes)) {
    foreach ($object->field_entities_nodes as $entity) {
      $tid = $entity->field_entity[0]['value'];
      if (in_array($tid, $temp_object['Entities']['Entity']))
        $temp_object['Entities']['Entity'][$tid] = wallytoolbox_destinations_flow_set_entitytax($entity, $tid);
      else
        $temp_object['Entities']['Entity'][] = wallytoolbox_destinations_flow_set_entitytax($entity, $tid);
    }
  }
  $temp_object['Persons']['Person'] = array();
  if ($object->field_persons && is_array($object->field_persons) && !empty($object->field_persons)) {
    foreach ($object->field_persons as $person) {
      if ($person['value']) {
        $temp_object['Persons']['Person'][$person['value']] = $person['value'];
        unset($object->taxonomy[$person['value']]);
      }
    }
  }
  if ($object->field_persons_nodes && is_array($object->field_persons_nodes) && !empty($object->field_persons_nodes)) {
    foreach ($object->field_persons_nodes as $person) {
      $tid = $person->field_persontaxonomy[0]['value'];
      if (in_array($tid, $temp_object['Persons']['Person']))
        $temp_object['Persons']['Person'][$tid] = wallytoolbox_destinations_flow_set_persontax($person, $tid);
      else
        $temp_object['Persons']['Person'][] = wallytoolbox_destinations_flow_set_persontax($person, $tid);
    }
  }
  $temp_object['Authors']['Person'] = array();
  if ($object->field_authors && is_array($object->field_authors) && !empty($object->field_authors)) {
    foreach ($object->field_authors as $author) {
      if ($author['value']) {
        $temp_object['Authors']['Person'][$author['value']] = $author['value'];
        unset($object->taxonomy[$author['value']]);
      }
    }
  }
  if ($object->field_authors_nodes && is_array($object->field_authors_nodes) && !empty($object->field_authors_nodes)) {
    foreach ($object->field_authors_nodes as $author) {
      $tid = $author->field_authortaxonomy[0]['value'];
      if (in_array($tid, $temp_object['Authors']['Person']))
        $temp_object['Authors']['Person'][$tid] = wallytoolbox_destinations_flow_set_persontax($author, $tid);
      else
        $temp_object['Authors']['Person'][] = wallytoolbox_destinations_flow_set_persontax($author, $tid);
    }
  }
  $temp_object['Taxonomy']['Vocabulary'] = array();
  if ($object->taxonomy && is_array($object->taxonomy)) {
    foreach ($object->taxonomy as $taxonomy) {
      $temp_object['Taxonomy']['Vocabulary'][] = wallytoolbox_destinations_flow_set_taxonomytax($taxonomy);
    }
  }

  $temp_taxonomy = wallytoolbox_flow_to_array_struct_term($temp_object);

  return $temp_taxonomy;
}

/**
 * Set the Taxonomy elements
 */
function dpiexport_node_to_dpixml_fill_create_voc_items(&$valid_dom, &$object_node, $taxonomy_array){
  $taxonomy_node = $object_node->appendChild($valid_dom->createElement('Taxonomy'));

  foreach ($taxonomy_array['Vocabulary'] as $vocabulary_array){
    $vocabulary_node = $taxonomy_node->appendChild($valid_dom->createElement('Vocabulary'));
    $vocabulary_node->setAttribute('VocabularyName',$vocabulary_array['VocabularyName']);
    if (isset($vocabulary_array['VocabularyDescription']) & $vocabulary_array['VocabularyDescription'] != NULL & $vocabulary_array['VocabularyDescription'] != ''){
      $vocabulary_node->setAttribute('VocabularyDescription',$vocabulary_array['VocabularyDescription']);
    }
    if (isset($vocabulary_array['VocabularyType']) & $vocabulary_array['VocabularyType'] != NULL & $vocabulary_array['VocabularyType'] != ''){
      $vocabulary_node->setAttribute('VocabularyType',$vocabulary_array['VocabularyType']);
    }
    if (isset($vocabulary_array['Term'])){
      foreach($vocabulary_array['Term'] as $term_array){
        $term_node = $vocabulary_node->appendChild($valid_dom->createElement('Term'));
        $term_type = NULL;
        if (isset($term_array['type']) & $term_array['type'] != NULL & $term_array['type'] != ''){
          $term_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $term_array['type']);
          $term_type = $term_array['type'];
        }
        if (isset($term_array['Name']) & $term_array['Name'] != NULL & $term_array['Name'] != ''){
          $term_node->setAttribute('Name',$term_array['Name']);
        }
        if (isset($term_array['ID']) & $term_array['ID'] != NULL & $term_array['ID'] != ''){
          $term_node->setAttribute('ID',$term_array['ID']);
        }
        if (isset($term_array['ConfidenceScore']) & $term_array['ConfidenceScore'] != NULL & $term_array['ConfidenceScore'] != ''){
          $term_node->setAttribute('ConfidenceScore',$term_array['ConfidenceScore']);
        }
        if (isset($term_array['RelevancyScore']) & $term_array['RelevancyScore'] != NULL & $term_array['RelevancyScore'] != ''){
          $term_node->setAttribute('RelevancyScore',$term_array['RelevancyScore']);
        }
        if (isset($term_array['Variations']) & $term_array['Variations'] != NULL & $term_array['Variations'] != ''){
          $variations_node = $term_node->appendChild($valid_dom->createElement('Variations'));
          if (isset($term_array['Variations']['Variation'])){
            foreach ($term_array['Variations']['Variation'] as $variation_array){
              if ($variation_array['value'] != '' & $variation_array['value'] != NULL){
                $variations_node->appendChild($valid_dom->createElement('Variation', $variation_array['value']));
              }
            }
          }
        }
        switch ($term_type){
          case 'PersonType':
            _wallytoolbox_arraytowallyxml_fillPersonType($valid_dom,$term_node,$term_array);
            break;
          case 'EntityType':
            _wallytoolbox_arraytowallyxml_fillEntityType($valid_dom,$term_node,$term_array);
            break;
          case 'LocationType':
            _wallytoolbox_arraytowallyxml_fillLocationType($valid_dom,$term_node,$term_array);
            break;
        }
      }
    }
  }
}

/**
 * Return a well formatted date from a node field
 */
function dpiexport_get_date_from_field($node_wrapper, $field_name, $attribute = 'value', $default = FALSE) {
  // Make sure the field isn't empty or inexisting
  if (isset($node_wrapper->{$field_name}) && $field_value = $node_wrapper->{$field_name}->value()) {
    // Make sure we get the correct element in the field
    if (is_array($field_value) && isset($field_value[$attribute])) {
      $field_value = $field_value[$attribute];
    }

    // Make sure we get a simple value and not anything else
    if (is_string($field_value)) {
      // Make sure we have a timestamp
      if (!is_numeric($field_value)) {
        $datetime = new DateTime($field_value);
        $field_value = $datetime->getTimestamp();
      }

      if ($field_value) {
        return date('Y-m-d', $field_value).'T'.date('H:i:s', $field_value);
      }
    }
  }

  // Return default value
  if ($default == 'now') {
    $default = time();
  }
  if (is_numeric($default)) {
    return date('Y-m-d', $default).'T'.date('H:i:s', $default);
  }

  return FALSE;
}
