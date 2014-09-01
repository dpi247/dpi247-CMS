<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
</head>
<body>
	<div>
		<span>
		<?php
		if (!$datas['nid']) {
		  print t('Package not imported yet.');
		}
		else {
		  $content = t('Package').' : <a href="http://'.$datas['base_url'].'/node/'.$datas['nid'].'" onClick="return popup(this, \'DPI_web_page\');">'.$datas['title'].'</a>. ';
		  foreach ($datas['options'] as $option => $op_datas) {
		    $content .= !empty($op_datas['label']) ? $op_datas['label'].' : ' : '';
		    $content .= $op_datas['value'];
		    $content .= !empty($op_datas['label']) ? '. ' : ' ';
		  }
		  print $content;
		}
		?>
		</span>
	</div>
</body>
</html>