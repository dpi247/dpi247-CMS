<?php

/**
 * @file search-results.tpl.php
 * Default theme implementation for displaying search results.
 *
 * This template collects each invocation of theme_search_result(). This and
 * the child template are dependant to one another sharing the markup for
 * definition lists.
 *
 * Note that modules may implement their own search type and theme function
 * completely bypassing this template.
 *
 * Available variables:
 * - $results: All results as it is rendered through
 *   search-result.tpl.php
 *
 */
?>
<!--  Liste des réponses du DAM -->
<ul class="article-group">
  <?php if (isset($response) && isset($response['results']) && is_array($response['results'])) : ?>
  	<?php foreach ($response['results'] as $result) : ?>
      <?php print theme('damsearch_render_result', array('result' => $result, 'profile' => $profile)); ?>
    <?php endforeach; ?>
  <?php endif; ?>
</ul>
<!--  /Liste des réponses du DAM -->
