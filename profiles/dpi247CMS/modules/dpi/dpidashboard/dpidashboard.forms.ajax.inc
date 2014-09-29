<?php

function _dpidashboard_forms_ajax_get_perides() {
	
  $periodes = array(
	'1d' => t('Last 24 hours'),
	'1w' => t('Last week'),
	'1m' => t('Last month'),
	'1y' => t('Last year'),
	'all'=> t('Total')
  );
  
  return $periodes;	
}

function _dpidashboard_forms_ajax_get_values($form_state) {

	$values = (!empty($form_state['values'])?$form_state['values']:$form_state["build_info"]['args'][0]['values']);
	
	return $values;
	
}

function _comments_form_render_form($form,&$form_state) {
	
	$args=$form_state["build_info"]['args'][0];
  
    $values = _dpidashboard_forms_ajax_get_values($form_state);
	
	if (!empty($values['dpidashboard_comments_markup_id'])) {
	   $dpidashboard_comments_markup_id = $values['dpidashboard_comments_markup_id'];
	} else {
		global $dpidashboard_comments_markup_id; 
	}
	
	if (empty($dpidashboard_comments_markup_id)) $dpidashboard_comments_markup_id=0;
	
	if ($args['conf']['nb']==1) {
		$form["nbcomments"] = array(
			'#markup'=>t('Number of comments').': '.$values['nb_comments']
		);
	}
	
	$nboptions = 0;
	
	foreach ($args['conf']['options'] as $val) $nboptions+=$val;
	
	if ($nboptions>1) {
		$form['select']=array(
		'#type'=>'select',
		"#title"=>'Type',
		"#options"=>array (
			'top'	=> t('Top comments'),
			'lastest'	=> t('Latest comments')
		),
		'#default_value' => $values['select'],
		'#attributes'=>array(
		  'autocomplete' => 'off',
		),
		'#ajax' => array(
		  'callback' => 'comments_ajax_replace_form_markup_callback',
		  'wrapper' => 'comments_markup_content'.$dpidashboard_comments_markup_id,
		  'method' => 'replace',
		  'effect' => 'fade',
		),
	  );
	}
  
  
  $markup = 'ici le tableau '.print_r($values['select'],true);
  
  
  $form["markup"]=array(
    '#markup'=>$markup,
    '#prefix' => '<div id="comments_markup_content'.$dpidashboard_comments_markup_id.'">',
    '#suffix' => '</div>',
  );
  
  $form["nid"]=array(
  	'#type' => 'value', 
	'#value' => $values['nid']
  );
  
  $form["dpidashboard_comments_markup_id"]=array(
  	'#type' => 'value', 
	'#value' => $dpidashboard_comments_markup_id
  );
  
  $dpidashboard_comments_markup_id++;
	
  return $form;
  
}

function comments_ajax_replace_form_markup_callback($form, $form_state) {

  return $form['markup'];

}

function _viewsnode_form_render_form($form,&$form_state){
  $args=$form_state["build_info"]['args'][0];
  
  $values = _dpidashboard_forms_ajax_get_values($form_state);
  
  $periodes = _dpidashboard_forms_ajax_get_perides();

  $form['select']=array(
    '#type'=>'select',
    "#title"=>'Periode',
    "#options"=>$periodes,
	'#default_value' => $values['select'],
	'#attributes'=>array(
	  'autocomplete' => 'off',
	),
    '#ajax' => array(
      'callback' => 'viewsnode_ajax_replace_form_markup_callback',
      'wrapper' => 'viewsnode_markup_content',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  
  $stats = dpidashboard_get_views_node_for_periode($values['nid'],$values['select']);
  
  $markup = "
    
 	<script>
      function drawChart() {
        var data = google.visualization.arrayToDataTable([
         ['Page', 'Views'],
         ['Current', ".$stats['count']."],            // RGB value
         ['Average', ".$stats['avg']."],            // English color name
      ]);

        var options = {
         
          hAxis: {minValue: 0}
        };

        var chart = new google.visualization.BarChart(document.getElementById('viewsnode_markup_content_chart'));

        chart.draw(data, options);
      }
	  drawChart();
	  </script>
	 ";
	/* 
  if ($stats) $markup = $stats['count'].' / '.$stats['avg'];
  else $markup = 'n/a';
  */

  $form["markup"]=array(
    '#markup'=>$markup,
    '#prefix' => '<div id="viewsnode_markup_content_chart"></div><div id="viewsnode_markup_content">',
    '#suffix' => '</div>',
  );
  
  $form["nid"]=array(
  	'#type' => 'value', 
	'#value' => $values['nid']
  );
  
  $form['#attached']['js'] = array(
	"https://www.google.com/jsapi?autoload={'modules':[{'name':'visualization','version':'1','packages':['corechart']}]}",
  );
  
  return $form;
}



function viewsnode_ajax_replace_form_markup_callback($form, $form_state) {

  return $form['markup'];

}

function _dpidashboard_view_remove_item_from_periode($periode, &$view) {
  if ($periode!='1d') $view->set_item('default','field','daycount',NULL);
  
  if ($periode!='1w') $view->set_item('default','field','weekcount',NULL);
  
  if ($periode!='1m') $view->set_item('default','field','monthcount',NULL);
  
  if ($periode!='1y') $view->set_item('default','field','yearcount',NULL);
  
  if ($periode!='all') $view->set_item('default','field','totalcount',NULL);
  
}

function _dpidashboard_view_sort_from_periode($periode, &$view) { 

  $field = _dpidashboard_periode_to_field($periode);
  
  $sort_part = array(
  	'id' => '',
	'table' => 'dpistat_node_counter',
	'field' => '',
	'order' => 'DESC'
  );	
  
  $sort_part['id'] = $field;
  $sort_part['field'] = $field; 

  $sort = array(
	$field => $sort_part
  );
  
  $view->display_handler->set_option('sorts', $sort);
}

function _toparticles_form_render_form($form,&$form_state){
  $args=$form_state["build_info"]['args'][0];
  
  $values = _dpidashboard_forms_ajax_get_values($form_state);
  
  $periodes = _dpidashboard_forms_ajax_get_perides();

  $form['select']=array(
    '#type'=>'select',
    "#title"=>'Periode',
    "#options"=>$periodes,
	'#default_value' => $values['select'],
	'#attributes'=>array(
	  'autocomplete' => 'off',
	),
    '#ajax' => array(
      'callback' => 'toparticles_ajax_replace_form_markup_callback',
      'wrapper' => 'toparticles_markup_content',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  
  $markup = "";
  //$markup = views_embed_view('dpidashboard_top_articles');
  $view = views_get_view('dpidashboard_top_articles');
  $view->set_display('default');
  //$view->set_arguments(array($tid));
  // change the amount of items to show
  //$view->set_items_per_page(4);
  
  _dpidashboard_view_remove_item_from_periode($values['select'], $view);
  _dpidashboard_view_sort_from_periode($values['select'], $view);
  
  $view->pre_execute();
  $view->execute();
  $markup .=  $view->render();

  $form["markup"]=array(
    '#markup'=>$markup,
    '#prefix' => '<div id="toparticles_markup_content">',
    '#suffix' => '</div>',
  );
  
  $form["nid"]=array(
  	'#type' => 'value', 
  );
  
  
  return $form;
}



function toparticles_ajax_replace_form_markup_callback($form, $form_state) {

  return $form['markup'];

}
