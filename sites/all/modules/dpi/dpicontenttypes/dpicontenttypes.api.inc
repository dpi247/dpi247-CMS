<?php
/**
 * Return a array with the content types of type dpi (like article-package, photo, etc)
 */
function dpicontenttypes_api_getdpicontenttypes(){
  return array(
    'article-package',
    'gallery-package',
    'poll-package',
    'link',
    'link-list',
    'photo',
  );

}