<?php 

abstract class PackageHelper extends Importer {
  /**
   * Save a package
   *
   * @param Dom $dom
   * @param Object $package
   */
  protected function savePackage($dom, $package) {
    node_save($package);
  }

  /**
   * Construct the node base on the DOM element
   *
   * @param DOM $dom
   */
  protected function constructPackage($dom) {
    $dispatcher = $this->getDispatcher();
    $package = $this->createOrLoadEntity($dom);
    $package_wrapper = entity_metadata_wrapper('node', $package);
    if (!Logger::error()) {
      // Allow to alter entity after process
      module_invoke_all('dpimport_entity_beforeprocess', $package_wrapper, $package, 'node', $this, $dispatcher);

      $this->setPackageAttributes($dom, $package_wrapper, $package);
      if (!Logger::error()) {
        $dispatcher->processChildren($dom, $package_wrapper, $package);
      }

      // Allow to alter entity after process
      module_invoke_all('dpimport_entity_postprocess', $package_wrapper, $package, 'node', $this, $dispatcher);
    }
    return $package;
  }

  /**
   * Create or load a package based on it's exeternal reference and action
   *
   * @param DOM $dom
   * @param string $extref
   * @param string $action
   *
   * @return Object $node
   */
  protected function _createOrLoadEntityPackage($dom, $external_reference, $action) {
    $node = new stdClass();
    $node->type = 'package';
    $node->is_new = TRUE;
    node_object_prepare($node);
    $node->dpiproperties = new DPIPropertiesEntity(array('type' => 'package'), 'dpiproperties');
    $temp_node = dpicontenttypes_api_getnodebyexternalreference($external_reference);
    $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
    if ($dpiproperties !== FALSE){
      $temp_node->dpiproperties = $dpiproperties;
    }

    switch($action){
      case 'Delete':
        if ($temp_node != NULL) {
          // @todo delete node
        } else {
          Logger::logError('importer', 0, 'xmldelete', t('The package doesn\'t exist. It can\'t be deleted. Package rejected.'));
        }
        break;
      case 'CreateReplace':
        if ($temp_node != NULL) {
          $node->nid = $temp_node->nid;
          $node->vid = $temp_node->vid;
          unset($node->is_new);
        }
        break;
      case 'CreateUpdate':
        if ($temp_node != NULL) {
          $node = $temp_node;
          // @todo cckdestinations_force_dnc($node);
        }
        break;
      case 'Update':
        if ($temp_node != NULL) {
          $node = $temp_node;
          // @todo cckdestinations_force_dnc($node);
        } else {
          Logger::logError('importer', 0, 'xmlupdate', t('The package doesn\'t exist. It can\'t ben updated. Package rejected.'));
        }
        break;
    }

    if (isset($node->is_new)) {
      $node_wrapper = entity_metadata_wrapper('node', $node);
      $node_wrapper->field_external_reference->set($external_reference);
    }

    return $node;
  }

  /**
   * Create or load a photo object based on it's exeternal reference
   *
   * @param DOM $dom
   * @param string $extref
   *
   * @return Object $node
   */
  protected function _createOrLoadEntityScaldAtom($dom, $external_reference, $atom) {
    if ($temp_atom = dpicontenttypes_api_getatombyexternalreference($external_reference)){
      $atom = $temp_atom;
    } else {
      $atom_wrapper = entity_metadata_wrapper('scald_atom', $atom);
      $atom_wrapper->field_external_reference->set($external_reference);
    }

    $dpiproperties = dpiproperties_load_by_external_reference($external_reference);
    if ($dpiproperties !== FALSE) {
      $atom->dpiproperties = $dpiproperties;
    } else {
      $atom->dpiproperties = new DPIPropertiesEntity(array('type' => 'scald_atom'), 'dpiproperties');
    }

    return $atom;
  }

  /**
   * Create or load a photo object based on it's exeternal reference
   *
   * @param DOM $dom
   * @param string $extref
   *
   * @return Object $node
   */
  protected function _createOrLoadEntityPhotoObjectType($dom, $external_reference) {
    $atom = scald_atom_defaults('image');
    $atom->provider = 'scald_image';
    $atom->type = 'image';

    return $this->_createOrLoadEntityScaldAtom($dom, $external_reference, $atom);
  }

