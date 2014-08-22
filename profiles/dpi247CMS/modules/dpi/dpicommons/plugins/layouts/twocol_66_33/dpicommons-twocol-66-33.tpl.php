<?php
/**
 * @file
 * Template for a 3 column panel layout.
 *
 * This template provides a three column panel display layout, with
 * each column roughly equal in width.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   - $content['left']: Content in the left column.
 *   - $content['middle']: Content in the middle column.
 *   - $content['right']: Content in the right column.
 */
?>

<div class="panel-display  clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  
  <div class="panel-panel panel-2col-66 panel-col-first">
    <div class="inside"><?php print $content['left']; ?></div>
  </div>

  <div class="panel-panel panel-2col-33 panel-col-last">
    <div class="inside"><?php print $content['right']; ?></div>
  </div>
  
  
  
</div>

<div class="panel-display  clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <h3>Categorisation</h3>
<div class="panel-panel panel-2col-33 panel-col-last">
    <div class="inside"><?php print $content['tag1']; ?></div>
  </div>
  <div class="panel-panel panel-2col-33 panel-col-last">
    <div class="inside"><?php print $content['tag2']; ?></div>
  </div>
  <div class="panel-panel panel-2col-33 panel-col-last">
    <div class="inside"><?php print $content['tag3']; ?></div>
  </div>  
  
  
</div>


<div class="panel-display  clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  
<div class="panel-panel panel-2col-99 panel-col-last">
    <div class="inside"><?php print $content['bottom']; ?></div>
  </div>

  
</div>
