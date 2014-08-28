<?php print $content['#prefix']; ?>
  <h2 class="big-title">
	<?php print drupal_render($content["field_displaytitle"])?>
  </h2>
<?php print $content['#suffix']; ?>
<?php print drupal_render($content['field_package_infos']);?>
<figure class="no_width">
    <?php print drupal_render($content['field_mediabox']); ?>		
</figure>
<div class="description">
	<?php print drupal_render($content['field_textbody']);?>
</div>

<ul class="article-list">
	<?php print drupal_render($content['field_linkslists']);?>
</ul>
