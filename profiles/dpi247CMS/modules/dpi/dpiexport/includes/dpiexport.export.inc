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
  $node_dpiproperties = dpiproperties_load_by_type_and_id('node', $node->nid);

  $dom = new DomDocument('1.0', 'UTF-8');

  $packages_node = $dom->appendChild($dom->createElement('Packages'));
  $packages_node->setAttribute('xmlns', 'http://rossel.audaxis.com');
  $packages_node->setAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
  $packages_node->setAttribute('xsi:schemaLocation', 'http://rossel.audaxis.com http://rossel.audaxis.com/xsd/packages.1.21.xsd');

  $package_node = $packages_node->appendChild($dom->createElement('Package'));
  // Package Type
  $package_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', 'ArticlePackageType');

  // Set package fields
  dpiexport_node_to_dpixml_fill_package($dom, $package_node, $node, $node_wrapper, $node_dpiproperties);

  // Set main story
  dpiexport_node_to_dpixml_fill_article_package_type($dom, $package_node, $node, $node_wrapper, $node_dpiproperties);

  // Set objects
  if ($embeds = dpicontenttypes_api_get_embeds($node, FALSE, FALSE, TRUE)) {
    $embeddedcontent_node = $package_node->appendChild($dom->createElement('EmbeddedContent'));
    $embeddedobjects_node = $embeddedcontent_node->appendChild($dom->createElement('EmbeddedObjects'));
    foreach ($embeds as $embed) {
      $object_node = $embeddedobjects_node->appendChild($dom->createElement('Object'));
      $types_translator = array(
        'image' => 'PhotoObjectType',
        'video' => 'VideoObjectType',
      );
      $object_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $types_translator[$embed['type']]);
      $atom = $embed['atom'];
      $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);
      $atom_dpiproperties = dpiproperties_load_by_type_and_id('scald_atom', $atom->sid);
      dpiexport_node_to_dpixml_fill_object_type($dom, $object_node, $node, $node_wrapper, $atom, $atom_wrapper, $atom_dpiproperties);
      switch ($embed['type']) {
        case 'TextObjectType' :
          _wallytoolbox_arraytowallyxml_fillTextObjectType($valid_dom, $object_node, $object_array);
          break;
        case 'AudioObjectType':
          _wallytoolbox_arraytowallyxml_fillAudioObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
          break;
        case 'VideoObjectType':
          _wallytoolbox_arraytowallyxml_fillVideoObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
          break;
        case 'image':
          dpiexport_node_to_dpixml_fill_photo_object_type($dom, $object_node, $node, $atom, $atom_wrapper, $atom_dpiproperties, $files_to_zip);
          break;
        case 'PollObjectType':
          _wallytoolbox_arraytowallyxml_fillPollObjectType($valid_dom, $object_node, $object_array);
          break;
      }
    }
  }
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
  dpiexport_node_to_dpixml_fill_text_object_type($dom, $mainstory_node, $node, $node_wrapper, $dpiproperties);
}

/**
 * Set the Object elements
 */
function dpiexport_node_to_dpixml_fill_object_type($dom, &$object_node, $node, $node_wrapper, $atom, $atom_wrapper, $atom_dpiproperties) {
  // @todo : What is ObjectID ?
  //$object_node->setAttribute('ObjectID', $object_array['ObjectID']);
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialpublicationdate', 'value', 'now')) {
    $object_node->setAttribute('PublicationDate', $date);
  }
  $object_node->setAttribute('CreationDate', date('Y-m-d', $node->created).'T'.date('H:i:s', $node->created));
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialupdatedate', 'value', 'now')) {
    $object_node->setAttribute('LastUpdateDate', $date);
  }

  $object_node->appendChild($dom->createElement('InternalReference', 'Drupal_SID:'.$atom_dpiproperties->id));
  $object_node->appendChild($dom->createElement('ExternalReference', $atom_dpiproperties->external_reference));

  dpiexport_node_to_dpixml_fill_taxonomies($dom, $object_node, $atom_wrapper);

  if (isset($atom_wrapper->field_copyright) && $field_value = $atom_wrapper->field_copyright->value()) {
    $object_node->appendChild($dom->createElement('Copyright', $field_value));
  }
}

/**
 * Set the Text Object elements
 */
function dpiexport_node_to_dpixml_fill_text_object_type($dom, &$mainstory_node, $node, $node_wrapper, $dpiproperties) {
  // @todo : What is ObjectID ?
  //$object_node->setAttribute('ObjectID', $object_array['ObjectID']);
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialpublicationdate', 'value', 'now')) {
    $mainstory_node->setAttribute('PublicationDate', $date);
  }
  $mainstory_node->setAttribute('CreationDate', date('Y-m-d', $node->created).'T'.date('H:i:s', $node->created));
  if ($date = dpiexport_get_date_from_field($node_wrapper, 'field_editorialupdatedate', 'value', 'now')) {
    $mainstory_node->setAttribute('LastUpdateDate', $date);
  }

  $mainstory_node->appendChild($dom->createElement('InternalReference', 'Drupal_NID:'.$node->nid));
  $mainstory_node->appendChild($dom->createElement('ExternalReference', $dpiproperties->external_reference_2));

  dpiexport_node_to_dpixml_fill_taxonomies($dom, $mainstory_node, $node_wrapper);

  if (isset($node_wrapper->field_copyright) && $field_value = $node_wrapper->field_copyright->value()) {
    $mainstory_node->appendChild($dom->createElement('Copyright', $field_value));
  }

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

