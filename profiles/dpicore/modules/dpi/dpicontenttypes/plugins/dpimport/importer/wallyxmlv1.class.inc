<?php 

class WallyXmlV1 extends Importer{
  /**
   * 
   * Intercept dom at element <Package>
   * @param DOM $dom
   */
  public function buildPackage($dom){
    Logger::logExecution('package', TRUE);
    $package = $this->constructPackage($dom);
    if (!Logger::error()){
      $package->save();
    }
    
    //Set the CurrentLog to the previous one to stop children growing.
    Logger::setCurrentToPreviousExecution();
    
    //Indicates that the dispatcher must not process the children
    return FALSE;
  }
  
  /**
   * 
   * Construct the node base on the DOM element
   * @param DOM $dom
   */
  protected function constructPackage($dom){
    $dispatcher = $this->getDispatcher();
    $package = $this->createOrLoadEntity($dom);
    $package_wrapper = entity_metadata_wrapper('node', $package);
    //dpm($package_wrapper->getPropertyInfo());
    if (!Logger::error()){
      $this->setPackageAttributes($dom, $package_wrapper, $package);
      if (!Logger::error()){
        $dispatcher->processChildren($dom, $package_wrapper, $package);
      }
    }
    return $package_wrapper;
  }
  /**
  *
  * Determine the next callback function from the Dom element
  * If the xsi attribute exists set as callback, else set nodeName as callback
  * @param DOM $dom
  * @return String $callback
  */
  public function getCallback($dom){
    $callback = strtolower($dom->nodeName);
    if (get_class($dom) == 'DOMElement' && $dom->hasAttribute('xsi:type') && $dom->nodeName != 'Package' && $dom->getAttribute('xsi:type') != 'xs:string'){
      $callback = strtolower($dom->getAttributeNode('xsi:type')->value);
    }
    return $callback;
  }
  
  /**
   * 
   * Create or load an entity based on it's exeternal reference
   * @param DOM $dom
   * @return Object $node
   */
  protected function createOrLoadEntityPackage($dom){
    $node = new stdClass();
    $node->type = 'package';
    node_object_prepare($node);
    $node->dpiproperties = new DPIPropertiesEntity(array(), 'dpiproperties');
    
    //@todo Demander à dimitri pourquoi dans wallymport 6 on filtre l'external reference par filter_xss 
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    $temp_node = dpicontenttypes_api_getnodebyexternalreference($external_reference);
    $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
    if ($dpiproperties !== FALSE){
      $temp_node->dpiproperties = $dpiproperties;
    }
    
    $action = 'CreateReplace';
    if ($dom->hasAttribute('Action')){
      $action = $dom->getAttribute('Action');
    }
    
    switch($action){
      case 'Delete':
        if ($temp_node != NULL){
          //@todo delete node
        } else {
          Logger::logError('importer', 0, 'xmldelete', t('The package doesn\'t exist. It can\'t be deleted. Package rejected.'));
        }
        break;
      case 'CreateReplace':
        if ($temp_node != NULL){
          $node->nid = $temp_node->nid;
          $node->vid = $temp_node->vid;
        }
        break;
      case 'CreateUpdate':
        if ($temp_node != NULL){
          $node = $temp_node;
          //@todo cckdestinations_force_dnc($node);
        }
        break;
      case 'Update':
        if ($temp_node != NULL){
          $node = $temp_node;
          //@todo cckdestinations_force_dnc($node);
        } else {
          Logger::logError('importer', 0, 'xmlupdate', t('The package doesn\'t exist. It can\'t ben updated. Package rejected.'));
        }
        break;
    }

    return $node;
  }
  
