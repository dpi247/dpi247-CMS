<?php 
	$video_id = $vars["video_id"]; 
	$key = $vars["video_key"];
	$domain = $vars["video_domain"];
	$liveid = $vars["video_liveid"];

	if (!empty($video_id)) {
		$url = "http://www.rtl.be/" . $domain . "/page/rtl-video-en-embed/640.aspx?VideoID=" . $video_id . "&key=" . $key;
	} else {
		$url = "http://www.rtl.be/videos/page/rtl-video-en-embed/640.aspx?LiveID=" . $live_id;
	}
?>

<?php /* en 16:9 */ ?>
<div class="gr-media gr-media--16_9 gr-media--video gr-media--rtl">
    <iframe class="gr-media__content" frameborder="0" src="<?php print $url; ?>"></iframe>
</div>

<?php /* en 4:3 
<div class="gr-media gr-media--4_3 gr-media--video gr-media--rtl">
    <iframe class="gr-media__content" frameborder="0" src="<?php print $url; ?>"></iframe>
</div>
*/
?>