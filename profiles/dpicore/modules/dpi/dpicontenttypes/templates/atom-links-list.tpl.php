<aside class="hidden-phone" role="complementary">
  <?php if($content['field_displaytitle']) :?>
  <h2 class="section-title"><?php print drupal_render($content['field_displaytitle'])?></h2>
  <?php endif;?>
    <?php print drupal_render($content['field_linkitems'])?>  
</aside>



