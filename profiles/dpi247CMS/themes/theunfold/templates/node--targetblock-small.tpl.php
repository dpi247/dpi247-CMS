<?php print drupal_render($content['field_iptc']);?>

<h2>
    <?php print drupal_render($content["field_displaytitle"])?>
	
</h2>
	<?php print drupal_render($content['field_package_infos']);?>
<div class="description">
	<?php print drupal_render($content['field_textbody'])?>
</div>
<div>
<?php print drupal_render($content);?>
</div>