  /**
   * 
   * Get attributes of current DOM and set it to the package
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function setPackageAttributes($dom, $wrapper, $entity){
    //Language
    $wrapper->language->set($dom->getAttributeNode('Language')->value);
    //@todo path_autho check alias
    
    $this->buildEditorialPublicationDate($dom, $wrapper, $entity);
    $this->buildEditorialUpdateDate($dom, $wrapper, $entity);
    $this->buildAutopublishDate($dom, $wrapper, $entity);
  }
  
  /**
  *
  * Get attributes of current DOM and set it to the Photoojbecttype
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  protected function setPhotoObjectTypeAttributes($dom, $wrapper, $entity){
  
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }
  /**
   * 
   * Get attributes of current DOM and set it to the object
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function setObjectAttributes($dom, $wrapper, $entity){
    if ($dom->hasAttribute('EmbeddedObjectsOrder')){
      $entity->embeddedobjectsorder = $dom->getAttribute('EmbeddedObjectsOrder');
    }
  }
  /**
   * 
   * Return the timestamp of the datetime
   * Ceci retourne le timestamp en UTC
   * Exemple avec la date 2013-06-14T09:20:41+02:00 il retourne 1371194441 
   * qui donne pour date('d/m/Y H:i:s e', 1371194441) -> 14/06/2013 09:20:41 Europe/Brussels (e = timezone)
   * ou 14/06/2013 07:20:41 UTC Ce qui est correct donc merci de vérifier vos timezone avant de modifier cette fonction.
   * @param String $datetime
   */
  protected function getDateStampFromDateTime($date){
    $datetime = new DateTime($date);
    return $datetime->getTimestamp();
  }
  
