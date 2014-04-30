<?php 

$plugin = array(
  'label' => t('DPI XML'),
  'type'  => 'dpixml',
  'versions' => array(
    '1' => array('class' => 'DpiXmlV1'),
  ),
  'dispatchers' => array(
    'FolderDispatcher',
    'UrlDispatcher',
    'PostDispatcher',
  ),
);