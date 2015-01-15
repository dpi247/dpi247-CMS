<?php

$plugin = array(
  'label' => t('LinkedIn'),
  'handler' => array(
    'class' => 'dpisocial_linkedin_social_network',
  ),
  'type' => 'linkedin'
);

class dpisocial_linkedin_social_network extends dpisocial_social_network {
	
	var $whoAmI = 'linkedin';
	
	public function configForm() {
		  
		  $form['linkedin_share_article'] = array(
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
		
		  $form['linkedin_share_article']['linkedin_share_article'] = array (
			'#title' => t('Partager les articles'),
			'#type' => 'checkbox',
			'#default_value'=>$this->isActionActivated('share_article'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['linkedin_share_article']['linkedin_share_article_method'] = array(
		  	'#type' => 'radios',
			'#title' => t('Button de partage'),
			'#maxlength' => 1,
			'#description' => t('Sélectionner si vous voulez utiliser le bouton classique ou une version personnalisée'),
			'#options' => array( 'c' => 'Classique', 'p' => 'Personnalisé'),
			'#default_value' => dpi_variable_get('linkedin_share_article_method', 'c'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  
		  $form['linkedin_share_article']['linkedin_share_article_position'] = array(
		  	'#type' => 'textfield',
			'#title' => t('Position bouton de partage'),
			'#description' => t('Sélectionner la position du bouton de partage'),
			'#default_value' => dpi_variable_get('linkedin_share_article_position', 0),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  return $form;
		
	}
	
	public function saveConfigForm($form_state) {
		$this->saveActionsActivate('share_article',$form_state['values']['linkedin_share_article']);
		dpi_variable_set('linkedin_share_article_method',$form_state['values']['linkedin_share_article_method']);
		dpi_variable_set('linkedin_share_article_position',$form_state['values']['linkedin_share_article_position']);
	}
	
	public function getShareUrl($url) {	
	
		return 'https://www.linkedin.com/shareArticle?mini=true&url='.urlencode($url);
	
	}
	
	
	public function apiGetNbShares($url) {
		$api_url = 'http://www.linkedin.com/countserv/count/share?url='.urlencode($url).'&format=json';
			
		$data_share = file_get_contents($api_url);
		
		$data_share = json_decode($data_share, true);
		
		if (isset($data_share['count'])) return $data_share['count'];
		else return false;
	}
	
	public function getShareButtonHtml($nid, $url) {
		if (dpi_variable_get('linkedin_share_article_method', 'c')=='c') return $this->getShareButtonHtmlClassic($nid, $url);
		else return $this->getShareButtonHtmlPersonnalized($nid, $url);	
	}
	
	public function getShareButtonHtmlClassic($nid, $url) {
		return '<script src="//platform.linkedin.com/in.js" type="text/javascript">lang: en_US</script>
<script type="IN/Share" data-url="'.$url.'" data-counter="right"></script>';	
	}
}
