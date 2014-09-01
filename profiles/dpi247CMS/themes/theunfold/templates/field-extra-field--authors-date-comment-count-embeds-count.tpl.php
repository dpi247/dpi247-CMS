<?php
  $date = $variables['fields']['date'];
  $authors = $variables['fields']['authors'];
  $timestamp = $date['#items'][0]['value'];
  $comment_count = $variables['fields']['comment_count'];
?>
<div class="meta">
<?php if($authors):?>
<span class="author">By <span class="author-name"><?php print drupal_render($authors)?></span></span>
<?php endif;?>
  <span class="date">
    <?php print t('Published'); ?><time datetime="<?php print $timestamp; ?>"><?php print drupal_render($date)?></time>|
   </span>
    
    
    <span id="comment-count">
      <a href="#comments-container" class="comment-count-link"><i class="lsf">comment</i><?php print $comment_count; ?></a>
    </span>
</div>