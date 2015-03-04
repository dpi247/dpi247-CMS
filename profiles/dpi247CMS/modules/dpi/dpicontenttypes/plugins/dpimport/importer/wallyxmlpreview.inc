<?php 

$plugin = array(
  'label' => t('Wally XML Preview'),
  'type'  => 'wallyxmlpreview',
  'versions' => array(
    '1' => array('class' => 'WallyXmlPreview'),
  ),
  'dispatchers' => array(
    'FolderDispatcher',
    'UrlDispatcher',
    'PostDispatcher',
  ),
);