<ul class="article-group">
  <?php if (isset($facets) && is_array($facets)) : ?>
  	<?php foreach ($facets as $facet) : ?>
  	  <?php print theme('damsearch_render_facet', array('facet' => $facet, 'profile' => $profile)); ?>
    <?php endforeach; ?>
  <?php endif; ?>
</ul>