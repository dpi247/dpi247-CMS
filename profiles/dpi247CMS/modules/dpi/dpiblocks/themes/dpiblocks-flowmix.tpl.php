<?php

if (!function_exists('dpiblocks_flowmix_build_feed')) {
  function dpiblocks_flowmix_build_feed($feed) {
    $content = '';
    foreach ($feed as $k => $item) {
      $entity_wrapper = entity_metadata_wrapper('node', $item);
      $content .= '<li>';
      if (isset($entity_wrapper->field_externalurl) && $field_externalurl = $entity_wrapper->field_externalurl->value()) {
        $content .= '<a href="'.$field_externalurl['url'].'">';
      }
      if (isset($entity_wrapper->field_displaytitle) && $field_displaytitle = $entity_wrapper->field_displaytitle->value()) {
        $content .= isset($field_displaytitle['safe_value']) ? $field_displaytitle['safe_value'] : $field_displaytitle['value'];
      }
      elseif (isset($item->title)) {
        $content .= $item->title;
      }
      if ($field_externalurl) {
        $content .= '</a>';
      }
      if (isset($entity_wrapper->field_textbody) && $field_textbody = $entity_wrapper->field_textbody->value()) {
        $content .= '<span>';
        $content .= isset($field_textbody['safe_value']) ? $field_textbody['safe_value'] : check_plain($field_textbody['value']);
        $content .= '</span>';
      }
      $content .= '</li>';
    }
    return $content; 
  }
}

?>

<div class="flowmix-bloc">
	<h2><?php print check_plain($settings->title); ?></h2>
	<div class="flowmix-list">
		<ul>
		<?php print dpiblocks_flowmix_build_feed($feed); ?>
		</ul>
	</div>
</div>
