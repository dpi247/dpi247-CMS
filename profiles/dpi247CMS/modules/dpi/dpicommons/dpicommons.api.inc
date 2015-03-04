<?php

/**
 * This function return an url for an image
 * 
 * @param scald_atom $atom
 * @param String $preset
 * @param string $entity
 * @param array $context
 * @return String
 */
function dpiflexiscald_api_get_path_for_atom($atom, $preset, $entity=null, $context = array()){
  $path=$atom->file_source;
  return image_style_url($preset,$path);
}

/**
 * This function return a render of image
 * 
 * @param scald_atom $atom
 * @param string $preset
 * @param string $entity
 * @param array $context
 * @return array
 */
function dpiflexiscald_api_get_render_for_atom($atom, $preset, $entity=null, $context = array()){
  $path=$atom->file_source;
  $path= image_style_url($preset,$path);
  
  return scald_render($sid, $preset);
}