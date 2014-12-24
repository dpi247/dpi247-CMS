<?php
/**
 * @file
 *   Default theme implementation for the Scald Storify Player
 */
?>
<div class="storify">
<iframe  width="435" height="326" src="//storify.com/<?php print $vars['base_url'] ?>/embed?border=false&template=slideshow"
frameborder=no allowtransparency=true></iframe><script 
src="//storify.com/<?php print $vars['base_url'] ?>.js?border=false&template=slideshow"></script>
<noscript>[<a href="//storify.com/<?php print $vars['base_url'] ?>" target="_blank">View the story "<?php print $vars['title'] ?>" on Storify</a>]</noscript></div>
