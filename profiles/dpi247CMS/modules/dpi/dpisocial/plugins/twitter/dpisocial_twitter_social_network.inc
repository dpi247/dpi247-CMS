<?php

$plugin = array(
  'label' => t('Twitter'),
  'handler' => array(
    'class' => 'dpisocial_twitter_social_network',
  ),
  'type' => 'twitter'
);

class dpisocial_twitter_social_network extends dpisocial_social_network {
	
	var $whoAmI = 'twitter';
	
	public function configForm() {
		
		  $form['twitter_share_article'] = array(
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
			
		  $form['twitter_share_article']['twitter_share_article'] = array (
			'#title' => t('Partager les articles'),
			'#type' => 'checkbox',
			'#default_value'=>$this->isActionActivated('share_article'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['twitter_share_article']['twitter_share_phrase'] = array(
			'#type' => 'textfield',
			'#title' => t('Phrase de partage'),
			'#default_value' => dpi_variable_get('twitter_share_phrase', t('Voici un article intéressant')),
			'#description' => t('Phrase à mettre dans le tweet de partage.'),
			'#required' => false,
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['twitter_share_article']['twitter_share_article_method'] = array(
		  	'#type' => 'radios',
			'#title' => t('Button de partage'),
			'#maxlength' => 1,
			'#description' => t('Sélectionner si vous voulez utiliser le bouton classique ou une version personnalisée'),
			'#options' => array( 'c' => 'Classique', 'p' => 'Personnalisé'),
			'#default_value' => dpi_variable_get('twitter_share_article_method', 'c'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
			
		  );
		  
		  $form['twitter_share_article']['twitter_share_article_position'] = array(
		  	'#type' => 'textfield',
			'#title' => t('Position bouton de partage'),
			'#description' => t('Sélectionner la position du bouton de partage'),
			'#default_value' => dpi_variable_get('twitter_share_article_position', 0),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
			
		  );
		  
		  return $form;
		
	}
	
	public function saveConfigForm($form_state) {
		$this->saveActionsActivate('share_article',$form_state['values']['twitter_share_article']);
		dpi_variable_set('twitter_share_article_method',$form_state['values']['twitter_share_article_method']);
		dpi_variable_set('twitter_share_phrase',$form_state['values']['twitter_share_phrase']);	
		dpi_variable_set('twitter_share_article_position',$form_state['values']['twitter_share_article_position']);
	}
	
	public function getShareUrl($url) {	
	
		return 'https://twitter.com/home?status='.urlencode(dpi_variable_get('twitter_share_phrase', t('Voici un article intéressant')).' '.$url);
	
	}
	
	
	public function apiGetNbShares($url) {
		$api_url = 'http://urls.api.twitter.com/1/urls/count.json?url='.urlencode($url);
			
		$data_share = file_get_contents($api_url);
		
		$data_share = json_decode($data_share, true);
		
		if (isset($data_share['count'])) return $data_share['count'];
		else return false;	
	}
	
	public function getShareButtonHtml($nid, $url) {
		if (dpi_variable_get('twitter_share_article_method', 'c')=='c') return $this->getShareButtonHtmlClassic($nid, $url);
		else return $this->getShareButtonHtmlPersonnalized($nid, $url);	
	}
	
	public function getShareButtonHtmlClassic($nid, $url) {
		return '<a href="https://twitter.com/share" class="twitter-share-button" data-url="'.$url.'" data-text="'.dpi_variable_get('twitter_share_phrase', t('Voici un article intéressant')).'">Tweet</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?\'http\':\'https\';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+\'://platform.twitter.com/widgets.js\';fjs.parentNode.insertBefore(js,fjs);}}(document, \'script\', \'twitter-wjs\');</script>';	
	}
}


