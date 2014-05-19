<?php 

class WallyXmlPreview extends WallyXmlV1 {

  protected $prenode;

  /**
   * Intercept dom at element <Package>
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $package
   */
  public function savePackage($dom, $package) {
    // @todo : Create prenode and set ID into class variable to create de render form
    $prenode = new Prenode();
    $package->nid = $prenode->getID();
    $package->entity_id = $prenode->getID();
    $prenode->setNode($package);
    $prenode->save();
    $this->prenode = $prenode;
  }

  /**
   * Return a render in html (preview of the node)
   */
  public function render() {
    drupal_set_message(t('The preview is not available yet!'), 'error');
    drupal_goto();
    return;
  }
}
