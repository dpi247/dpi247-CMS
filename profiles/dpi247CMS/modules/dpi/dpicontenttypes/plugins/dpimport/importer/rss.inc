<?php 

$plugin = array(
  'label' => t('RSS'),
  'type'  => 'rss',
  'versions' => array(
    '1' => array('class' => 'RSSV1',),
  ),
  'dispatchers' => array(
    'UrlDispatcher',
  ),
);
