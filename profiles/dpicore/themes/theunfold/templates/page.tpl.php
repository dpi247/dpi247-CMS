<!DOCTYPE html>
<!--[if lt IE 7 ]> <html lang="en" class="no-js ie6"> <![endif]-->
<!--[if IE 7 ]>    <html lang="en" class="no-js ie7"> <![endif]-->
<!--[if IE 8 ]>    <html lang="en" class="no-js ie8"> <![endif]-->
<!--[if IE 9 ]>    <html lang="en" class="no-js ie9"> <![endif]-->
<!--[if (gt IE 9)|!(IE)]><!-->
<html lang="en" class="no-js">
<!--<![endif]-->

<body class="<?php print $classes?> " <?php print $attributes;?>>
	<div class="page">
		<header id="masthead" role="banner">

			<h1 class="h-sections visible-phone">
				<a href="#"><i class="icon-reorder"></i>Menu</a>
			</h1>

			<div id="toolbar" role="toolbar">
				<!-- Set tolls & utils menu here -->
				<nav class="utils">
					<?php print render($toolbarRight);?>
				</nav>
				<nav>
					<?php print render($toolbarLeft);?>
				</nav>


			</div>
			<!-- /toolbar -->
			<div id="logo">
				<?php if ($logo) : print '<img src="'.check_url($logo).'" alt="'.$site_name.'" id="logo" />'; endif; ?>
				<h1>
					<a id="branding" href="/"> <?php if ($site_name): print $site_name; endif; ?>
					</a>
					<?php if ($site_slogan): print '<p class="news-slogan">'.$site_slogan.'</p>'; endif; ?>
				</h1>
				<p class="news-date">
					<time datetime="2012-12-12">
						<?php print t(date('F')).' '.date('j').t(date('S')).', '.date('Y');?>
					</time>
				</p>
			</div>
			<!-- /logo -->
		</header>

	
		<div id="main" role="main">
    

			<div class="nav-primary">
				<nav id="nav-wrapper" role="navigation">
		            <?php if (isset($primary_nav)): print $primary_nav; endif; ?>
                    <?php if (isset($secondary_nav)) : ?>
					<div id="sub-menu">
						<nav>
					      <?php  print $secondary_nav; ?>
                    	</nav>
					</div>
					<?php endif; ?>
				</nav>
			</div>


			<?php if ($show_messages && $messages): ?>
			<?php print $messages; ?>
			<?php endif; ?>
			
			<?php if (!empty($page['content'])): ?>
			<?php print render($page['content']); ?>
			<?php endif; ?>

		</div>
		<!-- /main -->
		<div id="bottom">
			<?php  // print $bottom;?>
		</div>
		
		<?php //  if ($mission || $footer_message ) {?>
		<footer>
			<?php  // if ($mission): print "<p class=\"mission\">".$mission."</p>"; endif; ?>
			<?php // if ($footer_message): print "<p class=\"footer-message\">".$footer_message."</p>"; endif; ?>
		</footer>
		<?php // } ?>
	</div>
	<!-- /page -->


	<!-- Put Scripts at the Bottom  -->
	<?php //print $closure; ?>
</body>
</html>
