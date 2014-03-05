<?php
  $date = $variables['fields']['date'];
  $timestamp = $date['#items'][0]['value'];
  $comment_count = $variables['fields']['comment_count'];
?>
<span class="meta">
  <span class="author">
    <?php print t('Published'); ?><time datetime="<?php print $timestamp; ?>"><?php print drupal_render($date)?></time>
  </span>
  <span id="comment-count">
    <i class="icon-comment"></i><?php print $comment_count; ?>
  </span>
</span>