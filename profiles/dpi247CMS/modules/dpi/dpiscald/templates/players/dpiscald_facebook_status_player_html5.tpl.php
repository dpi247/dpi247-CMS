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

<div class="gr-media gr-media--facebook">
    <div class="fb-post" data-href="<?php print $vars['data_href']; ?>" data-width="<?php print $vars['width']; ?>"></div>
    <div class="gr-media__placeholder gr-media__placeholder--warning">
        <a class="gr-media__placeholder-link" href="<?php print $vars['data_href']; ?>" target="_blank">
            <span class="fa fa-facebook-square fa-2x"></span>Voir la vue optimisée
        </a>
    </div>
</div>

<!-- FACEBOOK script à ajouter une fois dans la page -->
<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/fr_FR/sdk.js#xfbml=1&appId=1009728159055743&version=v2.0";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));
</script>