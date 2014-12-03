<div class="dpiblocks-panel dpiblocks-panel-default">
<?php
foreach ( $variables ["rows"] as $node ) {
  '<div class="media">'.print render ( $node ).'</div>';
}
?>
</div>