<?php 

class WallyXmlPreview extends WallyXmlV1{
  /**
  *
  * Intercept dom at element <Package>
  * @param DOM $dom
  * @param EntityDrupalWrapper $package
  */
  public function savePackage($dom, $package){
      //@todo create prenode and set id into class variable to create de render form
    $prenode = new Prenode();
    $prenode->setNode($package);
    $prenode->save();
  }
  
  /**
   * 
   * Return a render in html (preview of the node)
   */
  public function render(){
    return 'cc';
  }
}