<?php print $content['#prefix'];?>
<figure>
	<?php print drupal_render($content['field_mediabox']);?>
</figure>
<?php print $content['#suffix'];?>
<?php print drupal_render($content['field_iptc']);?>
<?php print $content['#prefix'];?>
<h2>
	<?php print drupal_render($content["field_displaytitle"])?>
</h2>
<?php print $content['#suffix'];?>
<?php print drupal_render($content['field_package_infos']);?>
<?php print $content['#prefix'];?>
<div class="description">
	<?php print drupal_render($content['field_textbody'])?>
</div>
<?php print $content['#suffix'];?>