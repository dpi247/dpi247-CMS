<?php

print "<div class='damsearch damwrapper'>";

print "<div class='cat damsearch damnavigationinfo'>";
print "<span class='damsearch damcurrentpage'>".t("Page")." ".$current_page."</span>";
print " / ";
print "<span class='damsearch damtotalresults'>".$totalresults." ".t("Results")."</span>";
print "</div>";

print "<div class='cat damsearch dampagination'>";
if (isset($first)) {
  print "<a href=\"".$first["url"]."\">".t("first")."</a> / ";
}

if (isset($prev)) {
  print "<a href=\"".$prev["url"]."\">".t("prev")."</a> / ";
}

$phtml = array();
$tmppnumber=-1;
foreach($pages as $kapage=>$page) {
  if ($tmppnumber<>($page["number"]-1) && $tmppnumber<>-1) {
    $phtml[] = "...";
  }
  $tmppnumber = $page["number"];
  $phtml[] = "<a href=\"".$page["url"]."\">".$page["number"]."</a>";
}
print (implode(" / ", $phtml));

if (isset($next)) {
  print " / <a href=\"".$next["url"]."\">".t("next")."</a>";
}
if (isset($last)) {
  print " / <a href=\"".$last["url"]."\">".t("last")."</a>";
}
print "</div>";

print "</div>";
print "<div class='damsearch damclearboth'></div>";
