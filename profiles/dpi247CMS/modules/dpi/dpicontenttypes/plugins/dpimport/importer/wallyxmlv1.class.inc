<?php

require_once 'packagehelper.class.inc';

class WallyXmlV1 extends PackageHelper {

  /**
   * Intercept dom at element <Package>
   *
   * @param DOM $dom
   */
  public function buildPackage($dom) {
    Logger::logExecution('package', TRUE);
    $package = $this->constructPackage($dom);
    if (!Logger::error()) {
      // Allow to alter entity before save
      $dispatcher = $this->getDispatcher();
      $package_wrapper = entity_metadata_wrapper('node', $package);
      module_invoke_all('dpimport_entity_presave', $package_wrapper, $package, 'node', $this, $dispatcher);

      $this->_replace_inline_object($package);
      /* Hack to change external url */
      $this->_buildExternalUrl($package);

      $this->savePackage($dom, $package);

      if(isset($package->temp_crops) && is_array($package->temp_crops)){
        foreach ($package->temp_crops as $atom_id => $crops_infos) {
          _dpicontenttypes_cropings_update_cropings('node', $package->nid, $atom_id, $crops_infos);
        } 
      }
    }

    // Set the CurrentLog to the previous one to stop children growing.
    Logger::setCurrentToPreviousExecution();

    // Indicates that the dispatcher must not process the children
    return FALSE;
  }

  /**
   * Create or load a package
   *
   * @param DOM $dom          
   *
   * @return Object $node
   */
  protected function createOrLoadEntityPackage($dom) {
    // @todo Demander à dimitri pourquoi dans wallymport 6 on filtre l'external reference par filter_xss
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;

    $action = 'CreateReplace';
    if ($dom->hasAttribute('Action')) {
      $action = $dom->getAttribute('Action');
    }

    return $this->_createOrLoadEntityPackage($dom, $external_reference, $action);
  }

  /**
   * Create Atom Image
   *
   * @param DOM $dom          
   */
  protected function createOrLoadEntityPhotoObjectType($dom) {
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    $atom = $this->_createOrLoadEntityPhotoObjectType($dom, $external_reference);
    return $atom;
  }

  /**
   * Determine the next callback function from the Dom element
   * If the xsi attribute exists set as callback, else set nodeName as callback
   *
   * @param DOM $dom          
   *
   * @return String $callback
   */
  public function getCallback($dom) {
    $callback = strtolower($dom->nodeName);
    if (get_class($dom) == 'DOMElement' && $dom->hasAttribute('xsi:type') && $dom->nodeName != 'Package' && $dom->nodeName != 'MainStory' && $dom->getAttribute('xsi:type') != 'xs:string') {
      $callback = strtolower($dom->getAttributeNode('xsi:type')->value);
    }
    return $callback;
  }

  /**
   * Return the timestamp of the datetime
   * Ceci retourne le timestamp en UTC
   * Exemple avec la date 2013-06-14T09:20:41+02:00 il retourne 1371194441
   * qui donne pour date('d/m/Y H:i:s e', 1371194441) -> 14/06/2013 09:20:41 Europe/Brussels (e = timezone)
   * ou 14/06/2013 07:20:41 UTC Ce qui est correct donc merci de vérifier vos timezone avant de modifier cette fonction.
   *
   * @param String $datetime          
   */
  protected function getDateStampFromDateTime($date) {
    $datetime = new DateTime($date);
    return $datetime->getTimestamp();
  }

  protected function setPackageAttributes($dom, $wrapper, $entity) {
    // Language
    // @todo: use global lang
    if ($dom->getAttributeNode('Language')->value != "") {
      $wrapper->language->set($dom->getAttributeNode('Language')->value);
    }
    else {
      $wrapper->language->set("fr");
    }
    // @todo path_autho check alias
    $this->buildEditorialPublicationDate($dom, $wrapper, $entity);
    $this->buildEditorialUpdateDate($dom, $wrapper, $entity);
    $this->buildAutopublishDate($dom, $wrapper, $entity);
  }

