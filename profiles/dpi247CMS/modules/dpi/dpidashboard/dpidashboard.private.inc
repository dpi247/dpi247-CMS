<?php

function _dpidashboard_user_has_permission_displaydashboardnode() {
	return user_access('displaydashboardnode');	
}

function _dpidashboard_user_has_permission_displaydashboardterm() {
	return user_access('displaydashboardnode');	
}

function _dpidashboard_user_has_permission_displaydashsystem() {
	return user_access('displaydashboardnode');	
}

function _dpidashboard_periode_to_field($periode) {
	
	$theField = false;

	switch($periode) {
		case '1d':
			$theField = 'daycount';
			
			break;
		case '1w':
			$theField = 'weekcount';
			
			break;
		case '1m':
			$theField = 'monthcount';
			
			break;
		case '1y':
			$theField = 'yearcount';
			
			break;
		case 'all':
			$theField = 'totalcount';
			
			break;
	  }
	  
	  return $theField;

}

function dpidashboard_get_views_node($nid) {
	 $query = db_select('dpistat_node_counter', 'nc');
	 $query->fields('nc',array('nid'))->condition('nid',$nid);	
	 
	 $query->addField('nc','totalcount','count');
	 
	 $result = $query->execute();
	 
	 if (!$stats = $result->fetchAssoc()) { // stats not avaible yet !
			return 0;  
	  }
	  
	  return $stats['count'];
}

function dpidashboard_get_views_node_for_periode($nid, $periode) {

	  $query = db_select('dpistat_node_counter', 'nc');
	  $query->fields('nc',array('nid'))->condition('nid',$nid);
	  
	  if ($theField = _dpidashboard_periode_to_field($periode)) {
	  
		  $query->addField('nc',$theField,'count');
		  
		  $result = $query->execute();
		  
		  if (!$stats = $result->fetchAssoc()) { // stats not avaible yet !
				return false;  
		  }
		  
		  $resultG = db_query('SELECT AVG('.$theField.') AS avg FROM {dpistat_node_counter} WHERE '.$theField.'>0');
		  
		  $statsG = $resultG->fetchAssoc();
		  
		  if (empty($statsG['avg'])) $statsG['avg']=0;
		  
		  return array(
			'count'	=> $stats['count'],
			'avg'	=> $statsG['avg']
		  );
	  
	  } else {
			return false;  
	  }
	  	
}

function dpidashboard_get_nbcomments_node($nid) {
	dpm($nid);	
}

function dpidashboard_get_socialshares() {
	
	module_load_include('inc', 'dpidashboard', 'dpidashboard.private.getsocialshares');
	
	$data = array(
		'success'	=> false,
	);
	
	if (empty($_GET['url'])) {
		$data['error'] = 'NO_URL';
	}  elseif (empty($_GET['social'])) {
		$data['error'] = 'NO_SOCIAL';
	} else {
		$url_parts = explode(' ', $_GET['url']);
		$url = $url_parts[0];
		
		$social = $_GET['social'];
		
		$data = _dpidashboard_get_socialshares($social, $url);
	}
	
	if ($data['success']) {
		$data['count_str'] = number_format($data['count'],0,",",".");
	}
	
	header('Content-type: application/json');
	
	echo json_encode($data);	
}
