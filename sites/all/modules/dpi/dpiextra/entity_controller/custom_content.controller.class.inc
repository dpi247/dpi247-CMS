<?php 

/**
 * a child class of EntityApiController
 * @author jpl
 *
 */
class Custom_contentController extends EntityAPIControllerExportable{

	/**
	 * Overrides create function from EntityApiController
	 * @see EntityAPIController::create()
	 */
	public function create(array $values = array()) {
		global $user;
		$values += array(
				'cid' => '',
				'name' => '',
				'title' => '',
				'body' => '',
				'date' => '',
		);
		return parent::create($values);
	}
	
	/**
	 * Overrides buildContent function from EntityApiController
	 * @see EntityAPIController::buildContent()
	 */
	public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
		$wrapper = entity_metadata_wrapper('custom_content', $entity);
	
		// Make Description and Status themed like default fields.
		$content['description'] = array(
				'#theme' => 'field',
				'#weight' => 0,
				'#title' =>t('Description'),
				'#access' => TRUE,
				'#label_display' => 'above',
				'#view_mode' => 'full',
				'#language' => LANGUAGE_NONE,
				'#field_name' => 'field_fake_description',
				'#field_type' => 'text',
				'#entity_type' => 'custom_content',
				'#bundle' => 'custom_content',
				'#items' => array(array('value' => $entity->description)),
				'#formatter' => 'text_default',
				0 => array('#markup' => check_plain($entity->description))
		);
	
		return parent::buildContent($entity, $view_mode, $langcode, $content);
	}
}