  /**
   * Set field_editorialpublicationdate
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  protected function buildEditorialPublicationDate($dom, $wrapper, $entity) {
    if ($dom->hasAttribute("PublicationDate")) {
      $editorialpublicationdate = $this->getDateStampFromDateTime($dom->getAttributeNode("PublicationDate")->value);
    }
    else {
      $editorialpublicationdate = time();
    }
    $wrapper->field_editorialpublicationdate->set($editorialpublicationdate);
  }

  /**
   * Set field editorialupdatedate
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  protected function buildEditorialUpdateDate($dom, $wrapper, $entity) {
    // We don't save the editorial update date on new node.
    if ($wrapper->nid->value() != NULL) {
      if ($dom->hasAttribute("ForceLastUpdate") && $this->xmlbooleantophpboolean($dom->getAttribute("ForceLastUpdate"))) {
        if ($dom->hasAttribute("LastUpdateDate")) {
          $editorialupdatedate = $this->getDateStampFromDateTime($dom->getAttributeNode("LastUpdateDate")->value);
        }
        else {
          $editorialupdatedate = time();
        }
        $wrapper->field_editorialupdatedate->set($editorialupdatedate);
      }
    }
  }

  /**
   * Set field autopublishdate
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  protected function buildAutopublishDate($dom, $wrapper, $entity) {
    $embargodate = NULL;
    $unpublishdate = NULL;
    if($wrapper->field_autopublish->value()){
      if ($dom->hasAttribute("EmbargoDate")) {
        $embargodate = $this->getDateStampFromDateTime($dom->getAttributeNode("EmbargoDate")->value);
      }else{
        Logger::logError('importer', 0, 'embargonotfound', t('No embargo date with autopublish true !'));
      }
      if ($dom->hasAttribute("UnPublishDate")) {
        $unpublishdate = $this->getDateStampFromDateTime($dom->getAttributeNode("UnPublishDate")->value);
      }
    }
    $wrapper->field_autopublishdate->set(array(
      'value' => $embargodate,
      'value2' => $unpublishdate
    ));
  }

  /**
   * Set field productID
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildProductID($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $wrapper->field_product_id->set($value);
  }

  /**
   * Set field ExternalReference
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildExternalReference($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    // Astuce pour empecher que l'external reference du textobject remplace celle du package et soit mise en external_reference_2
    if (!isset($entity->external_reference_set)) {
      $wrapper->field_external_reference->set($value);
      Logger::logExecution('external_reference', FALSE, 1, '000', $value);
      $entity->external_reference_set = TRUE;
    }
    else {
      $wrapper->field_external_reference_2->set($value);
      Logger::logExecution('external_reference_2', FALSE, 1, '000', $value);
    }
  }

  /**
   * Set field Source
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildSource($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $wrapper->field_source->set($value);
  }

  /**
   * Set field Package to title and display title.
   * If a mainstory or an embed textboject exists, it will be erased by it
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildPackageTitle($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->title->set($this->filterHtmlTags($value));
    $wrapper->field_displaytitle->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Get the destinations from DOM and set it into an array
   *
   * @param DOM $dom          
   * @param Integer $vid          
   * @param EntityDrupalWrapper $wrapper          
   *
   * @return Array $destination_list
   */
  protected function getdestinationsfromdom($dom, $vid, $wrapper) {
    $destination_list = array();
    $keys = array();
    foreach ($dom->getElementsByTagName('Destination') as $destination) {
      $dest = array();
      $path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
      $terms = dpicontenttypes_api_taxonomy_get_term_by_path($path, $vid);
      if ($terms && $destination->hasAttribute("DocumentLayout")) {
        if ($destination->hasAttribute("Position")) {
          $dest ['tid'] = $terms [0]->tid;
          $dest ['target'] = $destination->getAttributeNode("Position")->value;
          $dest ['layout'] = $destination->getAttributeNode("DocumentLayout")->value;
          if ($wrapper->nid->value() != NULL && $this->destinationexists($wrapper, $dest)) {
            // If destination already exists and no rank -> DNC
            $dest ['rank'] = $destination->hasAttribute("DestinationRank") ? $destination->getAttributeNode("DestinationRank")->value : 'DNC';
          }
          else {
            // If destination does not exists and no rank -> default
            $dest ['rank'] = $destination->hasAttribute("DestinationRank") ? $destination->getAttributeNode("DestinationRank")->value : 'default';
          }
          // Add destination only once. If a destinations with the same tid and target exists, the destination is not readded
          if (!array_key_exists($dest ['tid'] . '-' . $dest ['target'], $keys)) {
            $keys [$dest ['tid'] . '-' . $dest ['target']] = $dest ['tid'] . '-' . $dest ['target'];
            $destination_list [] = $dest;
          }
        }
        else {
          Logger::logWarning('importer', 2, 'pathnotfound', t('Position not found. Destination rejected'));
        }
      }
      else {
        Logger::logWarning('importer', 2, 'pathnotfound', t('Path "!path" not found. Destination rejected', array(
          '!path' => $path
        )));
      }
    }
    return $destination_list;
  }

