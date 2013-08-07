<?php 

class WallyXmlV1 extends Importer{
  /**
   * 
   * Intercept dom at element <Package>
   * @param DOM $dom
   */
  function build_package($dom){
    Logger::logExecution('package', TRUE);
    $package = $this->constructPackage($dom);
    dsm($package);
    if (!Logger::error()){
      //$package->save();
      //save node
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
    $package = $this->createOrLoadEntity($dom);
    $package_wrapper = entity_metadata_wrapper('node', $package);
    dsm($package_wrapper);
    dpm($package_wrapper->getPropertyInfo());
    if (!Logger::error()){
      $this->setPackageAttributes($dom, $package_wrapper);
      //setfields
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
    if (get_class($dom) == 'DOMElement' && $dom->hasAttribute('xsi:type') && $dom->nodeName != 'Package'){
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
    
    return $node;
  }
  
  /**
   * 
   * Get attributes of current DOM and set its to the package
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  function setPackageAttributes($dom, $package){
    //Language
    $package->language->set($dom->getAttributeNode('Language')->value);
    //@todo path_autho check alias
    
    //Set Editorial Publication Date
    $this->buildPublicationDate($dom, $package);
    $this->buildLastUpdateDate($dom, $package);
    
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
  function getDateStampFromDateTime($date){
    $datetime = new DateTime($date);
    return $datetime->getTimestamp();
  }
  
  /**
   * 
   * Set field_editorialpublicationdate
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  function buildPublicationDate($dom, $package){
    if ($dom->hasAttribute("PublicationDate")) {
      $editorialpublicationdate = $this->getDateStampFromDateTime($dom->getAttributeNode("PublicationDate")->value);
    } else {
      $editorialpublicationdate = time();
    }
    $package->field_editorialpublicationdate->set($editorialpublicationdate);
  }
  
  function buildLastUpdateDate($dom, $package){
    dsm($dom->getAttributeNode("ForceLastUpdate")->value);
    if ($dom->hasAttribute("ForceLastUpdate") && $this->xmlbooleantophpboolean($dom->getAttribute("ForceLastUpdate"))) {
      if ($dom->hasAttributeNode("LastUpdateDate")){
        $editorialupdatedate = $this->getDateStampFromDateTime($dom->getAttributeNode("LastUpdateDate")->value);
      } else {
        $editorialupdatedate = time();
      }
      $package->field_editorialupdatedate->set($editorialpublicationdate);
    }
  }
  
}