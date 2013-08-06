<?php 

class WallyXmlV1 extends Importer{
  /**
   * 
   * Intercept dom at element <Package>
   * @param DOM $dom
   */
  function build_package($dom){
    $package = $this->constructPackage($dom);
    dsm($package);
    
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
    //setfields
    return $package;
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
   * Enter description here ...
   */
  protected function createOrLoadEntityPackage($dom){
    $node = new stdClass();
    $node->type = 'article-package';
    node_object_prepare($node);
    
    // @TODO Demander à dimitri pourquoi dans wallymport 6 on filtre l'external reference par filter_xss 
    $external_reference = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
    $temp_node = dpicontenttypes_api_getnodebyexternalreference($external_reference);
    

    $action = 'CreateReplace';
    if ($dom->hasAttribute('Action')){
      $action = $dom->getAttribute('Action');
    }
    
    switch($action){
      case 'Delete':
        if ($temp_node != NULL){
          //delete node
        } else {
          //indiquer erreur
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
          //cckdestinations_force_dnc($node);
        }
        break;
      case 'Update':
        if ($temp_node != NULL){
          $node = $temp_node;
          //cckdestinations_force_dnc($node);
        } else {
          //error
          $error_msg = t('The package doesn\'t exist. It can ben updated. Package rejected.');
          
        }
        break;
    }
    
    return $node;
  }
}