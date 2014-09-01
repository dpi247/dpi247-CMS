<?php
/**
 * @file
 *   Default theme implementation for the CoveritLive Player
 */
?>
<div id="piktowrapper-embed">
    <div class="pikto-canvas-wrap">
        <div class="pikto-canvas"></div>
    </div>
</div>
<script>
    (function(d){
        var js, id="pikto-embed-js", ref=d.getElementsByTagName("script")[0];
        if (d.getElementById(id)) {return;}
        js=d.createElement("script"); js.id=id; js.async=true;
        js.src="https://magic.piktochart.com/assets/embedding/embed.js?UID=<?php print $vars['id'] ?>";
        ref.parentNode.insertBefore(js, ref);
    }(document));
</script>
