<?php
/**
 * $term = array of info about the facet term
 * $facet = facet inforation
 * $profile = search profile
 */

print "<li class='facet-term facet-term-$term_display'>";

if ($term_active) {
	print "<span class='lsf symbol'>checkbox</span> ";
} else {
	print "<span class='lsf symbol'>checkboxempty</span> ";
} 
print "<a href='$term_url'>".t($term_display)." ($term_frequency)</a></li>";

