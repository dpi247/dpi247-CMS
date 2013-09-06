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
      //save node
      
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
    dpm($package_wrapper->getPropertyInfo());
   
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
    $node->type = 'article_package';
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
    if ($wrapper->field_external_reference->value() == NULL){
      $wrapper->field_external_reference->set($value);
    } else {
      $wrapper->field_external_reference_2->set($value);
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
    //@todo
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
    //@todo
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
    //@todo
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
          }
        }
        $embeds_list[$order] = $atom;
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
  * Set LinkLists and add it to the wrapper
  * @param DOM $dom
  * @param EntityDrupalWrapper $wrapper
  * @param Object $entity
  */
  public function buildLinksLists($dom, $wrapper, $entity){
    //@todo
    $dispatcher = $this->getDispatcher();
    $linkslist = $dispatcher->processList($dom, $wrapper, $entity);
  }
  /**
   * 
   * Set PhotoObjectType
   * @param DOM $dom
   * @param EntityDrupalWrapper $package_wrapper
   * @param Object $entity
   */
  public function buildPhotoObjectType($dom, $package_wrapper, $entity){
    $atom = $this->contructEntity($dom, 'scald_atom');
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
    $file = $this->createFile($value, scald_atom_thumbnail_path('image'));
    $wrapper->scald_thumbnail->set((array) $file);
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
  
}