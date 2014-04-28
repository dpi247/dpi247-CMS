<?php

require_once 'flowtonode.class.inc';

class RSSV1 extends FlowToNode {

  private $media_namespace;
  private $channel;
  private $description_image;

  function Importer($settings, $plugin) {
    $this::parent($settings, $plugin);
    $this->media_namespace = 'http://search.yahoo.com/mrss/';
    $this->channel = array();
    $this->description_image = array();
  }

  /**
   * Form to complete the settings of the importer
   *
   * @param Array $form_state
   * @param Array $values
   */
  public function getSettingsForm(&$form_state, $values) {
    $form = parent::getSettingsForm($form_state, $values);

    $form['importer_settings']['embed_img'] = array(
      '#title'            => t('Use the first image embeded in the text if no media image found'),
      '#type'             => 'checkbox',
      '#default_value'    => isset($values['embed_img']) ? $values['embed_img'] : 1,
    );

    $form['importer_settings']['channel_img'] = array(
      '#title'            => t('Use the default channel image if no media image found'),
      '#type'             => 'checkbox',
      '#default_value'    => isset($values['channel_img']) ? $values['channel_img'] : 1,
    );

    $form['importer_settings']['filter_body'] = array(
      '#title'            => t('Filter all tags from the text body'),
      '#type'             => 'checkbox',
      '#default_value'    => isset($values['filter_body']) ? $values['filter_body'] : 1,
    );

    return $form;
  }

  public function buildrss($dom) {
    Logger::logExecution('rss', TRUE);
    if ($dom->hasAttributeNS('http://www.w3.org/2000/xmlns/', 'media')) {
      $this->media_namespace = $dom->getAttributeNS('http://www.w3.org/2000/xmlns/', 'media');
    }

    // Go on with the children
    return TRUE;
  }

  public function buildchannel($dom) {
    Logger::logExecution('channel', TRUE);

    $this->setChannelProperties($dom);

    $count = 0;
    // Process the items
    foreach ($dom->getElementsByTagName('item') as $dom_item) {
      $this->builditem($dom_item);
      ++$count;
      if ($this->importer_settings['maxitem'] > 0 && $count >= $this->importer_settings['maxitem']) {
        break;
      }
    }

    // Indicates that the dispatcher must not process the children
    return FALSE;
  }

  /**
   * Intercept dom at element <item>
   *
   * @param DOM $dom
   */
  public function builditem($dom) {
    Logger::logExecution('item', FALSE);

    if ($this->importer_settings['embed_img']) {
      $this->extractImgFromDescription($dom->getElementsByTagName('description')->item(0)->nodeValue);
    }

    $package = $this->constructPackage($dom);

    $this->setImageFromDescription($dom, $package);

    if (!Logger::error()){
      $this->savePackage($dom, $package);
    }

    // Indicates that the dispatcher must not process the children
    return FALSE;
  }

  /**
   * Create or load a package from an item
   *
   * @param DOM $dom
   *
   * @return Object $node
   */
  protected function createOrLoadEntityitem($dom) {
    if ($guid = $this->checkIfElementExists($dom, 'guid')) {
      $extref = $guid;
    } else {
      $extref = $this->checkIfElementExists($dom, 'link');
    }
    $extref = md5($extref);
    $prefix = 'flow2node_'.$this->name.'_';
    $external_reference = $prefix.$extref;

    $action = 'CreateUpdate';

    return $this->_createOrLoadEntityPackage($dom, $external_reference, $action);
  }

  /**
   * Create or load a photo object from an enclosure
   *
   * @param DOM $dom
   *
   * @return Object $node
   */
  protected function createOrLoadEntityenclosure($dom) {
    if ($url = $this->checkIfAttributeExists($dom, 'url')) {
      $external_reference = 'flow2node_'.$this->name.'_'.md5($url);
      return $this->_createOrLoadEntityPhotoObjectType($dom, $external_reference);
    } else {
      // @todo : Add error if no url attribute
      return FALSE;
    }
  }

  /**
   * Create or load a photo object from a media:content
   *
   * @param DOM $dom
   *
   * @return Object $node
   */
  protected function createOrLoadEntitymedia_content($dom) {
    if ($url = $this->checkIfAttributeExists($dom, 'url')) {
      $external_reference = 'flow2node_'.$this->name.'_'.md5($url);
      return $this->_createOrLoadEntityPhotoObjectType($dom, $external_reference);
    } else {
      // @todo : Add error if no url attribute
      return FALSE;
    }
  }

