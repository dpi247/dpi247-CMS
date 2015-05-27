<?php 

module_load_include('class.inc', 'dpicontenttypes', 'plugins/dpimport/importer/wallyxmlpreview');

class DamArchiveV1 extends WallyXmlPreview {
  function render() {
    $prenode = $this->prenode;
    $node = $prenode->getNode();
    $return = array();
    if ($node && !Logger::error()) {
      $return[] = array(
        'error' => FALSE,
        'tempnode' => $node,
      );
    } else {
      // @todo : Best error handling
      $return[] = array(
        'error' => TRUE,
        'error_msg' => t('Error during import'),
      );
    }
    return $return;
  }

  protected function setPackageAttributes($dom, $wrapper, $entity) {
    parent::setPackageAttributes($dom, $wrapper, $entity);

    if ($dom->hasAttribute('NoIndex')) {
      $entity->noindex = $dom->getAttribute('NoIndex');
    }
    $entity->damarchive = TRUE;
  }

  /**
   * Set field Destination
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildDestinations($dom, $wrapper, $entity, $globalDom) {
    $items = dpi_variable_get('dpidam_archive_defaultdestination', array());
    if ($items && !empty($items)) {
      $wrapper->field_destinations->set($items);
    } else {
      // If no destinations at all > Package can't be created
      Logger::logError('importer', 0, 'nodestination', t('No destination AT ALL for current package.'));
    }
  }

  /**
   * Set field CustomFields
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  public function buildCustomFields($dom, $wrapper, $entity) {
    $temp_custom_fields = array();
    foreach ($dom->getElementsByTagName('CustomField') as $dom_custom_field) {
      if ($dom_custom_field->hasAttribute('FieldName')) {
        $temp_custom_fields[$dom_custom_field->getAttribute('FieldName')] = $dom_custom_field->nodeValue;
      }
    }
    $entity->custom_fields = $temp_custom_fields;
  }
  
  /**
   * Ajout du droit de réponse titre dans le wrapper
   * @param type $dom
   * @param type $wrapper
   * @param type $entity
   */
  public function buildRightOfReplyTitle($dom, $wrapper, $entity){
    if(isset($wrapper->dpidam_right_of_reply_title)){
      $wrapper->dpidam_right_of_reply_title->set($dom->nodeValue);
    }    
  }
  
  /**
   * Ajout du droit de réponse body dans le wrapper
   * @param type $dom
   * @param type $wrapper
   * @param type $entity
   */
  public function buildRightOfReplyBody($dom, $wrapper, $entity){
    if(isset($wrapper->dpidam_right_of_reply_body)){
      $wrapper->dpidam_right_of_reply_body->set($dom->nodeValue);
    }
  }
}
