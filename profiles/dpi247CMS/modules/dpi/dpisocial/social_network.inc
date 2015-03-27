<?php
/**
 * Basic class for social_network.
 *
 * All Example operation classes should inherit this basic class.
 */

abstract class dpisocial_social_network {
	
	public $whoAmI = 'generic';

	public function HelloWorld() {
		return 'Hello World !';
	}
	
	public function isNetworkInUse() {
		return dpi_variable_get('dpisocial_inuse_'.$this->whoAmI, false);	
	}
	
	public function setNetworkInUse($used = true) {
		dpi_variable_set('dpisocial_inuse_'.$this->whoAmI, $used);	
	}
	
	public function saveActionsActivate($action, $activated) {
		dpi_variable_set('dpisocial_'.$this->whoAmI.'_'.$action, $activated);	
	}
	
	public function getListActionsActivated() {
		$actions = 	dpisocial_getListActions();
		
		$actionsActivated = array();
		
		foreach ($actions as $action) {
			if (dpi_variable_get('dpisocial_'.$this->whoAmI.'_'.$action,0)) $actionsActivated[] = $action;
		}
		
		return $actionsActivated;
	}
	
	public function isActionActivated($action) {
		$actionsActivated = $this->getListActionsActivated();
		
		if (in_array($action,$actionsActivated)) return true;
		else return false;
		
	}
	
	public function configForm() {
		$form['title'] = array(
			'#markup'	=> 'Default title'
		);
		
		return $form;	
	}
	
	public function saveConfigForm($form) {
		
	}
	
	public function isFunctionActive($function) {
		return false;	
	}
	
	public function addCheckboxShareComment(&$form) {
	}
	
	public function getShareButtonHtml($nid, $url) {
		return $this->getShareButtonHtmlPersonnalized($nid, $url);	
	}
	
	public function getShareButtonHtmlClassic($nid, $url) {
		
	}
	
	public function getShareButtonHtmlPersonnalized($nid, $url) {
		return '<button class="btn btn-social-icon btn-'.$this->whoAmI.'" type="button" onclick="dpisocial_share_this(\''.$this->getShareUrl($nid, $url).'\', \''.$this->whoAmI.'\');"><i class="fa fa-'.$this->whoAmI.'"></i><span class="label">'.$this->getNbShares($nid, $url).'</span></button>'	;
	}
	
	public function getShareUrl($url) {	
	}
	
	public function apiGetNbShares($url) {
		return false;	
	}
	
	public function getNbShares($nid) {	
		return 0;
	}
	
	
	public function getNbSharesApi($url) {	
		return 0;
	}
	
	public function getSocialHTML() {
		return NULL;	
	}

	public function getSocialJS() {
		return NULL;
	}  	
}