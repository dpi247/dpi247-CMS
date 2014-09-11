<?php

function _dpidashboard_get_element() {
	global $_dpidashboard_element;
	
	if (empty($_dpidashboard_element)) $_dpidashboard_element = 1;

	$element = $_dpidashboard_element;
	
	$_dpidashboard_element++;

	return $element;
}

function _dpidashboard_get_toppackage($element) {

  $query = db_select('node','n')->extend('PagerDefault')->element($element);
  
  $query
  	->condition('type','package')
	->fields('n', array('title'))
	->limit(2);
	
  $result = $query->execute();
  
  return $result;

}

function _dpidashboard_get_latest_created_package($only_user=false,$element) {
  $query = db_select('node','n')->extend('PagerDefault')->element($element);
  $query
  	->condition('type','package')
	->fields('n', array('title'))
	->orderBy('created','DESC')
	->limit(3);
	
  if ($only_user) { // only for the current user
  
  }
	
  $result = $query->execute();
  
  return $result;

}