  /**
   * Set field Destination
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildDestinations($dom, $wrapper, $entity, $globalDom) {
    // @todo s'occuper des destinations multiples
    $term = array();
    $result = array();
    $vid = _dpidestinations_variable_get('dpidestinations_section_vid', NULL);
    $package_type = $globalDom->getElementsByTagName('PackageLayout')->item(0)->nodeValue;
    if ($package_type != 'Hors-texte' || !isset($package_type)) {
      if ($vid) {
        $destination_list = $this->getdestinationsfromdom($dom, $vid, $wrapper);

        if (count($destination_list)) {
          // Add destinations to the package
          $wrapper->field_destinations->set($destination_list);
        }
        else {
          // If no destinations at all > Package can't be created
          Logger::logError('importer', 0, 'nodestination', t('No destination AT ALL for current package.'));
        }
      }
      else {
        Logger::logError('importer', 0, 'novocpath', t('Destination vocabulary not set.'));
      }
    }
  }

  /**
   * Set field CommentsAllowed
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildCommentsAllowed($dom, $wrapper, $entity) {
    switch ($dom->nodeValue) {
      case "Disabled" :
        $status = 0;
        break;
      case "Read Only" :
        $status = 1;
        break;
      case "Read-Write" :
        $status = 2;
        break;
      default :
        $status = 0;
        break;
    }
    $wrapper->comment->set($status);
  }

  /**
   * Set field FreeAccess
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildFreeAccess($dom, $wrapper, $entity) {
    // @todo
  }

  /**
   * Set field AutoPublish
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildAutoPublish($dom, $wrapper, $entity) {
    $value = FALSE;
    if(strtolower($dom->nodeValue) == "true"){
      $value = TRUE;
    }
    $wrapper->field_autopublish->set($value);
  }

  /**
   * Set field Package Layout
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildPackageLayout($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $vid = dpi_variable_get('dpi_packagelayout', NULL);
    $vocabulary = taxonomy_vocabulary_load($vid);
    $vocabulary_name = $vocabulary->machine_name;
    if ($value == 'Hors-texte') {
      $vocabulary_name = "package_type";
    }
    else {
      $value = "Article";
    }
    $item = taxonomy_get_term_by_name($value, $vocabulary_name);
    if (!empty($item)) {
      $wrapper->field_packagetype->set(array_shift($item));
    }
    else {
      Logger::logWarning('importer', 2, 'packagelayout', t('The package layout !package_layout can not be found', array(
        '!package_layout' => $value
      )));
    }
  }

  /**
   * Set field ExternalURI
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildExternalURI($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $wrapper->field_externalurl->set(array(
      'url' => $value
    ));
  }

  /**
   * Set field LastUrlAlias
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildLastURLAlias($dom, $wrapper, $entity) {
    // @todo
  }

  /**
   * Set field MainStory
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildMainStory($dom, $wrapper, $entity) {
    $dispatcher = $this->getDispatcher();
    $dispatcher->processChildren($dom, $wrapper, $entity);
  }

  /**
   * Set field Copyright
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildCopyright($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $wrapper->field_copyright->set($value);
  }

  /**
   * Set field Marker
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildMarker($dom, $wrapper, $entity) {
    // @todo
  }

  /**
   * Set field TextBarette
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTextBarette($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textbarette->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field TextForeTitle
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTextForeTitle($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textforetitle->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field TextSubTitle
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTextSubTitle($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textsubtitle->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field TextTitle
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTextTitle($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->title->set($this->filterHtmlTags($value));
    $wrapper->field_displaytitle->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field TextChapo
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTextChapo($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textchapo->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field TextBody
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTextBody($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textbody->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field ByLine
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildByLine($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_byline->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set Embedded Content
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity
   *
   */
  public function buildEmbeddedContent($dom, $wrapper, $entity) {
    $dispatcher = $this->getDispatcher();
    $dispatcher->processChildren($dom, $wrapper, $entity);
  }

