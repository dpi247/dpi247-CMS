<div class="article-group box red">
	<blockquote>
		<a href="/<?php print drupal_get_path_alias("node/".$nid)?>" title="">
			<p>"<?php print drupal_render($content['field_displaytitle'])?>"</p> 
			<cite><?php print drupal_render($content['field_authors'])?></cite>
		</a>
	</blockquote>
</div>
