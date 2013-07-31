<?php 

/**
 * A child class of Entity
 * @author jpl
 *
 */
class RemoteHtmlEntity extends Entity {
	
	/**
	 * the id of remotehtml
	 * 
	 * @var $rid
	 */
	public $rid;
	
	/**
	 * the name of remotehtml
	 * @var $name
	 */
	public $name;
	
	/**
	 * the html adresse of remotehtml
	 * 
	 * @var $html_adresse
	 */
	public $html_adresse;
	
	/**
	 * the refresh time of remotehtml
	 * 
	 * @var $refresh_time
	 */
	public $refresh_time;
	 
	/**
	 * Overrides defaultLabel function from entity
	 * 
	 * @see Entity::defaultLabel()
	 */
	protected function defaultLabel() {
    	return $this->name;
  	}
  	
  	/**
  	 * Overrides defaultUri function from entity
  	 * 
  	 * @see Entity::defaultUri()
  	 */
  	protected function defaultUri() {
    	return array('path' => 'remotehtml/' . $this->name);
  	}

}