  /**
   * Set Embedded Objects and add it to the wrapper
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity
   *
   * @see buildCrops()
   * @see dpicontenttypes_node_presave()
   */
  public function buildEmbeddedObjects($dom, $wrapper, $entity) {
    // Empty the embededobjects field
    $wrapper->field_embededobjects->set(NULL);
    $dispatcher = $this->getDispatcher();
    $embeds = $dispatcher->processList($dom, $wrapper, $entity);

    $embeds_list = array();
    $i = 100;
    if (!Logger::error()) {
      foreach ($embeds as $atom) {
        if (isset($atom->original)) {
          $atom->original = null;
        }
        if ($atom != NULL) {
          if (isset($atom->embeddedobjectsorder)) {
            $order = $atom->embeddedobjectsorder;
          }
          else {
            $order = $i ++;
            while (array_key_exists($order, $embeds_list)) {
              $order = $i ++;
            }
          }
          $embeds_list [$order] = $atom;
        }
      }
      // Sort the embeds
      ksort($embeds_list);
      foreach ($embeds_list as $atom) {
        $embededobject = array(
          'sid' => $atom->sid,
          'type' => $atom->type,
          'provider' => $atom->provider,
          'inline' => '0'  // @todo
        );

        //We build cropping info into entity element
        // @see buildCrops()
        // @see dpicontenttypes_node_presave()
        $wrapper->field_embededobjects->set($embededobject);

        $entity->temp_crops[$atom->sid] = $atom->temp_crops;
      }
    }
  }

  /**
   * Process LinksLists element
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildLinksLists($dom, $wrapper, $entity) {
    // Empty the linkslist field
    $wrapper->field_linkslists->set(NULL);

    $dispatcher = $this->getDispatcher();
    $linkslists = $dispatcher->processList($dom, $wrapper, $entity);
    $linkslist_list = array();
    $i = 100;
    if (!Logger::error()) {
      foreach ($linkslists as $atom) {
        if ($atom != NULL) {
          if (isset($atom->embeddedobjectsorder)) {
            $order = $atom->embeddedobjectsorder;
          }
          else {
            $order = $i ++;
            while (array_key_exists($order, $linkslist_list)) {
              $order = $i ++;
            }
          }
          $linkslist_list [$order] = $atom;
        }
      }
      // Sort the links lists
      ksort($linkslist_list);
      foreach ($linkslist_list as $atom) {
        $linkslistobject = array(
          'sid' => $atom->sid,
          'type' => $atom->type,
          'provider' => $atom->provider,
          'inline' => '0'  // @todo
        );
        $wrapper->field_linkslists->set($linkslistobject);
      }
    }
  }

  /**
   * Set LinksList and add it to wrapper
   *
   * @param Dom $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildLinksList($dom, $wrapper, $entity) {
    Logger::logExecution('linkslist', TRUE);
    $atom = $this->constructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    return $atom;
  }

  /**
   * Set links to the linkslist
   *
   * @param Dom $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildLinks($dom, $wrapper, $entity) {
    $wrapper->field_linkitems->set(NULL);
    $i = 100;
    $links_list = array();
    foreach ($dom->getElementsByTagName('Link') as $link) {
      if ($link->hasAttribute('EmbeddedObjectsOrder')) {
        $order = $link->getAttribute('EmbeddedObjectsOrder');
      }
      else {
        $order = $i ++;
        while (array_key_exists($order, $links_list)) {
          $order = $i ++;
        }
      }
      $links_list [$order] = array(
        'url' => $link->getElementsByTagName('URI')->item(0)->nodeValue,
        'title' => $link->getElementsByTagName('Title')->item(0)->nodeValue
      );
    }
    // Sort the links
    ksort($links_list);
    $valid_key_array = array();
    foreach ($links_list as $link) {
      $valid_key_array[] = $link;
    }
    $wrapper->field_linkitems->set($valid_key_array);
  }

  /**
   * Set PhotoObjectType
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $package_wrapper          
   * @param Object $entity          
   */
  public function buildPhotoObjectType($dom, $package_wrapper, $entity) {
    Logger::logExecution('photobojecttype', TRUE);
    $atom = $this->constructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    return $atom;
  }

