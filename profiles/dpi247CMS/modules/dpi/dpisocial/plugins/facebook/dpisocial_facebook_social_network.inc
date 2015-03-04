<?php

$plugin = array(
  'label' => t('Facebook'),
  'handler' => array(
    'class' => 'dpisocial_facebook_social_network',
  ),
  'type' => 'facebook'
);

class dpisocial_facebook_social_network extends dpisocial_social_network {
	
	public $whoAmI = 'facebook';
	
	public function HelloWorld() {
		return 'Hello Facebook !';
	}
	
	public function configForm() {
		$form['facebook_app_id'] = array(
			'#type' => 'textfield',
			'#title' => t('Application ID'),
			'#default_value' => dpi_variable_get('facebook_app_id', ''),
			'#description' => t('ID of the associated Facebook application.'),
			'#required' => false,
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['facebook_app_secret'] = array(
			'#type' => 'textfield',
			'#title' => t('Application Secret'),
			'#default_value' => dpi_variable_get('facebook_app_secret', ''),
			'#description' => t('Secret of the associated Facebook application.'),
			'#required' => false,
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['facebook_share_article'] = array(
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
		  
		  $form['facebook_share_article']['facebook_share_article'] = array (
			'#title' => t('Partager les articles'),
			'#type' => 'checkbox',
			'#default_value'=>$this->isActionActivated('share_article'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['facebook_share_article']['facebook_share_article_method'] = array(
		  	'#type' => 'radios',
			'#title' => t('Bouton de partage'),
			'#maxlength' => 1,
			'#description' => t('Sélectionner si vous voulez utiliser le bouton classique ou une version personnalisée'),
			'#options' => array( 'c' => 'Classique', 'p' => 'Personnalisé'),
			'#default_value' => dpi_variable_get('facebook_share_article_method', 'c'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  ); 
		  
		  $form['facebook_share_article']['facebook_share_article_classic_display'] = array(
		  	'#type' => 'select',
			'#title' => t('Format du bouton de partage'),
			'#options' => array( 
				'box_count'		=> 'box_count',
				'button_count'	=> 'button_count',
				'button'		=> 'button',
				'icon_link'		=> 'icon_link',
				'icon'			=> 'icon',
				'link'			=> 'link',
			),
			'#default_value' => dpi_variable_get('facebook_share_article_classic_display', 'button_count'),
			'#states' => array(
			  'visible' => array(
			  	':input[name="social_network_in_use"]' => array('checked' => true),
				':input[name="facebook_share_article_method"]' => array('value' => 'c'),
			  ),
			),
		  ); 
		  
		  $form['facebook_share_article']['facebook_share_article_position'] = array(
		  	'#type' => 'textfield',
			'#title' => t('Position bouton de partage'),
			'#description' => t('Sélectionner la position du bouton de partage'),
			'#default_value' => dpi_variable_get('facebook_share_article_position', 0),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  
		  $form['facebook_share_comments'] = array(
			'#type' => 'fieldset',
			'#title' => t('Share comments'),
			//'#description' => t('This will permanently remove the log messages from the database.'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['facebook_share_comments']['facebook_share_comment'] = array (
			'#title' => t('Partager les commentaires'),
			'#type' => 'checkbox',
			'#default_value'=>$this->isActionActivated('share_comment'),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['facebook_share_comments']['facebook_share_phrase'] = array(
			'#type' => 'textfield',
			'#title' => t('Phrase de partage'),
			'#default_value' => dpi_variable_get('facebook_share_phrase', t('Partager sur Facebook')),
			'#description' => t('Phrase à coté de la case à cocher pour proposer le partage.'),
			'#required' => false,
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		  
		  $form['facebook_share_comments']['facebook_share_comment_with_application'] = array(
		  	'#title' => t('Utiliser l\'application pour le partage'),
			'#type' => 'checkbox',
			'#default_value'=>dpi_variable_get('facebook_share_comment_with_application', 0),
			'#states' => array(
			  'visible' => array(
				':input[name="social_network_in_use"]' => array('checked' => true),
			  ),
			),
		  );
		
		return $form;	
	}
	
	public function addCheckboxShareComment(&$form) {
		
		$form['dpisocial_facebook_share_comment'] = array(
		  	'#title' => dpi_variable_get('facebook_share_phrase', t('Partager sur Facebook')),
			'#type' => 'checkbox',
			'#default_value'=>1,
		  );
	}
	
	public function saveConfigForm($form_state) {
		dpi_variable_set('facebook_app_id',$form_state['values']['facebook_app_id']);
		dpi_variable_set('facebook_app_secret',$form_state['values']['facebook_app_secret']);
		$this->saveActionsActivate('share_article',$form_state['values']['facebook_share_article']);
		dpi_variable_set('facebook_share_article_method',$form_state['values']['facebook_share_article_method']);
		dpi_variable_set('facebook_share_article_classic_display',$form_state['values']['facebook_share_article_classic_display']);
		dpi_variable_set('facebook_share_article_position',$form_state['values']['facebook_share_article_position']);
		$this->saveActionsActivate('share_comment',$form_state['values']['facebook_share_comment']);
		dpi_variable_set('facebook_share_phrase',$form_state['values']['facebook_share_phrase']);
		dpi_variable_set('facebook_share_comment_with_application',$form_state['values']['facebook_share_comment_with_application']);
	}
	
	
	public function getShareUrl($url) {	
	
		return 'https://www.facebook.com/sharer/sharer.php?u='.urlencode($url);
	
	}
	
	public function getSocialHTML() {
		$html = array(
			'fb-root' => array(
				'#markup'	=> '<div id="fb-root"></div>'
			)
		);
		return $html;	
	}
	
	public function getSocialJS() {
		$js = array();
		
		$script = '';
		
		$asyncfunc = '';
		
		//$asyncfunc.='FB.login();';
		
		if ($this->isActionActivated('share_comment')) { // ajout poster les commentaires
			$func = "
			var url = window.location.href;
			if (url.indexOf('share_comment=1')>0) {
				url = url.replace(/share_comment=1/gi, '')
				FB.ui({
				  method: 'feed',
				  link: url,
				  caption: 'J\'ai commenté cet article'
				}, function(response){});
			}";
			$asyncfunc.=$func;	
		}
		
		$facebook_app_id = dpi_variable_get('facebook_app_id', false);
		if ($facebook_app_id) {
			$script = "
				window.fbAsyncInit = function() {
				FB.init({
				  appId      : '".$facebook_app_id."',
				  xfbml      : true,
				  version    : 'v2.1'
				});
				
				$asyncfunc
				
			  };";

		} 
		
		$script.="(function(d, s, id){
				 var js, fjs = d.getElementsByTagName(s)[0];
				 if (d.getElementById(id)) {return;}
				 js = d.createElement(s); js.id = id;
				 js.src = '//connect.facebook.net/fr_FR/sdk.js';
				 fjs.parentNode.insertBefore(js, fjs);
			   }(document, 'script', 'facebook-jssdk'));";
		
		$js[] = array(
			'type'		=> 'inline',
			'scope'		=> 'footer',
			'script' 	=> $script
		);
		
		return $js;
	} 
	
	
	public function apiGetNbShares($url) {
		$api_url = 'https://api.facebook.com/method/links.getStats?urls='.urlencode($url).'&format=json';
			
		$data_share = file_get_contents($api_url);
		
		$data_share = json_decode($data_share, true);
		
		if (isset($data_share[0]['total_count'])) return $data_share[0]['total_count'];
		else return false;
	}
	
	public function getShareButtonHtml($nid, $url) {
		if (dpi_variable_get('facebook_share_article_method', 'c')=='c') return $this->getShareButtonHtmlClassic($nid, $url);
		else return $this->getShareButtonHtmlPersonnalized($nid, $url);	
	}
	
	public function getShareButtonHtmlClassic($nid, $url) {
		return '<div class="fb-share-button" data-href="'.$url.'" data-layout="'.dpi_variable_get('facebook_share_article_classic_display', 'button_count').'"></div>';	
	}
}