  protected function setPackageAttributes($dom, $wrapper, $entity) {
    $term = array();
    $result = array();
    $vid = _dpidestinations_variable_get('dpidestinations_section_vid', NULL);
    if ($vid) {
      $destination_list = $this->importer_settings['destination'];
      // Add destinations to the package
      $wrapper->field_destinations->set($destination_list);
    } else {
      Logger::logError('importer', 0, 'novocpath', t('Destination vocabulary not set.'));
    }
  }

  public function buildtitle($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->title->set($this->filterHtmlTags($value));
    $wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
  }

  public function buildlink($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $wrapper->field_externalurl->set(array('url' => $value));
  }

  public function builddescription($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    if ($this->importer_settings['filter_body']) {
      $values = $this->filterHtmlTags($value);
    }
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textbody->set(array('value' => $value, 'format' => $format));
  }

  public function buildauthor($dom, $wrapper, $entity) {
    $dom_author_name = trim($dom->nodeValue);
    if (!empty($dom_author_name)) {
      $author = taxonomy_get_term_by_name($dom_author_name, 'authors');
      if (empty($author)) {
        if ($author_vid = dpi_variable_get('dpi_authors', FALSE)) {
          $new_author_term = new stdClass();
          $new_author_term->vid = $author_vid;
          $new_author_term->weight = 0;
          $new_author_term->vocabulary_machine_name = 'authors';
          $new_author_term->name = $dom_author_name;
          taxonomy_term_save($new_author_term);
          $author = $new_author_term;
        }
      } else {
        $author = array_shift($author);
      }
      $wrapper->field_authors->set(array($author->tid));
    }
  }

  public function buildcategory($dom, $wrapper, $entity) {
    $dom_category_name = trim($dom->nodeValue);
    if (!empty($dom_category_name)) {
      $category = taxonomy_get_term_by_name($dom_category_name, 'iptc');
      if (empty($category)) {
        if ($vid = dpi_variable_get('dpi_iptc', FALSE)) {
          $new_term = new stdClass();
          $new_term->vid = $vid;
          $new_term->weight = 0;
          $new_term->vocabulary_machine_name = 'iptc';
          $new_term->name = $dom_category_name;
          taxonomy_term_save($new_term);
          $category = $new_term;
        }
      } else {
        $category = array_shift($category);
      }
      $wrapper->field_iptc->set(array($category->tid));
    }
  }

  public function buildpubDate($dom, $wrapper, $entity) {
    $dom_pub_date = trim($dom->nodeValue);
    $pub_date = empty($dom_pub_date) ? time() : strtotime($dom_pub_date);
    $wrapper->field_editorialpublicationdate->set($pub_date);
    if (!isset($entity->is_new)) {
      $wrapper->field_editorialupdatedate->set($pub_date);
    }
  }

  public function buildsource($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_source->set(array('value' => $value, 'format' => $format));
  }

