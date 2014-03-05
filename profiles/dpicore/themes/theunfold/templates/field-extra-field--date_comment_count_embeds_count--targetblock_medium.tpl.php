<?php
  $date = $variables['fields']['date'];
  $timestamp = $date['#items'][0]['value'];
  $comment_count = $variables['fields']['comment_count'];
  $embeds_count = $variables['fields']['embeds_count'];
?>
<span class="meta">
  <span class="author">
    <?php print t('Published'); ?><time datetime="<?php print $timestamp; ?>"><?php print drupal_render($date)?></time>
  </span>
  <span id="comment-count">
    <i class="icon-comment"></i><?php print $comment_count; ?>
  </span>
  <?php if ($embeds_count) : ?>
    <?php foreach ($embeds_count as $embed_type => $embed_count) : ?>
      | <span class="hidden-phone lsf"><?php print $embed_type; ?></span> <?php print $embed_count; ?>
    <?php endforeach; ?>
  <?php endif; ?>
</span>