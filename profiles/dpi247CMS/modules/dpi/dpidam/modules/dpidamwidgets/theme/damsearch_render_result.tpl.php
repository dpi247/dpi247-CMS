<?php
/**
 * 
 */
?>
<li class="article-inline small">
  <a href="<?php print $url; ?>">
    <?php if ($img): ?>
		<figure>
			<?php print $img; ?>
		</figure>
		<?php endif; ?>
		<h2>
			<?php print $title; ?>
			<span class="meta">
			  <span id="comment-count">
		  	  <?php print $info; ?>
		  	</span>
			  <!-- <span class="author">
			    By <span class="author-name">Aenean sit</span>
			  </span>
			  <span id="comment-count">
			    <i class="lsf">comment</i>68
			  </span> -->
			</span>
		</h2>
  </a>
	<div class="description">
		<p><?php print $snippet; ?></p>
	</div>
</li>
