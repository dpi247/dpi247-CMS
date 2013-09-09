<?php
class DPIPropertiesEntity extends Entity{

  public $dpid;
  public $entity;
  public $id;
  public $external_reference;
  public $external_reference_2;
  public $source;
  public $product_id;
  
  protected function defaultLabel() {
    return $this->external_reference;
  }
  protected function defaultUri() {
    return array('path' => 'dpiproperties/' . $this->dpid);
  }
}

class DPIPropertiesTypeEntity extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'dpiproperties_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}



