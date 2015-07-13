<?php 
	$video_id = $vars["video_id"]; 
	$key = $vars["video_key"];
	$domain = $vars["video_domain"];
?>
<iframe frameborder="0" width="600" height="338" src="http://www.rtl.be/<?php print $domain ?>/page/rtl-video-en-embed/640.aspx?VideoID=<?php print $video_id ?>&key=<?php print $key ?>"></iframe>
