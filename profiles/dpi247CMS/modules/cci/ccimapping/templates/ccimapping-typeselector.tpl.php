<?php print t('Choose a content type :'); ?>
<ul>
<?php foreach (node_type_get_types() as $content_type) : ?>
   <li><?php print l($content_type->name, CCI_ADMIN_PATH.'/ccimapping/'.$type.'/editor/'.$content_type->type); ?></li>
<?php endforeach; ?>
</ul>