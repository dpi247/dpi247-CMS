<div class="panel panel-default">
<?php
foreach ( $variables ["list_node"] as $node ) {
  '<div class="media">'.print render ( $node ).'</div>';
}
?>
</div>