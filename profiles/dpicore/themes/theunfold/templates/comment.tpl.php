<?php
if ($comment->picture) {
  $image_style_vars = array(
    'path' => $comment->picture,
    'style_name' => '70x70',
    'width' => 70,
    'height' => 70,
  );
  print theme('image_style', $image_style_vars);
}
?>
<div class="comments-body">	
	<div class="meta">
		<span class="author"><?php print t('By')?> <?php print $author?></span>
		<time datetime="<?php print date('Y-m-d', $comment->changed);?>">
		  <?php print t(date('F', $comment->changed)).' '.date('j', $comment->changed).', '.date('Y', $comment->changed);?>
		</time>
	</div>
	<?php hide($content['links']); ?>
	<?php print drupal_render($content); ?>
	<div class="actions">
		<ul class="no-list">
			<li><a title="" href="/comment/reply/<?php print $node->nid.'/'.$comment->cid;?>"><i class="lsf">comment</i><?php print t('Reply');?></a></li>
			<li><a title="" href="/comment/edit/<?php print $comment->cid;?>"><span class="lsf">edit</span><?php print t('Edit');?></a></li>
			<li><a title="" href="/comment/delete/<?php print $comment->cid;?>"><span class="lsf">delete</span><?php print t('Delete');?></a></li>
		</ul>
	</div>
</div>