  /**
   * Get attributes of current DOM and set it to the package
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  abstract protected function setPackageAttributes($dom, $wrapper, $entity);

  /**
   * Get attributes of current DOM and set it to the Photoojbecttype
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function setPhotoObjectTypeAttributes($dom, $wrapper, $entity){
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }

  /**
   * Get attributes of current DOM and set it to the object
   *
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
   * Return the timestamp of the datetime
   * Ceci retourne le timestamp en UTC
   * Exemple avec la date 2013-06-14T09:20:41+02:00 il retourne 1371194441
   * qui donne pour date('d/m/Y H:i:s e', 1371194441) -> 14/06/2013 09:20:41 Europe/Brussels (e = timezone)
   * ou 14/06/2013 07:20:41 UTC Ce qui est correct donc merci de vérifier vos timezone avant de modifier cette fonction.
   *
   * @param String $datetime
   */
  protected function getDateStampFromDateTime($date){
    $datetime = new DateTime($date);
    return $datetime->getTimestamp();
  }

  /**
   * Check if destination already exists on the node
   *
   * @param EntityDrupalWrapper $entity
   * @param Array $dest
   *
   * @return boolean
   */
  protected function destinationexists($wrapper, $new_destination){
    $destination_exists = FALSE;
    $destinations = $wrapper->field_destinations->value();

    foreach ($destinations as $destination){
      if ($destination['tid'] == $new_destination['tid'] && $destination['target'] == $new_destination['target']){
        $destination_exists = TRUE;
        break;
      }
    }
    return $destination_exists;
  }

  /**
   * Get the default text format
   */
  protected function getDefaultTextFormat() {
    static $default_format = '';
    if (empty($default_format)) {
      $default_format = filter_default_format();
    }
    return $default_format;
  }

  /**
   * Create Atom Image
   *
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
   * Return a render in json
   */
  public function render(){
    // Set header to json
    drupal_add_http_header('Content-Type', 'application/json');

    if (Logger::error()){
      // If an error appears during the import, the global status is set to 0
      $base = new LoggerLog('dpimport', 0, 'dpimport_error', t('There was an error during the import'));
    } else {
      // If no error appears during the import, the global status is set to 1
      $base = new LoggerLog('dpimport', 1, 'dpimport_success', t('The import was proceded with success'));
    }
    // The execution log is attached to the base log.
    $execution = Logger::getLogExecution();
    $base->addChildren($execution);

    $json = json_encode($base);
    return $json;
  }

  /**
   * Create Atom Image
   *
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
   * Get attributes of current DOM and set it to the Link
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function setLinkAttributes($dom, $wrapper, $entity){
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }

  /**
   * Get attributes of current DOM and set it to the linkslist
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function setLinksListAttributes($dom, $wrapper, $entity){
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }

  /**
   * Create Atom Audio
   *
   * @param DOM $dom
   */
  protected function createOrLoadEntityAudioObjectType($dom){
    $atom = scald_atom_defaults('audio');
    $atom->provider = 'scald_audio';
    $atom->type = 'audio';

    return $this->_createOrLoadEntityScaldAtom($dom, $external_reference, $atom);
  }

  /**
   * Get attributes of current DOM and set it to the Audioojbecttype
   *
   * @param DOM $dom
   * @param EntityDrupalWrapper $wrapper
   * @param Object $entity
   */
  protected function setAudioObjectTypeAttributes($dom, $wrapper, $entity){
    $this->setObjectAttributes($dom, $wrapper, $entity);
  }

  /**
   * Check wheter a package contains images or not
   *
   * @param EntityDrupalWrapper $package_wrapper
   *
   * @return boolean
   */
  protected function checkPackageHasImage($package_wrapper) {
    $no_photo = TRUE;
    $current_embeds = $package_wrapper->field_embededobjects->value(array('identifier' => TRUE));
    if ($current_embeds && !empty($current_embeds)) {
      foreach ($current_embeds as $embed) {
        if ($embed['type'] == 'image'){
          $no_photo = FALSE;
          break;
        }
      }
    }
    return $no_photo;
  }

  /**
   * Add an atom reference to a field, checking it's presence
   */
  protected function addAtomToFied($wrapper, $entity, $field_name, $atom, $options = array()) {
    $array_atom = (array)$atom;

    if (!isset($array_atom['sid']) || empty($array_atom['sid']) || !is_numeric($array_atom['sid'])) {
      // This is a new atom, we don't have to check further
      $wrapper->{$field_name}->set((array)$array_atom);
      return;
    }

    $new_sid = $array_atom['sid'];
    $current_field = $wrapper->{$field_name}->value($options);
    if ($current_field && is_array($current_field)) {
      foreach ($current_field as $delta => $current_atom) {
        if (is_array($current_atom) && isset($current_atom['sid']) && $current_atom['sid'] === $new_sid) {
          return;
        }
      }
    }

    // @todo : Add try catch to avoid 'Invalid data value given. Be sure it matches the required data type and format.'
    $wrapper->{$field_name}->set($array_atom);
  }
}
