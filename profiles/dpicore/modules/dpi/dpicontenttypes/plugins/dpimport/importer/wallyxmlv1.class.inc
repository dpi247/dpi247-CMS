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
    dsm($package);
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
    $disatcher = $this->getDispatcher();
    $package = $this->createOrLoadEntity($dom);
    dsm($package);
    $package_wrapper = entity_metadata_wrapper('node', $package);
    dpm($package_wrapper->getPropertyInfo());
    if (!Logger::error()){
      $this->setPackageAttributes($dom, $package_wrapper);
      if (!Logger::error()){
        dsm($package_wrapper);
        //$disatcher->processChildren($dom, $package_wrapper);
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
    
    //@todo Demander à dimitri pourquoi dans wallymport 6 on filtre l'external reference par filter_xss 
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    $temp_node = dpicontenttypes_api_getnodebyexternalreference($external_reference);
    

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

    $node->dpiproperties = new DPIPropertiesEntity(array(), 'dpiproperties');
    return $node;
  }
  
  /**
   * 
   * Get attributes of current DOM and set its to the package
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  protected function setPackageAttributes($dom, $package){
    //Language
    $package->language->set($dom->getAttributeNode('Language')->value);
    //@todo path_autho check alias
    
    $this->buildEditorialPublicationDate($dom, $package);
    $this->buildEditorialUpdateDate($dom, $package);
    $this->buildAutopublishDate($dom, $package);
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
   * @param EntityDrupalWrapper $package
   */
  protected function buildEditorialPublicationDate($dom, $package){
    if ($dom->hasAttribute("PublicationDate")) {
      $editorialpublicationdate = $this->getDateStampFromDateTime($dom->getAttributeNode("PublicationDate")->value);
    } else {
      $editorialpublicationdate = time();
    }
    $package->field_editorialpublicationdate->set($editorialpublicationdate);
  }
  
  /**
   * 
   * Set field editorialupdatedate
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  protected function buildEditorialUpdateDate($dom, $package){
    if ($dom->hasAttribute("ForceLastUpdate") && $this->xmlbooleantophpboolean($dom->getAttribute("ForceLastUpdate"))) {
      if ($dom->hasAttribute("LastUpdateDate")){
        $editorialupdatedate = $this->getDateStampFromDateTime($dom->getAttributeNode("LastUpdateDate")->value);
      } else {
        $editorialupdatedate = time();
      }
      $package->field_editorialupdatedate->set($editorialupdatedate);
    }
  }
  /**
   * 
   * Set field autopublishdate
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  protected function buildAutopublishDate($dom, $package){
    $embargodate = NULL;
    $unpublishdate = NULL;
    if ($dom->hasAttribute("EmbargoDate")) {
      $embargodate = $this->getDateStampFromDateTime($dom->getAttributeNode("EmbargoDate")->value);
    }
    if ($dom->hasAttribute("UnPublishDate")) {
      $unpublishdate = $this->getDateStampFromDateTime($dom->getAttributeNode("UnPublishDate")->value);
    }
    $package->field_autopublishdate->set(array('value' => $embargodate, 'value2' => $unpublishdate));
  }
  
  /**
   * 
   * Set field productID
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  public function buildProductID($dom, $package){
    $value = $dom->nodeValue;
    $package->dpiproperties->product_id = $value;
  }
  
  /**
  *
  * Set field ExternalReference
  * @param DOM $dom
  * @param EntityDrupalWrapper $package
  */
  public function buildExternalReference($dom, $package){
    $value = $dom->nodeValue;
    $package->dpiproperties->external_reference = $value;
  }
  
  /**
  *
  * Set field Source
  * @param DOM $dom
  * @param EntityDrupalWrapper $package
  */
  public function buildSource($dom, $package){
    $value = $dom->nodeValue;
    $package->dpiproperties->source = $value;
  }
  
  /**
   * 
   * Set field Package to title and display title. If a mainstory or an embed textboject exists, it will be erased by it
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  public function buildPackageTitle($dom, $package){
    $value = $dom->nodeValue;
    dsm($package);
    $package->title->set($this->filterHtmlTags($value));
    $package->field_displaytitle->set($value);
  }
  /**
   * 
   * Set field Destination
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  public function buildDestinations($dom, $package){
    //@todo
  }
  /**
  *
  * Set field CommentsAllowed
  * @param DOM $dom
  * @param EntityDrupalWrapper $package
  */
  public function buildCommentsAllowed($dom, $package){
    //@todo
  }
  /**
  *
  * Set field FreeAccess
  * @param DOM $dom
  * @param EntityDrupalWrapper $package
  */
  public function buildFreeAccess($dom, $package){
    //@todo
  }
  /**
  *
  * Set field AutoPublish
  * @param DOM $dom
  * @param EntityDrupalWrapper $package
  */
  public function buildAutoPublish($dom, $package){
    //@todo
    $value = $this->xmlbooleantophpboolean($dom->nodeValue);
    $package->field_autopublish->set($value);
  }
}