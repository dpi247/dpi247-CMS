<?php 

require_once 'packagehelper.class.inc';

class WallyXmlV1 extends PackageHelper {
  /**
   * Intercept dom at element <Package>
   *
   * @param DOM $dom
   */
  public function buildPackage($dom) {
    // @todo : Pourquoi ne pas passer par la methode constructEntity?

    Logger::logExecution('package', TRUE);
    $package = $this->constructPackage ( $dom );
    if (! Logger::error ()) {
      // Allow to alter entity before save
      $dispatcher = $this->getDispatcher ();
      
//       /* Replace atom package from field_embededobjects to field_atompackage */
//       $package = $this->_buildFieldAtompackage($package);

      $package_wrapper = entity_metadata_wrapper('node', $package);
      module_invoke_all('dpimport_entity_presave', $package_wrapper, $package, 'node', $this, $dispatcher);
      $this->savePackage($dom, $package);

    }
    
    // Set the CurrentLog to the previous one to stop children growing.
    Logger::setCurrentToPreviousExecution ();
    
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
    $external_reference = $dom->getElementsByTagName ( 'ExternalReference' )->item ( 0 )->nodeValue;
    
    $action = 'CreateReplace';
    if ($dom->hasAttribute ( 'Action' )) {
      $action = $dom->getAttribute ( 'Action' );
    }
    
    return $this->_createOrLoadEntityPackage ( $dom, $external_reference, $action );
  }

