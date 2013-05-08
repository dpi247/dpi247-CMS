<?php 

/**
 * A child class of Entity
 * @author jpl
 *
 */
class RemoteHtmlEntity extends Entity {
	
	
	/**
	 * the rid of remotehtml
	 * @var $rid
	 */
	public $rid;
	
	/**
	 * the machine_name of remotehtml
	 * @var $machine_name
	 */
	public $machine_name;
	
	/**
	 * the title of remotehtml
	 * @var $title
	 */
	public $title;
	
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
    	return $this->title;
  	}
  	
  	/**
  	 * Overrides defaultUri function from entity
  	 * 
  	 * @see Entity::defaultUri()
  	 */
  	protected function defaultUri() {
    	return array('path' => 'remotehtml/' . $this->rid);
  	}

}