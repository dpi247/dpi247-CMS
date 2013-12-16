<?php if(isset($content['field_textbarette'])):?>
<span class="cat"><?php print drupal_render($content['field_textbarette'])?></span>
<?php endif;?>

<h1><?php print drupal_render($content['field_displaytitle']);?>ss</h1>

<div class="meta">
	<?php if (isset($content['field_copyright'])){?>
  	<p class="author"><?php print drupal_render($content['field_copyright'])?></p>
	<?php }?>
	<p class="date">
		<?php print t('Published') ?> <time datetime="<?php print $publicationdate?>">
		<?php print drupal_render($content['field_editorialpublicationdate']);?>)</time> |
		<span id="comment-count"><a href="#comments-container" class="comment-count-link"><i class="lsf">comment</i><?php print $comment_count; ?></a></span>
	</p>
</div>

<h2 class="heading"><?php print drupal_render($content['field_textchapo']);?></h2>

<?php print $top_html; ?>
<?php print drupal_render($content['field_textbody']);?>

<div class="article-body">
    <?php print drupal_render($content['field_linkslists']);?>
	<?php if ($related_html != NULL){?>
	<aside class="hidden-phone" role="complementary">
		<?php print $related_html;?>
	</aside>
	<?php }?>
	<div class="article-description">
	<?php //print drupal_render(['field_textbody']);?>
	</div>
</div>

<?php print $bottom_html; ?>
