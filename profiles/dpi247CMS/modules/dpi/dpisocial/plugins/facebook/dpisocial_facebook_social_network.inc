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
		  );
		  
		  $form['facebook_app_secret'] = array(
			'#type' => 'textfield',
			'#title' => t('Application Secret'),
			'#default_value' => dpi_variable_get('facebook_app_secret', ''),
			'#description' => t('Secret of the associated Facebook application.'),
			'#required' => false,
		  );
		  /*
		  $form['facebook_share_article'] = array (
			'#title' => t('Partager les articles'),
			'#type' => 'checkbox',
			'#default_value'=>dpisocial_api_is_function_activated('share article', 'facebook') ,
		  );
		  */
		  
		  $form['facebook_share_comment'] = array (
			'#title' => t('Partager les commentaires'),
			'#type' => 'checkbox',
			'#default_value'=>$this->isActionActivated('share_comment'),
		  );
		  
		  $form['facebook_share_phrase'] = array(
			'#type' => 'textfield',
			'#title' => t('Phrase de partage'),
			'#default_value' => dpi_variable_get('facebook_share_phrase', t('Partager sur Facebook')),
			'#description' => t('Phrase à coté de la case à cocher pour proposer le partage.'),
			'#required' => false,
		  );
		  
		  $form['facebook_share_comment_with_application'] = array(
		  	'#title' => t('Utiliser l\'application pour le partage'),
			'#type' => 'checkbox',
			'#default_value'=>dpi_variable_get('facebook_share_comment_with_application', 0),
		  );
		  
		  $form['submit'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Submit' ) 
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
		$this->saveActionsActivate('share_comment',$form_state['values']['facebook_share_comment']);
		dpi_variable_set('facebook_share_phrase',$form_state['values']['facebook_share_phrase']);
		dpi_variable_set('facebook_share_comment_with_application',$form_state['values']['facebook_share_comment_with_application']);
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
			$func = "FB.ui({
  method: 'feed',
  link: 'http://www.sudinfo.be/1160409/article/2014-12-01/david-beckham-victime-d-un-impressionnant-accident',
  caption: 'J\'ai commenté cet article'
}, function(response){});";
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
				 js.src = '//connect.facebook.net/en_US/sdk.js';
				 fjs.parentNode.insertBefore(js, fjs);
			   }(document, 'script', 'facebook-jssdk'));";
		
		$js[] = array(
			'type'		=> 'inline',
			'scope'		=> 'footer',
			'script' 	=> $script
		);
		
		return $js;
	} 
}
