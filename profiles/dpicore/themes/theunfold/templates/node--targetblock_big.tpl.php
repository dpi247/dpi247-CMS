<ul class="article-group">
  <li class="">
    <a href="/<?php print drupal_get_path_alias("node/".$nid)?>">
	  <h2 class="big-title"><?php print drupal_render($content['field_displaytitle'])?> 
	    <span class="meta"> 
	      <?php if(isset($content['field_authors'])):?>
	      <span class="author">By <span class="author-name"><?php print drupal_render($content['field_authors'])?></span>
  	      <?php endif;?>
  	      <span id="comment-count"><i class="lsf">comment</i>68</span>
        </span>
      </h2>
	  <figure>
	    <img src="img/greece.png">
        <figcaption>Donec pharetra venenatis mauris</figcaption>
	  </figure>
    </a>
		<div class="description">
			<p>Mauris nec elit sapien. Nulla imperdiet nibh et arcu aliquam non
				lacinia mi condimentum. Vivamus et fringilla dui. Morbi nec neque
				libero, ac mattis elit. Etiam ullamcorper elit quis enim cursus
				congue.</p>
		</div>
		<ul class="article-list">
			<li><a href="" title="">Vestibulum malesuada, dui nec vehicula
					egestas</a>
			</li>
			<li><a href="" title="">Aenean sit amet orci diam, ac iaculis sapien</a>
			</li>
		</ul></li>
</ul>
