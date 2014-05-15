<?php
/**
 * Arguments
 *
 * $facet = facet avec tous les terms.
 * $profile = profile de recherche
 */

print "<li class='cat facet facet-$facet_dname'>$facet_dname";
print "<ul class='facet'>";

if ($facet_active) {
  print "<li><span class='lsf symbol'>checkboxempty</span> <a href='$facet_url'>All</a></li>";
} else {
  print "<li><span class='lsf symbol'>checkbox</span>  All</li>";
}

print "<li>";
foreach($facet->terms as $term) {
  print "<ul class='cat facet facet-term'>";
  print theme('damsearch_render_facet_term', array('term' => $term, 'facet' => $facet, 'profile' => $profile));
  print "</ul>";
}
print "</li>";
print "</ul>";
print "</li>";
