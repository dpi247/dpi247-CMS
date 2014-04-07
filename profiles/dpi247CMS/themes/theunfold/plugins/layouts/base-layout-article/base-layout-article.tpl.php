<?php if (!empty($css_class)) { print check_plain($css_class); } ?>
<div class="panel-display " id="pane<?php if (!empty($css_id)) { print "-".$css_id; } ?>">

<div class="row">
	<section id="content-top">
		<div id="content-top">
			<?php if(isset($content['contenttop'])) print $content['contenttop']; ?>
		</div> <!-- /content-top -->
	</section>
</div> <!-- /row -->

<div class="row">
	<section id="main-content" class="article">
		<?php if(isset($content['mainContent'])) print $content['mainContent']; ?>
	</section> <!-- /main-content -->

	<aside role="complementary">
	  <?php if(isset($content['complementary'])) print $content['complementary'] ?>
	</aside> <!-- /complementary  -->
</div> <!-- /row -->
<?php 
  if(isset($content['subcontentleft']) || isset($content['subcontentright'])) :
?>
<div class="row">
	<section id="sub-content">
		<div id="sub-content-left">
			<?php if(isset($content['subcontentleft'])) print $content['subcontentleft']; ?>
		</div> <!-- /subcontentleft -->
		<div id="sub-content-right">
			<?php if(isset($content['subcontentright'])) print $content['subcontentright']; ?>
		</div> <!-- /sub-content-right -->
	</section>
</div> <!-- /row -->
<?php
	endif;
?>
<div class="row">
	<section id="content-bottom">
		<div id="content-bottom">
			<?php if(isset($content['contentbottom'])) print $content['contentbottom']; ?>
		</div> <!-- /content-top -->
	</section>
</div> <!-- /row -->

</div> <!-- /pane<?php if (!empty($css_id)) { print "-".$css_id; } ?> -->