/**
 * Set a media object
 */
function dpiexport_node_to_dpixml_fill_media_object_type($dom, &$object_node, $atom, $atom_wrapper, $atom_dpiproperties) {
  if (isset($atom_wrapper->field_caption) && $field_value = $atom_wrapper->field_caption->value()) {
    $field_caption_node = $object_node->appendChild($dom->createElement('Caption'));
    $field_caption_node->appendChild($dom->createCDATASection($field_value['safe_value']));
  }
  $object_node->appendChild($dom->createElement('URI', $atom->file_source));
  $object_node->appendChild($dom->createElement('FileSize', $atom->base_entity->filesize));
  $thumbnail_node = $object_node->appendChild($dom->createElement('Thumbnail'));
  $thumbnail_node->setAttribute('URI', $atom->thumbnail_source);
  if (isset($atom_wrapper->scald_thumbnail) && $field_value = $atom_wrapper->scald_thumbnail->value()) {
    $thumbnail_node->setAttribute('Type', $field_value['filemime']);
    $thumbnail_node->setAttribute('Title', $field_value['title']);
    $thumbnail_node->setAttribute('W', $field_value['width']);
    $thumbnail_node->setAttribute('H', $field_value['height']);
  }
  else {
    $thumbnail_node->setAttribute('xsi:nil', 'true');
  }
  if (isset($atom_wrapper->field_rating) && $field_value = $atom_wrapper->field_rating->value()) {
    $object_node->appendChild($dom->createElement('Rating', $field_value->name));
  }

  $object_node->setAttribute('MimeType', $atom->base_entity->filemime);
}

/**
 * Set a photo object
 */
function dpiexport_node_to_dpixml_fill_photo_object_type($dom, &$object_node, $node, $atom, $atom_wrapper, $atom_dpiproperties, &$files_to_zip) {
  $files_to_zip[] = $atom->base_entity;

  dpiexport_node_to_dpixml_fill_media_object_type($dom, $object_node, $atom, $atom_wrapper, $atom_dpiproperties);

  $crops_node = $object_node->appendChild($dom->createElement('Crops'));
  $cropings = dpicontenttypes_api_get_cropings_for_entity_and_atom('node', $node->nid, $atom->sid);
  if (!empty($cropings)) {
    foreach ($cropings as $type => $crop) {
      $crop_node = $crops_node->appendChild($dom->createElement('Crop'));
      $crop_node->setAttribute('Type', $type);
      $crop_node->setAttribute('X', $crop[0]);
      $crop_node->setAttribute('Y', $crop[1]);
      $crop_node->setAttribute('W', $crop[2]);
      $crop_node->setAttribute('H', $crop[3]);
    }
  }
  else {
    $crops_node->setAttribute('xsi:nil', 'true');
  }

  // @todo : LinkTitle, LinkURI, Width and Height ?
}

/**
 * Set taxonomies
 */
function dpiexport_node_to_dpixml_fill_taxonomies($dom, &$object_node, $node_wrapper) {
  $taxonomy_node = $object_node->appendChild($dom->createElement('Taxonomy'));

  if (isset($node_wrapper->field_locations) && $field_value = $node_wrapper->field_locations->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'Locations', 'categorized');
    foreach ($field_value as $location) {
      $location_node = dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $location, 'LocationType');
      dpiexport_node_to_dpixml_fill_location($dom, $location_node, $location);
    }
  }
  if (isset($node_wrapper->field_entities) && $field_value = $node_wrapper->field_entities->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'Entities', 'categorized');
    foreach ($field_value as $entity) {
      $entity_node = dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $entity, 'EntityType');
      dpiexport_node_to_dpixml_fill_entity($dom, $entity_node, $entity);
    }
  }
  if (isset($node_wrapper->field_persons) && $field_value = $node_wrapper->field_persons->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'Persons', 'categorized');
    foreach ($field_value as $person) {
      $person_node = dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $person, 'PersonType');
      dpiexport_node_to_dpixml_fill_person($dom, $person_node, $person);
    }
  }
  if (isset($node_wrapper->field_authors) && $field_value = $node_wrapper->field_authors->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'Authors', 'categorized');
    foreach ($field_value as $author) {
      $author_node = dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $author, 'PersonType');
      dpiexport_node_to_dpixml_fill_person($dom, $author_node, $author);
    }
  }
  if (isset($node_wrapper->field_freetags) && $field_value = $node_wrapper->field_freetags->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'Tags', 'free');
    foreach ($field_value as $tag) {
      dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $tag);
    }
  }
  if (isset($node_wrapper->field_concepts) && $field_value = $node_wrapper->field_concepts->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'Concepts', 'free');
    foreach ($field_value as $concept) {
      dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $concept);
    }
  }
  if (isset($node_wrapper->field_iptc) && $field_value = $node_wrapper->field_iptc->value()) {
    $vocabulary_node = dpiexport_node_to_dpixml_fill_vocabulary($dom, $taxonomy_node, $field_value[0]->vid, 'IPTC', 'categorized');
    foreach ($field_value as $iptc) {
      dpiexport_node_to_dpixml_fill_term($dom, $vocabulary_node, $iptc);
    }
  }
}