  /**
   * Create Atom Image
   *
   * @param DOM $dom          
   */
  protected function createOrLoadEntityPhotoObjectType($dom) {
    $external_reference = $dom->getElementsByTagName ( 'ExternalReference' )->item ( 0 )->nodeValue;
    $atom = $this->_createOrLoadEntityPhotoObjectType ( $dom, $external_reference );
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
    $callback = strtolower ( $dom->nodeName );
    if (get_class ( $dom ) == 'DOMElement' && $dom->hasAttribute ( 'xsi:type' ) && $dom->nodeName != 'Package' && $dom->nodeName != 'MainStory' && $dom->getAttribute ( 'xsi:type' ) != 'xs:string') {
      $callback = strtolower ( $dom->getAttributeNode ( 'xsi:type' )->value );
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
    $datetime = new DateTime ( $date );
    return $datetime->getTimestamp ();
  }

  protected function setPackageAttributes($dom, $wrapper, $entity) {
    // Language
    
    // @todo: use global lang
    if ($dom->getAttributeNode ( 'Language' )->value != "") {
      $wrapper->language->set ( $dom->getAttributeNode ( 'Language' )->value );
    } else {
      $wrapper->language->set ( "fr" );
    }
    // @todo path_autho check alias
    $this->buildEditorialPublicationDate ( $dom, $wrapper, $entity );
    $this->buildEditorialUpdateDate ( $dom, $wrapper, $entity );
    $this->buildAutopublishDate ( $dom, $wrapper, $entity );
  }

  /**
   * Set field_editorialpublicationdate
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  protected function buildEditorialPublicationDate($dom, $wrapper, $entity) {
    if ($dom->hasAttribute ( "PublicationDate" )) {
      $editorialpublicationdate = $this->getDateStampFromDateTime ( $dom->getAttributeNode ( "PublicationDate" )->value );
    } else {
      $editorialpublicationdate = time ();
    }
    $wrapper->field_editorialpublicationdate->set ( $editorialpublicationdate );
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
    if ($wrapper->nid->value () != NULL) {
      if ($dom->hasAttribute ( "ForceLastUpdate" ) && $this->xmlbooleantophpboolean ( $dom->getAttribute ( "ForceLastUpdate" ) )) {
        if ($dom->hasAttribute ( "LastUpdateDate" )) {
          $editorialupdatedate = $this->getDateStampFromDateTime ( $dom->getAttributeNode ( "LastUpdateDate" )->value );
        } else {
          $editorialupdatedate = time ();
        }
        $wrapper->field_editorialupdatedate->set ( $editorialupdatedate );
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
    if ($dom->hasAttribute ( "EmbargoDate" )) {
      $embargodate = $this->getDateStampFromDateTime ( $dom->getAttributeNode ( "EmbargoDate" )->value );
    }
    if ($dom->hasAttribute ( "UnPublishDate" )) {
      $unpublishdate = $this->getDateStampFromDateTime ( $dom->getAttributeNode ( "UnPublishDate" )->value );
    }
    $wrapper->field_autopublishdate->set ( array (
      'value' => $embargodate,
      'value2' => $unpublishdate 
    ) );
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
    $wrapper->field_product_id->set ( $value );
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

    if (! isset ( $entity->provider )) {
      // Astuce pour empecher que l'external reference du textobject remplace celle du package et soit mise en external_reference_2
      if (! isset ( $entity->external_reference_set )) {
        $wrapper->field_external_reference->set ( $value );
        Logger::logExecution ( 'external_reference', FALSE, 1, '000', $value );
        $entity->external_reference_set = TRUE;
      } else {
        $wrapper->field_external_reference_2->set ( $value );
        Logger::logExecution ( 'external_reference_2', FALSE, 1, '000', $value );
      }
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
    $wrapper->field_source->set ( $value );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->title->set ( $this->filterHtmlTags ( $value ) );
    $wrapper->field_displaytitle->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $destination_list = array ();
    $keys = array ();
    foreach ( $dom->getElementsByTagName ( 'Destination' ) as $destination ) {
      $dest = array ();
      $path = $destination->getElementsByTagName ( "DestinationPath" )->item ( 0 )->nodeValue;
      $terms = dpicontenttypes_api_taxonomy_get_term_by_path ( $path, $vid );
      if ($terms && $destination->hasAttribute ( "DocumentLayout" )) {
        $dest ['tid'] = $terms [0]->tid;
        if ($destination->hasAttribute ( "Position" )) {
          $dest ['target'] = $destination->getAttributeNode ( "Position" )->value;
          $dest ['layout'] = $destination->getAttributeNode ( "DocumentLayout" )->value;
          if ($wrapper->nid->value () != NULL && $this->destinationexists ( $wrapper, $dest )) {
            // If destination already exists and no rank -> DNC
            $dest ['rank'] = $destination->hasAttribute ( "DestinationRank" ) ? $destination->getAttributeNode ( "DestinationRank" )->value : 'DNC';
          } else {
            // If destination does not exists and no rank -> default
            $dest ['rank'] = $destination->hasAttribute ( "DestinationRank" ) ? $destination->getAttributeNode ( "DestinationRank" )->value : 'default';
          }
        } else {
          $dest ['target'] = 'none';
          $dest ['layout'] = 'none';
          $dest ['rank'] = 0;
        }
        // Add destination only once. If a destinations with the same tid and target exists, the destination is not readded
        if (! array_key_exists ( $dest ['tid'] . '-' . $dest ['target'], $keys )) {
          $keys [$dest ['tid'] . '-' . $dest ['target']] = $dest ['tid'] . '-' . $dest ['target'];
          $destination_list [] = $dest;
        }
      } else {
        Logger::logWarning ( 'importer', 2, 'pathnotfound', t ( 'Path "!path" not found. Destination rejected', array (
          '!path' => $path 
        ) ) );
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
    $term = array ();
    $result = array ();
    $vid = _dpidestinations_variable_get ( 'dpidestinations_section_vid', NULL );
    $package_type = $globalDom->getElementsByTagName ( 'PackageLayout' )->item ( 0 )->nodeValue;
    if ($package_type != 'Hors-texte' || ! isset ( $package_type )) {
      if ($vid) {
        $destination_list = $this->getdestinationsfromdom ( $dom, $vid, $wrapper );
        
        if (count ( $destination_list )) {
          // Add destinations to the package
          $wrapper->field_destinations->set ( $destination_list );
        } else {
          // If no destinations at all > Package can't be created
          Logger::logError ( 'importer', 0, 'nodestination', t ( 'No destination AT ALL for current package.' ) );
        }
      } else {
        Logger::logError ( 'importer', 0, 'novocpath', t ( 'Destination vocabulary not set.' ) );
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
    $wrapper->comment->set ( $status );
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
    $value = $this->xmlbooleantophpboolean ( $dom->nodeValue );
    $wrapper->field_autopublish->set ( $value );
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
    $vid = dpi_variable_get ( 'dpi_packagelayout', NULL );
    $vocabulary = taxonomy_vocabulary_load ( $vid );
    $vocabulary_name = $vocabulary->machine_name;
    if ($value == 'Hors-texte') {
      $vocabulary_name = "package_type";
    }
    $item = taxonomy_get_term_by_name ( $value, $vocabulary_name );
    if (! empty ( $item )) {
      if ($dom->nodeValue == "Hors-texte") {
        $wrapper->field_packagetype->set ( array_shift ( $item ) );
      } else {
        $wrapper->field_packagelayout->set ( array_shift ( $item ) );
      }
    } else {
      Logger::logWarning ( 'importer', 2, 'packagelayout', t ( 'The package layout !package_layout can not be found', array (
        '!package_layout' => $value 
      ) ) );
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
    $wrapper->field_externalurl->set ( array (
      'url' => $value 
    ) );
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
    $dispatcher = $this->getDispatcher ();
    $dispatcher->processChildren ( $dom, $wrapper, $entity );
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
    $wrapper->field_copyright->set ( $value );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_textbarette->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_textforetitle->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_textsubtitle->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->title->set ( $this->filterHtmlTags ( $value ) );
    $wrapper->field_displaytitle->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_textchapo->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_textbody->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_byline->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
  }

  /**
   * Set Embedded Content
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildEmbeddedContent($dom, $wrapper, $entity) {
    $dispatcher = $this->getDispatcher ();
    $dispatcher->processChildren ( $dom, $wrapper, $entity );
  }

  /**
   * Set Embedded Objects and add it to the wrapper
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildEmbeddedObjects($dom, $wrapper, $entity) {
    // Empty the embededobjects field
    $wrapper->field_embededobjects->set ( NULL );
    $dispatcher = $this->getDispatcher ();
    $embeds = $dispatcher->processList ( $dom, $wrapper, $entity );
    $embeds_list = array ();
    $i = 100;
    if (! Logger::error ()) {
      foreach ( $embeds as $atom ) {
        if ($atom != NULL) {
          if (isset ( $atom->embeddedobjectsorder )) {
            $order = $atom->embeddedobjectsorder;
          } else {
            $order = $i ++;
            while( array_key_exists ( $order, $embeds_list ) ) {
              $order = $i ++;
            }
          }
          $embeds_list [$order] = $atom;
        }
      }
      // Sort the embeds
      ksort ( $embeds_list );
      foreach ( $embeds_list as $atom ) {
        $embededobject = array (
          'sid' => $atom->sid,
          'type' => $atom->type,
          'provider' => $atom->provider,
          'inline' => '0'  // @todo
                );
        $wrapper->field_embededobjects->set ( $embededobject );
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
    $wrapper->field_linkslists->set ( NULL );
    
    $dispatcher = $this->getDispatcher ();
    $linkslists = $dispatcher->processList ( $dom, $wrapper, $entity );
    
    $linkslist_list = array ();
    $i = 100;
    if (! Logger::error ()) {
      foreach ( $linkslists as $atom ) {
        if ($atom != NULL) {
          if (isset ( $atom->embeddedobjectsorder )) {
            $order = $atom->embeddedobjectsorder;
          } else {
            $order = $i ++;
            while( array_key_exists ( $order, $linkslist_list ) ) {
              $order = $i ++;
            }
          }
          $linkslist_list [$order] = $atom;
        }
      }
      // Sort the links lists
      ksort ( $linkslist_list );
      foreach ( $linkslist_list as $atom ) {
        $linkslistobject = array (
          'sid' => $atom->sid,
          'type' => $atom->type,
          'provider' => $atom->provider,
          'inline' => '0'  // @todo
                );
        $wrapper->field_linkslists->set ( $linkslistobject );
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
    Logger::logExecution ( 'linkslist', TRUE );
    $atom = $this->constructEntity ( $dom, 'scald_atom' );
    Logger::setCurrentToPreviousExecution ();
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
    $wrapper->field_linkitems->set ( NULL );
    $i = 100;
    $links_list = array ();
    foreach ( $dom->getElementsByTagName ( 'Link' ) as $link ) {
      if ($link->hasAttribute ( 'EmbeddedObjectsOrder' )) {
        $order = $link->getAttribute ( 'EmbeddedObjectsOrder' );
      } else {
        $order = $i ++;
        while( array_key_exists ( $order, $links_list ) ) {
          $order = $i ++;
        }
      }
      $links_list [$order] = array (
        'url' => $link->getElementsByTagName ( 'URI' )->item ( 0 )->nodeValue,
        'title' => $link->getElementsByTagName ( 'Title' )->item ( 0 )->nodeValue 
      );
    }
    // Sort the links
    ksort ( $links_list );
    foreach ( $links_list as $link ) {
      $wrapper->field_linkitems->set ( array (
        $link 
      ) );
    }
  }

  /**
   * Set PhotoObjectType
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $package_wrapper          
   * @param Object $entity          
   */
  public function buildPhotoObjectType($dom, $package_wrapper, $entity) {
    Logger::logExecution ( 'photobojecttype', TRUE );
    $atom = $this->constructEntity ( $dom, 'scald_atom' );
    Logger::setCurrentToPreviousExecution ();
    return $atom;
  }

  /**
   * Set field Title
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $wrapper          
   * @param Object $entity          
   */
  public function buildTitle($dom, $wrapper, $entity){
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
    $format = $this->getDefaultTextFormat ();
    $wrapper->field_caption->set ( array (
      'value' => $value,
      'format' => $format 
    ) );
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
    switch ($parsedurl['scheme']){
      case 'embed':
        $file = $this->createFile($value, scald_atom_thumbnail_path($entity->type));
        switch ($entity->type){
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
        if (function_exists ( $function_parse_id )) {
          $id = $function_parse_id ( $value, FALSE );
          $wrapper->base_id->set ( $id );
          $function_info = $entity->provider . '_' . $entity->type;
          if (function_exists ( $function_info )) {
            $info = $function_info ( $id );
            $file = $this->createFile ( $info->thumbnail ['src'], scald_atom_thumbnail_path ( $entity->type ) );
            $wrapper->scald_thumbnail->set ( ( array ) $file );
          } else {
            Logger::logWarning ( 'importer', 2, 'thumbnail', t ( 'The thumbnail can not be loaded !uri', array (
              '!uri' => $value 
            ) ) );
          }
        } else {
          /* Gestion de la partie provenant de dpiscald - provider multiple */
          module_load_include ( 'inc', 'dpiscald', 'providers/' . $entity->type . '/' . $entity->type );
          $function_parse_id = 'dpiscald_' . $entity->type . '_parse_id';
          if (function_exists ( $function_parse_id )) {
            /* Gestion des video dailymotion - youtube */
            if ($entity->type == "dailymotion" || $entity->type == "youtube" || $entity->type == "vimeo") {
              $value = str_replace ( '\\', '', $value );
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id );
              $function_info = $entity->provider . '_' . $entity->type . '_video';
              if (function_exists ( $function_info )) {
                $info = $function_info ( $id );
                $file = $this->createFile ( $info->thumbnail ['src'], scald_atom_thumbnail_path ( $entity->type ) );
                $wrapper->scald_thumbnail->set ( ( array ) $file );
              }
              /* Gestion de deezer */
            } elseif ($entity->type == "deezer") {
              $entity->provider = 'dpiscald';
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id [1] );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $id [0], $id [1] );
                if ($id [0] == "tracks") {
                  $thumbnail_url = $infos->album->cover;
                } elseif ($id [0] == "album") {
                  $thumbnail_url = $infos->cover;
                } else {
                  $thumbnail_url = $infos->picture;
                }
                $file = $this->createFile ( $thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
                $wrapper->scald_thumbnail->set ( ( array ) $file );
                $entity->data ['url'] = 'http://www.deezer.com/plugins/player?type=' . $id [0] . '&id=' . $id [1];
              }
            } elseif ($entity->type == "soundcloud") {
              $value = str_replace ( '\\', '', $value );
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id );
              $function_info = $entity->provider . '_' . $entity->type . '_audio';
              if (function_exists ( $function_info )) {
                $info = $function_info ( $id );
                if (isset ( $info->thumbnail ['src'] )) {
                  $file = $this->createFile ( $info->thumbnail ['src'], scald_atom_thumbnail_path ( $entity->type ) );
                  $wrapper->scald_thumbnail->set ( ( array ) $file );
                }
                $entity->title = $info->title;
                $entity->data ['kind'] = $info->kind;
                $entity->data ['track_count'] = $info->track_count;
                $entity->data ['permalink_url'] = $info->permalink_url;
              }
            }elseif ($entity->type == "kewego"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id [1] );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $value );
                $entity->data['html'] = $infos->html;
                $entity->title = $infos->title;
                $file = $this->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
                $wrapper->scald_thumbnail->set ( ( array ) $file );
              }
            }elseif ($entity->type == "spotify"){
              $id = $function_parse_id ( $value, FALSE );              
              $wrapper->base_id->set ( $id[0] );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $value, $id[count($id)-1] );
                $entity->title = $infos->name;
                if(isset($infos->album->images[0])){
                  $file = $this->createFile ( $infos->album->images[0]->url, scald_atom_thumbnail_path ( $entity->type ) );
                  $wrapper->scald_thumbnail->set ( ( array ) $file );
                }
                $entity->data['url'] = $infos->uri;
                $entity->data['orig_identifier'] = $value;
              }
            }elseif ($entity->type == "scribblelive"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $value );
                $entity->title = $infos['title'];
                $file = $this->createFile ( $infos['thumbnail_url'], scald_atom_thumbnail_path ( $entity->type ) );
                $wrapper->scald_thumbnail->set ( ( array ) $file );
                $entity->data['id'] = $infos['id'];
                $entity->data['orig_identifier'] = $value;
              }
            }elseif ($entity->type == "twitter"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $value );
                $entity->data['html'] = $infos->html;
                $response = drupal_http_request($value);
                preg_match('/<title>(.*?)<\/title>/', $response->data, $title);
                $title = html_entity_decode($title[1], ENT_QUOTES);
                $entity->title = substr($title, 0, 126);
                preg_match('/<img class="avatar js-action-profile-avatar" src="(.*?)"/', $response->data, $thumbnail_url);
                $thumbnail_url = $thumbnail_url[1];
                $file = $this->createFile ( $thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
                $wrapper->scald_thumbnail->set ( ( array ) $file );
              }
            }elseif($entity->type == "picktochart"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $value );
                $entity->title = $infos['title'];
                $entity->data['id'] = $infos['id'];
                $file = $this->createFile ( $infos['thumbnail_url'], scald_atom_thumbnail_path ( $entity->type ) );
                $wrapper->scald_thumbnail->set ( ( array ) $file );
              }
            }elseif ($entity->type == "storify"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( $id );
              $function_info = 'dpiscald_' . $entity->type . '_get_media_infos_from_url';
              $url = $value;
              if (startsWith($url,'https')){
                $url = explode('https://storify.com/',$url);
              } else {
                $url = explode('http://storify.com/',$url);
              }               
              $url = $url[1];
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $url );
                $infos = $infos->content;                
                $entity->title = $infos->title;                
                $entity->data['url'] = $url;
                $entity->data['orig_identifier'] = $value;   
                if(isset($infos->thumbnail)){
                  $file = $this->createFile ( $infos->thumbnail, scald_atom_thumbnail_path ( $entity->type ) );
                  $wrapper->scald_thumbnail->set ( ( array ) $file );
                }
              }
            }elseif($entity->type == "timelinejs"){    
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( md5($id) );
              $entity->data['url'] = $value;              
              $entity->data['orig_identifier'] = $value;
              $thumbnail_url = 'http://www.outsystems.com/OSCore/Component_Image.aspx?ProjectImageId=1251';
              $file = $this->createFile ($thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
              $wrapper->scald_thumbnail->set ( ( array ) $file );
            }elseif ($entity->type == "flickr"){
              $id = $function_parse_id ( 'photo', $value );
              $wrapper->base_id->set ( $id );
              $function_info = $entity->provider . '_' . $entity->type . '_image';
              if (function_exists ( $function_info )) {
                $info = $function_info ( $id );
                $element = new stdClass();
                dpiscald_flickr_update_atom($element, $info, TRUE);
                $wrapper->base_id->set( $element->base_id );
                if(isset($element->scald_thumbnail[LANGUAGE_NONE][0])){
                  $wrapper->scald_thumbnail->set( ( array ) $element->scald_thumbnail[LANGUAGE_NONE][0] );
                }             
                if(isset($element->title->_content)){
                  $entity->title = $element->title->_content;
                }     
                $entity->data['orig_identifier'] = $value;       
              }
            }elseif ($entity->type == "googlemap"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( md5($id) );
              $function_info = $entity->provider . '_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $info = $function_info ( $id );
                if(isset($info->title)){
                  $entity->title = $info->title;
                }
                if(isset($info->scald_thumbnail['src'])){
                  $file = $this->createFile ( $info->scald_thumbnail['src'], scald_atom_thumbnail_path ( $entity->type ) );
                  $wrapper->scald_thumbnail->set ( ( array ) $file );
                }
                $entity->data['urlN'] = $info->data['urlN'];
              }
            }elseif($entity->type == "equipefr"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( md5($id) );
              $function_info = $entity->provider . '_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $id );
                $entity->title = $infos['title'];
                if(isset($infos['thumbnail_url'])){
                  $file = $this->createFile ( $infos['thumbnail_url'], scald_atom_thumbnail_path ( $entity->type ) );
                  $wrapper->scald_thumbnail->set ( ( array ) $file );
                }
                $entity->data['id'] = $infos['id'];
                $entity->data['orig_identifier'] = $value;
              }
            }elseif ($entity->type == "embedly"){
              $id = $function_parse_id ( $value, FALSE );
              $wrapper->base_id->set ( md5($id) );
              $function_info = $entity->provider . '_' . $entity->type . '_get_media_infos_from_url';
              if (function_exists ( $function_info )) {
                $infos = $function_info ( $id );
                $entity->title = $infos->title;
                if(isset($infos->thumbnail_url)){
                  $file = $this->createFile ( $infos->thumbnail_url, scald_atom_thumbnail_path ( $entity->type ) );
                  $wrapper->scald_thumbnail->set ( ( array ) $file );
                }
                $entity->data['url'] = $infos->url;
                $entity->data['orig_identifier'] = $value;
              }
            }
          } else {
            Logger::logError ( 'importer', 0, 'provideruri', t ( 'The provider can not analyse the uri !uri', array (
              '!uri' => $value 
            ) ) );
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
    $vid = dpi_variable_get ( 'dpi_rating', NULL );
    $item = dpicontenttypes_api_taxonomy_get_term_by_path ( $value, $vid );
    // We take the first match even is there are more.
    $wrapper->field_rating->set ( $item [0] );
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
  }

  /**
   * Set Link
   *
   * @param DOM $dom          
   * @param EntityDrupalWrapper $package_wrapper          
   * @param Object $entity          
   */
  public function buildLink($dom, $package_wrapper, $entity) {
    $uri_info = $dom->getElementsByTagName ( 'URI' )->item ( 0 )->nodeValue;
    if (strstr ( $uri_info, 'extref://' ) != FALSE) {
      $tmpExternalRef = substr ( $uri_info, 9 );
      $query = "SELECT sa.sid FROM {scald_atoms} sa WHERE sa.title IN (SELECT n.title FROM {node} n, {dpicontenttypes_properties} dpp WHERE n.nid=dpp.id AND dpp.entity='node' AND dpp.external_reference=:extref ) LIMIT 1";
      $args = array (
        ':extref' => $tmpExternalRef 
      );
      $result = db_query ( $query, $args )->fetchField ();
      if (isset ( $result ) && $var = scald_atom_load ( intval ( $result ) )) {
        $atom = new stdClass ();
        $atom->sid = $result;
        $atom->type = $var->type;
        $atom->provider = $var->provider;
        return $atom;
      }
    } else {
      Logger::logExecution ( 'linkbojecttype', TRUE );
      $atom = $this->constructEntity ( $dom, 'scald_atom' );
      Logger::setCurrentToPreviousExecution ();
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
    Logger::logExecution ( 'photobojecttype', TRUE );
    $atom = $this->constructEntity ( $dom, 'scald_atom' );
    Logger::setCurrentToPreviousExecution ();
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
    $field_embededobjects = array ();
    $field_atompackage = array ();
    foreach ( $package->field_embededobjects as $klang => $elements ) {
      foreach ( $elements as $element ) {
        if ($element ['type'] == "atom_package") {
          if (! isset ( $field_atompackage [$klang] ))
            $field_atompackage [$klang] = array ();
          $field_atompackage [$klang] [] = $element;
        } else {
          if (! isset ( $field_embededobjects [$klang] ))
            $field_embededobjects [$klang] = array ();
          $field_embededobjects [$klang] [] = $element;
        }
      }
    }
    if (! empty ( $field_embededobjects ))
      $package->field_embededobjects = $field_embededobjects;
    if (! empty ( $field_atompackage ))
      $package->field_atompackage = $field_atompackage;
    return $package;
  }
}

