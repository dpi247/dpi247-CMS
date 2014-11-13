<div class="dpiblocks-panel dpiblocks-panel-default">
<?php
foreach ( $variables ["list_node"] as $node ) {
  '<div class="media">'.print render ( $node ).'</div>';
}
?>
</div>