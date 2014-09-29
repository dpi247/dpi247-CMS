<?php

function _dpidashboard_get_socialshares_facebook($url) {
	$data = array(
		'success'	=> false
	);
	
	$api_url = 'https://api.facebook.com/method/links.getStats?urls='.urlencode($url).'&format=json';
				
	$data_share = file_get_contents($api_url);
	
	$data_share = json_decode($data_share, true);
	
	$data['count'] = $data_share[0]['total_count'];
	$data['extra'] = array(
		'share_count' => $data_share[0]['share_count'],
		'like_count' => $data_share[0]['like_count'],
		'comment_count' => $data_share[0]['comment_count'],
	);
	
	$data['success'] = true;
	
	return $data;
}

function _dpidashboard_get_socialshares_twitter($url) {
	$data = array(
		'success'	=> false
	);
	
	$api_url = 'http://urls.api.twitter.com/1/urls/count.json?url='.urlencode($url);
				
	$data_share = file_get_contents($api_url);
	
	$data_share = json_decode($data_share, true);
	
	$data['count'] = $data_share['count'];
	
	$data['success'] = true;	
	
	return $data;
}

function _dpidashboard_get_socialshares_linkedin($url) {
	$data = array(
		'success'	=> false
	);
	
	$api_url = 'http://www.linkedin.com/countserv/count/share?url='.urlencode($url).'&format=json';
				
	$data_share = file_get_contents($api_url);
	
	$data_share = json_decode($data_share, true);
	
	$data['count'] = $data_share['count'];
	
	$data['success'] = true;
	
	return $data;
				
}

function _dpidashboard_get_socialshares_googleplus($url) {
	$data = array(
		'success'	=> false
	);
	
	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, "https://clients6.google.com/rpc");
	curl_setopt($curl, CURLOPT_POST, 1);
	curl_setopt($curl, CURLOPT_POSTFIELDS, '[{"method":"pos.plusones.get","id":"p","params":{"nolog":true,"id":"' . $url . '","source":"widget","userId":"@viewer","groupId":"@self"},"jsonrpc":"2.0","key":"p","apiVersion":"v1"}]');
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
	$curl_results = curl_exec ($curl);
	curl_close ($curl);
	$json = json_decode($curl_results, true);
	// die($curl_results);
	$data['count'] = intval( $json[0]['result']['metadata']['globalCounts']['count'] );
	
	$data['success'] = true;
	
	return $data;
				
}

function _dpidashboard_get_socialshares($social, $url) {
	switch ($_GET['social']) {
		case 'facebook':
			return _dpidashboard_get_socialshares_facebook($url);
			break;
		case 'twitter':	
			return _dpidashboard_get_socialshares_twitter($url);
			break;
		case 'linkedin':
			return _dpidashboard_get_socialshares_linkedin($url);
			break;
		case 'googleplus':
			return _dpidashboard_get_socialshares_googleplus($url);
			break;
		default:
			return array('success'=>false);
	}
}