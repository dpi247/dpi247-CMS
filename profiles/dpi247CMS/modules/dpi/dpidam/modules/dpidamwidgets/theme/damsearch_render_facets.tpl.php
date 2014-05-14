<ul class="article-group">
	<?php foreach ($facets as $facet) {
	  print theme('damsearch_render_facet', $facet, $profile);
    } ?>
</ul>