  public function buildenclosure($dom, $wrapper, $entity) {
    $mime_type = $this->checkIfAttributeExists($dom, 'type');
    if (strstr($mime_type, 'image')) {
      Logger::logExecution('photobojecttype', TRUE);
      Logger::setCurrentToPreviousExecution();
      if ($atom = $this->constructEntity($dom, 'scald_atom')) {
        $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);

        $atom_wrapper->title->set($wrapper->title->value());
        $atom_wrapper->field_displaytitle->set($wrapper->field_displaytitle->value());

        $uri = $this->checkIfAttributeExists($dom, 'url');
        if ($file = _dpicontenttypes_create_or_update_file($uri, 'flowtonode/'.$this->name)) {
          $atom->base_entity = $file;
          $atom_wrapper->scald_thumbnail->set((array)$file);
          $atom->base_id = $file->fid;
          scald_atom_save($atom);

          $this->addAtomToFied($wrapper, $entity, 'field_embededobjects', $atom, array('identifier' => TRUE));
        }
      }
    } else {
      // @todo : Handle other types
    }
  }

  public function buildmedia_content($dom, $wrapper, $entity) {
    if ($this->checkIsImage($dom)) {
      Logger::logExecution('photobojecttype', TRUE);
      Logger::setCurrentToPreviousExecution();
      if ($atom = $this->constructEntity($dom, 'scald_atom', FALSE)) {
        $media_children = array();
        foreach ($dom->childNodes as $child) {
          $media_children[strtolower($child->nodeName)] = $child;
        }
        $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);

        if (!isset($media_children['media:title']) || empty($media_children['media:title']->nodeValue)) {
          $atom_wrapper->title->set($wrapper->title->value());
          $atom_wrapper->field_displaytitle->set($wrapper->field_displaytitle->value());
        }

        $uri = $this->checkIfAttributeExists($dom, 'url');
        if ($file = _dpicontenttypes_create_or_update_file($uri, 'flowtonode/'.$this->name)) {
          $atom->base_entity = $file;
          $atom->base_id = $file->fid;
          $atom->file_source = $file->uri;

          if (!isset($media_children['media:thumbnail']) /*|| empty($media_children['media:thumbnail']->nodeValue)*/) {
            dsm('coco');
            $atom_wrapper->scald_thumbnail->set((array)$file);
            $atom->thumbnail_source = $file->uri;
          }

          // Save entity
          if (!Logger::error()) {
            $this->saveEntity($dom, 'scald_atom', $atom);
          }

          $this->addAtomToFied($wrapper, $entity, 'field_embededobjects', $atom, array('identifier' => TRUE));
        }
      }
    } else {
      // @todo : Handle other types
    }
  }

  public function buildmedia_title($dom, $wrapper, $entity) {
    if ($this->entity_is_atom($wrapper, $entity)) {
      $value = $dom->nodeValue;
      $format = $this->getDefaultTextFormat();
      $wrapper->title->set($this->filterHtmlTags($value));
      $wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
    }
  }

  public function buildmedia_description($dom, $wrapper, $entity) {
    if ($this->entity_is_atom($wrapper, $entity)) {
      $value = $dom->nodeValue;
      $format = $this->getDefaultTextFormat();
      $wrapper->field_caption->set(array('value' => $value, 'format' => $format));
    }
  }

  public function buildmedia_thumbnail($dom, $wrapper, $entity) {
    if ($this->entity_is_atom($wrapper, $entity)) {
      $uri = $this->checkIfAttributeExists($dom, 'url');
      if ($file = _dpicontenttypes_create_or_update_file($uri, 'flowtonode/'.$this->name.'-thumb')) {
        $wrapper->scald_thumbnail->set((array)$file);
        $entity->thumbnail_source = $file->uri;
      }
    }
  }

  private function checkIfElementExists($dom_item, $name, $namespace = 'none', $default = '') {
    $return = $default;
    if (is_object($dom_item)) {
      if ($namespace != 'none') {
        $return = $dom_item->getElementsByTagNameNS($namespace, $name)->item(0) ? $dom_item->getElementsByTagNameNS($namespace, $name)->item(0)->firstChild->wholeText : '';
      }
      else {
        $return = $dom_item->getElementsByTagName($name)->item(0) ? $dom_item->getElementsByTagName($name)->item(0)->firstChild->wholeText : '';
      }
    }
    return $return;
  }

  private function checkIfAttributeExists($dom_item, $name, $namespace = 'none', $default = '') {
    $return = $default;
    if (is_object($dom_item)) {
      if ($namespace != 'none' && $dom_item->hasAttributeNS($namespace, $name)) {
        $return = $dom_item->getAttributeNS($namespace, $name);
      }
      elseif ($dom_item->hasAttribute($name)) {
        $return = $dom_item->getAttribute($name);
      }
    }
    return $return;
  }

  private function setChannelProperties($dom) {
    // Save every channel elements in a channel attribute
    $channel_elements = array(
      'title',
      'link',
      'description',
      'language',
      'copyright',
      'managingEditor',
      'webMaster',
      'pubDate',
      'lastBuildDate',
      'category',
      'generator',
      'docs',
      //'cloud',
      'ttl',
      //'image',
      'rating',
      'textInput',
      //'skipHours',
      //'skipDays',
    );
    foreach ($channel_elements as $element) {
      if ($val = $this->checkIfElementExists($dom, $element)) {
        $this->channel[$element] = $val;
      }
    }
    if ($channel_image = $dom->getElementsByTagName('image')->item(0)) {
      $channel_image_elements = array(
        'url',
        'title',
        'link',
        'width',
        'height',
        'description',
      );
      $this->channel['image'] = array();
      foreach ($channel_image_elements as $element) {
        if ($val = $this->checkIfElementExists($channel_image, $element)) {
          $this->channel['image'][$element] = $val;
        }
      }
    }
  }

  private function setImageFromDescription($dom, $package) {
    $no_photo = TRUE;
    $package_wrapper = entity_metadata_wrapper('node', $package);
    $current_embeds = $package_wrapper->field_embededobjects->value(array('identifier' => TRUE));
    if ($current_embeds && !empty($current_embeds)) {
      foreach ($current_embeds as $embed) {
        if ($embed['type'] == 'image'){
          $no_photo = FALSE;
          break;
        }
      }
    }

    // Get embeded images in the text body
    if ($this->importer_settings['embed_img'] && $no_photo) {
      $extracted_image = $this->extractImgFromDescription($dom->getElementsByTagName('description')->item(0)->nodeValue);
      $url = $extracted_image['url'];
      $external_reference = 'flow2node_'.$this->name.'_'.md5($url);

      $atom = $this->_createOrLoadEntityPhotoObjectType($dom, $external_reference);
      $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);

      if (!empty($extracted_image['alt'])) {
        $format = $this->getDefaultTextFormat();
        $atom_wrapper->title->set($this->filterHtmlTags($extracted_image['alt']));
        $atom_wrapper->field_displaytitle->set(array('value' => $extracted_image['alt'], 'format' => $format));
      } else {
        $atom_wrapper->title->set($package_wrapper->title->value());
        $atom_wrapper->field_displaytitle->set($package_wrapper->field_displaytitle->value());
      }

      if ($file = _dpicontenttypes_create_or_update_file($url, 'flowtonode/'.$this->name)) {
        $atom->base_entity = $file;
        $atom_wrapper->scald_thumbnail->set((array)$file);
        $atom->base_id = $file->fid;
        scald_atom_save($atom);

        $this->addAtomToFied($package_wrapper, $package, 'field_embededobjects', $atom, array('identifier' => TRUE));
      }
    }
  }

  private function extractImgFromDescription($description) {
    $substr = strstr($description, '<img');
    $substr = $substr ? $substr : strstr($description, rawurlencode('<img'));
    $tmp_substr = $substr ? strstr($substr, '>', TRUE) : FALSE;
    $substr = $tmp_substr ? $tmp_substr : ($substr ? strstr($substr, rawurlencode('>'), TRUE) : FALSE);

    $extracted_image = FALSE;
    if ($substr) {
      $src_substr = strstr($substr, 'src=');
      $src_delimiter = substr($src_substr, 4, 1);
      $src_substr = substr($src_substr, 5);
      $img_url = strstr($src_substr, $src_delimiter, TRUE);
      $img_url = urldecode(html_entity_decode($img_url));
    
      $alt_substr = strstr($substr, 'alt=');
      if ($alt_substr) {
        $alt_delimiter = substr($alt_substr, 4, 1);
        $alt_substr = substr($alt_substr, 5);
        $img_alt = strstr($src_substr, $alt_delimiter, TRUE);
      } else {
        $img_alt = '';
      }

      $extracted_image = array(
        'url' => $img_url,
        'alt' => $img_alt,
      );
    }
    return $extracted_image;
  }

  private function checkIsImage($media_content) {
    if ($media_content->getAttribute('type') && stristr($media_content->getAttribute('type'), 'image')) {
      return TRUE;
    } elseif ($media_content->getAttribute('medium') && stristr($media_content->getAttribute('medium'), 'image')) {
      return TRUE;
    } elseif ($media_url = $media_content->getAttribute('url')) {
      if (stristr($media_url,'.jpg') || stristr($media_url,'.jpeg') || stristr($media_url,'.gif') || stristr($media_url,'.png')) {
        return TRUE;
      }
    }

    return FALSE;
  }

  private function entity_is_atom($wrapper, $entity) {
    if ($wrapper->type() == 'scald_atom') {
      return TRUE;
    }
    return FALSE;
  }
}
