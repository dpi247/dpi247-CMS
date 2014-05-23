<?php
/**
 * @file
 *   Default theme implementation for the Scald Storify Player
 */
?>
<div id="flash_kplayer_<?php print $vars['id'] ?>" class="flash_kplayer" name="flash_kplayer" data-sig="<?php print $vars['id'] ?>" data-playerkey="306eedd58f91" 
style="width:400px; height:300px;"><object  width="100%" height="100%" type="application/x-shockwave-flash" 
data="http://sa.kewego.com/swf/kp.swf" name="kplayer_<?php print $vars['id'] ?>" id="kplayer_<?php print $vars['id'] ?>"><param name="bgcolor" value="0x000000" />
<param name="allowfullscreen" value="true" />
<param name="allowscriptaccess" value="always" />
<param name="flashVars" value="language_code=fr&playerKey=306eedd58f91&configKey=e0cfb40c94d8&suffix=&sig=<?php print $vars['id'] ?>&autostart=false" />
<param name="movie" value="http://sa.kewego.com/swf/kp.swf" /><param name="wmode" value="opaque" /><param name="SeamlessTabbing" value="false" />
<video poster="" height="100%" width="100%" preload="none"  controls="controls"></video>
<script src="http://sa.kewego.com/embed/assets/kplayer-standalone.js"></script>
<script defer="defer">kitd.html5loader("flash_kplayer_<?php print $vars['id'] ?>","http://api.kewego.com/video/getHTML5Thumbnail/?playerKey=306eedd58f91&sig=<?php print $vars['id'] ?>");</script></object></div>
