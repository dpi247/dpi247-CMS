<?php
/**
 * @file
 *   Default theme implementation for the Scald facebook status
 * Available vars are :
 * $vars['title']     : the atom title
 * $vars['base_id']   : the atom base id
 * $vars['user_id']   : the atom user id
 * $vars['data_href'] : the facebook data-href
 * $vars['country']   : the country
 * $vars['thumbnail'] : the atom thumbnail link
 */
?>
<div id="fb-root"></div>
<script>(function(d, s, id) { var js, fjs = d.getElementsByTagName(s)[0]; if (d.getElementById(id)) return; js = d.createElement(s); js.id = id; js.src = "//connect.facebook.net/<?php print $vars['country']; ?>/all.js#xfbml=1"; fjs.parentNode.insertBefore(js, fjs); }(document, 'script', 'facebook-jssdk'));</script>
<div class="fb-post" data-href="<?php print $vars['data_href']; ?>" data-width="550"></div>