  /**
   * 
   * Set field_editorialpublicationdate
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function buildEditorialPublicationDate($dom, $wrapper, $entity){
    if ($dom->hasAttribute("PublicationDate")) {
      $editorialpublicationdate = $this->getDateStampFromDateTime($dom->getAttributeNode("PublicationDate")->value);
    } else {
      $editorialpublicationdate = time();
    }
    $wrapper->field_editorialpublicationdate->set($editorialpublicationdate);
  }
  
  /**
   * 
   * Set field editorialupdatedate
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function buildEditorialUpdateDate($dom, $wrapper, $entity){
    //We don't save the editorial update date on new node.
    if ($wrapper->nid->value() != NULL){
      if ($dom->hasAttribute("ForceLastUpdate") && $this->xmlbooleantophpboolean($dom->getAttribute("ForceLastUpdate"))) {
        if ($dom->hasAttribute("LastUpdateDate")){
          $editorialupdatedate = $this->getDateStampFromDateTime($dom->getAttributeNode("LastUpdateDate")->value);
        } else {
          $editorialupdatedate = time();
        }
        $wrapper->field_editorialupdatedate->set($editorialupdatedate);
      }
    }
  }
  /**
   * 
   * Set field autopublishdate
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function buildAutopublishDate($dom, $wrapper, $entity){
    $embargodate = NULL;
    $unpublishdate = NULL;
    if ($dom->hasAttribute("EmbargoDate")) {
      $embargodate = $this->getDateStampFromDateTime($dom->getAttributeNode("EmbargoDate")->value);
    }
    if ($dom->hasAttribute("UnPublishDate")) {
      $unpublishdate = $this->getDateStampFromDateTime($dom->getAttributeNode("UnPublishDate")->value);
    }
    $wrapper->field_autopublishdate->set(array('value' => $embargodate, 'value2' => $unpublishdate));
  }
  
  /**
   * 
   * Set field productID
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildProductID($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $wrapper->field_product_id->set($value);
  }
  
  /**
  *
  * Set field ExternalReference
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildExternalReference($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    //Astuce pour empecher que l'external reference du textobject remplace celle du package et soit mise en external_reference_2
    if (!isset($entity->external_reference_set)){
      $wrapper->field_external_reference->set($value);
      Logger::logExecution('external_reference', FALSE, 1, '000', $value);
      $entity->external_reference_set = TRUE;
    } else {
      $wrapper->field_external_reference_2->set($value);
      Logger::logExecution('external_reference_2', FALSE, 1, '000', $value);
    }
  }
  
  /**
  *
  * Set field Source
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildSource($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $wrapper->field_source->set($value);
  }
  
  /**
   * 
   * Set field Package to title and display title. If a mainstory or an embed textboject exists, it will be erased by it
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildPackageTitle($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->title->set($this->filterHtmlTags($value));
    $wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
  }
  
  /**
   * 
   * Set field Destination
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildDestinations($dom, $wrapper, $entity){
    //@todo s'occuper des destinations multiples
    //@todo terminer avec le wrapper
    $term = array();
    $result = array();
    $vid = dpi_variable_get('dpidestinations_destination_vid', NULL);
    
    if ($vid) {
    /*  
      $temp_results = $temp_result = array();

      //$hierarchies = wallyctools_get_hierarchies();
      foreach ($dom->getElementsByTagName('Destination') as $destination) {
        dsm($destination->nodeName);
        $path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
        $terms = dpicontenttypes_api_taxonomy_get_term_by_path($path, $vid);
        
        if ($terms) {
          if ($destination->hasAttribute("Position")) {
            $temp_result['target'] = $destination->getAttributeNode("Position")->value;
            $temp_result['layout'] = $destination->getAttributeNode("DocumentLayout")->value;
            $temp_result['rank'] = $destination->hasAttribute("DestinationRank") ? $destination->getAttributeNode("DestinationRank")->value : 'DNC';
          } else {
            $temp_result['target'] = 'none';
            $temp_result['layout'] = 'none';
            $temp_result['rank'] = 0;
          }
    
          // As we check the taxonomy full path and specify a vocabulary
          // we can only have one result.
          $temp_result['tid'] = $terms[0]->tid;
          $temp_results[] = $temp_result;
          
        } else {
          drupal_set_message(t('Path "@path" not found in vocabulary (@vid). Destination rejected.', array('@path' => $path, '@vid' => $vid)), 'notice');
        }
      }
    /*
      $check_double = array();
      $i = 0;
      foreach ($temp_results as $temp_result) {
        if (!in_array($temp_result['tid'].':'.$temp_result['target'], $check_double)) {
          $check_double[] = $temp_result['tid'].':'.$temp_result['target'];
    
          if ($actual_dest = _wallyimportpackageV18_find_destination($node->field_destinations, $temp_result)) {
            $temp_result['rank'] = ($temp_result['rank'] == 'DNC') ? 'DNC'.$actual_dest['rank'] : $temp_result['rank'];
            $temp_result['time_rank'] = $actual_dest['time_rank'];
          }
    
          if ($temp_result['rank'] == 'DNC') {
            $temp_result['rank'] = 0;
          }
    
          $result[$i] = $temp_result;
          $i++;
        }
      }
    
      // If no destinations at all > Package can't be created.
      if (count($result)) {
        $node->field_destinations = $result;
      } else {
        // @TODO: FATAL ERROR - PACKAGE DONE - NEED CHECK
        $node->rejected = '';
        $error_msg = t('No destination AT ALL for current package. Package rejected.');
        drupal_set_message($error_msg,'error');
        $log['error'] = TRUE;
        $log['error_msg'] = $error_msg;
        return '';
      }*/
    } else {
      drupal_set_message(t('Destination Path vocabulary not set. See settings.'),'error');
      return '';
    }
  }
  
  /**
  *
  * Set field CommentsAllowed
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildCommentsAllowed($dom, $wrapper, $entity){
    switch ($dom->nodeValue) {
      case "Disabled":
        $status = 0;
        break;
      case "Read Only":
        $status = 1;
        break;
      case "Read-Write":
        $status = 2;
        break;
      default:
        $status = 0;
      break;
    }
    $wrapper->comment->set($status);
  }
  
  /**
  *
  * Set field FreeAccess
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildFreeAccess($dom, $wrapper, $entity){
    //@todo
  }
  
  /**
  *
  * Set field AutoPublish
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildAutoPublish($dom, $wrapper, $entity){
    $value = $this->xmlbooleantophpboolean($dom->nodeValue);
    $wrapper->field_autopublish->set($value);
  }
  
  /**
  *
  * Set field Package Layout
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildPackageLayout($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $vid = dpi_variable_get('dpi_packagelayout', NULL);
    $vocabulary = taxonomy_vocabulary_load($vid);
    $vocabulary_name = $vocabulary->machine_name;
    $item = taxonomy_get_term_by_name($value, $vocabulary_name);
    if (!empty($item)){
      $wrapper->field_packagelayout->set(array_shift($item));
    } else {
      Logger::logWarning('importer', 2, 'packagelayout', t('The package layout !package_layout can not be found', array('!package_layout' => $value)));
    }
  }
  
  /**
  *
  * Set field ExternalURI
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildExternalURI($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $wrapper->field_externalurl->set(array('url' => $value));
  }
  
  /**
  *
  * Set field LastUrlAlias
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildLastURLAlias($dom, $wrapper, $entity){
    //@todo
  }
  
  /**
  *
  * Set field MainStory
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildMainStory($dom, $wrapper, $entity){
    $dispatcher = $this->getDispatcher();
    $dispatcher->processChildren($dom, $wrapper, $entity);
  }
 
  /**
  *
  * Set field Copyright
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildCopyright($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $wrapper->field_copyright->set($value);
  }
  /**
  *
  * Set field Marker
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildMarker($dom, $wrapper, $entity){
    //@todo
  }
  
  /**
  *
  * Set field TextBarette
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTextBarette($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textbarette->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field TextForeTitle
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTextForeTitle($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textforetitle->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field TextSubTitle
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTextSubTitle($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textsubtitle->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field TextTitle
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTextTitle($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->title->set($this->filterHtmlTags($value));
    $wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field TextChapo
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTextChapo($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textchapo->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field TextBody
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTextBody($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_textbody->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field ByLine
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildByLine($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_byline->set(array('value' => $value, 'format' => $format));
  }
  
  /**
   * 
   * Get the default text format
   */
  protected function getDefaultTextFormat(){
    return filter_default_format();
  }
  
  /**
  *
  * Set Embedded Content
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildEmbeddedContent($dom, $wrapper, $entity){
    $dispatcher = $this->getDispatcher();
    $dispatcher->processChildren($dom, $wrapper, $entity);
  }
  /**
  *
  * Set Embedded Objects and add it to the wrapper
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildEmbeddedObjects($dom, $wrapper, $entity){
    //Empty the embededobjects field
    $wrapper->field_embededobjects->set(NULL);
    $dispatcher = $this->getDispatcher();
    $embeds = $dispatcher->processList($dom, $wrapper, $entity);
    $embeds_list = array();
    $i = 100;
    if (!Logger::error()){
      foreach ($embeds as $atom){
        if ($atom != NULL){
          if (isset($atom->embeddedobjectsorder)){
            $order = $atom->embeddedobjectsorder;
          } else {
            $order = $i++;
            while (array_key_exists($order, $embeds_list)){
              $order = $i++;
            }
          }
          $embeds_list[$order] = $atom;
        }
      }
      //Sort the embeds
      ksort($embeds_list);
      foreach ($embeds_list as $atom){
        $embededobject = array(
          'sid' => $atom->sid,
          'type' => $atom->type,
          'provider' => $atom->provider,
          'inline' => '0', //@todo
        );
        $wrapper->field_embededobjects->set($embededobject);
      }
    }
  }
  /**
  *
  * Process LinksLists element
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildLinksLists($dom, $wrapper, $entity){
    
    //Empty the linkslist field
    $wrapper->field_linkslists->set(NULL);
    
    $dispatcher = $this->getDispatcher();
    $linkslists = $dispatcher->processList($dom, $wrapper, $entity);
    
    $linkslist_list = array();
    $i = 100;
    if (!Logger::error()){
      foreach ($linkslists as $atom){
        if ($atom != NULL){
          if (isset($atom->embeddedobjectsorder)){
            $order = $atom->embeddedobjectsorder;
          } else {
            $order = $i++;
            while (array_key_exists($order, $linkslist_list)){
              $order = $i++;
            }
          }
          $linkslist_list[$order] = $atom;
        }
      }
      //Sort the embeds
      ksort($linkslist_list);
      foreach ($linkslist_list as $atom){
        $linkslistobject = array(
          'sid' => $atom->sid,
          'type' => $atom->type,
          'provider' => $atom->provider,
          'inline' => '0', //@todo
        );
        $wrapper->field_linkslists->set($linkslistobject);
      }
    }
  }
  
  /**
   * 
   * Set LinksList and add it to wrapper
   * @param Dom $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildLinksList($dom, $wrapper, $entity){
    Logger::logExecution('linkslist', TRUE);
    $atom = $this->contructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    return $atom;
  }
  
  /**
   * 
   * Set links to the linkslist
   * @param Dom $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildLinks($dom, $wrapper, $entity){
    $wrapper->field_linkitems->set(NULL);
    $i = 100;
    $links_list = array();
    foreach ($dom->getElementsByTagName('Link') as $link){
      if ($link->hasAttribute('EmbeddedObjectsOrder')){
        $order = $link->getAttribute('EmbeddedObjectsOrder');
      } else {
        $order = $i++;
        while (array_key_exists($order, $links_list)){
          $order = $i++;
        }
      }
      $links_list[$order] = array(
        'url' => $link->getElementsByTagName('URI')->item(0)->nodeValue,
        'title' => $link->getElementsByTagName('Title')->item(0)->nodeValue,
      );
      
    }
    //Sort the embeds
    ksort($links_list);
    foreach ($links_list as $link){
      $wrapper->field_linkitems->set(array($link));
    }
  }
  
  /**
  *
  * Create Atom Image
  * @param DOM $dom
  */
  protected function createOrLoadEntityLinksList($dom){
    $atom = scald_atom_defaults('links_list');
    $atom->provider = 'dpicontenttypes';
    $atom->type = 'links_list';
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    if ($temp_atom = dpicontenttypes_api_getatombyexternalreference($external_reference)){
      $atom = $temp_atom;
    }
    return $atom;
  }
  
  /**
   * 
   * Set PhotoObjectType
   * @param DOM $dom
   * @param EntityDrupalWrapper $package_wrapper
   * @param Object $entity
   */
  public function buildPhotoObjectType($dom, $package_wrapper, $entity){
    Logger::logExecution('photobojecttype', TRUE);
    $atom = $this->contructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    return $atom;
  }
  
  /**
  *
  * Create Atom Image
  * @param DOM $dom
  */
  protected function createOrLoadEntityPhotoObjectType($dom){
    $atom = scald_atom_defaults('image');
    $atom->provider = 'scald_image';
    $atom->type = 'image';
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    if ($temp_atom = dpicontenttypes_api_getatombyexternalreference($external_reference)){
      $atom = $temp_atom;
    }
    return $atom;
  }
  
  /**
  *
  * Set field Title
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildTitle($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->title->set($this->filterHtmlTags($value));
    $wrapper->field_displaytitle->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field Caption
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildCaption($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $format = $this->getDefaultTextFormat();
    $wrapper->field_caption->set(array('value' => $value, 'format' => $format));
  }
  
  /**
  *
  * Set field URI
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildURI($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $parsedurl = parse_url($value);
    switch ($parsedurl['scheme']){
      case 'embed':
        $file = $this->createFile($value, scald_atom_thumbnail_path($entity->type));
        switch ($entity->type){
          case 'photo':
            $wrapper->scald_thumbnail->set((array) $file);
            break;
          case 'audio':
            //no wrapper exists for audio so the file is directly set on the entity
            $entity->data['audio_file'] = $file->uri;
            $entity->data['audio_id'] = $file->fid;
            break;
        }
        break;
      case 'http' :
        //Look for a provider for url
        $function_parse_id = $entity->provider.'_parse_id';
        if (function_exists($function_parse_id)){
          $id = $function_parse_id($value, FALSE);
          $wrapper->base_id->set($id);
          $function_info = $entity->provider.'_'.$entity->type;
          if (function_exists($function_info)){
            $info = $function_info($id);
            $file = $this->createFile($info->thumbnail['src'], scald_atom_thumbnail_path($entity->type));
            $wrapper->scald_thumbnail->set((array) $file);
          } else {
            Logger::logWarning('importer', 2, 'thumbnail', t('The thumbnail can not be loaded !uri', array('!uri' => $value)));
          }
        } else {
          Logger::logError('importer', 0, 'provideruri', t('The provider can not analyse the uri !uri', array('!uri' => $value)));
        }
        break;
    }
  }
  /**
  *
  * Set field Rating
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildRating($dom, $wrapper, $entity){
    $value = $dom->nodeValue;
    $vid = dpi_variable_get('dpi_rating', NULL);
    $item = dpicontenttypes_api_taxonomy_get_term_by_path($value, $vid);
    //We take the first match even is there are more.
    $wrapper->field_rating->set($item[0]);
  }
  /**
  *
  * Set field Crops
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildCrops($dom, $wrapper, $entity){
    //@todo
  }
  
  /**
   * 
   * Return a render in json
   */
  public function render(){
    //Set header to json
    drupal_add_http_header('Content-Type', 'application/json');
    
    if (Logger::error()){
      //If an error appears during the import, the global status is set to 0
      $base = new LoggerLog('dpimport', 0, 'dpimport_error', t('There was an error during the import'));
    } else {
      //If no error appears during the import, the global status is set to 1
      $base = new LoggerLog('dpimport', 1, 'dpimport_success', t('The import was proceded with success'));
    }
    //The execution log is attached to the base log.
    $execution = Logger::getLogExecution();
    $base->addChildren($execution);
    
    $json = json_encode($base);
    return $json;
  }
  
  /**
  *
  * Set Link
  * @param DOM $dom
  * @param EntityDrupalWrapper $package_wrapper
  * @param Object $entity
  */
  public function buildLink($dom, $package_wrapper, $entity){
 
    Logger::logExecution('linkbojecttype', TRUE);
    $atom = $this->contructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    return $atom;
  }
  
  /**
  *
  * Create Atom Image
  * @param DOM $dom
  */
  protected function createOrLoadEntityLink($dom){
    $atom = NULL;
    $uri = $dom->getElementsByTagName('URI')->item(0)->nodeValue;
    $provider = dpicontenttypes_api_getprovider($uri);
    if ($provider == NULL){
      Logger::logError('importer', 0, 'provider', t('There is no provider for this uri !uri', array('!uri' => $uri)));
    } else {
      $atom = scald_atom_defaults($provider['type']);
      $atom->provider = $provider['name'];
      $atom->type = $provider['type'];
      $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
      if ($temp_atom = dpicontenttypes_api_getatombyexternalreference($external_reference)){
        $atom = $temp_atom;
      }
    }
    return $atom;
  }
  
  /**
  *
  * Get attributes of current DOM and set it to the Link
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  protected function setLinkAttributes($dom, $wrapper, $entity){
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }
  
  /**
  *
  * Get attributes of current DOM and set it to the linkslist
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  protected function setLinksListAttributes($dom, $wrapper, $entity){
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }
  
  /**
  *
  * Set AudioObjectType
  * @param DOM $dom
  * @param EntityDrupalWrapper $package_wrapper
  * @param Object $entity
  */
  public function buildAudioObjectType($dom, $package_wrapper, $entity){
    Logger::logExecution('photobojecttype', TRUE);
    $atom = $this->contructEntity($dom, 'scald_atom');
    Logger::setCurrentToPreviousExecution();
    //return $atom;
  }
  
  /**
   *
   * Create Atom Audio
   * @param DOM $dom
   */
  protected function createOrLoadEntityAudioObjectType($dom){
    $atom = scald_atom_defaults('audio');
    $atom->provider = 'scald_audio';
    $atom->type = 'audio';
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    if ($temp_atom = dpicontenttypes_api_getatombyexternalreference($external_reference)){
      $atom = $temp_atom;
    }
    return $atom;
  }
  /**
  *
  * Get attributes of current DOM and set it to the Audioojbecttype
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  protected function setAudioObjectTypeAttributes($dom, $wrapper, $entity){
  
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }
}