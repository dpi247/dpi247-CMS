<div class="article-group box red">
	<blockquote>
		<?php print $content['#prefix'];?>
			<p><?php print drupal_render($content['field_displaytitle']);?></p> 
			<cite><?php print drupal_render($content['field_authors']);?></cite>
		<?php print $content['#suffix'];?>
	</blockquote>
</div>