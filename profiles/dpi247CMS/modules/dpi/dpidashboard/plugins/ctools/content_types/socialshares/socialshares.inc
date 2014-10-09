<?php

$plugin = array(
  'title' => t('Social Shares'),
  'description' => t('Simple example of a ctools content type wit settings .'),
  
  // The default context.
  'defaults' => array(),
  'single' => TRUE,
  
  'admin info' => 'socialshares_admin_info',
 // 'content_types' => array('dummycontenttypes_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'socialshares_content_type_render',
  'callback' => '_dpidashboard_user_has_permission_displaydashboardnode',
  
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  'edit form' => 'socialshares_edit_form',
  
  
  'category' => t('Dashboard'),
  
  
  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'all contexts' => TRUE,
  //'required context' => new ctools_context_required(t('Destination'), 'destination'),
);

function socialshares_admin_info($subtype, $conf, $context = NULL) {
	
  	$block = new stdClass;
	$block->title = 'Socialshares ctools contenttype';
	$block->content = t('Number of shares of current URL on social medias');
	return $block;
}

function socialshares_edit_form($form, &$form_state) {
 
 $conf=$form_state['conf'];
 $args=$form_state['args'];
 $contexts=$form_state['contexts'];

 $form["options"]=array("#tree"=>TRUE);
 $form['options']['facebook']=array(
   '#type'=>'checkbox',
   '#title'=>'Facebook',
   '#Description'=>t('Count number of Facebook shares'),
   "#default_value"=>$conf['options']['facebook'],
   
   
   );

 $form['options']['twitter']=array(
   '#type'=>'checkbox',
   '#title'=>'Twitter',
   '#Description'=>t('Count number of Twitter shares'),
   "#default_value"=>$conf['options']['twitter'],
    
    
 );
 
 $form['options']['googleplus']=array(
   '#type'=>'checkbox',
   '#title'=>'Google+',
   '#Description'=>t('Count number of Google+ shares'),
   "#default_value"=>$conf['options']['googleplus'],
    
    
 );
 
 
 $form['options']['linkedin']=array(
   '#type'=>'checkbox',
   '#title'=>'LinkedIn',
   '#Description'=>t('Count number of LinkedIn shares'),
   "#default_value"=>$conf['options']['linkedin'],
    
    
 );
   
  return $form;
}

function socialshares_edit_form_submit($form, &$form_state) {
  
  $value_to_save=array('facebook','twitter','googleplus','linkedin',);
  
  foreach($form_state["values"]['options'] as $key=>$item){
    $form_state['conf']['options'][$key]=$form_state['values']['options'][$key];
    
  }
  
}

function socialshares_content_type_render($subtype, $conf, $args, $context) {
	
  $node = $context['argument_entity_id:node_1']->data;

  $description = (!empty($conf['description'])) ? check_plain($conf['description']) : '';
  $title = 'Social Shares';
  
  $output = '
  <div id="piechartSocialShare"></div>
  <script type="text/javascript">
  	  arraySocialShares = [
	  	["Network", "Shares"]
	  ];
  	
      dataSocialShares = google.visualization.arrayToDataTable(arraySocialShares);
     
      function drawChartSocialShares(data) {

        var chart = new google.visualization.PieChart(document.getElementById("piechartSocialShare"));
		
		var options = {};


        chart.draw(data, options);
      }
	  
	  drawChartSocialShares(dataSocialShares);
    </script>
';
  
  $script = '';
  
  foreach($conf['options'] as $key=>$value) {
    if ($value==1) {
		$script.="
		jQuery.ajax({
			url: '/dpidashboard/ajax/getsocialshares?social=".$key."&url='+document.URL,
			cache: false,
			success:function(result){
				if (result.success==false) {
					alert(result.error);
				} else {
					if (result.count_str>0) {
						arraySocialShares.push(['".$key."',result.count_str]);
						dataSocialShares = google.visualization.arrayToDataTable(arraySocialShares);
						drawChartSocialShares(dataSocialShares);
					}
				}
			}
		});
		";
	}
  }
  
  $values = array(
  	'script'	=> $script
  );

  $output .= '<script>'.$script.'</script>';

  $block = new stdClass();
  $block->module = 'socialshares';
  $block->delta = $subtype;
  $block->title = $title;
  $block->content = $output;

  return $block;

}

