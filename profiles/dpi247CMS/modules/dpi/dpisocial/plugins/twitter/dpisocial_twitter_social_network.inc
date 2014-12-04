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
	
	public function HelloWorld() {
		return 'Hello Twitter !';
	}
}
