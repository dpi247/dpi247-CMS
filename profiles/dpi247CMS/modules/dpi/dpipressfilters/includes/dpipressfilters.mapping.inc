<?php

function dpipressfilters_mapping_export($filter_name/*, $format*/) {
  global $user;
  $all_formats = filter_formats($user);
  $mapping = new stdClass();
  $mapping->filter = $filter_name;
  $mapping->formats = array();
  $mapping->tags = array();
  
  foreach($all_formats as $format => $format_infos){
    $mapping->formats[$format] = array(
      'format' => $format_infos->format,
      'name' => $format_infos->name,
    );
    $tags = dpipressfilters_gettagsfromfilterandformat($filter_name, $format);
    foreach ($tags as $tag) {
      if (!isset($mapping->tags[$tag->tag])) {
        $mapping->tags[$tag->tag] = array(
          'type' => $tag->type,
        );
      }
      $mapping->tags[$tag->tag][$format] = array(
        'operation' => $tag->settings['operation'],
        'html_tag' => $tag->settings['html_tag'],
        'classes' => $tag->settings['classes'],
      );
    }
  }

  drupal_json_output($mapping);
}
