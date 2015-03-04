<?php 

$plugin = array(
  'label' => t('RSS'),
  'type'  => 'rss',
  'versions' => array(
    '1' => array(
      'class' => 'RSSV1',
      'include' => array(
        'module' => 'dpicontenttypes',
        'path' => 'plugins/dpimport/importer/rssv1.class',
      ),
    ),
  ),
  'dispatchers' => array(
    'UrlDispatcher',
  ),
);
