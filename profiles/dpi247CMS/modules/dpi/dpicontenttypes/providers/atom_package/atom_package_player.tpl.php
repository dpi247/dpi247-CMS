<div class="<?php print $variables['classes']; ?>">
  <img class="<?php print $variables['classes']; ?>-img" src="<?php print $variables['vars']['thumbnail'];?>" alt="<?php print $variables['vars']['thumbnail'];?>" />
  <div class="<?php print $variables['classes']; ?>-content">
    <div class="<?php print $variables['classes']; ?>-type">
      <span><?php print $variables['vars']['node']->type; ?></span>
    </div>
    <h2><?php print $variables['vars']['node']->title; ?></h2>
  </div>
  <div class="<?php print $variables['classes']; ?>-author">
    <span>Created by <?php print $variables['atom']->data['author'][$variables['atom']->language][0]['name']; ?>.</span>
  </div>
</div>