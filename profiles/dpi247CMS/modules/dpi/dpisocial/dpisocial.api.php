<?php

function dpisocial_api_get_avaible_social_for_function($function) {
	switch ($function) {
		case 'share url': 
			return array('facebook','twitter','google');
		default:
			return array();	
	}
}

function dpisocial_api_get_selected_social_for_function($function) {
	switch ($function) {
		case 'share comment': 
			return dpi_variable_get('dpisocial_comments_share_networks', array());
		default:
			return array();	
	}
}

function dpisocial_api_is_function_activated($function, $network) {
	$networks_avaible = dpisocial_api_get_selected_social_for_function($function);
	
	return in_array($network, $networks_avaible);
}

function dpisocial_api_get_share_url($social, $url) {
	switch ($social) {
		case 'facebook':
			$return = 'https://www.facebook.com/sharer/sharer.php?u='.urlencode($url); 
			break;
		case 'twitter':
			$return = 'https://plus.google.com/share?url='.urlencode($url);
			break;
		case 'google':
			$return = 'https://twitter.com/intent/tweet?text='.urlencode($url);
			break;
		default:
			$return = false;
	}
	
	return $return;
}

function dpisocial_api_get_share_window_open($social, $url) {
	if ($surl = dpisocial_api_get_share_url($social, $url)) {
		switch ($social) {
			case 'facebook';
				$script = "window.open('".addslashes($surl)."','shareFacebook','width=500,height=332')";
				break;	
			case 'twitter';
				$script = "window.open('".addslashes($surl)."','shareTwitter','width=500,height=220')";
				break;
			case 'google';
				$script = "window.open('".addslashes($surl)."','shareGoogle','width=500,height=732')";
				break;
			default:
				return false;
		}
	} else {
		return false;
	}
}