/**
 * Set a vocabulary item
 */
function dpiexport_node_to_dpixml_fill_vocabulary($dom, &$taxonomy_node, $vid, $name, $type) {
  $vocabulary = taxonomy_vocabulary_load($vid);
  $vocabulary_node = $taxonomy_node->appendChild($dom->createElement('Vocabulary'));
  $vocabulary_node->setAttribute('VocabularyName', $name);
  $vocabulary_node->setAttribute('VocabularyDescription', $vocabulary->description);
  $vocabulary_node->setAttribute('VocabularyType', $type);
  return $vocabulary_node;
}

/**
 * Set a term item
 */
function dpiexport_node_to_dpixml_fill_term($dom, &$vocabulary_node, $term, $type = FALSE) {
  $term_node = $vocabulary_node->appendChild($dom->createElement('Term'));
  if ($type) {
    $term_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $type);
  }
  $term_node->setAttribute('Name', $term->name);
  $term_node->setAttribute('ID', $term->tid);
  // @todo : ConfidenceScore and RelevancyScore
  return $term_node;
}

/**
 * Set a location item
 */
function dpiexport_node_to_dpixml_fill_location($dom, $location_node, $location) {
  $term_wrapper = entity_metadata_wrapper('taxonomy_term', $location);
  if (isset($term_wrapper->field_address) && $field_value = $term_wrapper->field_address->value()) {
    $address_node = $location_node->appendChild($dom->createElement('Address'));
    $address_node->appendChild($dom->createElement('AddressLine', $field_value['thoroughfare']));
    if (!empty($field_value['premise'])) {
      $address_node->appendChild($dom->createElement('AddressLine', $field_value['premise']));
    }
    $address_node->appendChild($dom->createElement('City', $field_value['locality']));
    $address_node->appendChild($dom->createElement('Zip', $field_value['postal_code']));
    if (!empty($field_value['administrative_area'])) {
      $address_node->appendChild($dom->createElement('State', $field_value['administrative_area']));
    }
    $address_node->appendChild($dom->createElement('Country', $field_value['country']));
  }
  // @todo : LocationGeo
}

/**
 * Set an entity item
 */
function dpiexport_node_to_dpixml_fill_entity($dom, $entity_node, $entity) {
  $term_wrapper = entity_metadata_wrapper('taxonomy_term', $entity);
  $entity_node->appendChild($dom->createElement('EntityDescription', $entity->description));
  if (isset($term_wrapper->field_entity_services) && $field_value = $term_wrapper->field_entity_services->value()) {
    $entity_node->appendChild($dom->createElement('EntityServices', $field_value));
  }
}

/**
 * Set a person item
 */
function dpiexport_node_to_dpixml_fill_person($dom, $person_node, $person) {
  $term_wrapper = entity_metadata_wrapper('taxonomy_term', $person);
  if (isset($term_wrapper->field_personlastname) && $field_value = $term_wrapper->field_personlastname->value()) {
    $person_node->appendChild($dom->createElement('PersonLastName', $field_value));
  }
  if (isset($term_wrapper->field_personfirstname) && $field_value = $term_wrapper->field_personfirstname->value()) {
    $person_node->appendChild($dom->createElement('PersonFirstName', $field_value));
  }
  if (isset($term_wrapper->field_personnickname) && $field_value = $term_wrapper->field_personnickname->value()) {
    $person_node->appendChild($dom->createElement('PersonNickName', $field_value));
  }
  if (isset($term_wrapper->field_personbirthdate) && $field_value = $term_wrapper->field_personbirthdate->value()) {
    $person_node->appendChild($dom->createElement('PersonBirthDate', date('Y-m-d', $field_value)));
  }
  if (isset($term_wrapper->field_persongender) && $field_value = $term_wrapper->field_persongender->value()) {
    $person_node->appendChild($dom->createElement('PersonSex', $field_value));
  }
  if (isset($term_wrapper->field_personemail) && $field_value = $term_wrapper->field_personemail->value()) {
    $person_node->appendChild($dom->createElement('PersonEmail', $field_value));
  }
  if (isset($term_wrapper->field_personwebsite) && $field_value = $term_wrapper->field_personwebsite->value()) {
    $person_node->appendChild($dom->createElement('PersonWebSite', $field_value['url']));
  }
  if (isset($term_wrapper->field_person_phonenumber) && $field_value = $term_wrapper->field_person_phonenumber->value()) {
    $person_node->appendChild($dom->createElement('PersonPhoneNumber', $field_value));
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
