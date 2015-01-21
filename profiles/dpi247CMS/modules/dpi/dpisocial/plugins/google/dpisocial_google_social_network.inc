<?php

$plugin = array(
  'label' => t('Google'),
  'handler' => array(
    'class' => 'dpisocial_google_social_network',
  ),
  'type' => 'google'
);

class dpisocial_google_social_network extends dpisocial_social_network {
	
	var $whoAmI = 'google';
	
	public function configForm() {
		  
		  $form['google_share_article'] = array(
			'#type' => 'fieldset',
			'#title' => t('Share articles'),
			//'#description' => t('This will permanently remove the log messages from the database.'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['google_share_article']['google_share_article'] = array (
			'#title' => t('Partager les articles'),
			'#type' => 'checkbox',
			'#default_value'=>$this->isActionActivated('share_article'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['google_share_article']['google_share_article_method'] = array(
		  	'#type' => 'radios',
			'#title' => t('Button de partage'),
			'#maxlength' => 1,
			'#description' => t('Sélectionner si vous voulez utiliser le bouton classique ou une version personnalisée'),
			'#options' => array( 'c' => 'Classique', 'p' => 'Personnalisé'),
			'#default_value' => dpi_variable_get('google_share_article_method', 'c'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['google_share_article']['google_share_article_position'] = array(
		  	'#type' => 'textfield',
			'#title' => t('Position bouton de partage'),
			'#description' => t('Sélectionner la position du bouton de partage'),
			'#default_value' => dpi_variable_get('google_share_article_position', 0),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  return $form;
		
	}
	
	public function saveConfigForm($form_state) {
		$this->saveActionsActivate('share_article',$form_state['values']['google_share_article']);
		dpi_variable_set('google_share_article_method',$form_state['values']['google_share_article_method']);
		dpi_variable_set('google_share_article_position',$form_state['values']['google_share_article_position']);
	}
	
	public function getShareUrl($url) {	
	
		return 'https://plus.google.com/share?url='.urlencode($url);
	
	}
	
	
	public function apiGetNbShares($url) {
		  $curl = curl_init();
		  curl_setopt($curl, CURLOPT_URL, "https://clients6.google.com/rpc");
		  curl_setopt($curl, CURLOPT_POST, 1);
		  curl_setopt($curl, CURLOPT_POSTFIELDS, '[{"method":"pos.plusones.get","id":"p","params":{"nolog":true,"id":"' . $url . '","source":"widget","userId":"@viewer","groupId":"@self"},"jsonrpc":"2.0","key":"p","apiVersion":"v1"}]');
		  curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		  curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
		  $curl_results = curl_exec ($curl);
		  curl_close ($curl);
		  $json = json_decode($curl_results, true);
		 
		  if (isset( $json[0]['result']['metadata']['globalCounts']['count'] )) return $json[0]['result']['metadata']['globalCounts']['count'];
		  else return false;
	}
	
	public function getShareButtonHtml($nid, $url) {
		if (dpi_variable_get('google_share_article_method', 'c')=='c') return $this->getShareButtonHtmlClassic($nid, $url);
		else return $this->getShareButtonHtmlPersonnalized($nid, $url);	
	}
	
	public function getShareButtonHtmlClassic($nid, $url) {
		return '<!-- Place this tag in your head or just before your close body tag. -->
<script src="https://apis.google.com/js/platform.js" async defer></script>

<!-- Place this tag where you want the +1 button to render. -->
<div class="g-plusone" data-annotation="inline" data-width="300" data-href="'.$url.'"></div>';	
	}

	public function getShareButtonHtmlPersonnalized($nid, $url) {
		return '<button class="btn btn-social btn-google-plus" type="button" onclick="dpisocial_share_this(\''.$this->getShareUrl($nid, $url).'\', \''.$this->whoAmI.'\');"><i class="fa fa-'.$this->whoAmI.'"></i><span class="label">'.$this->getNbShares($nid, $url).'</span></button>'	;
	}
	
}