  /**
   * Set field Title
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTitle($dom, $wrapper, $entity) {
    try {
      $value = $dom->nodeValue;
      $format = $this->getDefaultTextFormat();
      $wrapper->title->set($this->filterHtmlTags($value));
      $wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
    } catch (Exception $e) {
      $e->getMessage();
    }
  }

  /**
   * Set field Caption
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildCaption($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_caption->set(array(
      'value' => $value,
      'format' => $format
    ));
  }

  /**
   * Set field URI
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildURI($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $parsedurl = parse_url($value);
    switch ($parsedurl['scheme']) {
      case 'embed':
        $file = $this->createFile($value, scald_atom_thumbnail_path($entity->type));
        switch ($entity->type) {
          case 'image':
            $wrapper->scald_thumbnail->set((array) $file);
            break;
          case 'audio' :
            // No existing wrapper for audio so the file is directly set on the entity
            $entity->data ['audio_file'] = $file->uri;
            $entity->data ['audio_id'] = $file->fid;
            break;
        }
        break;
      case 'http' || 'https' :
        // Look for a provider for URL
        $function_parse_id = $entity->provider . '_parse_id';
        if (function_exists($function_parse_id)) {
          $id = $function_parse_id($value, FALSE);
          $wrapper->base_id->set($id);
          $function_info = $entity->provider . '_' . $entity->type;
          if (function_exists($function_info)) {
            $info = $function_info($id);
            $file = $this->createFile($info->thumbnail ['src'], scald_atom_thumbnail_path($entity->type));
            $wrapper->scald_thumbnail->set((array) $file);
          }
          else {
            Logger::logWarning('importer', 2, 'thumbnail', t('The thumbnail can not be loaded !uri', array(
              '!uri' => $value
            )));
          }
        }
        else {
          /* Gestion de la partie provenant de dpiscald - provider multiple */
          try {
            module_load_include('inc', 'dpiscald', 'providers/' . $entity->type . '/' . $entity->type);
            $function_parse_id = 'dpiscald_' . $entity->type . '_parse_id';
            if (function_exists($function_parse_id)) {
              $functionbyentity = 'dpiscald_' . $entity->type . '_get_import_info';
              if(function_exists($functionbyentity)){
                $functionbyentity($this, $entity, $wrapper, $value);
              }else{
                Logger::logError('importer', 0, 'providerfunction', t('function cannot be found', array(
                  '!uri' => $value
                )));
              }              
            }
            else {
              Logger::logError('importer', 0, 'provideruri', t('The provider can not analyse the uri !uri', array(
                '!uri' => $value
              )));
            }
          } catch (Exception $exc) {
            Logger::logWarning('importer', 2, 'dpiscaldatom_'.$entity->type, t('General error detected for entity !type', array(
              '!type' => $entity->type
            )));
          }
        }
        break;
    }
  }

  /**
   * Set field Rating
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildRating($dom, $wrapper, $entity) {
    $value = $dom->nodeValue;
    $vid = dpi_variable_get('dpi_rating', NULL);
    $item = dpicontenttypes_api_taxonomy_get_term_by_path($value, $vid);
    // We take the first match even is there are more.
    $wrapper->field_rating->set($item [0]);
  }

  /**
   * Set field Crops
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildCrops($dom, $wrapper, $entity) {
    // @todo


    $dom_crops = $dom->getElementsByTagName('Crop');
    $crops = array();
    foreach ($dom_crops as $dom_crop) {
      if ($dom_crop->hasAttribute('X')) {
        $x = $dom_crop->getAttributeNode('X')->value;
        $y = $dom_crop->getAttributeNode('Y')->value;
        $w = $dom_crop->getAttributeNode('W')->value;
        $h = $dom_crop->getAttributeNode('H')->value;
      }


      $preset_name = $dom_crop->getAttributeNode('Type')->value;
      $crops[$preset_name] = array($x, $y, $w, $h);
    }

    $entity->temp_crops = $crops;
  }

  /**
   * Set Link
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $package_wrapper          
   * @param Object $entity          
   */
  public function buildLink($dom, $package_wrapper, $entity) {
    $uri_info = $dom->getElementsByTagName('URI')->item(0)->nodeValue;
    if (strstr($uri_info, 'extref://') != FALSE) {
      $tmpExternalRef = substr($uri_info, 9);
      $query = "SELECT sa.sid FROM {scald_atoms} sa WHERE sa.title IN (SELECT n.title FROM {node} n, {dpicontenttypes_properties} dpp WHERE n.nid=dpp.id AND dpp.entity='node' AND dpp.external_reference=:extref ) LIMIT 1";
      $args = array(
        ':extref' => $tmpExternalRef
      );
      $result = db_query($query, $args)->fetchField();
      if (isset($result) && $var = scald_atom_load(intval($result))) {
        $atom = new stdClass ();
        $atom->sid = $result;
        $atom->type = $var->type;
        $atom->provider = $var->provider;
        return $atom;
      }
    }
    else {
      Logger::logExecution('linkbojecttype', TRUE);
      $atom = $this->constructEntity($dom, 'scald_atom');
      Logger::setCurrentToPreviousExecution();
      return $atom;
    }
  }

  /**
   * Set AudioObjectType
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $package_wrapper          
   * @param Object $entity          
   */
  public function buildAudioObjectType($dom, $package_wrapper, $entity) {
    Logger::logExecution('photobojecttype', TRUE);
    $atom = $this->constructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    // return $atom;
  }

  /**
   * This function is use to reorder different element for field_atompackage
   * XML specification
   *
   * @param unknown $package          
   * @return unknown
   */
  public function _buildFieldAtompackage($package) {
    $field_embededobjects = array();
    $field_atompackage = array();
    foreach ($package->field_embededobjects as $klang => $elements) {
      foreach ($elements as $element) {
        if ($element ['type'] == "atom_package") {
          if (!isset($field_atompackage [$klang]))
            $field_atompackage [$klang] = array();
          $field_atompackage [$klang] [] = $element;
        } else {
          if (!isset($field_embededobjects [$klang]))
            $field_embededobjects [$klang] = array();
          $field_embededobjects [$klang] [] = $element;
        }
      }
    }
    if (!empty($field_embededobjects))
      $package->field_embededobjects = $field_embededobjects;
    if (!empty($field_atompackage))
      $package->field_atompackage = $field_atompackage;
    return $package;
  }

  function _replace_inline_object(& $package) {
    try {
      $text = current($package->field_textbody);
      $text = current($text);
      $text = $text["value"];
      $embeddedObjects = current($package->field_embededobjects);

      if (isset($text) && isset($embeddedObjects) && !empty($embeddedObjects)) {
        foreach ($embeddedObjects as $k => $embed) {
          $tag_regex = "/ \[scald=" . $embed["sid"] . ":(.*?)\]/i";
          if (preg_match_all($tag_regex, $text, $tag_matches)) {
            if (isset($package->field_embededobjects[$package->language])) {
              $package->field_embededobjects[$package->language][$k]["inline"] = 1;
            }
            elseif (isset($package->field_embededobjects['und'])) {
              $package->field_embededobjects['und'][$k]["inline"] = 1;
            }
          }
        }
      }
    } catch (Exception $e) {
      
    }
  }

  function _buildExternalUrl(& $package) {
    try {
      $extUrl = current($package->field_externalurl);
      $extUrl = current($extUrl);
      if (!isset($extUrl["url"]) || $extUrl["url"] == "") {
        $package->field_externalurl = array();
      }
    } catch (Exception $e) {
      
    }
  }

}
