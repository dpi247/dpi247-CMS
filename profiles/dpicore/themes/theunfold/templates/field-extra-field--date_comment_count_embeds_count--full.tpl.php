<?php
  $date = $variables['fields']['date'];
  $timestamp = $date['#items'][0]['value'];
  $comment_count = $variables['fields']['comment_count'];
?>
<div class="meta">
  <p class="date">
    <?php print t('Published'); ?><time datetime="<?php print $timestamp; ?>"><?php print drupal_render($date)?></time>
    |
    <span id="comment-count">
      <a href="#comments-container" class="comment-count-link"><i class="lsf">comment</i><?php print $comment_count; ?></a>
    </span>
  </p>
</div>