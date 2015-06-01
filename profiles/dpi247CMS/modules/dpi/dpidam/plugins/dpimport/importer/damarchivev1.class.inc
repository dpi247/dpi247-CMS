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
      
      if(isset($package->temp_crops) && is_array($package->temp_crops)){
        foreach ($package->temp_crops as $atom_id => $crops_infos) {          
          /* Test pour ajouter d'office un default cropping */
          $keys_crop = array_keys($crops_infos);
          if(!empty($crops_infos) && !in_array('default', $keys_crop)){
            $crops_infos['default'] = current($crops_infos);
          }
          /* Set the niew value to crop base */
          $cid = md5('node_'.$package->nid.'_'.$atom_id);
          cache_set($cid, $crops_infos);
        } 
      }
    }

    // Set the CurrentLog to the previous one to stop children growing.
    Logger::setCurrentToPreviousExecution();

    // Indicates that the dispatcher must not process the children
    return FALSE;
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
      $format = $this->getDefaultTextFormat();
      $wrapper->dpidam_right_of_reply_title->set(array(
        'value' => $dom->nodeValue,
        'format' => $format
      ));
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
      $format = $this->getDefaultTextFormat();
      $wrapper->dpidam_right_of_reply_body->set(array(
        'value' => $dom->nodeValue,
        'format' => $format
      ));
    }
  }
  
  public function buildFreeAccess($dom, $wrapper, $entity) {    
    $entity->freeaccess = $dom->nodeValue;
  }
}
