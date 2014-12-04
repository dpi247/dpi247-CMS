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
	
	public function HelloWorld() {
		return 'Hello Google !';
	}
}
