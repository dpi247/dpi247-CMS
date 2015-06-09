<?php

require_once 'packagehelper.class.inc';

class WallyXmlV1 extends PackageHelper {

  /**
   * Construct the node base on the DOM element
   * Surcharge de la méthode car besoin d'un remplacement des inlines dans le textbody
   * @param DOM $dom
   */
  protected function constructPackage($dom) {
    $dispatcher = $this->getDispatcher();
    $package = $this->createOrLoadEntity($dom);
    $package_wrapper = entity_metadata_wrapper('node', $package);
    if (!Logger::error()) {
      // Allow to alter entity after process
      module_invoke_all('dpimport_entity_beforeprocess', $package_wrapper, $package, 'node', $this, $dispatcher);

      $this->setPackageAttributes($dom, $package_wrapper, $package);
      if (!Logger::error()) {
        $dispatcher->processChildren($dom, $package_wrapper, $package);
      }
      
      /* Change external reference into the textbody to be recognize by the system */
      if(isset($package->tmpinline) && !empty($package->tmpinline)){
        $field_textbody = field_get_items('node', $package, 'field_textbody');
        if(is_array($field_textbody) && !empty($field_textbody) && isset($field_textbody[0]['value'])){
          $text = $field_textbody[0]['value'];
          $tag_regex = "/<ccix:inline(.*?)(<\/ccix:inline(\s.*)?>|\/>)/i";
          $tag_matches = array();          
          preg_match_all($tag_regex, $text, $tag_matches);
          if (!empty($tag_matches[0])) {
            $tmp_inline = array();
            foreach ($tag_matches[0] as $match_id => $match_val) {
              $tags_info = $this->cci_get_tag_value(trim($tag_matches[1][$match_id]));
              if(isset($tags_info['extref']) && $tags_info['extref']!=""){
                if(is_array($package->tmpinline)){
                  foreach($package->tmpinline as $sid => $inline_infos){
                    if($tags_info['extref'] == $inline_infos['body_ext_ref']){
                      try {
                        $text = str_replace($tags_info['extref'], $inline_infos["external_reference"], $text);
                        /* Sav new textbody information */                      
                        $format = $this->getDefaultTextFormat();
                        $package_wrapper->field_textbody->set(array(
                          'value' => $text,
                          'format' => $format
                        ));
                        $tmp_inline[$sid] = $inline_infos + array(
                          "cci_inline_true_value" => $match_val,
                        );                        
                      }
                      catch (Exception $exc) {
                        Logger::logWarning('importer', 2, 'inline package', t('Inline pacakage have problème with external reference %extref', array(
                          '%extref' => $tags_info['extref']
                        )));
                      }
                      break;
                    }
                  }
                }                
              }
            }
            $package->tmpinline = $tmp_inline;
          }          
        }
      }
      
      // Allow to alter entity after process
      module_invoke_all('dpimport_entity_postprocess', $package_wrapper, $package, 'node', $this, $dispatcher);
    }
    return $package;
  }  
  
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
      $this->_save_external_package_informations($package);
      
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
    if ($dom->hasAttribute("EmbargoDate")) {
      $embargodate = $this->getDateStampFromDateTime($dom->getAttributeNode("EmbargoDate")->value);
      if ($embargodate > time()){
        $entity->status = 0;
      } else {
        $entity->status = 1;
      }
    } else {
      $embargodate = time();
      $entity->status = 1;
    }
    if ($dom->hasAttribute("UnPublishDate")) {
      $unpublishdate = $this->getDateStampFromDateTime($dom->getAttributeNode("UnPublishDate")->value);
      if ($unpublishdate < time()){
        $entity->status = 0;
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
            $dest ['rank'] = $destination->hasAttribute("DestinationRank") ? ($destination->getAttributeNode("DestinationRank")->value != 0) ? $destination->getAttributeNode("DestinationRank")->value : "DNC" : 'DNC';
            /* need to reset virtual rank */
            $sql = "SELECT field_destinations_virtual_rank as virtual_rank FROM field_data_field_destinations WHERE field_destinations_tid=:tid AND field_destinations_target=:target AND entity_id=:eid";
            $rez = db_query($sql, array(
              ':tid' => $dest ['tid'],
              ':target' => $dest ['target'],
              ':eid' => $wrapper->nid->value(),
            ))->fetchObject();
            $dest ['virtual_rank'] = $rez->virtual_rank;
          }
          else {
            // If destination does not exists and no rank -> default
            $dest ['rank'] = $destination->hasAttribute("DestinationRank") ? (intval($destination->getAttributeNode("DestinationRank")->value) != 0) ? $destination->getAttributeNode("DestinationRank")->value : 'default' : 'default';
          }
          // Add destination only once. If a destinations with the same tid and target exists, the destination is not readded
          if (!array_key_exists($dest ['tid'] . '-' . $dest ['target'], $keys)) {
            $keys [$dest ['tid'] . '-' . $dest ['target']] = $dest ['tid'] . '-' . $dest ['target'];
            $destination_list [] = $dest;
          }
        }
        else {          
          /* Destinations without target */
          $dest ['tid'] = $terms [0]->tid;
          $dest ['target'] = 'none';
          $dest ['layout'] = $destination->getAttributeNode("DocumentLayout")->value;
          $dest ['rank'] = $destination->hasAttribute("DestinationRank") ? $destination->getAttributeNode("DestinationRank")->value : 0;
          $destination_list [] = $dest;
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
    
    /* Manage package layout */
    if($value == 'Hors-texte'){
      $item = taxonomy_get_term_by_name($value, "package_type");
      if (!empty($item)) {
        $wrapper->field_packagetype->set(array_shift($item));
      }else {
        Logger::logWarning('importer', 2, 'packagetype', t('The package layout !package_layout can not be found', array(
          '!package_layout' => $value
        )));
      }
    }else{
      $item = taxonomy_get_term_by_name("Article", "package_type");
      if (!empty($item)) {
        $wrapper->field_packagetype->set(array_shift($item));
      }else {
        Logger::logWarning('importer', 2, 'packagetype', t('The package layout !package_layout can not be found', array(
          '!package_layout' => $value
        )));
      }
    }
    
    /* Manage package type */
    if($value != 'Hors-texte' && $value != 'Article' && $value != 'Aucun'){
      $vid = dpi_variable_get('dpi_packagelayout', NULL);
      $vocabulary = taxonomy_vocabulary_load($vid);
      $vocabulary_name = $vocabulary->machine_name;
      $item = taxonomy_get_term_by_name($value, $vocabulary_name);
      if (!empty($item)) {
        $wrapper->field_packagelayout->set(array_shift($item));
      }
      else {
        Logger::logWarning('importer', 2, 'packagelayout', t('The package layout !package_layout can not be found', array(
          '!package_layout' => $value
        )));
      }
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
        if(isset($atom->temp_crops) && $atom->temp_crops!=null){
          $entity->temp_crops[$atom->sid] = $atom->temp_crops;
        }
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
      
      /* Récupération de l'atom_package lié au nid de l'external reference */
      $query = "SELECT dp.id, dp.external_reference FROM {dpicontenttypes_properties} dp WHERE dp.entity='scald_atom' AND dp.id IN (SELECT sa.sid FROM {scald_atoms} sa WHERE sa.title IN (SELECT n.title FROM {node} n, {dpicontenttypes_properties} dpp WHERE n.nid=dpp.id AND dpp.entity='node' AND dpp.external_reference=:extref )) LIMIT 1";
      $args = array(
        ':extref' => $tmpExternalRef
      );      
      $result = db_query($query, $args)->fetchObject();
      
      if (isset($result) && $result != "") {        
        /* Sav information about external reference for inline elements */
        $entity->tmpinline[$result->id] = array(
          'sid' => $result->id,
          'external_reference' => $result->external_reference,
          'body_ext_ref' => $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue,
        );
        
        /* Return atom for nex step */
        $scald_atom = scald_atom_load($result->id);
        if(isset($scald_atom) && is_object($scald_atom)){
          $atom = new stdClass();
          $atom->sid = $scald_atom->sid;
          $atom->type = $scald_atom->type;
          $atom->provider = $scald_atom->provider;
          return $atom;
        }        
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
      
      if(isset($package->field_embededobjects)){
        $embeddedObjects = current($package->field_embededobjects);
      }        

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
      if(isset($package->field_externalurl) && is_array($package->field_externalurl)){
        $extUrl = current($package->field_externalurl);
        if(isset($package->field_externalurl) && is_array($package->field_externalurl)){
          $extUrl = current($extUrl);
          if (!isset($extUrl["url"]) || $extUrl["url"] == "") {
            $package->field_externalurl = array();
          }
        }        
      }
    } catch (Exception $e) {
      
    }
  }

  function buildTaxonomy($dom, $package_wrapper, $entity){
    $dispatcher = $this->getDispatcher();
    $dispatcher->processChildren($dom, $package_wrapper, $entity);
  }
  
  function buildVocabulary($dom, $package_wrapper, $entity){
    try {
      $taxonomy_name = $dom->getAttribute('VocabularyName');
      /* glitch for tags that would be free tags */
      if($taxonomy_name == "Tags"){
        $taxonomy_name = "Free Tags";
      }elseif ($taxonomy_name == "Authors") {
        $taxonomy_name = "DPI Authors";
      }
      $conditions = array('name' => trim($taxonomy_name));
      $vocabularies = taxonomy_vocabulary_load_multiple(FALSE, $conditions);
      
      /* test que le vocabulaire exist bien dans le drupal */
      if($vocabularies && !empty($vocabularies)){
        $dispatcher = $this->getDispatcher();
        $dispatcher->processChildren($dom, $package_wrapper, $entity);
        $global_list_term = &drupal_static('sav_global_list_term');
        if(is_array($global_list_term) && !empty($global_list_term)){
          if($taxonomy_name == "DPI Authors"){
            $taxonomy_name = "Authors";
          }
          $t_machine_name = strtolower($taxonomy_name);
          $t_machine_name = str_replace(' ', '', $t_machine_name);
          $field_name = 'field_'.strtolower($t_machine_name);  
          if(isset($entity->type) && $entity->type != "package"){
            switch($t_machine_name){
              case "authors":
                  $field_name = 'scald_'.strtolower($t_machine_name);
                 break;
              case "thumbnail":
                   $field_name = 'scald_'.strtolower($t_machine_name);
                 break;
            }
          }
          $package_wrapper->$field_name->set($global_list_term);
          drupal_static_reset('sav_global_list_term');
        }
      }else{
        Logger::logWarning('importer', 2, 'pathnotfound', t('Taxonomy term don\'t exist into this instance of drupal : @term' , array(
          '@term' => $taxonomy_name,
        )));
      }      
    }
    catch (Exception $exc) {
      print_r($exc->getMessage());
      Logger::logError('importer', 0, 'provideruri', t('Taxonomy term cannot be imported : ' . $exc->getMessage()));
    }
  }
  
  function buildTerm($dom, $wrapper, $entity, $dom_parent){   
    $global_list_term = &drupal_static('sav_global_list_term', array());    
    $term = $dom->getAttribute('Name');
    $taxonomy_name = $dom_parent->getAttribute('VocabularyName');
    if($taxonomy_name == "Tags"){
      $taxonomy_name = "Free Tags";
    }elseif ($taxonomy_name == "Authors") {
      $taxonomy_name = "DPI Authors";
    }
    $t_machine_name = strtolower($taxonomy_name);
    $t_machine_name = str_replace(' ', '', $t_machine_name);
    $field_name = 'field_' . $t_machine_name;    
    $conditions = array('name' => trim($taxonomy_name));
    $vocabularies = taxonomy_vocabulary_load_multiple(FALSE, $conditions);
    $vocabulary = current($vocabularies);
    
    /* check if term exist */
    $sql = "SELECT tid FROM taxonomy_term_data WHERE vid=:vid AND name=:name";
    $tid = db_query($sql, array(
      ':vid' => $vocabulary->vid,
      ':name' => $term,
    ))->fetchObject();

    /* sav term at the first */
    if(isset($tid) && isset($tid->tid) && is_numeric($tid->tid)){
      $term_taxo = new stdClass();
      $term_taxo->tid = $tid->tid;
    }else{
      $term_taxo = new stdClass();
      $term_taxo->name = $term;
      $term_taxo->vid = $vocabulary->vid;
      taxonomy_term_save($term_taxo);
    }    
    
    /* add new tid into the specifique element */
    $global_list_term[] = $term_taxo->tid;
  } 
  
  function cci_get_tag_value($raw_attributes){
    $tag_values = array();
    $attr_regex = '@\w.*\s?=\s?[\'"].*[\'"]@U';
    $attr_matches = array();
    preg_match_all($attr_regex, $raw_attributes, $attr_matches);

    foreach ($attr_matches[0] as $raw_attr) {
      $expl_attr = explode('=', $raw_attr);
      $tag_values[$expl_attr[0]] = trim($expl_attr[1], '\'"');
    }

    return $tag_values;
  }
  
  function _save_external_package_informations($package){
    if(isset($package->tmpinline) && is_array($package->tmpinline)){
      foreach($package->tmpinline as $sid => $sa_info){
        try {
          $fields = array(
            'nid' => $package->nid,
            'sid' => $sid,
            'ext_ref' => $sa_info['external_reference'],
            'original_cci_link' => $sa_info['cci_inline_true_value'],
          );
          db_insert('dpicontenttypes_inline_export_information') // Table name no longer needs {}
            ->fields($fields)
            ->execute();
        }
        catch (Exception $exc) {
          Logger::logWarning('importer', 2, 'dpicontenttypes_inline_export_information', t('Error with registering of retrieve information. base cci : !type', array(
            '!type' => original_cci_link
          )));
        }
      }
    }
  }
}
