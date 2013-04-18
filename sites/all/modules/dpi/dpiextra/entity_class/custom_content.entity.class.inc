<?php 


/**
 * A child class of Entity
 * @author jpl
 *
 */
class Custom_contentEntity extends Entity {

	/**
	 * the id of custom content
	 *
	 * @var $cid
	 */
	public $cid;

	/**
	 * the name of custom content
	 * @var $name
	 */
	public $name;
	
	/**
	 * the title of custom content
	 * @var $title
	 */
	public $title;

	/**
	 * the body of custom content
	 *
	 * @var $body
	 */
	public $body;
	
	/**
	 * the date of custom content
	 *
	 * @var $date
	 */
	public $date;
	
	/**
	 * Creates a new entity.
	 *
	 * @see entity_create()
	 */
	public function __construct(array $values = array(), $entityType = NULL) {
		Entity::__construct($values, 'custom_content');
	}
	
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
		return array('path' => 'custom_content/' . $this->name);
	}

}