<?php

/**
 * Get a md5 from some elements of a package
 */
function _dpiblocks_get_md5_from_package($package) {
  $entity_wrapper = entity_metadata_wrapper('node', $package);

  $md5_string = '';

  if (isset($entity_wrapper->field_editorialpublicationdate) && $field_editorialpublicationdate = $entity_wrapper->field_editorialpublicationdate->value()) {
    $md5_string .= $field_editorialpublicationdate['value'];
  }
  if (isset($entity_wrapper->field_displaytitle) && $field_displaytitle = $entity_wrapper->field_displaytitle->value()) {
    $md5_string .= $field_displaytitle['value'];
  }
  if (isset($entity_wrapper->field_externalurl) && $field_externalurl = $entity_wrapper->field_externalurl->value()) {
    $md5_string .= $field_externalurl['url'];
  }
  if (isset($entity_wrapper->field_textbody) && $field_textbody = $entity_wrapper->field_textbody->value()) {
    $md5_string .= $field_textbody['value'];
  }

  return md5($md5_string);
}
